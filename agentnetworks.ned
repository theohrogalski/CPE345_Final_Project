
network agentNetwork
{
    @display("bgb=194,164");
    submodules:
        agent_0: agent {
            @display("i=display");
        }
        agent_1: agent {
            @display("i=display;p=141,61");
        }
    connections:
        // TODO: Add custom connection class
        agent_0.out <--> agent_1.out;
        agent_1.out <--> agent_0.in;
}




