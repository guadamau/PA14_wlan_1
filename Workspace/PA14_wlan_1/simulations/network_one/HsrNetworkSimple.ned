package network_one;
import channels.MyFastChannel;
import endDevices.HsrEndNode;
import testControl.TestControl;


network HsrNetworkSimple
{
    submodules:

        R1EndNode1: HsrEndNode {
            @display("p=50,200");
            macAddress = "00-15-12-14-88-01";
            ringID = 1;        
        }

        R1EndNode2: HsrEndNode {
            @display("p=150,100");
            macAddress = "00-15-12-14-88-02";
            ringID = 1;
        }

        R1EndNode3: HsrEndNode {
            @display("p=250,200");
            macAddress = "00-15-12-14-88-03";
            ringID = 1;
        }


        TestControl1: TestControl {
            @display("p=500,100");
        }

    connections:

        R1EndNode1.gateA <--> MyFastChannel <--> R1EndNode2.gateB;
        R1EndNode2.gateA <--> MyFastChannel <--> R1EndNode3.gateB;
        R1EndNode3.gateA <--> MyFastChannel <--> R1EndNode1.gateB;
        
        R1EndNode1.gateAExp <--> MyFastChannel <--> R1EndNode2.gateBExp;
        R1EndNode2.gateAExp <--> MyFastChannel <--> R1EndNode3.gateBExp;
        R1EndNode3.gateAExp <--> MyFastChannel <--> R1EndNode1.gateBExp;
}
