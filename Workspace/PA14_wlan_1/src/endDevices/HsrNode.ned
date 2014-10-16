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
        inout gateB;  // declare two way connections
        
    submodules:
       

        duplikatlistePortA: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=43,144");
        }

        duplikatlistePortB: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=282,144");
        }

        duplikatlistePortCPU: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=243,238");
        }

        cpu: CPU {
            @display("p=342,389");
            myAddress = myAddress;
        }
        delay3: ForwardDelay {
            delay = delay;
            storeAndForward = storeAndForward;
            @display("p=206,296");
        }

        delay1: ForwardDelay {
            delay = delay;
            storeAndForward = storeAndForward;
            @display("p=113,60");
        }


        mac1: ModifiedEtherMACBase {
            parameters:
                promiscuous = true;
                address = myAddress;
                duplicateDetection = "duplikatlistePortA";
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=43,60");
        }

        delay2: ForwardDelay {
            delay = delay;
            storeAndForward = storeAndForward;
            @display("p=282,60");
        }

        mac2: ModifiedEtherMACBase {
            parameters:
                promiscuous = true;
                address = myAddress;
                duplicateDetection = "duplikatlistePortB";
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=342,60");
        }

        nodeTable: nodeTable {
            @display("p=69,358");
        }
        
    connections:
        
}
