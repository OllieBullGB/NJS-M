#ifndef NJSLEGACYLOGGINGAPP_NJSLEGACYLOGGINGAPP_H_
#define NJSLEGACYLOGGINGAPP_NJSLEGACYLOGGINGAPP_H_

#include <omnetpp.h>
#include <math.h>
#include <stack>
#include <fstream>
#include <iomanip>

#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/physicallayer/contract/packetlevel/IRadioMedium.h"
#include "inet/applications/pingapp/PingApp.h"

#include "../SNRPingApp/SNRReportElement.h"
#include "./SNRNode.h"
#include "../NJSJammerApp/NJSJammerApp.h"

using namespace inet;

class SNRLoggingApp : public PingApp
{
    public:
        SNRLoggingApp();
        virtual ~SNRLoggingApp();

        virtual void handleMessageWhenUp(cMessage *msg) override;
        virtual void initialize(int stage) override;

        virtual void addReport(std::vector<SNRReportElement> report);
        virtual SNRNode* relevantNode(SNRReportElement reportElement);

        virtual double calculateDistanceToCoord(Coord position);
        virtual Coord localiseJammer();
        virtual Coord calculateAveragePoint(const std::vector<Coord>& coords);

        virtual void recordAndSaveMetricsAtTimestep();
        virtual void saveResultsToFile(double jle, std::array<int, 4> accuracy, double ttfd, const std::string& filename);
        virtual double calculateJammerLocalisationError(Coord& predictedPosition, const char* jammerName);
        virtual std::array<int, 4> calculateJammingAccuracy(const char* jammerName);

    protected:
        physicallayer::IRadio *radio = nullptr;
        IMobility *mobility = nullptr;
        Coord centerPoint;
        double coverageRadius;
        double d = 50;
        double dSqrt2 = 35.355339;

        cMessage* processReportsTimer;
        double processReportsInterval = 1.5;
        double processReportsDelay = 1.5;
        bool cleanAfterEachCalculation;

        unsigned long runName;
        bool hadFirstDetection = false;
        simtime_t timeToFirstDetection;

        std::vector<SNRNode> nodes;
};

#endif /* NJSLEGACYLOGGINGAPP_NJSLEGACYLOGGINGAPP_H_ */
