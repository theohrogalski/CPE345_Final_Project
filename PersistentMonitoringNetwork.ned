import inet.mobility.single.LinearMobility

simple Agent
{
    parameters:
		@display("i=device/pocketpc;is=s");
    gates:
        input in[];
        output out[];
    submodules:
        mobility: LinearMobility {
            
        }
}

simple Target
{
    parameters:
        int sensingRange = default(10);
    gates:
        input in;
        output out;
}

network PersistentMonitoringNetwork
{
    @display("bgb=446,301");
    submodules:
        target1: Target {

            @display("p=260,27");
        }

        target2: Target {

            @display("p=105,94");
        }

        target3: Target {

            @display("p=144,234");
        }

        target4: Target {

            @display("p=321,234");
        }

        target5: Target {

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
        target1.out --> agent1.in;
        agent1.out --> target1.in;
        target2.out --> agent1.in;
        agent1.out --> target2.in;
        target3.out --> agent1.in;
        agent1.out --> target3.in;
        target4.out --> agent1.in;
        agent1.out --> target4.in;
        target5.out --> agent1.in;
        agent1.out --> target5.in;

        target1.out --> agent2.in;
        agent2.out --> target1.in;
        target2.out --> agent2.in;
        agent2.out --> target2.in;
        target3.out --> agent2.in;
        agent2.out --> target3.in;
        target4.out --> agent2.in;
        agent2.out --> target4.in;
        target5.out --> agent2.in;
        agent2.out --> target5.in;

        target1.out --> agent3.in;
        agent3.out --> target1.in;
        target2.out --> agent3.in;
        agent3.out --> target2.in;
        target3.out --> agent3.in;
        agent3.out --> target3.in;
        target4.out --> agent3.in;
        agent3.out --> target4.in;
        target5.out --> agent3.in;
        agent3.out --> target5.in;

}
