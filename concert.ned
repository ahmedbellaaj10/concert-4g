package whythehellisthisfailing;

// ------------------ SIMPLE ------------------//
simple Smartphone
{
    parameters:
        int interval = default(intuniform(1, 10));
        @display("i=device/cellphone");
    gates:
        inout antennaGate;
}

simple Antenna
{
    parameters:
        @display("i=device/antennatower");
    gates:
        inout deviceGate[];
        inout headerGate;
}

simple Header
{
    parameters:
        @display("i=misc/cloud");
    gates:
        inout antennaGate[];
}

// ------------------ CHANNELS ------------------//
channel FibberChannel extends ned.DatarateChannel
{
    datarate = 1Gbps;
    delay = 100us;
    @display("ls=orange,2");
}

// Wifi and 4G channels have similar bandwidth and differs in range.
channel WirelessChannel extends ned.DatarateChannel
{
    // Average between 4G upstream (75Mbps) and downstream (300Mbps).
    datarate = 180Mbps;
    delay = 100us;
    @display("ls=blue,2,da");
}

// ------------------ MODULES ------------------//
module IspNetwork
{
    parameters:
        int numHosts = default(10);
        int numAntennas = default(2);
        @display("i=block/network");
    submodules:
        header: Header {
            @display("p=60,71");
        }
        antenna[numAntennas]: Antenna {
            @display("p=60,144,r,200");
        }
        smartphone[numHosts]: Smartphone {
            @display("p=59,219,m,5");
        }
    connections:
        // connects antenas to the header
        for i=0..sizeof(antenna)-1 {
            antenna[i].headerGate <--> FibberChannel <--> header.antennaGate++;
        }
        // distributes smartphones nodes between antenas
        for i=0..sizeof(smartphone)-1 {
            smartphone[i].antennaGate <--> WirelessChannel <--> antenna[intuniform(0, numAntennas-1)].deviceGate++;
        }
}

// ------------------ NETWORKS ------------------//
network ConcertNetwork
{
    parameters:
        @display("i=block/network2");
    submodules:
        AtAndTNetwork: IspNetwork {
            @display("p=77,90");
        }
        VerizonNetwork: IspNetwork {
            @display("p=276,90");
        }
    connections:
}