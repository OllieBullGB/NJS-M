package inetwirelesstutorial.SNRPingApp;

import inet.applications.pingapp.PingApp;

simple SNRPingApp extends PingApp
{
    double rsuRadioTransmissionPower = default(5);
    double vehicleRadioTransmissionPower = default(5);
    double jammerRadioTransmissionPower = default(1.5);
    @class(SNRPingApp);
    @display("i=block/app");
}
