package verhaltensueberpruefung;
import channels.MyFastChannel;
import endDevices.HsrEndNode;
import delayLogger.DelayLogger;


network HsrNetworkSimple
{
    submodules:

		delayLogger : DelayLogger { @display( "i=block/wheelbarrow" ); }
        
        Node1: HsrEndNode {
            @display("p=50,200");
            macAddress = "00-15-12-14-88-01";       
        }

        Node2: HsrEndNode {
            @display("p=150,100");
            macAddress = "00-15-12-14-88-02";
        }

        Node3: HsrEndNode {
            @display("p=250,200");
            macAddress = "00-15-12-14-88-03";
        }

    connections:

        Node1.gateA <--> MyFastChannel <--> Node2.gateB;
        Node2.gateA <--> MyFastChannel <--> Node3.gateB;
        Node3.gateA <--> MyFastChannel <--> Node1.gateB;
        
        Node1.gateAExp <--> MyFastChannel <--> Node2.gateBExp;
        Node2.gateAExp <--> MyFastChannel <--> Node3.gateBExp;
        Node3.gateAExp <--> MyFastChannel <--> Node1.gateBExp;
}
