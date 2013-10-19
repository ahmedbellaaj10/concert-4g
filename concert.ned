package whythehellisthisfailing;

// ------------------ SIMPLE ------------------//
simple Smartphone
{
    parameters:
        @display("i=device/cellphone");
    gates:
        inout gate;
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


// ------------------ MODULES ------------------//
module IspNetwork
{
    parameters:
        int networkAddress; // positive numbes 0, 1, 2
        int numHosts = default(10);
        int numAntennas = default(2);
        int selAnt;
        @display("i=block/network");
    submodules:
        header: Header {
            @display("p=60,71");
        }
        antenna[numAntennas]: Antenna {
            @display("p=60,144");
        }
        smartphone[numHosts]: Smartphone {
            @display("p=59,219");
        }
    connections:
        // connects antenas to the header
        for i=0..sizeof(antenna)-1 {
            antenna[i].headerGate <--> header.antennaGate++;
        }
        // distributes smartphones nodes between antenas
        for i=0..sizeof(smartphone)-1 {
            // selects a random antena
            smartphone[i].gate <--> antenna[intrand(numAntennas)].deviceGate++;
        }
}

// ------------------ NETWORKS ------------------//
network ConcertNetwork
{
    double connectedness = 0.1;
    int numNodes = 100;
    @display("i=block/network2");
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        AtAndTNetwork: IspNetwork {
            @display("p=77,90");
        }
        VerizonNetwork: IspNetwork {
            @display("p=276,90");
        }
    connections:
}

network OtheNetwork
{
    int numNodes = default(100);
    @display("i=block/network2");
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        AtAndTNetwork: IspNetwork {
            @display("p=77,90");
        }
        VerizonNetwork: IspNetwork {
            @display("p=276,90");
        }
    connections:
}
