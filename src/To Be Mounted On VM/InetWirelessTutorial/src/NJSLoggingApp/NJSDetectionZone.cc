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

#include "NJSDetectionZone.h"

NJSDetectionZone::~NJSDetectionZone() {
    // TODO Auto-generated destructor stub
}

void NJSDetectionZone::record(std::vector<NJSReportElement> report)
{
    recordings.insert(recordings.end(), report.begin(), report.end());
}


void NJSDetectionZone::record(NJSReportElement reportElement)
{
    recordings.push_back(reportElement);
}


bool NJSDetectionZone::isInBoundary(const Coord& location)
{
    return vertex3.x < location.x && location.x < vertex2.x &&
           vertex3.y < location.y && location.y < vertex2.y;
}


bool NJSDetectionZone::includes(Coord location)
{
    return isInBoundary(location);
}

bool NJSDetectionZone::includesDz(NJSDetectionZone dz)
{
    // If DZ shares same centerPoint (and thus same corners)
    if (dz.centerPoint == centerPoint) return true;

    return isInBoundary(dz.vertex1) || isInBoundary(dz.vertex2) || isInBoundary(dz.vertex3) || isInBoundary(dz.vertex4);
}

cRectangleFigure* NJSDetectionZone::visualize()
{
    cRectangleFigure* square = new cRectangleFigure("DZViz");
    double width = vertex2.x - vertex1.x;
    double height = vertex1.y - vertex3.y;


    square->setBounds(cFigure::Rectangle(vertex3.x, vertex3.y, height, width));

    square->setFillColor(cFigure::BLACK);
    square->setFilled(false);


    square->setLineWidth(1);
    square->setLineColor(cFigure::BLACK);
    square->setZIndex(-5);


    if (njsExp != 0)
    {
        square->setLineColor(cFigure::RED);
        square->setZIndex(0);
    }



    return square;
}

