simple Agent {
    parameters:
        
    gates:
        input in;
        output out;
}

simple Target {
    parameters;
        int sensingRange = default(10);
    gates;
        input in;
        output out;
}

network PersistentMonitoringNetwork
{
   submodules:
        // Using arrays is much cleaner than target1, target2, etc.
        target[5]: Target; 
        agent[3]: Agent;

    connections:
        // Nested loop: For every target (i), connect to every agent (j)
        for i=0..4, for j=0..2 {
            // Target to Agent
            target[i].out++ --> agent[j].in++;
            
            // Agent to Target
            agent[j].out++ --> target[i].in++;
        }
}


