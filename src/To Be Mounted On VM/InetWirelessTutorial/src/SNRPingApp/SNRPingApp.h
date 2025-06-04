#ifndef SNRPINGAPP_SNRPINGAPP_H_
#define SNRPINGAPP_SNRPINGAPP_H_

#include <omnetpp.h>
#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/physicallayer/contract/packetlevel/IRadioMedium.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/applications/pingapp/PingApp.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Mac.h"
#include "../veins_inet/VeinsInetMobility.h"

#include "./SNRReportElement.h"
#include "../SNRLoggingApp/SNRLoggingApp.h"

using namespace inet;

class SNRPingApp : public PingApp
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

        double rsuRadioTransmissionPower;
        double vehicleRadioTransmissionPower;
        double jammerRadioTransmissionPower;

        std::vector<SNRReportElement> reportElements;

        virtual void handleMessageWhenUp(cMessage *msg) override;
        virtual void initialize(int stage) override;

        virtual void logState();
        virtual void sendToRSU(std::vector<SNRReportElement> report, const char* rsuName);
        virtual int getCurrentTransmissionState();
        virtual double getTransmissionStrength(double initialTransmissionPower, double distance, double frequency, double systemLoss, double alpha, double propagationSpeed);

        virtual Coord getCurrentPosition();
        virtual void listSubmodules(cModule *module);
        virtual double calculateDistanceTo(const char* nodeName);

    public:
        SNRPingApp();
        virtual ~SNRPingApp();
};



#endif /* SNRPINGAPP_SNRPINGAPP_H_ */
