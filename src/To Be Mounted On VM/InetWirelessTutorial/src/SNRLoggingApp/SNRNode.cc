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

#include "SNRNode.h"

SNRNode::~SNRNode() {
    // TODO Auto-generated destructor stub
}

void SNRNode::record(SNRReportElement reportElement)
{
    recordings.push_back(reportElement);
}

void SNRNode::constructTimelines(simtime_t start, simtime_t end, simtime_t logInterval)
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

void SNRNode::computeSNR(double thresholdMetricValue)
{
    if (timelineLength == 0) {
        EV_WARN << "computeSNR(): Timeline length is zero. No elements to process.\n";
        return;
    }

    if ((int)timeline.size() < timelineLength) {
        EV_ERROR << "computeSNR(): Timeline size (" << timeline.size()
                 << ") is smaller than expected length (" << timelineLength << ").\n";
        return;
    }

    for (int slot = 0; slot < timelineLength; ++slot)
    {
        SNRReportElement el = timeline[slot];

        if (!el.isValid()) {  // Optional, if SNRReportElement supports this
            EV_WARN << "Skipping invalid SNRReportElement at slot " << slot << ".\n";
            continue;
        }

        if (useFixedApproach)
        {
            double snr = el.getSNR();
            double txStrength = el.getRsuTransmissionStrength();

            if (std::isnan(snr) || std::isnan(txStrength)) {
                EV_WARN << "NaN detected in SNR or transmission strength for node "
                        << el.getNode() << " at slot " << slot << ". Skipping.\n";
                continue;
            }

            // Hardcoded 400: consider replacing with a constant or parameter
            double numerator = 400 - (txStrength + x * snr);
            double metricValue = numerator / 400.0;

            EV_INFO << el.getNode() << ": metricValue=" << metricValue << "\n";

            if (metricValue > thresholdMetricValue)
            {
                Coord pos = el.getPosition();
                if (std::isnan(pos.x) || std::isnan(pos.y) || std::isnan(pos.z)) {
                    EV_WARN << "Invalid position detected for node " << el.getNode()
                            << ". Skipping jammed point addition.\n";
                    continue;
                }
                jammedPoints.push_back(pos);
            }
        }
        else {
            EV_WARN << "computeSNR(): useFixedApproach is false; skipping computation for slot " << slot << ".\n";
        }
    }
}


