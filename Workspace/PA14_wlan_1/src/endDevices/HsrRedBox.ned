package endDevices;

import forwardDelay.ForwardDelay;
import duplicateDetection.ListenErkennung;
import switch.RedBoxSwitch;
import mac.ModifiedEtherMACBase;


module HsrRedBox extends HsrNode
{
    parameters:

        int redBoxCfg = default(0); //0=Regular , 1=HSR , 2=PRP-A , 3=PRP-B
        @display( "i=hsr/redbox" );
    
    gates:
        
        inout gateInterlink @loose;
        
    submodules:

        redBoxSwitch: RedBoxSwitch {
            macAddress = macAddress;
            redBoxCfg = redBoxCfg;
            ringID = ringID;
            @display("p=167,216");
        }

        duplikatlistePortInterlink: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=43,238");
        }

        delay4: ForwardDelay {
            delay = delay;
            storeAndForward = storeAndForward;
            @display("p=206,296");
        }

        mac3: ModifiedEtherMACBase {
            parameters:
                promiscuous = true;
                address = myAddress;
                duplicateDetection = "duplikatlistePortInterlink";
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=30,389");
        }
        
    connections:

        //Macs
        mac1.upperLayerIn <-- redBoxSwitch.gateA$o;

        mac1.upperLayerOut --> delay1.in;
        delay1.out --> redBoxSwitch.gateA$i;

		mac2.upperLayerIn <-- redBoxSwitch.gateB$o;

        mac2.upperLayerOut --> delay2.in;
        delay2.out --> redBoxSwitch.gateB$i;

		mac3.upperLayerIn <-- redBoxSwitch.gateInterlink$o;

        mac3.upperLayerOut --> delay3.in;
        delay3.out --> redBoxSwitch.gateInterlink$i;


		//CPU        
		cpu.gate$i <-- duplikatlistePortCPU.gate$o;
        duplikatlistePortCPU.gate$i <-- redBoxSwitch.gateCPU$o;

		cpu.gate$o --> delay4.in;
        delay4.out --> redBoxSwitch.gateCPU$i;

		//Gegen aussen
        gateA <--> mac1.phys;
        gateB <--> mac2.phys;
        gateInterlink <--> mac3.phys;

}
