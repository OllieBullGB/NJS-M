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

#ifndef NJSWIRELESSHOST_NJSWIRELESSHOST_H_
#define NJSWIRELESSHOST_NJSWIRELESSHOST_H_

#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/mobility/contract/IMobility.h"
#include "inet/networklayer/contract/INetworkProtocol.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"

#include "NJSReport_m.h"

using namespace inet;

class NJSWirelessHost : public cSimpleModule {
    protected:
        cMessage *stateCheckTimer = nullptr;
        simtime_t logInterval = 0.005; // tau (default 5ms)

        virtual void initialize(int stage) override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void logState();
        virtual void sendMessage();
        virtual void finish() override;

        virtual void listSubmodules(cModule *module);

    public:
        NJSWirelessHost();
        virtual ~NJSWirelessHost();
};

#endif /* NJSWIRELESSHOST_NJSWIRELESSHOST_H_ */
