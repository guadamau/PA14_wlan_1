//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package endDevices;


import cpu.CPU;
import duplicateDetection.ListenErkennung;
import nodeTable.nodeTable;
import mac.NetworkInterfaceCard;
import util.PropagationDelay;



module HsrNode
{
    parameters:
        string macAddress;
        int ringID = default( 1 ); //ring number
        int outputQueueCapacityPerPort = default( 100 );
        int duplicateDetectionListSize = default( 1000 );

        @display("bgb=807,562");
    gates:

        inout gateA;  // declare two way connections
        inout gateAExp;  // declare two way connections
        inout gateB;  // declare two way connections
        inout gateBExp;  // declare two way connections

    submodules:

        propagationDelayA: PropagationDelay {
            delay = 6us;
            @display("p=130,124");
        }

        propagationDelayAExp: PropagationDelay {
            delay = 6us;
            @display("p=123,203");
        }

        propagationDelayB: PropagationDelay {
            delay = 6us;
            @display("p=282,124");
        }

        propagationDelayBExp: PropagationDelay {
            delay = 6us;
            @display("p=534,111");
        }

        propagationDelayCpu: PropagationDelay {
            delay = 6us;
            @display("p=372,471");
        }

        propagationDelayCpuExp: PropagationDelay {
            delay = 6us;
            @display("p=442,416");
        }

        duplikatlistePortCPU: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=468,335");
        }
        duplikatlistePortCPUExp: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=560,269");
        }

        cpu: CPU {
            @display("p=621,498");
            macAddress = macAddress;
        }

        duplikatlistePortEth0: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=170,60");
        }
        duplikatlistePortEth0Exp: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=82,281");
        }

        eth0: NetworkInterfaceCard {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=43,60");
        }

        eth0Exp: NetworkInterfaceCard {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=43,160");
        }

        duplikatlistePortEth1: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=429,60");
        }
        duplikatlistePortEth1Exp: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=630,160");
        }

        eth1: NetworkInterfaceCard {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=302,60");
        }

        eth1Exp: NetworkInterfaceCard {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=637,60");
        }

        nodeTable: nodeTable {
            @display("p=719,477");
        }

    connections:

}
