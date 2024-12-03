#include "NJSPingApp.h"


Define_Module(NJSPingApp);

NJSPingApp::NJSPingApp()
{
    stateCheckTimer = nullptr;
    mobility = nullptr;
}

NJSPingApp::~NJSPingApp()
{
    cancelAndDelete(stateCheckTimer);
}

void NJSPingApp::initialize(int stage)
{
    PingApp::initialize(stage);

    if (stage == INITSTAGE_LOCAL)
    {
        mobility = dynamic_cast<IMobility*>(getParentModule()->getSubmodule("mobility"));

        if (mobility == nullptr)
        {
            EV_ERROR << "No Mobility Module on Parent!" << endl;
        }
        else
        {
            EV_INFO << "Mobility Module Found on Parent!" << endl;
        }

        radio = dynamic_cast<physicallayer::IRadio*>(getParentModule()->getSubmodule("wlan", 0)->getSubmodule("radio"));

        if (radio == nullptr)
        {
            EV_ERROR << "No Radio Module on Parent!" << endl;
        }
        else
        {
            EV_INFO << "Radio Module Found on Parent!" << endl;
        }

        stateCheckTimer = new cMessage("checkStateTimer");
        scheduleAt(simTime() + logInterval, stateCheckTimer);
    }

    bubble("Initialised!");
    EV << "NJS Ping App Successfully Init" << endl;
}

void NJSPingApp::handleMessageWhenUp(cMessage *msg)
{
    EV_INFO << "NJS Enabled Node Received Message: " << msg->getFullName() << endl;

    if (msg == stateCheckTimer)
    {
        logState();
        scheduleAt(simTime() + logInterval, stateCheckTimer);
    }
    else
    {
        PingApp::handleMessageWhenUp(msg);
    }


}

void NJSPingApp::logState()
{
    bubble("Logging State!");
    EV << "------------------LOGGING STATE (" << logsMade << ")------------------" << endl;

    Coord position = getCurrentPosition();
    EV_INFO << "Current Position: " << position.str() << endl;
    NJSCoords[logsMade] = position;

    int njsState = getCurrentNJSState();
    EV_INFO << "Current NJS State: " << njsState << endl;
    NJSState[logsMade] = njsState;

    logsMade++;

    if (logsMade >= 200)
    {
        EV_INFO << "----------------------- EXTRACTING ACTIVITY LOG -----------------------" << endl;
        // Transmit logState & logCoords to RSU
        // For current sake, just run the outline of the logState
        int numZero = 0;
        int numOne = 0;
        int numTwo = 0;
        int numUnknown = 0;

        for (int i=0; i<200; i++)
        {
            int state = NJSState[i];

            switch(state)
            {
                case 0:
                    numZero++;
                    break;
                case 1:
                    numOne++;
                    break;
                case 2:
                    numTwo++;
                    break;
                default:
                    numUnknown++;
                    break;
            }
        }

        EV_INFO << "------ NJS STATE LOGS" << " ------" << endl;
        EV_INFO << "IDLE: " << numZero << endl;
        EV_INFO << "RECEIVING: " << numOne << endl;
        EV_INFO << "TRANSMITTING: " << numTwo << endl;
        EV_INFO << "UNKNOWN: " << numUnknown << endl;

        logsMade=0;
    }
}

Coord NJSPingApp::getCurrentPosition()
{
    if (mobility != nullptr)
    {
        return mobility->getCurrentPosition();
    }
    return Coord::ZERO;
}

int NJSPingApp::getCurrentNJSState()
{
    if (radio != nullptr)
    {
        physicallayer::IRadio::RadioMode radioMode = radio->getRadioMode();
        EV_INFO << "NJS Radio State:" << physicallayer::IRadio::getRadioModeName(radioMode) << endl;

        physicallayer::IRadio::ReceptionState receptionState = radio->getReceptionState();
        physicallayer::IRadio::TransmissionState transmissionState = radio->getTransmissionState();

        switch(radioMode)
        {
            case physicallayer::IRadio::RADIO_MODE_RECEIVER:
                if (receptionState == physicallayer::IRadio::RECEPTION_STATE_RECEIVING) return 1;
                break;
            case physicallayer::IRadio::RADIO_MODE_TRANSMITTER:
                if (transmissionState == physicallayer::IRadio::TRANSMISSION_STATE_TRANSMITTING) return 2;
                break;
            default:
                break;
        }
        return 0;
    }
    else
    {
        EV_ERROR << "No Radio Module Onboard! Listing Parent Submodules" << endl;
        this->listSubmodules(this->getParentModule()->getSubmodule("wlan", 0));
        return -1;
    }
}

void NJSPingApp::listSubmodules(cModule *module)
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
            EV_INFO << "Submodule: " << submodule->getFullPath() << ". Type: " << submodule->getNedTypeName() << endl;
        }
    }
}

