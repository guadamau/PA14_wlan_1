
//
// TODO auto-generated module
//

import switch.EndNodeSwitch;
import cpu.CPU;
import forwardDelay.ForwardDelay;
// import util.packer.EncapsulateMessage;
// import util.packer.DecapsulateMessage;
import duplicateDetection.ListenErkennung;
import mac.ModifiedEtherMACBase;
import nodeTable.nodeTable;

module EndNode3
{
    parameters:
        string myAddress;
        int ringID; //ring number
        int outputQueueCapacityPerPort;
        int duplicateDetectionListSize;
        double delay @unit(s); // propagation delay
        bool storeAndForward;
        @display("i=hsr/endnode");
    gates:
        inout gateA;  // declare two way connections
        inout gateB;  // declare two way connections
    submodules:
        hsrSwitch: EndNodeSwitch {
            myAddress = myAddress;
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

        duplikatlisteInterlink: ListenErkennung {
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

        nodeTable: nodeTable {
            @display("p=69,358");
        }
    connections:

        //Gate A 
        gateA <--> mac1.phys;
//        mac1.upperLayerIn <-- duplikatlistePortA.gate$o;
//        duplikatlistePortA.gate$i <-- hsrSwitch.gateA$o;
		mac1.upperLayerIn <-- hsrSwitch.gateA$o;
        mac1.upperLayerOut --> delay1.in;
        delay1.out --> hsrSwitch.gateA$i;

        //Gate B
        gateB <--> mac2.phys;
//        mac2.upperLayerIn <-- duplikatlistePortB.gate$o;
//        duplikatlistePortB.gate$i <-- hsrSwitch.gateB$o;
		mac2.upperLayerIn <-- hsrSwitch.gateB$o;
        mac2.upperLayerOut --> delay2.in;
        delay2.out --> hsrSwitch.gateB$i;

        //CPU      
//        decap.gate$o;
//        decap.gate$i <--
//        encap.gate$i;
//        encap.gate$o -->
        cpu.gate$i <--  duplikatlisteInterlink.gate$o;
        duplikatlisteInterlink.gate$i <-- hsrSwitch.gateCPU$o;
        cpu.gate$o -->  delay3.in;
        delay3.out --> hsrSwitch.gateCPU$i;
}
