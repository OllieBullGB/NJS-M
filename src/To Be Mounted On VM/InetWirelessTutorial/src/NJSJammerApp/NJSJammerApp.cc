#include "NJSJammerApp.h"

Define_Module(NJSJammerApp);

NJSJammerApp::NJSJammerApp()
{
    jammingSignalTimer = nullptr;
}

NJSJammerApp::~NJSJammerApp()
{
    //cancelAndDelete(jammingSignalTimer);
}

void NJSJammerApp::initialize(int stage)
{
    PingApp::initialize(stage);
    EV_INFO << "Jammer initialising " << endl;

    if (stage == 12)
    {

        transmittingJammingSignal = true;
        jammingInterval = par("jammingInterval");
        delay = par("startingDelay");
        jammingRadius = par("jammingRadius");

        isProactive = par("isProactive");
        useProximity = par("useProximity");

        EV_INFO << "Jammer initialised with radius: " << jammingRadius << ", proactive status: " << isProactive << endl;

        jammingSignalTimer = new cMessage("jammingSignalTimer");
        scheduleAt(simTime() + delay, jammingSignalTimer);
        if (!isProactive)
        {
            EV << "Checking Mac" << endl;
            inet::ieee80211::Ieee80211Mac* mac = check_and_cast<inet::ieee80211::Ieee80211Mac*>(getParentModule()->getSubmodule("wlan", 0)->getSubmodule("mac"));
            transmittingJammingSignal = mac->isReceiving;

        }
        EV_INFO << "Jammer is Transmitting Signal: " << transmittingJammingSignal << endl;

        updateJammingRadiusFigure();

        radio = dynamic_cast<physicallayer::IRadio*>(getParentModule()->getSubmodule("wlan", 0)->getSubmodule("radio"));

        if (radio == nullptr)
        {
            EV_ERROR << "No Radio Module on Parent!" << endl;
        }
        else
        {
            EV_INFO << "Radio Module Found on Parent!" << endl;
        }

    }
}

void NJSJammerApp::handleMessageWhenUp(cMessage* msg)
{
    EV_INFO << "NJS Enabled Node Received Message: " << msg->getFullName() << endl;
    //updateJammingRadiusFigure();


    if (msg == jammingSignalTimer)
    {
        if (!isProactive)
        {
            if (useProximity)
            {
                // check all nodes to see which ones the jammer can hear

                // for nodes that the jammer can hear, check to see which ones are transmitting
            }
            else
            {
                // get MAC layer
                EV << "Checking Mac" << endl;
                inet::ieee80211::Ieee80211Mac* mac = check_and_cast<inet::ieee80211::Ieee80211Mac*>(getParentModule()->getSubmodule("wlan", 0)->getSubmodule("mac"));
                transmittingJammingSignal = mac->isReceiving;

                if (transmittingJammingSignal)
                {
                    bubble("I am Jamming!");
                    EV_INFO << "I am Jamming!" << endl;
                }
                else
                {
                    bubble("I am not Jamming!");
                    EV_INFO << "I am not Jamming!" << endl;
                }


                if (!jammingSignalTimer->isScheduled())
                {
                    jammingSignalTimer = new cMessage("jammingSignalTimer");
                    scheduleAt(simTime() + jammingInterval, jammingSignalTimer);
                }
            }

        }
    }

    delete msg;
    //PingApp::handleMessage(msg);
}

void NJSJammerApp::updateJammingRadiusFigure()
{

    EV_INFO << "Updating Jamming Radius Figure" << endl;
    EV_INFO << "Transmitting Jamming Signal: " << transmittingJammingSignal << endl;

    if (!jammingRadiusFigure) {
        EV_WARN << "Jamming Radius Figure is null" << endl;

        jammingRadiusFigure = new cOvalFigure("circle");
        jammingRadiusFigure->setOutlined(true);
        jammingRadiusFigure->setFilled(true);
        jammingRadiusFigure->setLineWidth(2);
        getCanvas()->addFigure(jammingRadiusFigure);
    }

    // Get the canvas for the module's network
    cCanvas* canvas = getParentModule()->getCanvas();
    if (!canvas)
    {
        EV_WARN << "Could not find canvas" << endl;
        return;
    }

    // Remove existing figure if it exists
    if (jammingRadiusFigure->getParentFigure() == nullptr)
    {
        canvas->addFigure(jammingRadiusFigure);
    }

    // Get module's position
    cModule* hostModule = getParentModule();
    if (!hostModule)
    {
        EV_WARN << "Could not find parent" << endl;
        return;
    }

    // Determine color based on jamming status
    cFigure::Color color = transmittingJammingSignal ? cFigure::RED : cFigure::BLUE;

    jammingRadiusFigure->setLineColor(color);

    // Assume we want to center the circle on the module
    IMobility* mobility = dynamic_cast<IMobility*>(getParentModule()->getSubmodule("mobility"));

    Coord curPos = mobility->getCurrentPosition();
    double x = curPos.x;
    double y = curPos.y;

    // Create oval/circle
    jammingRadiusFigure->setBounds(
        cFigure::Rectangle(
            x - jammingRadius,
            y - jammingRadius,
            2 * jammingRadius,
            2 * jammingRadius
        )
    );

    EV << "Created Figure" << endl;

}
