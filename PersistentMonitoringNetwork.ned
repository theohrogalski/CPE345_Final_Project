import inet.mobility.agentMobility

simple agentMobility {
	parameters:
		@class(inet::AgentMobility);
		double speed = default(10mps) @unit(mps);
		double waitTime = default(0s) @unit(s);
		bool isStationary = default(false);
}

module Agent
{
    parameters:
		@display("i=device/pocketpc;is=s");
    gates:
        input in[];
        output out[];
    submodules:
		mobility: agentMobility {
			@display("p=50,50");
		}
}

simple Target
{
    parameters:
        double sensingRange = default(10.0m);
		double reductionRate = default(0.5);
		double decayRate = default(0.1);
		double uncertainty = default(0);
		int id;
    gates:
        input in[];
        output out[];
}

network PersistentMonitoringNetwork
{
    @display("bgb=446,301");
    submodules:
        target1: Target {
			id = 1;
            @display("p=260,27");
        }

        target2: Target {
			id = 2;
            @display("p=105,94");
        }

        target3: Target {
			id = 3;
            @display("p=144,234");
        }

        target4: Target {
			id = 4;
            @display("p=321,234");
        }

        target5: Target {
			id = 5;
            @display("p=386,94");
        }

        agent1: Agent {

            @display("p=183,94");
        }

        agent2: Agent {

            @display("p=243,195");
        }

        agent3: Agent {

            @display("p=299,115");
        }

    connections:
        target1.out++ --> agent1.in++;
        agent1.out++ --> target1.in++;
        target2.out++ --> agent1.in++;
        agent1.out++ --> target2.in++;
        target3.out++ --> agent1.in++;
        agent1.out++ --> target3.in++;
        target4.out++ --> agent1.in++;
        agent1.out++ --> target4.in++;
        target5.out++ --> agent1.in++;
        agent1.out++ --> target5.in++;

        target1.out++ --> agent2.in++;
        agent2.out++ --> target1.in++;
        target2.out++ --> agent2.in++;
        agent2.out++ --> target2.in++;
        target3.out++ --> agent2.in++;
        agent2.out++ --> target3.in++;
        target4.out++ --> agent2.in++;
        agent2.out++ --> target4.in++;
        target5.out++ --> agent2.in++;
        agent2.out++ --> target5.in++;

        target1.out++ --> agent3.in++;
        agent3.out++ --> target1.in++;
        target2.out++ --> agent3.in++;
        agent3.out++ --> target2.in++;
        target3.out++ --> agent3.in++;
        agent3.out++ --> target3.in++;
        target4.out++ --> agent3.in++;
        agent3.out++ --> target4.in++;
        target5.out++ --> agent3.in++;
        agent3.out++ --> target5.in++;
		
}
