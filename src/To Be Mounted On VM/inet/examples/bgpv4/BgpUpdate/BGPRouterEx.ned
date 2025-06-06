//
// Copyright (C) 2010 Helene Lageber
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

package inet.examples.bgpv4.BgpUpdate;

import inet.common.MessageDispatcher;
import inet.common.misc.MessageChecker;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.contract.IPppInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.Ipv4NetworkLayer;
import inet.routing.bgpv4.Bgp;
import inet.routing.ospfv2.Ospfv2;
import inet.transportlayer.contract.ITcp;


//
// Example IP router with BGPv4 and OSPFv4 support.
//
module BGPRouterEx
{
    parameters:
        @networkNode();
        @labels(node,ethernet-node);
        @display("i=abstract/router");
        bool forwarding = true;
        bool multicastForwarding = default(false);
        *.forwarding = this.forwarding;
        *.multicastForwarding = this.multicastForwarding;
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        *.routingTableModule = default(absPath(".ipv4.routingTable"));
    gates:
        inout pppg[] @labels(PppFrame-conn);
        inout ethg[] @labels(EtherFrame-conn);
    submodules:
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=100,100;i=block/table_s");
        }
        ospf: Ospfv2 {
            parameters:
                @display("p=400,400,row;i=block/network2");
        }
        bgp: Bgp {
            parameters:
                ospfRoutingModule = "^.ospf";
                @display("p=325,80;i=block/network");
        }
        snifferIn: MessageChecker {
            parameters:
                @display("i=block/uparrow_s;p=250,160");
        }
        snifferOut: MessageChecker {
            parameters:
                @display("i=block/downarrow_s;p=400,160");
        }
        tcp: <default(firstAvailable("Tcp","TcpLwip","TcpNsc"))> like ITcp {
            parameters:
                @display("p=325,240;i=block/transport");
        }
        tn: MessageDispatcher {
            parameters:
                @display("b=400,5,,,,1;p=325,320");
        }
        ipv4: Ipv4NetworkLayer {
            parameters:
                routingTable.routerId = "auto";
                @display("p=250,400;i=block/layer;q=queue");
        }
        nl: MessageDispatcher {
            parameters:
                @display("b=400,5,,,,1;p=325,480");
        }
        ppp[sizeof(pppg)]: <default("PppInterface")> like IPppInterface {
            parameters:
                @display("p=250,560,row,110;q=l2queue;i=block/ifcard");
        }
        eth[sizeof(ethg)]: <default("EthernetInterface")> like IEthernetInterface {
            parameters:
                @display("p=400,560,row,110;q=l2queue;i=block/ifcard");
        }
    connections allowunconnected:
        bgp.socketOut --> { @display("m=s"); } --> snifferOut.in;
        bgp.socketIn <-- { @display("m=s"); } <-- snifferIn.out;

        snifferOut.out --> { @display("m=s"); } --> tcp.appIn;
        snifferIn.in <-- { @display("m=s"); } <-- tcp.appOut;

        tcp.ipOut --> { @display("m=s"); } --> tn.in++;
        tcp.ipIn <-- { @display("m=s"); } <-- tn.out++;
        ospf.ipOut --> { @display("m=s"); } --> tn.in++;
        ospf.ipIn <-- { @display("m=s"); } <-- tn.out++;

        tn.out++ --> { @display("m=s"); } --> ipv4.transportIn;
        tn.in++ <-- { @display("m=s"); } <-- ipv4.transportOut;

        ipv4.ifOut --> { @display("m=s"); } --> nl.in++;
        ipv4.ifIn <-- { @display("m=s"); } <-- nl.out++;

        // connections to network outside
        for i=0..sizeof(pppg)-1 {
            pppg[i] <--> ppp[i].phys;
            ppp[i].upperLayerOut --> nl.in++;
            ppp[i].upperLayerIn <-- nl.out++;
        }

        for i=0..sizeof(ethg)-1 {
            ethg[i] <--> eth[i].phys;
            eth[i].upperLayerOut --> nl.in++;
            eth[i].upperLayerIn <-- nl.out++;
        }
}

