#include "NJSWirelessHost.h"

Define_Module(NJSWirelessHost);

NJSWirelessHost::NJSWirelessHost()
{
    stateCheckTimer = nullptr;
}

NJSWirelessHost::~NJSWirelessHost()
{
    cancelAndDelete(stateCheckTimer);
}

void NJSWirelessHost::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == INITSTAGE_LOCAL)
    {
        stateCheckTimer = new cMessage("stateCheckTimer");
        scheduleAt(simTime() + logInterval, stateCheckTimer);

        listSubmodules(this);
    }
}

void NJSWirelessHost::handleMessage(cMessage *msg)
{
    if (msg->getKind() == 65)
    {
        EV << "Dropping Unknown Protocol Message" << endl;
        delete msg;
        return;
    }

    EV << msg->getFullName() << endl;
    bubble(msg->getFullName());

    if (msg == stateCheckTimer)
    {
        logState();
        scheduleAt(simTime() + logInterval, stateCheckTimer);
    }
    else
    {
        cSimpleModule::handleMessage(msg);
    }
}

void NJSWirelessHost::logState()
{
    cModule *wlanModule = this->getSubmodule("wlan", 0)->getSubmodule("radio");
    if (!wlanModule)
    {
        EV << "Error: wlan[0] mac not found" << endl;
        return;
    }

    //listSubmodules(wlanModule);

    //EV << "Module: " << wlanModule->getFullPath() << ". Type: " << wlanModule->getNedTypeName() << endl;

    inet::physicallayer::IRadio *radio = dynamic_cast<inet::physicallayer::IRadio*>(wlanModule);
    if (!radio)
    {
        EV << "Failed to cast to IRadio" << endl;
        return;
    }

    auto radioMode = radio->getRadioMode();
    EV << "Radio State of wlan[0]: ";
    switch(radioMode)
    {
        case inet::physicallayer::IRadio::RADIO_MODE_OFF:
            EV << "OFF" << endl;
            bubble("OFF");
            return;
        case inet::physicallayer::IRadio::RADIO_MODE_SLEEP:
            EV << "SLEEP" << endl;
            bubble("SLEEP");
            return;
        case inet::physicallayer::IRadio::RADIO_MODE_RECEIVER:
            //EV << "RECEIVE" << endl;
            bubble("RECEIVE");
            return;
        case inet::physicallayer::IRadio::RADIO_MODE_TRANSMITTER:
            EV << "TRANSMIT" << endl;
            bubble("TRANSMIT");
            return;
        default:
            EV << "UNKNOWN" << endl;
            return;
    }

    //auto macState = wlanModule->par("radioMode").stringValue();
    //EV << "Host State: " << macState << endl;
}

void NJSWirelessHost::sendMessage()
{
    return;
}

void NJSWirelessHost::finish()
{
    EV << "NJSWirelessHost finished execution." << endl;
}

void NJSWirelessHost::listSubmodules(cModule *module)
{
    EV << "Listing Submodules of: ";
    if(!module)
    {
        EV << "Nothing" << endl;
    }

    EV << module->getName() << endl;

    for(cModule::SubmoduleIterator it(module); !it.end(); ++it)
    {
        cModule *submodule = *it;
        if (submodule)
        {
            EV << "Submodule: " << submodule->getFullPath() << ". Type: " << submodule->getNedTypeName() << endl;
        }
    }
}

