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

#include "NJSLoggingApp.h"

Define_Module(NJSLoggingApp);

NJSLoggingApp::NJSLoggingApp() {
    processReportsTimer = nullptr;
}

NJSLoggingApp::~NJSLoggingApp() {
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

void NJSLoggingApp::initialize(int stage)
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



        processReportsTimer = new cMessage("processReportsTimer");
        processReportsInterval = par("processReportsInterval");
        processReportsDelay = par("processReportsDelay");
        cleanAfterEachCalculation = par("cleanAfterEachCalculation");

        scheduleAt(simTime() + processReportsDelay + processReportsInterval, processReportsTimer);

        runName = getRNG(0)->intRand();

        EV << "NJS Logging App Successfully Init" << endl;
    }

    if (stage == 12)
    {
        coverageRadius = check_and_cast<const physicallayer::IRadioMedium *>(radio->getMedium())->getMediumLimitCache()->getMaxCommunicationRange(radio).get();
        centerPoint = mobility->getCurrentPosition();
        claimDetectionZoneCoverage(centerPoint);
    }
}

void NJSLoggingApp::handleMessageWhenUp(cMessage *msg)
{
    if (msg == processReportsTimer)
    {
        processReportsTimer = new cMessage("processReportsTimer");
        int nodesRegistered = 0;

        for (NJSDetectionZone& existingDz : detectionZones)
        {
            existingDz.constructTimelines(simTime() - processReportsInterval, simTime(), 0.007);
            nodesRegistered += existingDz.uniqueNodeNames.size();

            NJSDetectionZone rsuDz = detectionZones[0];

            if (existingDz.centerPoint == rsuDz.centerPoint) continue;

            int rsuTimelinePosition = rsuDz.getNodeTimelinePosition("RSU");
            if (rsuTimelinePosition == -1)
            {
                EV_WARN << "COULDNT FIND RSU TIMELINE (VERY BAD)" << endl;
                continue;
            }

            std::vector<NJSState> rsuTimeline = rsuDz.timelines[rsuTimelinePosition];

            double distanceToRSU = calculateDistanceToCoord(existingDz.centerPoint);

            cModule* otherHost = getSimulation()->getModuleByPath("jammer");
            // Get the mobility module of the other host
            IMobility* otherMobility = check_and_cast<IMobility*>(otherHost->getSubmodule("mobility"));
            Coord otherPosition = otherMobility->getCurrentPosition();
            double distanceToJammer = otherPosition.distance(existingDz.centerPoint);
            bool isActuallyJammed = distanceToJammer < 215;


            existingDz.computeNJS("RSU", rsuTimeline, distanceToRSU, coverageRadius, isActuallyJammed);

            if(existingDz.njsExp + existingDz.njsImp > 0 && !hadFirstDetection)
            {
                timeToFirstDetection = simTime();
                hadFirstDetection = true;
            }
        }

        EV_INFO << nodesRegistered << " nodes registered on this RSU" << endl;

        visualizeDetectionZones();
        Coord predictedJammerPosition = localiseJammer();
        EV_INFO << "EXPECTED JAMMER AT: " << predictedJammerPosition << " JLE = " << calculateJammerLocalisationError(predictedJammerPosition, "jammer") << endl;
        scheduleAt(simTime() + processReportsInterval, processReportsTimer);

        recordAndSaveMetricsAtTimestep();
        EV_INFO << "SAVED METRICS";
        EV_INFO << "RECORDED " << validReportsProcessed << " REPORTS" << endl;

        if (cleanAfterEachCalculation)
        {
            for (NJSDetectionZone& existingDz : detectionZones)
            {
                existingDz.njsImp = 0;
                existingDz.njsExp = 0;
            }
        }
    }

    //ApplicationBase::handleMessageWhenUp(msg);
}

