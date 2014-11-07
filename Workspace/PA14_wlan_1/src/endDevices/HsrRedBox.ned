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
        inout gateInterlinkExp @loose;
        
    submodules:

        redBoxSwitch: RedBoxSwitch {
            macAddress = macAddress;
            redBoxCfg = redBoxCfg;
            ringID = ringID;
            @display("p=167,216");
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
        mac1.upperLayerInExp <-- redBoxSwitch.gateAExp$o;

        mac1.upperLayerOut --> redBoxSwitch.gateA$i;
        mac1.upperLayerOutExp --> redBoxSwitch.gateAExp$i;

		mac2.upperLayerIn <-- redBoxSwitch.gateB$o;
		mac2.upperLayerInExp <-- redBoxSwitch.gateBExp$o;

        mac2.upperLayerOut --> redBoxSwitch.gateB$i;
        mac2.upperLayerOutExp --> redBoxSwitch.gateBExp$i;

		mac3.upperLayerIn <-- redBoxSwitch.gateInterlink$o;
		mac3.upperLayerInExp <-- redBoxSwitch.gateInterlinkExp$o;

        mac3.upperLayerOut --> redBoxSwitch.gateInterlink$i;
        mac3.upperLayerOutExp --> redBoxSwitch.gateInterlinkExp$i;


		//CPU        
		cpu.gate$i <-- duplikatlistePortCPU.gate$o;
		cpu.gateExp$i <-- duplikatlistePortCPUExp.gate$o;
        duplikatlistePortCPU.gate$i <-- redBoxSwitch.gateCPU$o;
        duplikatlistePortCPUExp.gate$i <-- redBoxSwitch.gateCPUExp$o;

		cpu.gate$o --> redBoxSwitch.gateCPU$i;
		cpu.gateExp$o --> redBoxSwitch.gateCPUExp$i;

		//Gegen aussen
        gateA <--> mac1.phys;
        gateAExp <--> mac1.physExp;
        gateB <--> mac2.phys;
        gateBExp <--> mac2.physExp;
        gateInterlink <--> mac3.phys;
        gateInterlinkExp <--> mac3.physExp;
}
