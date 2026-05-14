
simple agentMobility
{
    parameters:
        double initialX @unit(m) = default(uniform(this.constraintAreaMinX, this.constraintAreaMaxX));
        double initialY @unit(m) = default(uniform(this.constraintAreaMinY, this.constraintAreaMaxY));
        double initialZ @unit(m) = default(nanToZero(uniform(this.constraintAreaMinZ, this.constraintAreaMaxZ)));
        double constraintAreaMinX @unit(m) = default(0 m); // min x position of the constraint area, unconstrained by default (negative infinity)
        double constraintAreaMinY @unit(m) = default(0 m); // min y position of the constraint area, unconstrained by default (negative infinity)
        double constraintAreaMinZ @unit(m) = default(0 m); // min z position of the constraint area, unconstrained by default (negative infinity)
        double constraintAreaMaxX @unit(m) = default(500 m); // max x position of the constraint area, unconstrained by default (positive infinity)
        double constraintAreaMaxY @unit(m) = default(500 m); // max y position of the constraint area, unconstrained by default (positive infinity)
        double constraintAreaMaxZ @unit(m) = default(0 m); // max z position of the constraint area, unconstrained by default (positive infinity)
        
        @class(inet::agentMobility);
        @display("i=device/pocketpc;is=s");
        bool faceForward = false;
        
        string subjectModule = "";
        string coordinateSystemModule = "";
        int num_targets = default(3);
        bool updateDisplayString = true;
        double updateInterval @unit(s) = 10s;

        string displayStringTextFormat = "";
        bool initFromDisplayString = false;
        double speed @unit(mps) = default(1mps);
        double waitTime @unit(s) = default(0s);
        bool isStationary = default(false);
    gates:
        input in[];
        output out[];
}

module Target
{
    parameters:
        
        int num_agents;
        double sensingRange = default(10.0);
        double reductionRate = default(0.5);
        double decayRate = default(0.1);
        volatile double uncertainty @mutable = default(0);
     	@class(Target);
        int id;

    gates:
        input in[];
        output out[];

    submodules:
        mobility: inet.mobility.static.StationaryMobility {
            parameters:
         	 initialX = uniform(0m,500m);
         	 initialY = uniform(0m,500m);
         	 constraintAreaMinX  = 0 m; // min x position of the constraint area, unconstrained by default (negative infinity)
        constraintAreaMinY= 0 m; // min y position of the constraint area, unconstrained by default (negative infinity)
        constraintAreaMinZ  = 0 m; // min z position of the constraint area, unconstrained by default (negative infinity)
        constraintAreaMaxX = 500 m; // max x position of the constraint area, unconstrained by default (positive infinity)
       constraintAreaMaxY  = 500 m; // max y position of the constraint area, unconstrained by default (positive infinity)
         constraintAreaMaxZ = 0 m; // max z position of the constraint area, unconstrained by default (positive infinity)
        
        }}
network PersistentMonitoringNetwork
{
    parameters:
        int num_agents;

        @display("bgb=500,500");
    submodules:

        target1: Target {
            id = 1;
            
          
        }

        target2: Target {
            id = 2;
           

            @display("p=105,94");
        }

        target3: Target {
            id = 3;
            
        }

        target4: Target {
            id = 4;
            
        }

        target5: Target {
            id = 5;
            
        }

        agent1: agentMobility {

        }

        agent2: agentMobility {

        }

        agent3: agentMobility {

        }

}
