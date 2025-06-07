//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "MLNode.h"

MLNode::~MLNode() {
    // TODO Auto-generated destructor stub
}



void MLNode::record(SNRReportElement reportElement)
{
    recordings.push_back(reportElement);
}

void MLNode::constructTimelines(simtime_t start, simtime_t end, simtime_t logInterval)
{
    // Count the number of timesteps between start and end using logIntervals
    int numIntervals = (end - start) / logInterval;

    // Validate that the number of intervals is positive
    if (numIntervals <= 0)
    {
        EV_ERROR << "Invalid number of intervals (" << numIntervals << "). Exiting constructTimelines." << endl;
        return;
    }

    timelineLength = numIntervals;


    // Store the iterators of the used recordings
    std::list<SNRReportElement>::iterator itUsedRecording;

    timeline.resize(numIntervals, *recordings.begin());

    int oobWarning = 0;
    for (auto it = recordings.begin(); it != recordings.end(); ++it)
    {
        reportsProcessed++;

        const SNRReportElement& el = *it;
        if (el.getTimestamp() - start < 0) continue;

        int relevantTimeInterval = (el.getTimestamp() - (start)) / logInterval;

        if (relevantTimeInterval >= 0 && relevantTimeInterval < numIntervals)
        {
            // Put in the full SNR formula / any changes you want here.
            timeline[relevantTimeInterval] = el;
        }
        else
        {
            oobWarning++;
        }

        itUsedRecording = it;
    }

    // Optional: Iterate over all recordings, pruning all the recordings that are listed as starting < start
    if (!recordings.empty())
    {
        //EV_INFO << "Size before pruning: " << recordings.size();
        for (auto it = recordings.begin(); it != recordings.end();)
        {
            if (it->getTimestamp() < end)
            {
                it = recordings.erase(it); // Erase and move the iterator to the next element
            }
            else
            {
                ++it; // Move to the next element if not erased
            }
        }
        //EV_INFO << " after: " << recordings.size() << endl;
    }
}

void MLNode::runMLModel()
{
    if (timelineLength == 0) {
        EV_WARN << "runMLModel(): Timeline length is zero. No elements to process.\n";
        return;
    }

    if ((int)timeline.size() < timelineLength) {
        EV_ERROR << "runMLModel(): Timeline size (" << timeline.size()
                 << ") is smaller than expected length (" << timelineLength << ").\n";
        return;
    }

    for (int slot = 0; slot < timelineLength; ++slot)
    {
        SNRReportElement el = timeline[slot];



        if (!el.isValid()) {
            EV_WARN << "runMLModel(): Skipping invalid SNRReportElement at slot " << slot << ".\n";
            continue;
        }

        double pSignal = el.getRsuTransmissionStrength();
        double pNoise  = el.getJammerTransmissionStrength();

        if (!std::isfinite(pSignal) || !std::isfinite(pNoise)) {
            EV_WARN << "runMLModel(): Invalid signal or noise power at slot " << slot
                    << " for node " << el.getNode() << ".\n";
            continue;
        }

        if (logTrainingData == 1)
        {
            logJammingState(pSignal, pNoise, "/home/veins/workspace.omnetpp/InetWirelessTutorial/simulations/NJS_ap/results/mlTrainingData.txt", el.getPosition());
        }

        std::array<double, 3> input;
        try {
            input = model.convert_to_input(pSignal, pNoise);
        } catch (const std::exception& e) {
            EV_ERROR << "runMLModel(): Exception in convert_to_input at slot " << slot
                     << ": " << e.what() << "\n";
            continue;
        }

        logSNR(slot, input[2], input[0], input[1], el.isValid());

        std::array<double, 4> debugOutput = {0.0, 0.0, NAN, NAN}; // Padding
        try {
            model.score(input.data(), debugOutput.data());
        } catch (const std::exception& e) {
            EV_ERROR << "runMLModel(): Exception during model scoring at slot " << slot
                     << ": " << e.what() << "\n";
            continue;
        }
        std::array<double, 2> output = {debugOutput[0], debugOutput[1]};

        // Sanity check on output
        if (!std::isfinite(output[0]) || !std::isfinite(output[1])) {
            EV_WARN << "runMLModel(): Model returned invalid output for node "
                    << el.getNode() << " at slot " << slot << ".\n";
            continue;
        }

        if (output[1] > modelThreshold) {
            Coord pos = el.getPosition();
            if (!std::isfinite(pos.x) || !std::isfinite(pos.y) || !std::isfinite(pos.z)) {
                EV_WARN << "runMLModel(): Invalid position for jammed point at slot "
                        << slot << ". Skipping.\n";
                continue;
            }

            jammedPoints.push_back(pos);
        }
    }
}


