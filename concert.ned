package concert_4g;

// ------------------ MODULES ------------------//
simple Smartphone
{
    parameters:
        @display("i=device/cellphone");
    gates:
        inout gate[];
}


// ------------------ NETWORKS ------------------//
network ArrayNetwork
{
        @display("bgb=221,118");
    types:
        channel Channel extends ned.DelayChannel {
                delay = 100ms;
        }
    submodules:
        tic[6]: Smartphone {
                @display("p=74,51,ri");
        }
    connections:
        tic[0].gate++ <--> Channel <--> tic[1].gate++;
        tic[1].gate++ <--> Channel <--> tic[2].gate++;
        tic[1].gate++ <--> Channel <--> tic[4].gate++;
        tic[3].gate++ <--> Channel <--> tic[4].gate++;
        tic[4].gate++ <--> Channel <--> tic[5].gate++;
}
