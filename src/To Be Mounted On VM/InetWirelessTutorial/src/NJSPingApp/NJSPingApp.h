#ifndef NJSPINGAPP_NJSPINGAPP_H_
#define NJSPINGAPP_NJSPINGAPP_H_

#include <omnetpp.h>
#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/applications/pingapp/PingApp.h"
#include "../veins_inet/VeinsInetMobility.h"

#include "../NJSState/NJSState.h"
#include "../NJSJammerApp/NJSJammerApp.h"
#include "../NJSReport/NJSReportElement.h"
#include "../NJSLoggingApp/NJSLoggingApp.h"
#include "../NJSLegacyLoggingApp/NJSLegacyLoggingApp.h"

using namespace inet;

class NJSPingApp : public PingApp
{
    protected:
        IMobility *mobility = nullptr;
        veins::VeinsInetMobility *veinsMobility = nullptr;
        physicallayer::IRadio *radio = nullptr;
        inet::ieee80211::Ieee80211Mac *mac = nullptr;
        cMessage *stateCheckTimer = nullptr;

        simtime_t logInterval = 0.007; // tau default 7ms @ 2Mbps & 1518B data frame
        int logsTillTransmission = 200; // m default 200
        int logsMade = 0; // m_remaining
        const char* connectedRSU;
        const char* jammerName;

        std::vector<NJSReportElement> reportElements;

        virtual void handleMessageWhenUp(cMessage *msg) override;
        virtual void initialize(int stage) override;
        virtual void logState();

        virtual Coord getCurrentPosition();
        virtual int getCurrentNJSState();

        virtual void sendToRSU(std::vector<NJSReportElement> report, const char* rsuName);

        virtual void listSubmodules(cModule *module);

        virtual double calculateDistanceTo(const char* nodeName);
        virtual int isJammedBy(const char* jammerName);

    public:
        NJSPingApp();
        virtual ~NJSPingApp();
};



#endif /* NJSPINGAPP_NJSPINGAPP_H_ */
