#include "SNRPingApp.h"
#include "../MLLoggingApp/MLLoggingApp.h"


Define_Module(SNRPingApp);

SNRPingApp::SNRPingApp()
{
    stateCheckTimer = nullptr;
    mobility = nullptr;
}

SNRPingApp::~SNRPingApp()
{
    cancelAndDelete(stateCheckTimer);
}

void SNRPingApp::initialize(int stage)
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

        rsuRadioTransmissionPower = par("rsuRadioTransmissionPower");
        vehicleRadioTransmissionPower = par("vehicleRadioTransmissionPower");
        jammerRadioTransmissionPower = par("jammerRadioTransmissionPower");
    }

    bubble("Initialised!");
    EV << "SNR Ping App Successfully Init" << endl;
}

void SNRPingApp::handleMessageWhenUp(cMessage *msg)
{
    EV_INFO << "SNR Enabled Node Received Message: " << msg->getFullName() << endl;

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

void SNRPingApp::logState()
{
    int transmissionState = getCurrentTransmissionState();

    double distanceToRSU = calculateDistanceTo("RSU");
    double distanceToJammer = calculateDistanceTo("jammer");

    std::default_random_engine generator(simTime().raw());
    std::normal_distribution<double> gaussianNoise(0.0, 0.05);  // ±5% noise
    std::lognormal_distribution<double> lognormNoise(0.0, 0.1); // optional shadowing

    double rsuBaseStrength = getTransmissionStrength(rsuRadioTransmissionPower, distanceToRSU, 2400000000, 0.0, 2.0, 299792458);
    double jammerBaseStrength = getTransmissionStrength(jammerRadioTransmissionPower, distanceToJammer, 2400000000, 0.0, 2.0, 299792458);

    // Prevent zero values (flooring)
    rsuBaseStrength = std::max(rsuBaseStrength, 1e-12);
    jammerBaseStrength = std::max(jammerBaseStrength, 1e-12);

    // Multiplicative Gaussian noise (symmetric)
    double rsuStrength = rsuBaseStrength * (1.0 + gaussianNoise(generator));
    double jammerStrength = jammerBaseStrength * (1.0 + gaussianNoise(generator));

    // Optional: multiplicative lognormal shadowing
    rsuStrength *= lognormNoise(generator);
    jammerStrength *= lognormNoise(generator);

    // Add noise floor
    double noiseFloor_mW = pow(10, -90 / 10.0);  // -90 dBm = ~1e-12 mW

    double snr = rsuStrength / (jammerStrength + noiseFloor_mW);

    Coord position = getCurrentPosition();
    const char* myName = getParentModule()->getFullName();

    SNRReportElement reportElement = SNRReportElement(position, rsuStrength, jammerStrength, snr, simTime(), myName);

    reportElements.push_back(reportElement);

    logsMade++;

    if (logsMade >= logsTillTransmission)
    {
        sendToRSU(reportElements, connectedRSU);
        reportElements.clear();
        logsMade = 0;
    }
}

// This is an "instant transmission" method as I really don't want to handle sending a non-fixed length vector in a packet
void SNRPingApp::sendToRSU(std::vector<SNRReportElement> report, const char* rsuName)
{
    // locate the SNRLoggingApp on the object with name rsuName
    cModule* rsuModule = getSimulation()->getModuleByPath(rsuName);
    SNRLoggingApp* loggingApp = check_and_cast<SNRLoggingApp*>(rsuModule->getSubmodule("app", 3));
    MLLoggingApp* ap = check_and_cast<MLLoggingApp*>(rsuModule->getSubmodule("app", 4));

    // push the contents of the report onto the back of the SNRLoggingApp's unprocessed reports
    loggingApp->addReport(report);
    ap->addReport(report);
}

double SNRPingApp::getTransmissionStrength(double initialTransmissionPower, double distance, double frequency, double systemLoss = 1.0, double alpha = 2.0, double propagationSpeed = 299792458)
{
    double waveLength = propagationSpeed / frequency;
    double pathLossFactor = distance == 0.0 ? 1.0 : (waveLength * waveLength) / (16 * M_PI * M_PI * pow(distance, alpha)); // From FreeSpacePathLoss
    return initialTransmissionPower * pathLossFactor;
}

int SNRPingApp::getCurrentTransmissionState()
{
    if (radio != nullptr)
    {
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

Coord SNRPingApp::getCurrentPosition()
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

void SNRPingApp::listSubmodules(cModule *module)
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

double SNRPingApp::calculateDistanceTo(const char* nodeName)
{
    // Get the position of the current host
    Coord myPosition = mobility->getCurrentPosition();

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
