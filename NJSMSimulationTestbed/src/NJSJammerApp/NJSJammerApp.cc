#include "NJSJammerApp.h"

Define_Module(NJSJammerApp);

NJSJammerApp::NJSJammerApp()
{
    jammingSignalTimer = nullptr;
}

NJSJammerApp::~NJSJammerApp()
{
    cancelAndDelete(jammingSignalTimer);
}

void NJSJammerApp::finish()
{
    cancelAndDelete(jammingSignalTimer);
    socket.close();
}

void NJSJammerApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL)
    {
        jammingInterval = par("jammingInterval");
        simtime_t delay = par("startingDelay");

        jammingSignalTimer = new cMessage("jammingSignalTimer");
        scheduleAt(simTime() + delay, jammingSignalTimer);

        socket.setOutputGate(gate("socketOut"));
        socket.bind(L3Address(), -1);
    }
}

void NJSJammerApp::handleMessageWhenUp(cMessage* msg)
{
    if (msg == jammingSignalTimer)
    {
        sendJammingSignal();
        scheduleAt(simTime() + jammingInterval, jammingSignalTimer);
    }
    else
    {
        delete msg;
    }
}

void NJSJammerApp::sendJammingSignal()
{
    auto packet = new Packet("JammingSignal");
    auto broadcastAddress = L3AddressResolver().resolve("255.255.255.255");

    socket.sendTo(packet, broadcastAddress, par("destPort").intValue());
}
