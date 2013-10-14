package concert_4g;

// ------------------MODULES ----------------//
simple Smartphone
{
    parameters:
        @display("i=device/cellphone");
    gates:
        input in;
        output out;
}


// ----------------NETWORKS ------------------//
network BasicNetwork
{
    submodules:
        tic: Smartphone {
            parameters:
                @display("p=48,56");
        }
        toc: Smartphone {
            parameters:
                @display("p=196,56");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
