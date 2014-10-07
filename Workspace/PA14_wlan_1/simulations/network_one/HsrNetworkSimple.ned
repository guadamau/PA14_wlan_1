package network_one;
import channels.MyFastChannel;
import testControl.TestControl;
import endDevices.HsrEndNode;
import endDevices.HsrRedBox;

network HsrNetworkSimple
{
    submodules:
        R1EndNode1: HsrEndNode {
            @display("p=60,85");
            myAddress = "00-15-12-14-88-01";
            ringID = 1;
        }
        R1EndNode2: HsrEndNode {
            @display("p=160,85");
            myAddress = "00-15-12-14-88-02";
            ringID = 1;
        }
        R1RedBox3: HsrRedBox {
            @display("p=260,85");
            myAddress = "00-15-12-14-88-03";
            ringID = 1;
            redBoxCfg = 1;
        }
        R1RedBox4: HsrRedBox {
            @display("p=260,293");
            myAddress = "00-15-12-14-88-04";
            ringID = 1;
            redBoxCfg = 1;
        }
        R1EndNode5: HsrEndNode {
            @display("p=153,294");
            myAddress = "00-15-12-14-88-05";
            ringID = 1;
        }
        R1EndNode6: HsrEndNode {
            @display("p=60,294");
            myAddress = "00-15-12-14-88-06";
            ringID = 1;
        }





        R2EndNode7: HsrEndNode {
            @display("p=580,85");
            myAddress = "00-15-12-14-88-07";
            ringID = 2;
        }
        R2EndNode8: HsrEndNode {
            @display("p=480,85");
            myAddress = "00-15-12-14-88-08";
            ringID = 2;
        }
        R2RedBox9: HsrRedBox {
            @display("p=380,85");
            myAddress = "00-15-12-14-88-09";
            ringID = 2;
            redBoxCfg = 1;
        }
        R2RedBox10: HsrRedBox {
            @display("p=380,293");
            myAddress = "00-15-12-14-88-10";
            ringID = 2;
            redBoxCfg = 1;
        }
        R2EndNode11: HsrEndNode {
            @display("p=480,294");
            myAddress = "00-15-12-14-88-11";
            ringID = 2;
        }
        R2EndNode12: HsrEndNode {
            @display("p=580,294");
            myAddress = "00-15-12-14-88-12";
            ringID = 2;
        }
        TestControl1: TestControl {
            @display("p=668,191");
        }


    connections:

        R1EndNode1.gateA <--> MyFastChannel <--> R1EndNode2.gateB;
        R1EndNode2.gateA <--> MyFastChannel <--> R1RedBox3.gateB;
        R1RedBox3.gateA <--> MyFastChannel <--> R1RedBox4.gateB;
        R1RedBox4.gateA <--> MyFastChannel <--> R1EndNode5.gateB;
        R1EndNode5.gateA <--> MyFastChannel <--> R1EndNode6.gateB;
        R1EndNode6.gateA <--> MyFastChannel <--> R1EndNode1.gateB;

        R1RedBox3.gateInterlink <--> MyFastChannel <--> R2RedBox9.gateInterlink;
        R1RedBox4.gateInterlink <--> MyFastChannel <--> R2RedBox10.gateInterlink;

        R2EndNode7.gateA <--> MyFastChannel <--> R2EndNode8.gateB;
        R2EndNode8.gateA <--> MyFastChannel <--> R2RedBox9.gateB;
        R2RedBox9.gateA <--> MyFastChannel <--> R2RedBox10.gateB;
        R2RedBox10.gateA <--> MyFastChannel <--> R2EndNode11.gateB;
        R2EndNode11.gateA <--> MyFastChannel <--> R2EndNode12.gateB;
        R2EndNode12.gateA <--> MyFastChannel <--> R2EndNode7.gateB;
}
