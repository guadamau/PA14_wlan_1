package endDevices;

import switch.EndNodeSwitch;
import channels.MyInternalChannel;

module HsrEndNode extends HsrNode
{
    parameters:



    gates:



    submodules:

        endNodeSwitch: EndNodeSwitch {
            macAddress = macAddress;
            ringID = ringID;

            @display("p=282,257");
        }

    connections:

        //Gate A 
        gateA <--> eth0.phys;
        gateAExp <--> eth0Exp.phys;

        eth0.upperLayerIn <-- MyInternalChannel <-- duplikatlistePortEth0.gate$o;
        duplikatlistePortEth0.gate$i <-- MyInternalChannel <-- endNodeSwitch.gateA$o;
        eth0Exp.upperLayerIn <-- MyInternalChannel <-- duplikatlistePortEth0Exp.gate$o;
        duplikatlistePortEth0Exp.gate$i <-- MyInternalChannel <-- endNodeSwitch.gateAExp$o;

        eth0.upperLayerOut --> MyInternalChannel --> propagationDelayA.in;
        propagationDelayA.out --> MyInternalChannel --> endNodeSwitch.gateA$i;

        eth0Exp.upperLayerOut --> MyInternalChannel --> propagationDelayAExp.in;
        propagationDelayAExp.out --> MyInternalChannel --> endNodeSwitch.gateAExp$i;


        //Gate B
        gateB <--> eth1.phys;
        gateBExp <--> eth1Exp.phys;

        eth1.upperLayerIn <-- MyInternalChannel <-- duplikatlistePortEth1.gate$o;
        duplikatlistePortEth1.gate$i <-- MyInternalChannel <-- endNodeSwitch.gateB$o;
        eth1Exp.upperLayerIn <-- MyInternalChannel <-- duplikatlistePortEth1Exp.gate$o;
        duplikatlistePortEth1Exp.gate$i <-- MyInternalChannel <-- endNodeSwitch.gateBExp$o;

        eth1.upperLayerOut --> MyInternalChannel --> propagationDelayB.in;
        propagationDelayB.out --> MyInternalChannel --> endNodeSwitch.gateB$i;

        eth1Exp.upperLayerOut --> MyInternalChannel --> propagationDelayBExp.in;
        propagationDelayBExp.out --> MyInternalChannel --> endNodeSwitch.gateBExp$i;

        //CPU      
        cpu.gate$i <-- MyInternalChannel <-- duplikatlistePortCPU.gate$o;
        cpu.gateExp$i <-- MyInternalChannel <-- duplikatlistePortCPUExp.gate$o;
        duplikatlistePortCPU.gate$i <-- MyInternalChannel <-- endNodeSwitch.gateCPU$o;
        duplikatlistePortCPUExp.gate$i <-- MyInternalChannel <-- endNodeSwitch.gateCPUExp$o;

        cpu.gate$o --> MyInternalChannel --> propagationDelayCpu.in;
        propagationDelayCpu.out --> MyInternalChannel --> endNodeSwitch.gateCPU$i;

        cpu.gateExp$o --> MyInternalChannel --> propagationDelayCpuExp.in;
        propagationDelayCpuExp.out --> MyInternalChannel --> endNodeSwitch.gateCPUExp$i;

}
