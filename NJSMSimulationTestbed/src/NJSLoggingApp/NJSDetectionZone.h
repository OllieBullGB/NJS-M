#ifndef NJSLOGGINGAPP_NJSDETECTIONZONE_H_
#define NJSLOGGINGAPP_NJSDETECTIONZONE_H_

#include <cmath>
#include <omnetpp.h>
#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"

#include "../NJSState/NJSState.h"
#include "../NJSReport/NJSReportElement.h"

class NJSDetectionZone {
    public:
        NJSDetectionZone(Coord centerPoint, Coord vertex1, Coord vertex2, Coord vertex3, Coord vertex4): centerPoint(centerPoint), vertex1(vertex1), vertex2(vertex2), vertex3(vertex3), vertex4(vertex4) {};
        NJSDetectionZone(Coord centerPoint, double d)
                : centerPoint(centerPoint), // Initialize member variable
                  vertex1(centerPoint.x - d / (2), centerPoint.y + d / (2)),
                  vertex2(centerPoint.x + d / (2), centerPoint.y + d / (2)),
                  vertex3(centerPoint.x - d / (2), centerPoint.y - d / (2)),
                  vertex4(centerPoint.x + d / (2), centerPoint.y - d / (2)) {};


        virtual ~NJSDetectionZone();

        virtual void record(std::vector<NJSReportElement> report);
        virtual void record(NJSReportElement reportElement);
        virtual bool includes(const Coord location);
        virtual bool includesDz(const NJSDetectionZone dz);
        virtual bool isInBoundary(const Coord& location);
        virtual cRectangleFigure* visualize();

        virtual void constructTimelines(simtime_t start, simtime_t end, simtime_t logInterval);
        virtual void destroyTimelines();
        virtual void computeNJS(const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed);
        virtual bool checkJammedNode(int slot, const char* node, const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU, bool debugIsActuallyBeingJammed);
        virtual bool isThisAJammedSlot(int slot, const char* node, const char* rsuNode, std::vector<NJSState> rsuTimeline, double distanceToRSU, double coverageRadiusRSU);

        virtual int getNodeTimelinePosition(const char* node);

        Coord centerPoint;
        Coord vertex1;
        Coord vertex2;
        Coord vertex3;
        Coord vertex4;

        std::vector<std::vector<NJSState>> timelines;
        int njsExp = 0;
        int njsImp = 0;
        int reportsProcessed = 0;

        std::list<NJSReportElement> recordings;
        std::set<std::string> uniqueNodeNames;
        int timelineLength = 0;

    protected:
        /*
         *  v1 ------- v2
         *   |         |
         *   |         |
         *   |         |
         *  v3 ------- v4
         */

        // IDLE = 0
        // RECEIVING = 1
        // TRANSMITTING = 2
        // CORRUPTED = 3





        // Timeline_{i}
        // Timeline_{rsu}




};

#endif /* NJSLOGGINGAPP_NJSDETECTIONZONE_H_ */
