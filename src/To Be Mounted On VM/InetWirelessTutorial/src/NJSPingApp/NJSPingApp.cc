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
        connectedRSU = "RSU";
        jammerName = "jammer";


        mobility = dynamic_cast<IMobility*>(getParentModule()->getSubmodule("mobility"));

        if (mobility == nullptr)
        {
            EV_ERROR << "No Mobility Module on Parent!" << endl;
        }
        else
        {
            EV_INFO << "Mobility Module Found on Parent!" << endl;
        }

        veinsMobility = dynamic_cast<veins::VeinsInetMobility*>(getParentModule()->getSubmodule("mobility"));
        if (veinsMobility == nullptr)
        {
            EV_WARN << "No veinsMobility Module on Parent!" << endl;
        }
        else
        {
            EV_INFO << "veinsMobility Module Found on Parent!" << endl;
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

        mac = dynamic_cast<inet::ieee80211::Ieee80211Mac*>(getParentModule()->getSubmodule("wlan", 0)->getSubmodule("mac"));

        if (mac == nullptr)
        {
            EV_ERROR << "No MAC Module on Parent!" << endl;
        }
        else
        {
            EV_INFO << "MAC Module Found on Parent!" << endl;
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
    else if (msg->getKind() == 65)
    {
        EV << "Dropping Unknown Protocol Message" << endl;
        delete msg;
        return;
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
    int njsState = getCurrentNJSState();

    const char* myName = getParentModule()->getFullName();
    NJSReportElement currentLog(position, njsState, simTime(), myName);
    reportElements.push_back(currentLog);

    EV_INFO << currentLog.toString() << endl;

    logsMade++;

    if (logsMade >= logsTillTransmission)
    {
        sendToRSU(reportElements, connectedRSU);
        reportElements.clear();
        logsMade = 0;
    }
}

Coord NJSPingApp::getCurrentPosition()
{
    if (mobility != nullptr)
    {
        // Using TraCIMobility to get the current position
        if (veinsMobility != nullptr)
        {
            EV_INFO << "USED TRACIMOBILITY" << endl;
            return veinsMobility->getCurrentPosition();
        }
        else
        {
            EV_INFO << "USED INETMOBILITY" << endl;
            return mobility->getCurrentPosition();
        }
    }
    return Coord::ZERO;
}


int NJSPingApp::getCurrentNJSState()
{
    if (radio != nullptr)
    {
        // are we jammed
        if (isJammedBy("jammer") == 1)
        {
            radio->setRadioMode(physicallayer::IRadio::RADIO_MODE_SLEEP); // If jammed disable radio
            return 3;
        }

        if (radio->getRadioMode() == physicallayer::IRadio::RADIO_MODE_SLEEP)
        {
            radio->setRadioMode(physicallayer::IRadio::RADIO_MODE_RECEIVER);
        }

        physicallayer::IRadio::RadioMode radioMode = radio->getRadioMode();
        EV_INFO << "NJS Radio State:" << physicallayer::IRadio::getRadioModeName(radioMode) << endl;

        physicallayer::IRadio::ReceptionState receptionState = radio->getReceptionState();
        physicallayer::IRadio::TransmissionState transmissionState = radio->getTransmissionState();

        switch(radioMode)
        {
            case physicallayer::IRadio::RADIO_MODE_RECEIVER:
                if (receptionState == physicallayer::IRadio::RECEPTION_STATE_RECEIVING && !mac->macIgnoreNonRSU) return 1;
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

// This is an "instant transmission" method as I really don't want to handle sending a non-fixed length vector in a packet
void NJSPingApp::sendToRSU(std::vector<NJSReportElement> report, const char* rsuName)
{
    // locate the NJSLoggingApp on the object with name rsuName
    cModule* rsuModule = getSimulation()->getModuleByPath(rsuName);
    NJSLoggingApp* loggingApp = check_and_cast<NJSLoggingApp*>(rsuModule->getSubmodule("app", 1));
    NJSLegacyLoggingApp* legacyLoggingApp = check_and_cast<NJSLegacyLoggingApp*>(rsuModule->getSubmodule("app", 2));

    // push the contents of the report onto the back of the NJSReportingApp's unprocessed reports
    loggingApp->addReport(report);
    legacyLoggingApp->addReport(report);
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

double NJSPingApp::calculateDistanceTo(const char* nodeName)
{
    // Get the position of the current host
    Coord myPosition = mobility->getCurrentPosition();

    if (veinsMobility)
    {
        myPosition = veinsMobility->getCurrentPosition();
    }

    // Find the other host by its name
    cModule* otherHost = getSimulation()->getModuleByPath(nodeName);
    if (!otherHost)
    {
        EV << "Host " << jammerName << " not found!" << endl;
        return -1;  // Return a negative value to indicate an error
    }

    IMobility* otherMobility = check_and_cast<IMobility*>(otherHost->getSubmodule("mobility"));
    Coord otherPosition = otherMobility->getCurrentPosition();

    // Calculate the Euclidean distance between the two positions
    double distance = myPosition.distance(otherPosition);

    return distance;
}

int NJSPingApp::isJammedBy(const char* jammerName)
{
    EV_INFO << "Testing to see if jammed" << endl;
    cModule* otherHost = getSimulation()->getModuleByPath(jammerName);
    EV_INFO << "otherHost may exist" << endl;
    if (!otherHost)
    {
        EV_ERROR << "Host " << jammerName << " not found!" << endl;
        return -1;  // Return a negative value to indicate an error
    }

    double distance = calculateDistanceTo(jammerName);
    if (distance == -1)
    {
        EV_ERROR << "Host " << jammerName << " has no valid distance to me!" << endl;
        return -1;  // Return a negative value to indicate an error
    }

    NJSJammerApp* jammerApp = check_and_cast<NJSJammerApp*>(otherHost->getSubmodule("app", 0));

    // Check if we are in range of the jammer
    if (distance > jammerApp->jammingRadius) return 0;
    EV_INFO << "Host in Range of Jammer (" << distance << " <= " << jammerApp->jammingRadius << ")" << endl;

    // Check if the jammer is currently broadcasting a jamming signal
    if (!jammerApp->transmittingJammingSignal) return 0;
    EV_INFO << "Jammer in range is transmitting! We are jammed!" << endl;

    return 1;
}

