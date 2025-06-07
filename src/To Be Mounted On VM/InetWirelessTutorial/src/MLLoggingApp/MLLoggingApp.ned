package inetwirelesstutorial.MLLoggingApp;

import inet.applications.pingapp.PingApp;

simple MLLoggingApp extends PingApp
{
    double processReportsInterval = 2.0;
    double processReportsDelay = 2.0;
    bool cleanAfterEachCalculation = default(true);
    @class(MLLoggingApp);
    @display("i=block/app");
}