void NJSLoggingApp::addReport(std::vector<NJSReportElement> report)
{
    EV_INFO << "Logging App Received report of size: " << report.size() << " from " << report[0].getNode() << endl;

    for (NJSReportElement& reportElement : report)
    {

        NJSDetectionZone* relevantDz = relevantDetectionZone(reportElement);
        if (relevantDz == nullptr)
        {
            EV_WARN << "Couldn't attribute to any DZ, skipping" << endl;
            continue;
        }
        relevantDz->record(reportElement);

        //EV << "Sent Log to DZ: " << relevantDz->centerPoint << endl;
    }

    NJSReportElement& lastElement = report.back();
    double dist = lastElement.getPosition().distance(centerPoint);

    if (dist <= coverageRadius) {
        validReportsProcessed += report.size();
    }

    return;
}

NJSDetectionZone* NJSLoggingApp::relevantDetectionZone(NJSReportElement reportElement)
{
    NJSDetectionZone* ptr = detectionZones.data();

    for (NJSDetectionZone* end = ptr + detectionZones.size(); ptr != end; ++ptr)
    {
        if (ptr->includes(reportElement.getPosition())) return ptr;
    }

    return nullptr;
}

void NJSLoggingApp::claimDetectionZoneCoverage(Coord point)
{
    EV_INFO << "Starting DZ Claiming Procedure" << endl;
    int limit = 20000;
    int i = 0;

    std::stack<NJSDetectionZone*> dzStack;
    std::stack<int> parentDirectionStack;

    NJSDetectionZone* initialDz = new NJSDetectionZone(point, d);
    dzStack.push(initialDz);

    while (!dzStack.empty() && i < limit)
    {

        NJSDetectionZone* currentDz = dzStack.top();
        dzStack.pop();

        Coord centerPoint = currentDz->centerPoint;


        // Is DZ centerpoint in range?
        if (calculateDistanceToCoord(centerPoint) >= coverageRadius)
        {
            continue;
        }

        if (!claimDetectionZone(*currentDz))
        {
            continue;
        }

        i++;

        Coord top(centerPoint.x, centerPoint.y + d);
        Coord bottom(centerPoint.x, centerPoint.y - d);
        Coord left(centerPoint.x - d, centerPoint.y);
        Coord right(centerPoint.x + d, centerPoint.y);

        NJSDetectionZone* topDz = new NJSDetectionZone(top, d);
        NJSDetectionZone* bottomDz = new NJSDetectionZone(bottom, d);
        NJSDetectionZone* leftDz = new NJSDetectionZone(left, d);
        NJSDetectionZone* rightDz = new NJSDetectionZone(right, d);

        dzStack.push(topDz);
        dzStack.push(bottomDz);
        dzStack.push(leftDz);
        dzStack.push(rightDz);
    }

    visualizeDetectionZones();

}

bool NJSLoggingApp::claimDetectionZone(NJSDetectionZone dz)
{
    // check if the new DZ intersects with any of the claimed DZs
    for (NJSDetectionZone& existingDz : detectionZones)
    {
        if (existingDz.includesDz(dz)) return false;
    }

    detectionZones.push_back(dz);
    return true;
}

double NJSLoggingApp::calculateDistanceToCoord(Coord position)
{
    if (position == centerPoint) return 0;

    return centerPoint.distance(position);
}

void NJSLoggingApp::visualizeDetectionZones()
{
    cCanvas* canvas = getParentModule()->getParentModule()->getCanvas();
    canvas->setBackgroundColor(cFigure::RED);
    visualizations.clear();

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

    // Add new detection zones
    for (size_t i = 0; i < detectionZones.size(); ++i)
    {
        NJSDetectionZone& dz = detectionZones[i];

        cRectangleFigure* square = dz.visualize();
        square->setName("DzViz"); // Assign the name "DzViz" to the new figures

        canvas->addFigure(square); // Add the new figure to the canvas
    }
}

