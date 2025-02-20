package inetwirelesstutorial.NJSJammerApp;

import inet.applications.pingapp.PingApp;

simple NJSJammerApp extends PingApp
{
    parameters:
        double jammingInterval @unit(s) = default(0.001s);
        double startingDelay @unit(s) = default(0.01s);
        double jammingRadius @unit(m) = default(250m);
        bool isProactive = default(true);
        int destPort = default(1000);
        @display("i=block/app");
        @class(NJSJammerApp);
}
