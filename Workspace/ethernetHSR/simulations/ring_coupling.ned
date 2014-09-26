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



import ethernethsr.channels.*;
import ethernethsr.testControl.TestControl;
import ethernethsr.EndNode3;
import ethernethsr.RedBox3;

network ring_coupling
{
    parameters:
        int numClientsPerRing = default(40);
        int numRings = default(12);
    submodules:
        Messeinheit[numClientsPerRing*numRings]: EndNode3 {
            myAddress = "00-15-12-14-"+string(((index%numClientsPerRing))+10)+"-"+string( ( int(index/numClientsPerRing) )+10);
            //if index > 10 {}//:myAddress = "00-15-12-14-88-0"+string(index);
            ringID = 1;
        }
        RedBox[(numRings*4)-4]: RedBox3 {
            myAddress = "00-15-12-14-AA-"+string(index+10);
            ringID = 1;
            redBoxCfg = 1;
        }

        TestControl1: TestControl {
        }
    connections:
        //Bilde Ketten aller Knoten aller Ringe
        for ring=0..numRings-1, for client=0..numClientsPerRing-2 {
            Messeinheit[(ring*numClientsPerRing)+client].gateA <--> MyFastChannel <--> Messeinheit[(ring*numClientsPerRing)+client+1].gateB;
        }

        //Es gibt nur einen Ring und keine Redboxen: schliese den Ring und fertig
        if numRings==1 {
            Messeinheit[numClientsPerRing-1].gateA <--> MyFastChannel <--> Messeinheit[0].gateB;
        }

        //Es gibt mehr als einen Ring: F�r den Anfang und das Ende f�ge zwei Redboxen ein und schliese den Ring
        if numRings!=1 {
            //Erster Ring
            Messeinheit[numClientsPerRing-1].gateA <--> MyFastChannel <--> RedBox[0].gateB;
            RedBox[0].gateA <--> MyFastChannel <--> RedBox[1].gateB;
            RedBox[1].gateA <--> MyFastChannel <--> Messeinheit[0].gateB;
            RedBox[0].gateInterlink <--> MyFastChannel <--> RedBox[2].gateInterlink;
            RedBox[1].gateInterlink <--> MyFastChannel <--> RedBox[3].gateInterlink;

            //Letzter Ring
            Messeinheit[(numClientsPerRing*numRings)-1].gateA <--> MyFastChannel <--> RedBox[(numRings*4)-4-2].gateB;
            RedBox[(numRings*4)-4-2].gateA <--> MyFastChannel <--> RedBox[(numRings*4)-4-1].gateB;
            RedBox[(numRings*4)-4-1].gateA <--> MyFastChannel <--> Messeinheit[(numClientsPerRing*numRings)-numClientsPerRing].gateB;
        }

        //Es gibt mehr als einen Ring: In der Mitte f�ge vier Redboxen ein und schliese den Ring
        for ring=1..numRings-2 {
            Messeinheit[((ring+1)*numClientsPerRing)-1].gateA <--> MyFastChannel <--> RedBox[((ring*4)-2)+0].gateB;
            RedBox[((ring*4)-2)+0].gateA <--> MyFastChannel <--> RedBox[((ring*4)-2)+1].gateB;
            RedBox[((ring*4)-2)+1].gateA <--> MyFastChannel <--> RedBox[((ring*4)-2)+2].gateB;
            RedBox[((ring*4)-2)+2].gateA <--> MyFastChannel <--> RedBox[((ring*4)-2)+3].gateB;
            RedBox[((ring*4)-2)+3].gateA <--> MyFastChannel <--> Messeinheit[(ring*numClientsPerRing)].gateB;
            RedBox[((ring*4)-2)+2].gateInterlink <--> MyFastChannel <--> RedBox[((ring*4)-2)+4].gateInterlink;
            RedBox[((ring*4)-2)+3].gateInterlink <--> MyFastChannel <--> RedBox[((ring*4)-2)+5].gateInterlink;
        }
}
