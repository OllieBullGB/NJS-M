#ifndef NJSLOGGINGAPP_NJSLOGGINGAPP_H_
#define NJSLOGGINGAPP_NJSLOGGINGAPP_H_

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

#include "../NJSState/NJSState.h"
#include "../NJSReport/NJSReportElement.h"
#include "./NJSDetectionZone.h"
#include "../NJSJammerApp/NJSJammerApp.h"

using namespace inet;

class NJSLoggingApp : public PingApp
{
    public:
        NJSLoggingApp();
        virtual ~NJSLoggingApp();

        virtual void handleMessageWhenUp(cMessage *msg) override;
        virtual void initialize(int stage) override;

        virtual void addReport(std::vector<NJSReportElement> report);
        virtual void claimDetectionZoneCoverage(Coord point);
        virtual double calculateDistanceToCoord(Coord position);
        virtual bool claimDetectionZone(NJSDetectionZone dz);
        virtual NJSDetectionZone* relevantDetectionZone(NJSReportElement reportElement);
        virtual void visualizeDetectionZones();
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

        std::vector<NJSReportElement> unprocessedReports;
        std::vector<NJSDetectionZone> detectionZones;
        std::vector<cRectangleFigure> visualizations;
};

#endif /* NJSLOGGINGAPP_NJSLOGGINGAPP_H_ */
