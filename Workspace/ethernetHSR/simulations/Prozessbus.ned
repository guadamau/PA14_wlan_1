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

import ethernethsr.channels.MyFastChannel;
import ethernethsr.testControl.TestControl;
import ethernethsr.EndNode3;

network Prozessbus
{
    submodules:
        Messeinheit1: EndNode3 {
            @display("p=60,60");
            myAddress = "00-15-12-14-88-01";
            ringID = 1;
        }
        Messeinheit2: EndNode3 {
            @display("p=160,60");
            myAddress = "00-15-12-14-88-02";
            ringID = 1;
        }
        Messeinheit3: EndNode3 {
            @display("p=260,60");
            myAddress = "00-15-12-14-88-03";
            ringID = 1;
        }
        Messeinheit4: EndNode3 {
            @display("p=367,60");
            myAddress = "00-15-12-14-88-04";
            ringID = 1;
        }
        Messeinheit5: EndNode3 {
            @display("p=463,60");
            myAddress = "00-15-12-14-88-05";
            ringID = 1;
        }
        Messeinheit6: EndNode3 {
            @display("p=463,154");
            myAddress = "00-15-12-14-88-06";
            ringID = 1;
        }
        Messeinheit7: EndNode3 {
            @display("p=367,154");
            myAddress = "00-15-12-14-88-07";
            ringID = 1;
        }
        Messeinheit8: EndNode3 {
            @display("p=260,154");
            myAddress = "00-15-12-14-88-08";
            ringID = 1;
        }


        MergingUnit1: EndNode3 {
            @display("p=160,154;i=,#0080FF");
            myAddress = "00-15-12-14-88-09";
            ringID = 1;
        }
        MergingUnit2: EndNode3 {
            @display("p=60,154;i=,#0080FF");
            myAddress = "00-15-12-14-88-10";
            ringID = 1;
        }

        TestControl1: TestControl {
            @display("p=552,107");
        }


    connections:

        Messeinheit1.gateA <--> MyFastChannel <--> Messeinheit2.gateB;
        Messeinheit2.gateA <--> MyFastChannel <--> Messeinheit3.gateB;
        Messeinheit3.gateA <--> MyFastChannel <--> Messeinheit4.gateB;
        Messeinheit4.gateA <--> MyFastChannel <--> Messeinheit5.gateB;
        Messeinheit5.gateA <--> MyFastChannel <--> Messeinheit6.gateB;
        Messeinheit6.gateA <--> MyFastChannel <--> Messeinheit7.gateB;
        Messeinheit7.gateA <--> MyFastChannel <--> Messeinheit8.gateB;
        Messeinheit8.gateA <--> MyFastChannel <--> MergingUnit1.gateB;
        MergingUnit1.gateA <--> MyFastChannel <--> MergingUnit2.gateB;
        MergingUnit2.gateA <--> MyFastChannel <--> Messeinheit1.gateB;

}
