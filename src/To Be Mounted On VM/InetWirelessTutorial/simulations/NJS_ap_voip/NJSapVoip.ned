//
// Copyright (C) 2005 Andras Varga
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

package simulations.NJS_ap_voip;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.WirelessHost;
import inet.node.inet.AdhocHost;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;

network NJSApVoip
{
    parameters:
        int numHosts;
        **.mgmt.numChannels = 2;
        @display("bgb=648.35626,473.74377");
    submodules:
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            parameters:
                @display("p=100,300;is=s");
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
        RSU: AdhocHost {
            @display("i=device/antennatower;p=242,200");
            wlan[0].mac.typename = "Ieee80211Mac";
        }
        node[numHosts]: AdhocHost {
            @display("i=misc/car2");
            wlan[0].mac.typename = "Ieee80211Mac";
        }
        jammer: AdhocHost {
            @display("i=misc/sensor2;p=465.15625,343.5");
            wlan[0].mac.typename = "Ieee80211Mac";
        }
        otherHost: AdhocHost {
            @display("i=device/antennatower;p=293,200");
            wlan[0].mac.typename = "Ieee80211Mac";
        }
}
