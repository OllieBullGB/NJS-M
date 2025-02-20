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

#include "NJSLegacyLoggingApp.h"

Define_Module(NJSLegacyLoggingApp);

NJSLegacyLoggingApp::NJSLegacyLoggingApp() {
    processReportsTimer = nullptr;
}

NJSLegacyLoggingApp::~NJSLegacyLoggingApp() {
    cancelAndDelete(processReportsTimer);

    cCanvas* canvas = getParentModule()->getParentModule()->getCanvas();
    // Remove all existing figures with the name "DzViz"
    for (int i = 0; i < canvas->getNumFigures(); ++i)
    {
        cFigure* figure = canvas->getFigure(i);
        if (strcmp(figure->getName(), "DzViz") == 0) // Check if the figure's name is "DzViz"
        {
            canvas->removeFigure(figure); // Remove the figure from the canvas
            --i; // Decrement the index since the figure list is now one element shorter
        }
    }
}

void NJSLegacyLoggingApp::initialize(int stage)
{
    EV_INFO << "Stage " << stage << endl;
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
    {
        radio = dynamic_cast<physicallayer::IRadio*>(getParentModule()->getSubmodule("wlan", 0)->getSubmodule("radio"));
        if (radio == nullptr)
        {
            EV_ERROR << "No Radio Module on Parent!" << endl;
        }
        else
        {
            EV_INFO << "Radio Module Found on Parent!" << endl;
        }

        mobility = dynamic_cast<IMobility*>(getParentModule()->getSubmodule("mobility"));
        if (mobility == nullptr)
        {
            EV_ERROR << "No Mobility Module on Parent!" << endl;
        }
        else
        {
            EV_INFO << "Mobility Module Found on Parent!" << endl;
        }

        // establish RSU as the first node
        NJSLegacyNode rsuNode = NJSLegacyNode("RSU", mobility->getCurrentPosition());
        nodes.push_back(rsuNode);


        processReportsTimer = new cMessage("processReportsTimer");
        processReportsInterval = par("processReportsInterval");
        processReportsDelay = par("processReportsDelay");
        cleanAfterEachCalculation = par("cleanAfterEachCalculation");

        scheduleAt(simTime() + processReportsDelay + processReportsInterval, processReportsTimer);

        runName = getRNG(0)->intRand();

        EV << "NJS Legacy Logging App Successfully Init" << endl;
    }
}

void NJSLegacyLoggingApp::handleMessageWhenUp(cMessage *msg)
{
    if (msg == processReportsTimer)
    {
        processReportsTimer = new cMessage("processReportsTimer");


        for (NJSLegacyNode& existingNode : nodes)
        {
            existingNode.constructTimelines(simTime() - processReportsInterval, simTime(), 0.007);

            NJSLegacyNode rsuNode = nodes[0];

            //if (rsuNode.myNodeName == "RSU") continue;

            std::vector<NJSState> rsuTimeline = rsuNode.timeline;

            double distanceToRSU = calculateDistanceToCoord(existingNode.latestPosition);

            existingNode.computeNJS("RSU", rsuTimeline, distanceToRSU, coverageRadius, false);

            EV_INFO << existingNode.myNodeName << " : " << existingNode.njsExp + existingNode.njsImp << " Due to " << rsuNode.myNodeName << endl;

            if(existingNode.njsExp + existingNode.njsImp > 0 && !hadFirstDetection)
            {
                timeToFirstDetection = simTime();
                hadFirstDetection = true;
            }
        }


        Coord predictedJammerPosition = localiseJammer();
        EV_INFO << "EXPECTED JAMMER AT: " << predictedJammerPosition << " JLE = " << calculateJammerLocalisationError(predictedJammerPosition, "jammer") << endl;
        scheduleAt(simTime() + processReportsInterval, processReportsTimer);

        recordAndSaveMetricsAtTimestep();
        EV_INFO << "SAVED METRICS";

        if (cleanAfterEachCalculation)
        {
            for (NJSLegacyNode& existingNode : nodes)
            {
                existingNode.njsImp = 0;
                existingNode.njsExp = 0;
            }
        }
    }

    //ApplicationBase::handleMessageWhenUp(msg);
}

double NJSLegacyLoggingApp::calculateDistanceToCoord(Coord position)
{
    if (position == centerPoint) return 0;

    return centerPoint.distance(position);
}

void NJSLegacyLoggingApp::addReport(std::vector<NJSReportElement> report)
{
    EV_INFO << "Legacy Logging App Received report of size: " << report.size() << " there are " << nodes.size() << " nodes registered" << endl;

    for (NJSReportElement& reportElement : report)
    {
        // add report to recordings
        NJSLegacyNode* node = relevantNode(reportElement);
        if (node == nullptr)
        {
            // Create a new NJSLegacyNode directly and add it to the nodes vector
            NJSLegacyNode newNode(reportElement.getNode(), reportElement.getPosition());
            nodes.push_back(newNode);
            node = &nodes.back();  // Get a pointer to the newly added node
            EV_INFO << "Added New Node: " << node->myNodeName << " at position " << node->originalPosition << endl;
        }

        node->record(reportElement);
    }

    return;
}

