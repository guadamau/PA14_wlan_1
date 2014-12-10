package cpu;

//
// TODO auto-generated module
//
simple CPU
{
    parameters:
        string macAddress;
        xml xmlparam;
        int multicastListener = default(0);
        int framebytelimitPerSecond = default(0);
        @display("i=device/cpu");
    gates:
        inout gate;  // declare two way connections
        inout gateExp;  // declare two way connections
}
