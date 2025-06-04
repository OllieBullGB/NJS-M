package inetwirelesstutorial.NJSLoggingApp;

import inet.applications.pingapp.PingApp;

simple NJSLoggingApp extends PingApp
{
    double processReportsInterval = 2.0;
    double processReportsDelay = 2.0;
    bool cleanAfterEachCalculation = default(false);
    @class(NJSLoggingApp);
    @display("i=block/app");
}
