package inetwirelesstutorial.NJSJammerApp;

import inet.applications.contract.IApp;

simple NJSJammerApp like IApp
{
    parameters:
        double interval @unit(s) = default(0.005s);
        int destPort = default(1000);
        @display("i=block/app");
        @class(NJSJammerApp);
        
    gates:
        input socketIn;
        output socketOut;
        
    
}
