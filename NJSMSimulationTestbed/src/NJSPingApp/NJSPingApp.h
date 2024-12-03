#ifndef NJSPINGAPP_NJSPINGAPP_H_
#define NJSPINGAPP_NJSPINGAPP_H_

#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/applications/pingapp/PingApp.h"

using namespace inet;

class NJSPingApp : public PingApp
{
    protected:
        IMobility *mobility = nullptr;
        physicallayer::IRadio *radio = nullptr;
        cMessage *stateCheckTimer = nullptr;

        simtime_t logInterval = 0.007; // tau default 7ms @ 2Mbps & 1518B data frame
        int logsTillTransmission = 200; // m default 200
        int logsMade = 0; // m_remaining

        Coord NJSCoords[200];
        int NJSState[200];

        virtual void handleMessageWhenUp(cMessage *msg) override;
        virtual void initialize(int stage) override;
        virtual void logState();

        virtual Coord getCurrentPosition();
        virtual int getCurrentNJSState();

        virtual void listSubmodules(cModule *module);

    public:
        NJSPingApp();
        virtual ~NJSPingApp();
};



#endif /* NJSPINGAPP_NJSPINGAPP_H_ */
