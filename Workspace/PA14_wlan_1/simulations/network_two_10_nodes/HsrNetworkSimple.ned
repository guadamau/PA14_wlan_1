package network_two_10_nodes;
import channels.MyFastChannel;
import endDevices.HsrEndNode;
import delayLogger.DelayLogger;


network HsrNetworkSimple
{
    @display("bgb=808,462");
    submodules:

        delayLogger: DelayLogger {
            @display("i=block/wheelbarrow;p=738,400");
        }

        MU1: HsrEndNode {
            macAddress = "00-15-12-14-88-01";
            ringID = 1;
            @display("i=devices/mu;p=124,96");
        }

        MU2: HsrEndNode {
            macAddress = "00-15-12-14-88-02";
            ringID = 1;
            @display("i=devices/mu;p=124,257");
        }

        MU3: HsrEndNode {
            macAddress = "00-15-12-14-88-03";
            ringID = 1;
            @display("i=devices/mu;p=240,257");
        }

        MU4: HsrEndNode {
            macAddress = "00-15-12-14-88-04";
            ringID = 1;
            @display("i=devices/mu;p=354,257");
        }

        MU5: HsrEndNode {
            macAddress = "00-15-12-14-88-05";
            ringID = 1;
            @display("i=devices/mu;p=450,257");
        }

        MU6: HsrEndNode {
            macAddress = "00-15-12-14-88-06";
            ringID = 1;
            @display("i=devices/mu;p=560,257");
        }

        MU7: HsrEndNode {
            macAddress = "00-15-12-14-88-07";
            ringID = 1;
            @display("i=devices/mu;p=658,257");
        }

        MU8: HsrEndNode {
            macAddress = "00-15-12-14-88-08";
            ringID = 1;
            @display("i=devices/mu;p=658,96");
        }

        PU1: HsrEndNode {
            macAddress = "00-15-12-14-88-0A";
            ringID = 1;
            @display("i=devices/pu;p=504,96");
        }

        PU2: HsrEndNode {
            macAddress = "00-15-12-14-88-0B";
            ringID = 1;
            @display("i=devices/pu;p=287,96");
        }

    connections:

        MU1.gateA <--> MyFastChannel <--> MU2.gateB;
        MU2.gateA <--> MyFastChannel <--> MU3.gateB;
        MU3.gateA <--> MyFastChannel <--> MU4.gateB;
        MU4.gateA <--> MyFastChannel <--> MU5.gateB;
        MU5.gateA <--> MyFastChannel <--> MU6.gateB;
        MU6.gateA <--> MyFastChannel <--> MU7.gateB;
        MU7.gateA <--> MyFastChannel <--> MU8.gateB;

        MU8.gateA <--> MyFastChannel <--> PU1.gateB;
        PU1.gateA <--> MyFastChannel <--> PU2.gateB;
        PU2.gateA <--> MyFastChannel <--> MU1.gateB;



        MU1.gateAExp <--> MyFastChannel <--> MU2.gateBExp;
        MU2.gateAExp <--> MyFastChannel <--> MU3.gateBExp;
        MU3.gateAExp <--> MyFastChannel <--> MU4.gateBExp;
        MU4.gateAExp <--> MyFastChannel <--> MU5.gateBExp;
        MU5.gateAExp <--> MyFastChannel <--> MU6.gateBExp;
        MU6.gateAExp <--> MyFastChannel <--> MU7.gateBExp;
        MU7.gateAExp <--> MyFastChannel <--> MU8.gateBExp;

        MU8.gateAExp <--> MyFastChannel <--> PU1.gateBExp;
        PU1.gateAExp <--> MyFastChannel <--> PU2.gateBExp;
        PU2.gateAExp <--> MyFastChannel <--> MU1.gateBExp;
}
