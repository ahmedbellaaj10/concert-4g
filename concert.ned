package concert_4g;

// ------------------MODULES ----------------//
simple Smartphone
{
    parameters:
        @display("i=device/cellphone");
    gates:
        input in[];
        output out[];
}



// ----------------NETWORKS ------------------//
network BasicNetwork
{
    submodules:
        tic[2]: Smartphone {
            parameters:
                @display("p=48,56");
        }
    connections:
        tic[0].out++ --> {  delay = 10ms; } --> tic[1].in++;
        tic[0].in++ <-- {  delay = 10ms; } <-- tic[1].out++;
}

network ArrayNetwork
{
    @display("bgb=221,118");
    submodules:
        tic[6]: Smartphone {
            @display("p=74,51,ri");
        }
    connections:
        tic[0].out++ --> {  delay = 10ms; } --> tic[1].in++;
        tic[0].in++ <-- {  delay = 10ms; } <-- tic[1].out++;

        tic[1].out++ --> {  delay = 10ms; } --> tic[2].in++;
        tic[1].in++ <-- {  delay = 10ms; } <-- tic[2].out++;

        tic[1].out++ --> {  delay = 10ms; } --> tic[4].in++;
        tic[1].in++ <-- {  delay = 10ms; } <-- tic[4].out++;

        tic[3].out++ --> {  delay = 10ms; } --> tic[4].in++;
        tic[3].in++ <-- {  delay = 10ms; } <-- tic[4].out++;

        tic[4].out++ --> {  delay = 10ms; } --> tic[5].in++;
        tic[4].in++ <-- {  delay = 10ms; } <-- tic[5].out++;
}
