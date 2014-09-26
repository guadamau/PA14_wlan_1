package ethernethsr.simulations;
import ethernethsr.channels.MyFastChannel;
import ethernethsr.testControl.TestControl;
import ethernethsr.EndNode3;
import ethernethsr.RedBox3;

network hsrNetz3
{
    submodules:
        R1EndNode1: EndNode3 {
            @display("p=60,85");
            myAddress = "00-15-12-14-88-01";
            ringID = 1;
        }
        R1EndNode2: EndNode3 {
            @display("p=160,85");
            myAddress = "00-15-12-14-88-02";
            ringID = 1;
        }
        R1RedBox3: RedBox3 {
            @display("p=260,85");
            myAddress = "00-15-12-14-88-03";
            ringID = 1;
            redBoxCfg = 1;
        }
        R1RedBox4: RedBox3 {
            @display("p=260,293");
            myAddress = "00-15-12-14-88-04";
            ringID = 1;
            redBoxCfg = 1;
        }
        R1EndNode5: EndNode3 {
            @display("p=153,294");
            myAddress = "00-15-12-14-88-05";
            ringID = 1;
        }
        R1EndNode6: EndNode3 {
            @display("p=60,294");
            myAddress = "00-15-12-14-88-06";
            ringID = 1;
        }





        R2EndNode7: EndNode3 {
            @display("p=580,85");
            myAddress = "00-15-12-14-88-07";
            ringID = 2;
        }
        R2EndNode8: EndNode3 {
            @display("p=480,85");
            myAddress = "00-15-12-14-88-08";
            ringID = 2;
        }
        R2RedBox9: RedBox3 {
            @display("p=380,85");
            myAddress = "00-15-12-14-88-09";
            ringID = 2;
            redBoxCfg = 1;
        }
        R2RedBox10: RedBox3 {
            @display("p=380,293");
            myAddress = "00-15-12-14-88-10";
            ringID = 2;
            redBoxCfg = 1;
        }
        R2EndNode11: EndNode3 {
            @display("p=480,294");
            myAddress = "00-15-12-14-88-11";
            ringID = 2;
        }
        R2EndNode12: EndNode3 {
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