void MLNode::logJammingState(double pSignal, double pNoise, const std::string& filename, Coord myPosition)
{
    int isJammed = isJammedBy("jammer", myPosition);
    double distToJammer = calculateDistanceTo("jammer", myPosition);

    std::ofstream file;
    bool addHeader = false;

    std::ifstream infile(filename);
    if (!infile.good()) {
        addHeader = true;
    } else {
        std::string firstLine;
        std::getline(infile, firstLine);
        if (firstLine.empty()) {
            addHeader = true;
        }
    }
    infile.close();

    file.open(filename, std::ios::app);

    if (file.is_open()) {
        if (addHeader) {
            file << "p_signal,p_noise,is_jammed,dist_to_jammer\n";
        }

        file << std::fixed << std::setprecision(12);
        file << pSignal << "," << pNoise << "," << isJammed << "," << distToJammer << "\n";

        file.close();
    } else {
        std::cerr << "Unable to open jamming log file: " << filename << std::endl;
    }
}

double MLNode::calculateDistanceTo(const char* nodeName, Coord myPosition)
{
    // Find the other host by its name
    cModule* otherHost = getSimulation()->getModuleByPath(nodeName);
    if (!otherHost)
    {
        EV << "Host " << nodeName << " not found!" << endl;
        return -1;  // Return a negative value to indicate an error
    }

    IMobility* otherMobility = check_and_cast<IMobility*>(otherHost->getSubmodule("mobility"));
    Coord otherPosition = otherMobility->getCurrentPosition();

    // Calculate the Euclidean distance between the two positions
    double distance = myPosition.distance(otherPosition);

    return distance;
}

int MLNode::isJammedBy(const char* jammerName, Coord myPosition)
{
    EV_INFO << "Testing to see if jammed" << endl;
    cModule* otherHost = getSimulation()->getModuleByPath(jammerName);
    EV_INFO << "otherHost may exist" << endl;
    if (!otherHost)
    {
        EV_ERROR << "Host " << jammerName << " not found!" << endl;
        return -1;  // Return a negative value to indicate an error
    }


    double distance = calculateDistanceTo(jammerName, myPosition);
    if (distance == -1)
    {
        EV_ERROR << "Host " << jammerName << " has no valid distance to me!" << endl;
        return -1;  // Return a negative value to indicate an error
    }

    NJSJammerApp* jammerApp = check_and_cast<NJSJammerApp*>(otherHost->getSubmodule("app", 0));

    // Check if we are in range of the jammer
    if (distance > jammerApp->jammingRadius) return 0;
    EV_INFO << "Host in Range of Jammer (" << distance << " <= " << jammerApp->jammingRadius << ")" << endl;

    // Check if the jammer is currently broadcasting a jamming signal
    if (!jammerApp->transmittingJammingSignal) return 0;
    EV_INFO << "Jammer in range is transmitting! We are jammed!" << endl;

    return 1;
}

std::ofstream MLNode::snrLogFile;
bool MLNode::snrLogFileInitialized = false;

void MLNode::logSNR(int slot, double snr, double rsu, double jam, bool valid)
{
    if (snrLogFile.is_open())
    {
        snrLogFile << slot << ","
                   << myNodeName << ","
                   << position.x << ","
                   << position.y << ","
                   << position.z << ","
                   << snr << ","
                   << rsu << ","
                   << jam << ","
                   << valid
                   << "\n";
        snrLogFile.flush();  // optional: flush after every write
    }
}


void MLNode::openLogFile()
{
    if (!snrLogFileInitialized)
    {
        snrLogFile.open("snr_log_all_nodes.csv", std::ios::out);
        if (snrLogFile.is_open())
        {
            snrLogFile << "Slot,Node,PosX,PosY,PosZ,SNR,RSU,Jam,Valid\n";
            snrLogFileInitialized = true;
        } else {
            EV_ERROR << "Failed to open shared SNR log file\n";
        }
    }
}

void MLNode::closeLogFile()
{
    if (snrLogFile.is_open()) {
        snrLogFile.close();
        snrLogFileInitialized = false;
    }
}

