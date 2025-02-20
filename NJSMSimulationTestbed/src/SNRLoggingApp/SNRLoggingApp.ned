package inetwirelesstutorial.SNRLoggingApp;

import inet.applications.pingapp.PingApp;

simple SNRLoggingApp extends PingApp
{
    double processReportsInterval = 2.0;
    double processReportsDelay = 2.0;
    bool cleanAfterEachCalculation = default(true);
    @class(SNRLoggingApp);
    @display("i=block/app");
}