void NJSDetectionZone::constructTimelines(simtime_t start, simtime_t end, simtime_t logInterval)
{
    bool verbose = true;

    // Count the number of timesteps between start and end using logIntervals
    int numIntervals = (end - start) / logInterval;

    // Validate that the number of intervals is positive
    if (numIntervals <= 0)
    {
        EV_ERROR << "Invalid number of intervals (" << numIntervals << "). Exiting constructTimelines." << endl;
        return;
    }

    timelineLength = numIntervals;

    // Count the number of unique nodes that exist in the collected reportElements
    uniqueNodeNames.clear();
    for (const NJSReportElement& el : recordings) // Now recordings is a std::list
    {
        uniqueNodeNames.insert(el.getNode());
    }
    int numUniqueNodes = uniqueNodeNames.size();

    // Validate that the number of unique nodes is positive
    if (numUniqueNodes <= 0)
    {
        //EV_ERROR << "Invalid number of unique nodes (" << numUniqueNodes << "). Exiting constructTimelines." << endl;
        return;
    }

    // Resize the timelines vector to fit numUniqueNodes
    timelines.resize(numUniqueNodes);

    // Initialize timelines for each unique node
    for (int i = 0; i < numUniqueNodes; i++)
    {
        timelines[i].resize(numIntervals, NJSState::IDLE); // Initialize all values to NJSState::IDLE
    }

    // Store the iterators of the used recordings
    std::list<NJSReportElement>::iterator itUsedRecording;

    // Fill in timelines based on recordings
    int oobWarning = 0;
    int numJammedWarning = 0;
    for (auto it = recordings.begin(); it != recordings.end(); ++it)
    {
        reportsProcessed++;

        const NJSReportElement& el = *it;
        int relevantTimeline = getNodeTimelinePosition(el.getNode());
        if (relevantTimeline == -1) continue;
        if (el.getTimestamp() - start < 0) continue;

        int relevantTimeInterval = (el.getTimestamp() - (start)) / logInterval;

        // Validate that the relevantTimeInterval is within bounds
        if (relevantTimeInterval >= 0 && relevantTimeInterval < numIntervals)
        {
            timelines[relevantTimeline][relevantTimeInterval] = el.getNJSState();
            if (el.getNJSState() == NJSState::CORRUPTED) numJammedWarning++;
        }
        else
        {
            oobWarning++;
        }

        // Save the iterator of the used recording
        itUsedRecording = it;
    }

    if (oobWarning > 0 && verbose)
    {
        EV_WARN << oobWarning << " Out of bounds warnings, out of " << recordings.size() << endl;
    }

    if (numJammedWarning > 0 && verbose)
    {
        EV_WARN << numJammedWarning << " Jammed warnings, out of " << recordings.size() << endl;
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



void NJSDetectionZone::destroyTimelines()
{
    EV_INFO << "Starting deletion of timelines..." << endl;

    // Clear the vector, releasing all memory
    if (!timelines.empty())
    {
        timelines.clear();  // Clears memory managed by vector
    }
}

void NJSDetectionZone::computeNJS(const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed = false)
{
    bool verbose = true;

    // Compute NJSexp based on jammed nodes
    int preRunExp = njsExp;
    for (auto it = uniqueNodeNames.begin(); it != uniqueNodeNames.end(); it++)
    {
        auto val = &(*it);
        const char* vehicle = val->c_str();

        for (int slot=0; slot<timelineLength; slot++)
        {
            if (checkJammedNode(slot, vehicle, rsuNode, rsuTimeline, distanceToRSU, coverageRadiusRSU, debugIsActuallyBeingJammed))
            {
                njsExp++;

                if (!debugIsActuallyBeingJammed)
                {
                    EV_INFO << "Something bad happended, checkJammedNode True when should be False" << endl;
                }
            }
        }
    }
    int postRunExp = njsExp;

    if (njsExp > 0 && verbose) EV_INFO << "njsExp = " << postRunExp - preRunExp << " of " << timelineLength << endl;

    if (postRunExp - preRunExp < 100)
    {
        njsExp = preRunExp;
    }

    // Compute NJSimp based on non-jammed slots
    /**
    for (auto it = uniqueNodeNames.begin(); it != uniqueNodeNames.end(); it++)
    {
        auto val = &(*it);
        const char* vehicle = val->c_str();
        int relevantTimeline = getNodeTimelinePosition(vehicle);
        if (relevantTimeline == -1) continue;

        for (int slot=0; slot<timelineLength; slot++)
        {
            if (isThisAJammedSlot(slot, vehicle, rsuNode, rsuTimeline, distanceToRSU, coverageRadiusRSU) && njsExp > 0 && !checkJammedNode(slot, vehicle, rsuNode, rsuTimeline, distanceToRSU, coverageRadiusRSU) && timelines[relevantTimeline][slot] != NJSState::IDLE)
            {
                njsImp++;
            }
        }
    }
    */
}

int NJSDetectionZone::getNodeTimelinePosition(const char* node)
{
    auto it = uniqueNodeNames.find(node);

    if (it != uniqueNodeNames.end())
    {
        int position = std::distance(uniqueNodeNames.begin(), it);
        return position;
    }
    else
    {
        return -1;
    }

    return -1;
}

bool NJSDetectionZone::isThisAJammedSlot(int slot, const char* node, const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU)
{
    int relevantTimeline = getNodeTimelinePosition(node);
    if (relevantTimeline == -1) return false;

    bool result = false;

    if (timelines[relevantTimeline][slot] != NJSState::IDLE)
    {
        if (distanceToRSU < 2 * coverageRadiusRSU && checkJammedNode(slot, node, rsuNode, rsuTimeline, distanceToRSU, coverageRadiusRSU, true))
        {
            result = true;
        }
    }

    return result;
}

bool NJSDetectionZone::checkJammedNode(int slot, const char* node, const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed = false)
{
    int relevantTimeline = getNodeTimelinePosition(node);
    if (relevantTimeline == -1) return false;

    int numBeforeToConsider = 5;
    bool validSender = false;


    std::string reason = "Weird One";
    for (int i=numBeforeToConsider; i>=0; i--)
    {
        if (timelines[relevantTimeline][slot] == NJSState::RECEIVING)
        {

            if (distanceToRSU < coverageRadiusRSU)
            {
                if (rsuTimeline[slot-i] == NJSState::TRANSMITTING)
                {
                    validSender = true;
                    reason = "Receiver Transmitter";
                }
                else
                {
                    reason = "RSU not transmitting";
                }
            }
        }

        if (timelines[relevantTimeline][slot] == NJSState::TRANSMITTING || timelines[relevantTimeline][slot] == NJSState::IDLE)
        {
            validSender = true;
            reason = "Transmitting or Idle";
        }
        else if (timelines[relevantTimeline][slot] != NJSState::RECEIVING)
        {
            reason = "Receiving(2) or Corrupted(3)";
        }
    }


    if (timelines[relevantTimeline][slot] == NJSState::CORRUPTED)
    {
        validSender = false;
        reason = "Known Corruption";
    }


    if (!validSender && !debugIsActuallyBeingJammed)
    {
        EV_INFO << reason << endl;
    }

    return !validSender;
}


