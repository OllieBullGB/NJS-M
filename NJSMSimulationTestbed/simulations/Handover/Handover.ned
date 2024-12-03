package inetwirelesstutorial.simulations.Handover;

import inet.node.inet.WirelessHost;
import inet.node.inet.StandardHost;
import inet.node.wireless.AccessPoint;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

import inetwirelesstutorial.NJSWirelessHost.NJSWirelessHost;

network Handover
{
    parameters:
        @display("bgb=641,528");

    submodules:
        visualizer: IntegratedCanvasVisualizer {
            parameters:
                @display("p=100,200");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100, 150");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=100,100");
        }
        hostA: WirelessHost {
            parameters:
                @display("p=50,280;r=,,#707070");
        }
        //hostB: NJSWirelessHost {
        //    parameters:
        //        @display("p=453,224;r=,,#707070");
        //}
        ap1: AccessPoint {
            parameters:
                @display("p=100,350;r=,,#707070");
        }
        ap2: AccessPoint {
            parameters:
                @display("p=500,350;r=,,#707070");
        }
        centralProcessor: StandardHost {
            parameters:
                @display("p=294,438;r=,,#707070");
        }

    connections allowunconnected:
        ap1.ethg++ <--> Eth100M <--> centralProcessor.ethg++;
        ap2.ethg++ <--> Eth100M <--> centralProcessor.ethg++;
}
