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

import inet.linklayer.ethernet.EtherMACFullDuplex;

import cpu.CPU;
import duplicateDetection.ListenErkennung;
import nodeTable.nodeTable;
import mac.NetworkInterfaceCard;



module HsrNode
{
    parameters:
        string macAddress;
        int ringID; //ring number
        int outputQueueCapacityPerPort;
        int duplicateDetectionListSize;
        double propagationDelay @unit(s) = default( 6us );
        
    gates:
        
        inout gateA;  // declare two way connections
        inout gateAExp;  // declare two way connections
        inout gateB;  // declare two way connections
        inout gateBExp;  // declare two way connections
        
    submodules:
       
        duplikatlistePortCPU: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=333,288");
        }
        duplikatlistePortCPUExp: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=473,258");
        }

        cpu: CPU {
            @display("p=390,489");
            macAddress = macAddress;
        }
        
        duplikatlistePortEth0: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=143,60");
        }
        duplikatlistePortEth0Exp: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=83,280");
        }
        
        eth0: NetworkInterfaceCard
        {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=43,60");
        }
        
        eth0Exp: NetworkInterfaceCard
        {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=43,160");
        }
        
        duplikatlistePortEth1: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=400,60");
        }
        duplikatlistePortEth1Exp: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=520,163");
        }
        
        eth1: NetworkInterfaceCard
        {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=302,60");
        }
        
        eth1Exp: NetworkInterfaceCard
        {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=520,60");
        }

        nodeTable: nodeTable {
            @display("p=569,358");
        }
        
    connections:
        
}
