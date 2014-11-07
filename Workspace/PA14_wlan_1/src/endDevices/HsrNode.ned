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
import forwardDelay.ForwardDelay;
import duplicateDetection.ListenErkennung;
import mac.ModifiedEtherMACBase;
import nodeTable.nodeTable;

module HsrNode
{
    parameters:
        string macAddress;
        int ringID; //ring number
        int outputQueueCapacityPerPort;
        int duplicateDetectionListSize;
        double delay @unit(s); // propagation delay
        bool storeAndForward;
        
    gates:
        
        inout gateA;  // declare two way connections
        inout gateAExp;  // declare two way connections
        inout gateB;  // declare two way connections
        inout gateBExp;  // declare two way connections
        
    submodules:
       
        duplikatlistePortCPU: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=243,238");
        }
        duplikatlistePortCPUExp: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=353,188");
        }

        cpu: CPU {
            @display("p=242,389");
            macAddress = macAddress;
        }

        mac1: ModifiedEtherMACBase {
            parameters:
                promiscuous = true;
                address = macAddress;
                duplicateDetection = "duplikatlistePortA";
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=43,60");
        }

        mac2: ModifiedEtherMACBase {
            parameters:
                promiscuous = true;
                address = macAddress;
                duplicateDetection = "duplikatlistePortB";
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=342,60");
        }

        nodeTable: nodeTable {
            @display("p=69,358");
        }
        
    connections:
        
}
