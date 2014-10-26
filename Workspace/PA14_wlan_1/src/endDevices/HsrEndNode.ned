package endDevices;

import switch.EndNodeSwitch;

module HsrEndNode extends HsrNode
{
    parameters:

        @display("i=hsr/endnode");
        
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

		mac1.upperLayerIn <-- endNodeSwitch.gateA$o;
        mac1.upperLayerOut --> delay1.in;
        delay1.out --> endNodeSwitch.gateA$i;

        //Gate B
        gateB <--> mac2.phys;

		mac2.upperLayerIn <-- endNodeSwitch.gateB$o;
        mac2.upperLayerOut --> delay2.in;
        delay2.out --> endNodeSwitch.gateB$i;

        //CPU      
        cpu.gate$i <--  duplikatlistePortCPU.gate$o;
        duplikatlistePortCPU.gate$i <-- endNodeSwitch.gateCPU$o;
        cpu.gate$o -->  delay3.in;
        delay3.out --> endNodeSwitch.gateCPU$i;
        
}
