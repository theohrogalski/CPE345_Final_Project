#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <customMessage_m.h>
#include "agentMobility.h"
using namespace omnetpp;
using namespace inet;
using namespace std;
class agent : public cSimpleModule
{
private:
    double decisionInterval;
    int num_targets;
    int id;
    std::vector<int> target_positions;
    std::vector<int> targetUncertainties;
    agentMobility *mobility;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void actionSelection();
    void sendAllUncertainties();
    Coord getCoordForTarget();

};

Define_Module(agent);

void agent::initialize(){
    cModule *node = getParentModule();

    cModule *mobility = node->getSubmodule("agentMobility");

    simtime_t decisionInterval = par("decisionInterval");

    cMessage *selectAction = new cMessage("selectAction");

    scheduleAt(simTime()+decisionInterval,selectAction);

    id = 0;
    //This gets the x, y values for each target to be later accessed by target_position[targ_idx*2]
    num_targets = par("num_targets");

    for(int i=0; i<num_targets; i++){
        targetUncertainties.push_back(0);

    }
    }

void agent::sendAllUncertainties(){
    for(int T=0;T<num_targets;T++){

        cMessage *getUnc = new cMessage("Not_empty");
        string target = "targ"+std::to_string(T);
        send(getUnc,target.c_str());
    }

}

Coord agent::getCoordForTarget() {
    Coord max_target_coord = {0, 0};
    int max_uncertainty = -1;

    // Clear or prepare your vector to store the new values
    targetUncertainties.clear();

    cModule *network = getParentModule();

    for (cModule::SubmoduleIterator it(network); !it.end(); ++it) {
        cModule *submodule = *it;

        // 1. Identify if the node is actually a target
        // (Adjust "target" to match the name in your .ned file)
        if (std::string(submodule->getName()).find("target") != std::string::npos) {

            // 2. Extract parameters directly from the node
            int current_u = submodule->par("uncertainty").intValue();
            int current_x = submodule->par("x").intValue();
            int current_y = submodule->par("y").intValue();

            // 3. Move the uncertainty into your vector
            targetUncertainties.push_back(current_u);

            // 4. Track the coordinate with the highest uncertainty
            if (current_u > max_uncertainty) {
                max_uncertainty = current_u;
                max_target_coord.x = current_x;
                max_target_coord.y = current_y;
            }
        }
    }

    return max_target_coord;
}//Handles observations
void agent::handleMessage(cMessage *msg){
    if (msg->isSelfMessage()){
       //Implement action choosing here
            delete msg;
           customMessage *actionSelection = new customMessage("actionSelection");
           inet::Coord coordinates_for_target = getCoordForTarget();
           EV<<(int)coordinates_for_target.x;

           EV<<(int)coordinates_for_target.y;
           mobility->setCoordinates(coordinates_for_target);

           scheduleAt(SimTime()+decisionInterval,actionSelection);

    }

    else{

        customMessage *newmsg = check_and_cast<customMessage*>(msg);
        int gate_num = newmsg->getGate_num();
        targetUncertainties.at(gate_num) = newmsg->getUncertainty();

    }
}
//




