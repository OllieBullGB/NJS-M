//
// Copyright (C) 2006-2017 Christoph Sommer <sommer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

package simulations.veins_inet_durham_demo;

//#if INET_VERSION < 0x0403
import inet.physicallayer*.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
//#else
import inet.physicallayer*.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
//#endif
import inetwirelesstutorial.veins_inet.VeinsInetCar;
import inetwirelesstutorial.veins_inet.VeinsInetManager;
//#if INET_VERSION < 0x0403
import inet.visualizer*.integrated.IntegratedVisualizer;
//#else
import inet.visualizer*.common.IntegratedVisualizer;
//#endif
import inet.environment.common.PhysicalEnvironment;
import org.car2x.veins.visualizer.roads.RoadsCanvasVisualizer;
import org.car2x.veins.visualizer.roads.RoadsOsgVisualizer;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.WirelessHost;
import inet.node.inet.AdhocHost;

network Scenario
{
    parameters:
        bool useOsg = default(false);
        @display("bgb=200,200");
    submodules:
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=64,224");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
                config = xml("<config><interface hosts='*' address='145.236.x.x' netmask='255.255.0.0'/></config>");
        }
        manager: VeinsInetManager {
            @display("p=192,320");
        }
        visualizer: IntegratedVisualizer {
            @display("p=64,320");
        }
        physicalEnvironment: PhysicalEnvironment {
            @display("p=192,224");
        }
        roadsCanvasVisualizer: RoadsCanvasVisualizer {
            @display("p=64,416");
        }
        roadsOsgVisualizer: RoadsOsgVisualizer if useOsg {
            @display("p=192,416");
        }
        node[0]: VeinsInetCar;
        // Duplicate RSU as an AdhocHost from INET (from NJS_ap)
        RSU: VeinsInetCar {
            @display("i=device/antennatower;p=1030, 620");
            wlan[0].mac.typename = "Ieee80211Mac";
        }
        // Jammer module implementing the jamming signal procedure
        jammer: VeinsInetCar {
            @display("i=misc/sensor2;p=1190, 1050");
            wlan[0].mac.typename = "Ieee80211Mac";
        }
}
