package simulation_1_substation_automation;
import channels.MyFastChannel;
import endDevices.HsrEndNode;
import delayLogger.DelayLogger;


network SubstationHSR
{
    @display("bgb=971,535");
    submodules:

        delayLogger: DelayLogger {
            @display("i=block/wheelbarrow;p=895,481");
        }

        MU1: HsrEndNode {
            macAddress = "00-15-12-14-88-01";
            @display("i=devices/mu;p=759,53");
        }

        MU2: HsrEndNode {
            macAddress = "00-15-12-14-88-02";
            @display("i=devices/mu;p=759,152");
        }

        MU3: HsrEndNode {
            macAddress = "00-15-12-14-88-03";
            @display("i=devices/mu;p=759,259");
        }

        MU4: HsrEndNode {
            macAddress = "00-15-12-14-88-04";
            @display("i=devices/mu;p=759,367");
        }

        MU5: HsrEndNode {
            macAddress = "00-15-12-14-88-05";
            @display("i=devices/mu;p=759,481");
        }

        MU6: HsrEndNode {
            macAddress = "00-15-12-14-88-06";
            @display("i=devices/mu;p=614,481");
        }

        MU7: HsrEndNode {
            macAddress = "00-15-12-14-88-07";
            @display("i=devices/mu;p=479,481");
        }

        MU8: HsrEndNode {
            macAddress = "00-15-12-14-88-08";
            @display("i=devices/mu;p=338,481");
        }

        MU9: HsrEndNode {
            macAddress = "00-15-12-14-88-09";
            @display("i=devices/mu;p=195,481");
        }

        MU10: HsrEndNode {
            macAddress = "00-15-12-14-88-0C";
            @display("i=devices/mu;p=54,481");
        }

        MU11: HsrEndNode {
            macAddress = "00-15-12-14-88-0D";
            @display("i=devices/mu;p=54,367");
        }

        MU12: HsrEndNode {
            macAddress = "00-15-12-14-88-0E";
            @display("i=devices/mu;p=54,259");
        }

        MU13: HsrEndNode {
            macAddress = "00-15-12-14-88-0F";
            @display("i=devices/mu;p=54,152");
        }

        MU14: HsrEndNode {
            macAddress = "00-15-12-14-88-10";
            @display("i=devices/mu;p=54,53");
        }

        PU1: HsrEndNode {
            macAddress = "00-15-12-14-88-0A";
            @display("i=devices/pu;p=326,52");
        }

        PU2: HsrEndNode {
            macAddress = "00-15-12-14-88-0B";
            @display("i=devices/pu;p=518,52");
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
        MU14.gateA <--> MyFastChannel <--> PU1.gateB;
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
        MU14.gateAExp <--> MyFastChannel <--> PU1.gateBExp;
        PU1.gateAExp <--> MyFastChannel <--> PU2.gateBExp;
        PU2.gateAExp <--> MyFastChannel <--> MU1.gateBExp;

}
