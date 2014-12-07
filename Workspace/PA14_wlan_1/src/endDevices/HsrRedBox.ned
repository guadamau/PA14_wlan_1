package endDevices;

import inet.util.Delayer;

import duplicateDetection.ListenErkennung;
import switch.RedBoxSwitch;
import mac.NetworkInterfaceCard;
import channels.MyInternalChannel;


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
            @display("p=295,258");
        }

        propagationDelayInterlink: Delayer {
            delay = 6us;
            @display("p=123,335");
        }

        propagationDelayInterlinkExp: Delayer {
            delay = 6us;
            @display("p=276,376");
        }

        duplikatlistePortInterlink: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=82,499");
        }

        duplikatlistePortInterlinkExp: ListenErkennung {
            ListSize = duplicateDetectionListSize;
            @display("p=182,429");
        }

        ethInterlink: NetworkInterfaceCard {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=40,389");
        }

        ethInterlinkExp: NetworkInterfaceCard {
            parameters:
                promiscuous = true;
                address = macAddress;
                txQueueLimit = outputQueueCapacityPerPort;
                @display("p=250,503");
        }

    connections:

        //Gate A 
        gateA <--> eth0.phys;
        gateAExp <--> eth0Exp.phys;

        eth0.upperLayerIn <-- MyInternalChannel <-- duplikatlistePortEth0.gate$o;
        duplikatlistePortEth0.gate$i <-- MyInternalChannel <-- redBoxSwitch.gateA$o;
        eth0Exp.upperLayerIn <-- MyInternalChannel <-- duplikatlistePortEth0Exp.gate$o;
        duplikatlistePortEth0Exp.gate$i <-- MyInternalChannel <-- redBoxSwitch.gateAExp$o;

        eth0.upperLayerOut --> MyInternalChannel --> propagationDelayA.in;
        propagationDelayA.out --> MyInternalChannel --> redBoxSwitch.gateA$i;

        eth0Exp.upperLayerOut --> MyInternalChannel --> propagationDelayAExp.in;
        propagationDelayAExp.out --> MyInternalChannel --> redBoxSwitch.gateAExp$i;


        //Gate B
        gateB <--> eth1.phys;
        gateBExp <--> eth1Exp.phys;

        eth1.upperLayerIn <-- MyInternalChannel <-- duplikatlistePortEth1.gate$o;
        duplikatlistePortEth1.gate$i <-- MyInternalChannel <-- redBoxSwitch.gateB$o;
        eth1Exp.upperLayerIn <-- MyInternalChannel <-- duplikatlistePortEth1Exp.gate$o;
        duplikatlistePortEth1Exp.gate$i <-- MyInternalChannel <-- redBoxSwitch.gateBExp$o;

        eth1.upperLayerOut --> MyInternalChannel --> propagationDelayB.in;
        propagationDelayB.out --> MyInternalChannel --> redBoxSwitch.gateB$i;

        eth1Exp.upperLayerOut --> MyInternalChannel --> propagationDelayBExp.in;
        propagationDelayBExp.out --> MyInternalChannel --> redBoxSwitch.gateBExp$i;

        //CPU      
        cpu.gate$i <-- MyInternalChannel <-- duplikatlistePortCPU.gate$o;
        cpu.gateExp$i <-- MyInternalChannel <-- duplikatlistePortCPUExp.gate$o;
        duplikatlistePortCPU.gate$i <-- MyInternalChannel <-- redBoxSwitch.gateCPU$o;
        duplikatlistePortCPUExp.gate$i <-- MyInternalChannel <-- redBoxSwitch.gateCPUExp$o;

        cpu.gate$o --> MyInternalChannel --> propagationDelayCpu.in;
        propagationDelayCpu.out --> MyInternalChannel --> redBoxSwitch.gateCPU$i;

        cpu.gateExp$o --> MyInternalChannel --> propagationDelayCpuExp.in;
        propagationDelayCpuExp.out --> MyInternalChannel --> redBoxSwitch.gateCPUExp$i;


        // Interlink port        
        ethInterlink.upperLayerOut --> MyInternalChannel --> propagationDelayInterlink.in;
        propagationDelayInterlink.out --> MyInternalChannel --> redBoxSwitch.gateInterlink$i;


        duplikatlistePortInterlink.gate$i <-- MyInternalChannel <-- redBoxSwitch.gateInterlink$o;
        duplikatlistePortInterlink.gate$o --> MyInternalChannel --> ethInterlink.upperLayerIn;

        ethInterlinkExp.upperLayerOut --> MyInternalChannel --> propagationDelayInterlinkExp.in;
        propagationDelayInterlinkExp.out --> MyInternalChannel --> redBoxSwitch.gateInterlinkExp$i;


        duplikatlistePortInterlinkExp.gate$i <-- MyInternalChannel <-- redBoxSwitch.gateInterlinkExp$o;
        duplikatlistePortInterlinkExp.gate$o --> MyInternalChannel --> ethInterlinkExp.upperLayerIn;

        gateInterlink <--> ethInterlink.phys;
        gateInterlinkExp <--> ethInterlinkExp.phys;
}
