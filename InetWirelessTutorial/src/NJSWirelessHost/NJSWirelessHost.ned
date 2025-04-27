package inetwirelesstutorial.NJSWirelessHost;

import inet.node.inet.WirelessHost;

module NJSWirelessHost extends WirelessHost
{
	parameters:   
	    @class(NJSWirelessHost);
	    double interval = default(1);
        int destinationId;
}
