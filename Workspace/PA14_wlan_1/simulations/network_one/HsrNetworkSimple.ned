package network_one;
import channels.MyFastChannel;
import testControl.TestControl;
import endDevices.HsrEndNode;
import endDevices.HsrRedBox;

network HsrNetworkSimple
{
    submodules:

        R1EndNode1: HsrEndNode {
            @display("p=50,200");
            macAddress = "00-15-12-14-88-01";
            ringID = 1;
        }

        R1EndNode2: HsrEndNode {
            @display("p=150,200");
            macAddress = "00-15-12-14-88-02";
            ringID = 1;
        }

        R1EndNode3: HsrEndNode {
            @display("p=250,200");
            macAddress = "00-15-12-14-88-03";
            ringID = 1;
        }

        R1EndNode4: HsrEndNode {
            @display("p=350,200");
            macAddress = "00-15-12-14-88-04";
            ringID = 1;
        }

        R1RedBox1: HsrRedBox {
            @display("p=200,80");
            macAddress = "00-15-12-14-88-0A";
            ringID = 1;
            redBoxCfg = 1;
        }


		TestControl1: TestControl {
            @display("p=500,100");
        }

    connections:

        R1RedBox1.gateA <--> MyFastChannel <--> R1EndNode1.gateB;
        R1EndNode1.gateA <--> MyFastChannel <--> R1EndNode2.gateB;
        R1EndNode2.gateA <--> MyFastChannel <--> R1EndNode3.gateB;
        R1EndNode3.gateA <--> MyFastChannel <--> R1EndNode4.gateB;
        R1EndNode4.gateA <--> MyFastChannel <--> R1RedBox1.gateB;
}