Coord NJSLoggingApp::localiseJammer()
{
    std::vector<NJSDetectionZone> jammedDetectionZones;
    std::vector<Coord> uniqueCoords;

    // 1. Get all DZ with NJS > 0
    for (const auto& dz : detectionZones)
    {
        if (dz.njsExp + dz.njsImp > 0)
        {
            jammedDetectionZones.push_back(dz);
        }
    }

    // 2. Get all the unique points from corners of the jammed DZs
    for (const NJSDetectionZone& dz : jammedDetectionZones)
    {
        Coord coords[4] = { dz.vertex1, dz.vertex2, dz.vertex3, dz.vertex4 };

        // Iterate over each vertex in the current detection zone
        for (const Coord& pt : coords)
        {
            // Check if the point is already in the uniquePoints list
            bool isUnique = true;
            for (const Coord& uniquePt : uniqueCoords)
            {
                if (pt == uniquePt) // Assuming the '==' operator is defined for the Point type
                {
                    isUnique = false;
                    break;
                }
            }

            // If the point is unique, add it to the list
            if (isUnique)
            {
                uniqueCoords.push_back(pt);
            }
        }
    }

    // 3. Conduct a Graham Scan to get the Jammed Area


    // 4. Localise using points.
    return calculateAveragePoint(uniqueCoords);
}

Coord NJSLoggingApp::calculateAveragePoint(const std::vector<Coord>& coords)
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

double NJSLoggingApp::calculateJammerLocalisationError(Coord& predictedPosition, const char* jammerName)
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

    double error = sqrt(predictedPosition.sqrdist(otherPosition));

    if (std::isnan(error)) {
        return -1;
    }

    return error;
}

std::array<int, 4> NJSLoggingApp::calculateJammingAccuracy(const char* jammerName)
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
    for (const auto& dz : detectionZones)
    {
        i++;

        double v1Dist = dz.vertex1.distance(otherPosition);
        double v2Dist = dz.vertex1.distance(otherPosition);
        double v3Dist = dz.vertex1.distance(otherPosition);
        double v4Dist = dz.vertex1.distance(otherPosition);

        double minDist = v1Dist;
        if (v2Dist < minDist) {
            minDist = v2Dist;
        }
        if (v3Dist < minDist) {
            minDist = v3Dist;
        }
        if (v4Dist < minDist) {
            minDist = v4Dist;
        }

        if (minDist > jammingRadius)
        {
            if (dz.njsExp + dz.njsImp > 0)
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
            if (dz.njsExp + dz.njsImp > 0)
            {
                tp++;
            }
            else if (dz.reportsProcessed > 0)
            {
                fn++;
            }
        }
    }

    EV_INFO << "Accuracy Stats: " << tp << " " << tn << " " << fp << " " << fn << " " << i << endl;

    return {tp, fp, tn, fn};
}

void NJSLoggingApp::recordAndSaveMetricsAtTimestep()
{
    // Jammer Localisation Error
    Coord predictedJammerPosition = localiseJammer();
    double jle = calculateJammerLocalisationError(predictedJammerPosition, "jammer");

    // Accuracy Stats
    std::array<int, 4> accuracy = calculateJammingAccuracy("jammer");

    // Time to First Detection
    double ttfd = timeToFirstDetection.dbl();

    // Vehicle Speed
    cModule* otherHost = getSimulation()->getModuleByPath("node[0]");
    IMobility* otherMobility = check_and_cast<IMobility*>(otherHost->getSubmodule("mobility"));
    double speed = otherMobility->getMaxSpeed();

    saveResultsToFile(jle, accuracy, ttfd, speed, "/home/veins/workspace.omnetpp/InetWirelessTutorial/simulations/NJS_ap/results/results.txt");
}

void NJSLoggingApp::saveResultsToFile(double jle, std::array<int, 4> accuracy, double ttfd, double speed, const std::string& filename)
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
            file << "runName,time,JLE,tp,fp,tn,fn,accuracy,ttfd,vSpeed,validReportsProcessed\n";
        }

        // Write values to file
        file << std::fixed << std::setprecision(5);
        file << runName << "," << simTime().dbl() << "," << jle << ","
             << accuracy[0] << "," << accuracy[1] << "," << accuracy[2] << ","
             << accuracy[3] << "," << accuracyMetric << "," << ttfd << "," << speed << ',' << validReportsProcessed << std::endl;
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
        // Throw an exception if unable to open the file
        throw std::ios_base::failure("Unable to open file: " + filename);
    }
}

