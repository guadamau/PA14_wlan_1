package selftest_single_frame_high;
import channels.MyFastChannel;
import endDevices.HsrEndNode;
import delayLogger.DelayLogger;


network HsrNetworkSimple
{
    @display("bgb=1184,462");
    submodules:

        delayLogger: DelayLogger {
            @display("i=block/wheelbarrow;p=1114,395");
        }

        MU1: HsrEndNode {
            macAddress = "00-15-12-14-88-01";
            ringID = 1;
            @display("i=devices/mu;p=319,96");
        }

        MU2: HsrEndNode {
            macAddress = "00-15-12-14-88-02";
            ringID = 1;
            @display("i=devices/mu;p=192,96");
        }

        MU3: HsrEndNode {
            macAddress = "00-15-12-14-88-03";
            ringID = 1;
            @display("i=devices/mu;p=57,96");
        }

        MU4: HsrEndNode {
            macAddress = "00-15-12-14-88-04";
            ringID = 1;
            @display("i=devices/mu;p=57,257");
        }

        MU5: HsrEndNode {
            macAddress = "00-15-12-14-88-05";
            ringID = 1;
            @display("i=devices/mu;p=161,257");
        }

        MU6: HsrEndNode {
            macAddress = "00-15-12-14-88-06";
            ringID = 1;
            @display("i=devices/mu;p=255,257");
        }

        MU7: HsrEndNode {
            macAddress = "00-15-12-14-88-07";
            ringID = 1;
            @display("i=devices/mu;p=358,257");
        }

        MU8: HsrEndNode {
            macAddress = "00-15-12-14-88-08";
            ringID = 1;
            @display("i=devices/mu;p=455,257");
        }

        MU9: HsrEndNode {
            macAddress = "00-15-12-14-88-09";
            ringID = 1;
            @display("i=devices/mu;p=565,257");
        }

        MU10: HsrEndNode {
            macAddress = "00-15-12-14-88-0C";
            ringID = 1;
            @display("i=devices/mu;p=663,257");
        }

        MU11: HsrEndNode {
            macAddress = "00-15-12-14-88-0D";
            ringID = 1;
            @display("i=devices/mu;p=764,257");
        }

        MU12: HsrEndNode {
            macAddress = "00-15-12-14-88-0E";
            ringID = 1;
            @display("i=devices/mu;p=865,257");
        }

        MU13: HsrEndNode {
            macAddress = "00-15-12-14-88-0F";
            ringID = 1;
            @display("i=devices/mu;p=974,257");
        }

        MU14: HsrEndNode {
            macAddress = "00-15-12-14-88-10";
            ringID = 1;
            @display("i=devices/mu;p=1056,257");
        }

        MU15: HsrEndNode {
            macAddress = "00-15-12-14-88-11";
            ringID = 1;
            @display("i=devices/mu;p=1056,96");
        }

        MU16: HsrEndNode {
            macAddress = "00-15-12-14-88-12";
            ringID = 1;
            @display("i=devices/mu;p=935,96");
        }

        MU17: HsrEndNode {
            macAddress = "00-15-12-14-88-13";
            ringID = 1;
            @display("i=devices/mu;p=803,96");
        }

        PU1: HsrEndNode {
            macAddress = "00-15-12-14-88-0A";
            ringID = 1;
            @display("i=devices/pu;p=624,57");
        }

        PU2: HsrEndNode {
            macAddress = "00-15-12-14-88-0B";
            ringID = 1;
            @display("i=devices/pu;p=494,57");
        }

    connections:

        // regular channels for low / high priority
        MU1.gateA <--> MyFastChannel <--> MU2.gateB;
        MU2.gateA <--> MyFastChannel <--> MU3.gateB;
        MU3.gateA <--> MyFastChannel <--> MU4.gateB;
        MU4.gateA <--> MyFastChannel <--> MU5.gateB;
        MU5.gateA <--> MyFastChannel <--> MU6.gateB;
        MU6.gateA <--> MyFastChannel <--> MU7.gateB;
        MU7.gateA <--> MyFastChannel <--> MU8.gateB;
        MU8.gateA <--> MyFastChannel <--> MU9.gateB;
        MU9.gateA <--> MyFastChannel <--> MU10.gateB;
        MU10.gateA <--> MyFastChannel <--> MU11.gateB;
        MU11.gateA <--> MyFastChannel <--> MU12.gateB;
        MU12.gateA <--> MyFastChannel <--> MU13.gateB;
        MU13.gateA <--> MyFastChannel <--> MU14.gateB;
        MU14.gateA <--> MyFastChannel <--> MU15.gateB;
        MU15.gateA <--> MyFastChannel <--> MU16.gateB;
        MU16.gateA <--> MyFastChannel <--> MU17.gateB;

        MU17.gateA <--> MyFastChannel <--> PU1.gateB;
        PU1.gateA <--> MyFastChannel <--> PU2.gateB;
        PU2.gateA <--> MyFastChannel <--> MU1.gateB;


		// express channels
        MU1.gateAExp <--> MyFastChannel <--> MU2.gateBExp;
        MU2.gateAExp <--> MyFastChannel <--> MU3.gateBExp;
        MU3.gateAExp <--> MyFastChannel <--> MU4.gateBExp;
        MU4.gateAExp <--> MyFastChannel <--> MU5.gateBExp;
        MU5.gateAExp <--> MyFastChannel <--> MU6.gateBExp;
        MU6.gateAExp <--> MyFastChannel <--> MU7.gateBExp;
        MU7.gateAExp <--> MyFastChannel <--> MU8.gateBExp;
        MU8.gateAExp <--> MyFastChannel <--> MU9.gateBExp;
        MU9.gateAExp <--> MyFastChannel <--> MU10.gateBExp;
        MU10.gateAExp <--> MyFastChannel <--> MU11.gateBExp;
        MU11.gateAExp <--> MyFastChannel <--> MU12.gateBExp;
        MU12.gateAExp <--> MyFastChannel <--> MU13.gateBExp;
        MU13.gateAExp <--> MyFastChannel <--> MU14.gateBExp;
        MU14.gateAExp <--> MyFastChannel <--> MU15.gateBExp;
        MU15.gateAExp <--> MyFastChannel <--> MU16.gateBExp;
        MU16.gateAExp <--> MyFastChannel <--> MU17.gateBExp;

        MU17.gateAExp <--> MyFastChannel <--> PU1.gateBExp;
        PU1.gateAExp <--> MyFastChannel <--> PU2.gateBExp;
        PU2.gateAExp <--> MyFastChannel <--> MU1.gateBExp;
}
