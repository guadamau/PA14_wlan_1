package endDevices;

import inet.linklayer.ethernet.EtherMACFullDuplex;

import duplicateDetection.ListenErkennung;
import switch.RedBoxSwitch;
import mac.NetworkInterfaceCard;


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
            propagationDelay = propagationDelay;
            @display("p=167,216");
        }
        
        duplikatlistePortInterlink: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=103,480");
        }
        
        duplikatlistePortInterlinkExp: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=170,400");
        }
        
        ethInterlink: NetworkInterfaceCard
        {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=40,389");
        }
        
        ethInterlinkExp: NetworkInterfaceCard
        {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=250,489");
        }
        
    connections:

        //Macs
        eth0.upperLayerIn <-- duplikatlistePortEth0.gate$o;
        duplikatlistePortEth0.gate$i <-- redBoxSwitch.gateA$o;
        eth0Exp.upperLayerIn <-- duplikatlistePortEth0Exp.gate$o;
        duplikatlistePortEth0Exp.gate$i <-- redBoxSwitch.gateAExp$o;

        eth0.upperLayerOut --> redBoxSwitch.gateA$i;
        eth0Exp.upperLayerOut --> redBoxSwitch.gateAExp$i;

		eth1.upperLayerIn <-- duplikatlistePortEth1.gate$o;
        duplikatlistePortEth1.gate$i <-- redBoxSwitch.gateB$o;
		eth1Exp.upperLayerIn <-- duplikatlistePortEth1Exp.gate$o;
        duplikatlistePortEth1Exp.gate$i <-- redBoxSwitch.gateBExp$o;

        eth1.upperLayerOut --> redBoxSwitch.gateB$i;
        eth1Exp.upperLayerOut --> redBoxSwitch.gateBExp$i;

        
        ethInterlink.upperLayerOut --> redBoxSwitch.gateInterlink$i;
        duplikatlistePortInterlink.gate$i <-- redBoxSwitch.gateInterlink$o;
        duplikatlistePortInterlink.gate$o --> ethInterlink.upperLayerIn;
        
        ethInterlinkExp.upperLayerOut --> redBoxSwitch.gateInterlinkExp$i;
        duplikatlistePortInterlinkExp.gate$i <-- redBoxSwitch.gateInterlinkExp$o;
        duplikatlistePortInterlinkExp.gate$o --> ethInterlinkExp.upperLayerIn;


		//CPU        
		cpu.gate$i <-- duplikatlistePortCPU.gate$o;
		cpu.gateExp$i <-- duplikatlistePortCPUExp.gate$o;
        duplikatlistePortCPU.gate$i <-- redBoxSwitch.gateCPU$o;
        duplikatlistePortCPUExp.gate$i <-- redBoxSwitch.gateCPUExp$o;

		cpu.gate$o --> redBoxSwitch.gateCPU$i;
		cpu.gateExp$o --> redBoxSwitch.gateCPUExp$i;

		//Gegen aussen
        gateA <--> eth0.phys;
        gateAExp <--> eth0Exp.phys;
        gateB <--> eth1.phys;
        gateBExp <--> eth1Exp.phys;
        
        gateInterlink <--> ethInterlink.phys;
        gateInterlinkExp <--> ethInterlinkExp.phys;
}
