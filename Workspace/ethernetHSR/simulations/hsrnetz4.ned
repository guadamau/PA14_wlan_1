package ethernethsr.simulations;
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

//
// TODO auto-generated module
//

import ethernethsr.channels.*;
import ethernethsr.testControl.TestControl;
import ethernethsr.EndNode3;

network hsrNetz4
{
    parameters:
        int numClients = default(40);
    submodules:
        Messeinheit[numClients]: EndNode3 {
            myAddress = "00-15-12-14-88-"+string(index+10);
            //if index > 10 {}//:myAddress = "00-15-12-14-88-0"+string(index);
            ringID = 1;                    
        }
        MergingUnit1: EndNode3 {
            myAddress = "00-15-12-14-88-09";
            ringID = 1;     
            @display("p=101,40;i=,#0080FF");
        }
        MergingUnit2: EndNode3 {
            myAddress = "00-15-12-14-88-08";
            ringID = 1;            
            @display("p=181,40;i=,#0080FF");
        }
        TestControl1: TestControl {
            @display("p=31,40");
        }
    connections:
        Messeinheit[numClients-1].gateA <--> MyFastChannel <--> MergingUnit1.gateB;
        MergingUnit1.gateA <--> MyFastChannel <--> MergingUnit2.gateB;
        MergingUnit2.gateA <--> MyFastChannel <--> Messeinheit[0].gateB;
        Messeinheit[i].gateA <--> MyFastChannel <--> Messeinheit[i+1].gateB for i=0..numClients-2;

}
