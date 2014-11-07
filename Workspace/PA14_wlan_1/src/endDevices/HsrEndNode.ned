package endDevices;

import switch.EndNodeSwitch;

module HsrEndNode extends HsrNode
{
    parameters:

        @display("i=hsr/endnode;bgl=2");

    gates:



    submodules:

        endNodeSwitch: EndNodeSwitch {
            macAddress = macAddress;
            ringID = ringID;
			//schedulerMode = schedulerMode;
            @display("p=167,216");
        }

    connections:

        //Gate A 
        gateA <--> mac1.phys;
        gateAExp <--> mac1.physExp;

        mac1.upperLayerIn <-- endNodeSwitch.gateA$o;
        mac1.upperLayerInExp <-- endNodeSwitch.gateAExp$o;
        mac1.upperLayerOut --> endNodeSwitch.gateA$i;
        mac1.upperLayerOutExp --> endNodeSwitch.gateAExp$i;


        //Gate B
        gateB <--> mac2.phys;
        gateBExp <--> mac2.physExp;

        mac2.upperLayerIn <-- endNodeSwitch.gateB$o;
        mac2.upperLayerInExp <-- endNodeSwitch.gateBExp$o;
        mac2.upperLayerOut --> endNodeSwitch.gateB$i;
        mac2.upperLayerOutExp --> endNodeSwitch.gateBExp$i;

        //CPU      
        cpu.gate$i <-- duplikatlistePortCPU.gate$o;
        cpu.gateExp$i <-- duplikatlistePortCPUExp.gate$o;
        duplikatlistePortCPU.gate$i <-- endNodeSwitch.gateCPU$o;
        duplikatlistePortCPUExp.gate$i <-- endNodeSwitch.gateCPUExp$o;
        cpu.gate$o --> endNodeSwitch.gateCPU$i;
        cpu.gateExp$o --> endNodeSwitch.gateCPUExp$i;

}
