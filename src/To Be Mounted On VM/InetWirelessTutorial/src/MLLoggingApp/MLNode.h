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

#ifndef MLLOGGINGAPP_MLNODE_H
#define MLLOGGINGAPP_MLNODE_H

#include <cmath>
#include <fstream>
#include <iomanip>
#include <array>
#include <omnetpp.h>
#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"

#include "../NJSState/NJSState.h"
#include "../SNRPingApp/SNRReportElement.h"
#include "../NJSJammerApp/NJSJammerApp.h"
#include "./SNRMLModel.h"


class MLNode {
public:
    MLNode(const char* nodeName, Coord position): myNodeName(nodeName), position(position), model(SNRMLModel::instance()) {openLogFile();};
    virtual ~MLNode();

    virtual void record(SNRReportElement reportElement);
    virtual void constructTimelines(simtime_t start, simtime_t end, simtime_t logInterval);

    virtual void runMLModel();
    SNRMLModel& model;

    virtual void logJammingState(double pSignal, double pNoise, const std::string& filename, Coord myPosition);
    int logTrainingData = 1;

    virtual double calculateDistanceTo(const char* nodeName, Coord myPosition);
    virtual int isJammedBy(const char* jammerName, Coord myPosition);

    const char* myNodeName;

    Coord position;

    std::list<Coord> jammedPoints;
    int reportsProcessed = 0;

    std::list<SNRReportElement> recordings;

    std::vector<SNRReportElement> timeline;
    int timelineLength = -1;


    double modelThreshold = 0.5;

    // Logger
    static std::ofstream snrLogFile;
    static bool snrLogFileInitialized;

    virtual void logSNR(int slot, double snr, double rsu, double jam, bool valid);
    static void openLogFile();
    static void closeLogFile();
};

#endif /* MLLOGGINGAPP_MLNODE_H */