NJSLegacyNode* NJSLegacyLoggingApp::relevantNode(NJSReportElement reportElement)
{
    NJSLegacyNode* ptr = nodes.data();

    for (NJSLegacyNode* end = ptr + nodes.size(); ptr != end; ++ptr)
    {
        if (ptr->myNodeName == reportElement.getNode()) return ptr;
    }

    // if not found, add a new node to the pool

    return nullptr;
}

Coord NJSLegacyLoggingApp::localiseJammer()
{
    std::vector<NJSLegacyNode> jammedNodes;
    std::vector<Coord> uniqueCoords;

    for (const auto& node: nodes)
    {
        if (node.njsExp + node.njsImp > 0)
        {
            jammedNodes.push_back(node);
            uniqueCoords.push_back(node.originalPosition);
        }
    }

    return calculateAveragePoint(uniqueCoords);
}

Coord NJSLegacyLoggingApp::calculateAveragePoint(const std::vector<Coord>& coords)
{
    double sumX = 0.0;
    double sumY = 0.0;
    double sumZ = 0.0;
    size_t count = coords.size();

    for (const Coord& coord : coords)
    {
        sumX += coord.x;
        sumY += coord.y;
        sumZ += coord.z;
    }

    return Coord(sumX/count, sumY/count, sumZ/count);
}

double NJSLegacyLoggingApp::calculateJammerLocalisationError(Coord& predictedPosition, const char* jammerName)
{
    cModule* otherHost = getSimulation()->getModuleByPath(jammerName);
    if (!otherHost)
    {
        EV_ERROR << "Host " << jammerName << " not found!" << endl;
        return -1;  // Return a negative value to indicate an error
    }

    // Get the mobility module of the other host
    IMobility* otherMobility = check_and_cast<IMobility*>(otherHost->getSubmodule("mobility"));
    Coord otherPosition = otherMobility->getCurrentPosition();

    return sqrt(predictedPosition.sqrdist(otherPosition));
}

std::array<int, 4> NJSLegacyLoggingApp::calculateJammingAccuracy(const char* jammerName)
{
    // for each DZ
    //  determine whether if it has >=1 vertex in jammed area

    int tp = 0;
    int fp = 0;
    int tn = 0;
    int fn = 0;

    cModule* otherHost = getSimulation()->getModuleByPath(jammerName);
    IMobility* otherMobility = check_and_cast<IMobility*>(otherHost->getSubmodule("mobility"));
    Coord otherPosition = otherMobility->getCurrentPosition();
    NJSJammerApp* otherJammer = check_and_cast<NJSJammerApp*>(otherHost->getSubmodule("app", 0));

    double jammingRadius = otherJammer->jammingRadius;


    int i = 0;
    for (const auto& node: nodes)
    {
        i++;

        double minDist = node.originalPosition.distance(otherPosition);

        if (minDist > jammingRadius)
        {
            if (node.njsExp + node.njsImp > 0)
            {
                fp++;
            }
            else
            {
                tn++;
            }
        }
        else
        {
            if (node.njsExp + node.njsImp > 0)
            {
                tp++;
            }
            else if (node.reportsProcessed > 0)
            {
                fn++;
            }
        }
    }

    EV_INFO << "Accuracy Stats: " << tp << " " << tn << " " << fp << " " << fn << " " << i << endl;

    return {tp, fp, tn, fn};
}

void NJSLegacyLoggingApp::recordAndSaveMetricsAtTimestep()
{
    // Jammer Localisation Error
    Coord predictedJammerPosition = localiseJammer();
    double jle = calculateJammerLocalisationError(predictedJammerPosition, "jammer");

    // Accuracy Stats
    std::array<int, 4> accuracy = calculateJammingAccuracy("jammer");

    // Time to First Detection
    double ttfd = timeToFirstDetection.dbl();


    saveResultsToFile(jle, accuracy, ttfd, "/home/veins/workspace.omnetpp/InetWirelessTutorial/simulations/NJS_ap/results/legacyResults.txt");
}

void NJSLegacyLoggingApp::saveResultsToFile(double jle, std::array<int, 4> accuracy, double ttfd, const std::string& filename)
{
    // Open file in append mode
    std::ofstream file;
    bool addHeader = false;

    // Check if the file exists and has a header
    std::ifstream infile(filename);
    if (!infile.good()) {
        addHeader = true; // File doesn't exist, header needs to be added
    } else {
        std::string firstLine;
        std::getline(infile, firstLine);
        if (firstLine.empty()) {
            addHeader = true; // File is empty, add the header
        }
    }
    infile.close();

    file.open(filename, std::ios::app);

    double accuracyMetric = (accuracy[0] + accuracy[1]) / (accuracy[0] + accuracy[1] + accuracy[2] + accuracy[3]);

    if (file.is_open()) {
        // Add the header if needed
        if (addHeader) {
            file << "runName,time,JLE,tp,fp,tn,fn,accuracy,ttfd\n";
        }

        // Write values to file
        file << std::fixed << std::setprecision(5);
        file << runName << "," << simTime().dbl() << "," << jle << ","
             << accuracy[0] << "," << accuracy[1] << "," << accuracy[2] << ","
             << accuracy[3] << "," << accuracyMetric << "," << ttfd << std::endl;
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
        // Throw an exception if unable to open the file
        throw std::ios_base::failure("Unable to open file: " + filename);
    }
}

