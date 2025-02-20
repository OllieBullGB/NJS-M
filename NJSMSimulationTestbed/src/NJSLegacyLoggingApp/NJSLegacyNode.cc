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

#include "NJSLegacyNode.h"

NJSLegacyNode::~NJSLegacyNode() {
    // TODO Auto-generated destructor stub
}

void NJSLegacyNode::record(NJSReportElement reportElement)
{
    recordings.push_back(reportElement);
}

void NJSLegacyNode::constructTimelines(simtime_t start, simtime_t end, simtime_t logInterval)
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

    timeline.resize(numIntervals, NJSState::IDLE);

    // Store the iterators of the used recordings
    std::list<NJSReportElement>::iterator itUsedRecording;

    int oobWarning = 0;
    for (auto it = recordings.begin(); it != recordings.end(); ++it)
    {
        reportsProcessed++;

        const NJSReportElement& el = *it;
        if (el.getTimestamp() - start < 0) continue;

        int relevantTimeInterval = (el.getTimestamp() - (start)) / logInterval;

        if (relevantTimeInterval >= 0 && relevantTimeInterval < numIntervals)
        {
            timeline[relevantTimeInterval] = el.getNJSState();
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

void NJSLegacyNode::computeNJS(const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed)
{
    bool verbose = true;

    int preRunExp = njsExp;
    for (int slot=0; slot<timelineLength; slot++)
    {
        if (checkJammedNode(slot, myNodeName, rsuNode, rsuTimeline, distanceToRSU, coverageRadiusRSU, debugIsActuallyBeingJammed))
        {
            njsExp++;
        }
    }
    int postRunExp = njsExp;

    if (postRunExp - preRunExp < 100)
    {
        njsExp = preRunExp;
    }
}

bool NJSLegacyNode::isThisAJammedSlot(int slot, const char* node, const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed)
{
    bool result = false;

    if (timeline[slot] != NJSState::IDLE)
    {
        if (distanceToRSU < 2 * coverageRadiusRSU && checkJammedNode(slot, node, rsuNode, rsuTimeline, distanceToRSU, coverageRadiusRSU, true))
        {
            result = true;
        }
    }

    return result;
}

bool NJSLegacyNode::checkJammedNode(int slot, const char* node, const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed = false)
{
    int numBeforeToConsider = 5;
    bool validSender = false;


    std::string reason = "Weird One";
    for (int i=numBeforeToConsider; i>=0; i--)
    {
        if (timeline[slot] == NJSState::RECEIVING)
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

        if (timeline[slot] == NJSState::TRANSMITTING || timeline[slot] == NJSState::IDLE)
        {
            validSender = true;
            reason = "Transmitting or Idle";
        }
        else if (timeline[slot] != NJSState::RECEIVING)
        {
            reason = "Receiving(2) or Corrupted(3)";
        }
    }


    if (timeline[slot] == NJSState::CORRUPTED)
    {
        validSender = false;
        reason = "Known Corruption";
    }


    if (!validSender && !debugIsActuallyBeingJammed && true == false)
    {
        EV_INFO << reason << endl;
    }

    return !validSender;
}

