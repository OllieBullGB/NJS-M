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

#ifndef NJSLEGACYLOGGINGAPP_NJSLEGACYNODE_H_
#define NJSLEGACYLOGGINGAPP_NJSLEGACYNODE_H_

#include <cmath>
#include <omnetpp.h>
#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"

#include "../NJSState/NJSState.h"
#include "../NJSReport/NJSReportElement.h"


class NJSLegacyNode {
public:
    NJSLegacyNode(const char* nodeName, Coord originalPosition): myNodeName(nodeName), originalPosition(originalPosition) {};
    virtual ~NJSLegacyNode();

    virtual void record(NJSReportElement reportElement);
    virtual void constructTimelines(simtime_t start, simtime_t end, simtime_t logInterval);
    virtual void computeNJS(const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed);
    virtual bool checkJammedNode(int slot, const char* node, const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed);
    virtual bool isThisAJammedSlot(int slot, const char* node, const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed);

    const char* myNodeName;

    Coord originalPosition;
    Coord latestPosition; // for checking if in range of RSU, not used for logging

    int njsImp = 0;
    int njsExp = 0;
    int reportsProcessed = 0;

    std::list<NJSReportElement> recordings;

    std::vector<NJSState> timeline;
    int timelineLength = -1;
};

#endif /* NJSLEGACYLOGGINGAPP_NJSLEGACYNODE_H_ */
