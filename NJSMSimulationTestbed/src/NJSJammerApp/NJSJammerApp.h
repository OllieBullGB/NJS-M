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

#ifndef NJSJAMMERAPP_NJSJAMMERAPP_H_
#define NJSJAMMERAPP_NJSJAMMERAPP_H_

#include <omnetpp.h>
#include "inet/applications/pingapp/PingApp.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/networklayer/common/L3AddressResolver.h"

using namespace inet;

class NJSJammerApp : public PingApp
{
    protected:
        UdpSocket socket;
        cMessage *jammingSignalTimer = nullptr;
        simtime_t jammingInterval = 0.005;

        virtual void initialize(int stage) override;
        virtual void handleMessageWhenUp(cMessage* msg) override;
        virtual void finish() override;

        void sendJammingSignal();

    public:
        NJSJammerApp();
        virtual ~NJSJammerApp();
};

#endif /* NJSJAMMERAPP_NJSJAMMERAPP_H_ */
