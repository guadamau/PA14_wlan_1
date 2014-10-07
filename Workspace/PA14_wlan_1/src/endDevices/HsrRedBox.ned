package endDevices;
//
// TODO auto-generated module
//

import cpu.CPU;
import forwardDelay.ForwardDelay;
import duplicateDetection.ListenErkennung;
import switch.RedBoxSwitch;
// import util.packer.EncapsulateMessage;
// import util.packer.DecapsulateMessage;
import mac.ModifiedEtherMACBase;



module HsrRedBox
{
    parameters:
        string myAddress;
        int outputQueueCapacityPerPort;
        int duplicateDetectionListSize;
        int ringID; //ring number
        int redBoxCfg = default(0); //0=Regular , 1=HSR , 2=PRP-A , 3=PRP-B
        double delay @unit(s); // propagation delay
        bool storeAndForward;
        @display("i=hsr/redbox");
    gates:
        inout gateA;  // declare two way connections
        inout gateB;  // declare two way connections
        inout gateInterlink;  // declare two way connections   
    submodules:

        hsrSwitch: RedBoxSwitch {
            myAddress = myAddress;
            redBoxCfg = redBoxCfg;
            ringID = ringID;
            @display("p=167,216");
        }

        duplikatlistePortA: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=43,144");
        }

        duplikatlistePortB: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=282,144");
        }

        duplikatlistePortInterlink: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=43,238");
        }

        duplikatlistePortCPU: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=243,238");
        }

//        encap: EncapsulateMessage {
//            ringID = ringID;
//            @display("p=269,358");
//        }
//
//        decap: DecapsulateMessage {
//            @display("p=321,302");
//        }

        cpu: CPU {
            @display("p=342,389");
            myAddress = myAddress;
        }
        delay4: ForwardDelay {
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
                //queueModule = "queue1";
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
                //queueModule = "queue2";
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=342,60");
        }

        delay3: ForwardDelay {
            delay = delay;
            storeAndForward = storeAndForward;
            @display("p=113,296");
        }

        mac3: ModifiedEtherMACBase {
            parameters:
                promiscuous = true;
                address = myAddress;
                duplicateDetection = "duplikatlistePortInterlink";
                //queueModule = "queue3";
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=30,389");
        }
    connections:

        //Macs
        mac1.upperLayerIn <-- hsrSwitch.gateA$o;
        //mac1.upperLayerIn <-- duplikatlistePortA.gate$o;
        //duplikatlistePortA.gate$i <-- hsrSwitch.gateA$o;
        mac1.upperLayerOut --> delay1.in;
        delay1.out --> hsrSwitch.gateA$i;

		mac2.upperLayerIn <-- hsrSwitch.gateB$o;
        //mac2.upperLayerIn <-- duplikatlistePortB.gate$o;
        //duplikatlistePortB.gate$i <-- hsrSwitch.gateB$o;
        mac2.upperLayerOut --> delay2.in;
        delay2.out --> hsrSwitch.gateB$i;

		mac3.upperLayerIn <-- hsrSwitch.gateInterlink$o;
        //mac3.upperLayerIn <-- duplikatlistePortInterlink.gate$o;
        //duplikatlistePortInterlink.gate$i <-- hsrSwitch.gateInterlink$o;
        mac3.upperLayerOut --> delay3.in;
        delay3.out --> hsrSwitch.gateInterlink$i;

		//CPU        
//        cpu.gate$i <-- decap.gate$o;
//        decap.gate$i <-- duplikatlistePortCPU.gate$o;
		cpu.gate$i <-- duplikatlistePortCPU.gate$o;
        duplikatlistePortCPU.gate$i <-- hsrSwitch.gateCPU$o;
//        cpu.gate$o --> encap.gate$i;
//        encap.gate$o --> delay4.in;
		cpu.gate$o --> delay4.in;
        delay4.out --> hsrSwitch.gateCPU$i;

		//Gegen aussen
        gateA <--> mac1.phys;
        gateB <--> mac2.phys;
        gateInterlink <--> mac3.phys;

}
