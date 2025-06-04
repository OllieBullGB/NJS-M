package simulations.NJS_RSU_Veins_Jammer;

import org.car2x.veins.nodes.RSU;
import org.car2x.veins.nodes.Car;
import org.car2x.veins.modules.obstacle.ObstacleControl;
import org.car2x.veins.modules.world.annotations.AnnotationManager;
import org.car2x.veins.base.connectionManager.ConnectionManager;
import org.car2x.veins.base.modules.BaseWorldUtility;
import org.car2x.veins.modules.mobility.traci.TraCIScenarioManagerLaunchd;
import org.car2x.veins.visualizer.roads.RoadsCanvasVisualizer;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.node.inet.AdhocHost;


network NJS_RSU_Veins_Jammer
{
    parameters:
        int numJammers = default(1);
        int numHosts = default(20);
        double playgroundSizeX @unit(m);
        double playgroundSizeY @unit(m);
        double playgroundSizeZ @unit(m);
        **.mgmt.numChannels = 2;
        @display("bgb=$playgroundSizeX,$playgroundSizeY");
    submodules:
        // Additional modules from NJS_RSU_Veins_Jammer
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer{
            parameters:
                @display("p=100,300;is=s");
        }
        
        // Submodules from the original Veins Scenario
        obstacles: ObstacleControl {
            @display("p=240,50");
        }
        annotations: AnnotationManager {
            @display("p=260,50");
        }
        connectionManager: ConnectionManager {
            parameters:
                @display("p=150,0;i=abstract/multicast");
        }
        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = veins_eval_by_version(veins_omnetpp_buildnum(), "playgroundSizeX", 1525, "parent.playgroundSizeX");
                playgroundSizeY = veins_eval_by_version(veins_omnetpp_buildnum(), "playgroundSizeY", 1525, "parent.playgroundSizeY");
                playgroundSizeZ = veins_eval_by_version(veins_omnetpp_buildnum(), "playgroundSizeZ", 1525, "parent.playgroundSizeZ");
                @display("p=30,0;i=misc/globe");
        }
        manager: TraCIScenarioManagerLaunchd {
            parameters:
                @display("p=512,128");
        }
        roadsCanvasVisualizer: RoadsCanvasVisualizer {
            @display("p=300,0");
        }
        
        node[0]: Car {
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
                config = xml("<config><interface hosts='*' address='145.236.x.x' netmask='255.255.0.0'/></config>");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=100,200;is=s");
        }
        // Duplicate RSU as an AdhocHost from INET (from NJS_ap)
        RSU: AdhocHost {
            @display("i=device/antennatower;p=1600,1500");
            wlan[0].mac.typename = "Ieee80211Mac";
        }
        // Jammer module implementing the jamming signal procedure
        jammer: AdhocHost {
            @display("i=misc/sensor2;p=1800,1725");
            wlan[0].mac.typename = "Ieee80211Mac";
        }
    connections allowunconnected:
}
