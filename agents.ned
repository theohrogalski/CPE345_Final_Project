simple agentMobility extends MovingMobilityBase {
    

	parameters: 
    	double updateInterval @unit(s) = default(0.1s); // The simulation time interval used to regularly signal mobility state changes and update the display
        bool faceForward = default(true);
    		
}
simple agent
{
@display("i=abstract/penguin");
gates:
    inout in;
    inout out;
    submodules:
        mobility : agentMobility{
        @display("p=100,100");
        }

}


