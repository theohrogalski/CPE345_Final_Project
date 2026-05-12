import target;
import agents; 
network PersistentMonitoringNetwork
{
    parameters:
        int num_agents = default(3);
        int num_targets = default(5);
        double globalSensingRange = default(20m);
        @display("bgb=500,500");

  submodules: 
        visualizer: IntegratedMultiVisualizer {
           @display("p=50,50");
        }

        agent[num_agents]: Agent {
            parameters:
                @display("p=100,100,ring;i=device/pocketpc,red");
                sensingRange = parent.globalSensingRange;
        }

        target[num_targets]: Target {
            parameters:
                @display("p=250,250,matrix,100,100;i=block/target,gold");
                id = index; // Assigns the array index as the ID
        }

        connections:
        for (i=0; i<num_targets;i++) {
            for (j=0; j<num_agents; j++) {
            target[i].out++ --> { delay = 10ms; } --> agent[j].in++;
            }
        }
}




