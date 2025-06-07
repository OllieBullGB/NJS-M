package inetwirelesstutorial.NJSLegacyLoggingApp;

import inet.applications.pingapp.PingApp;

simple NJSLegacyLoggingApp extends PingApp
{
    double processReportsInterval = 2.0;
    double processReportsDelay = 2.0;
    bool cleanAfterEachCalculation = default(true);
    
    @class(NJSLegacyLoggingApp);
    @display("i=block/app");
}
