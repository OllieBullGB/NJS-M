//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

package inet.examples.inet.udpclientserver;

import inet.common.misc.NetAnimTrace;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;

network UdpClientServer
{
    parameters:
        double per = default(0);
    types:
        channel C extends DatarateChannel
        {
            datarate = 10Mbps;
            delay = 0.1us;
            per = parent.per;
        }
    submodules:
        client1: StandardHost {
            parameters:
                @display("p=250,100;i=device/pc3");
        }
        server: StandardHost {
            parameters:
                @display("p=400,100;i=device/pc2");
        }
        unavserver: StandardHost {
            parameters:
                @display("p=250,200;i=device/pc2");
        }
        unavserver2: StandardHost {
            parameters:
                @display("p=400,200;i=device/pc2");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
        }
//        configurator: NextHopNetworkConfigurator {
//            parameters:
//                @display("p=53,134");
//        }
        netAnimTrace: NetAnimTrace {
            @display("p=100,200;is=s");
        }
    connections allowunconnected:
        client1.pppg++ <--> C <--> server.pppg++;
        unavserver.pppg++ <--> C <--> unavserver2.pppg++;
}

