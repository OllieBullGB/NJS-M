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

#ifndef SNRLOGGINGAPP_SNRNODE_H_
#define SNRLOGGINGAPP_SNRNODE_H_

#include <cmath>
#include <omnetpp.h>
#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"

#include "../NJSState/NJSState.h"
#include "../SNRPingApp/SNRReportElement.h"


class SNRNode {
public:
    SNRNode(const char* nodeName, Coord position): myNodeName(nodeName), position(position) {};
    virtual ~SNRNode();

    virtual void record(SNRReportElement reportElement);
    virtual void constructTimelines(simtime_t start, simtime_t end, simtime_t logInterval);
    virtual void computeSNR(double thresholdMetricValue);
    const char* myNodeName;

    Coord position;

    std::list<Coord> jammedPoints;
    int reportsProcessed = 0;

    std::list<SNRReportElement> recordings;

    std::vector<SNRReportElement> timeline;
    int timelineLength = -1;


    /* SNR CONFIG SETTINGS */
    bool useFixedApproach = true; // Set True for T.Justice approach, False for Moving Average.
    double x = 2.589; // static "x" value used in T.Justice's approach.
};

#endif /* SNRLOGGINGAPP_SNRNODE_H_ */
