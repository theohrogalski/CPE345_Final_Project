
network agentNetwork
{
    @display("bgb=314.16,176.88");
    submodules:
        agent_0: agent {
            @display("i=display");
        }
        agent_1: agent {
            @display("i=display;p=147.84,26.4");
        }
    connections:
        // TODO: Add custom connection class
        agent_0.out <--> agent_1.out;
        agent_1.out <--> agent_0.in;
}




