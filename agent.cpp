#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <customMessage_m.h>
#include "agentMobility.h"

using namespace omnetpp;

class agent : public cSimpleModule
{
private:
    double decisionInterval;
    int num_targets;
    int id;
    std::vector<int> target_positions;
    vector<int> targetUncertainties;


protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void actionSelection();
    void sendAllUncertainties();
    Coord getCoordForTarget();

};

Define_Module(agent);

void agent::initialize(){
    getParentModule()->getSubmodule("mobility");
    simetime_t decisionInterval = par("decisionInterval");

    cMessage selectAction = new cMessage("selectAction");

    scheduleAt(simTime()+decisionInterval,selectAction);

    id = 0;
    //This gets the x, y values for each target to be later accessed by target_position[targ_idx*2]
    num_targets = par("num_targets");

    for(int i=0; i<num_targets; i++){
        targetUncertainties.push_back(0);

    }
    }

void agent::sendAllUncertainties(){
    for(int T=0;T<num_Targets;T++){
        cMessage getUnc = new cMessage("getUncertainty");
        send(("targ%d",T),getUnc);
    }

}

Coord agent::getCoordForTarget(){
    Coord max_target_coord;
    int max_uncertainty=-1;
for(int i=0; i<num_targets;i++){
if(target_uncertainties.at(i)>max_uncertainty){
    max_uncertainty=target_uncertainties.at(i);
    int x_c =(par(("targ%dx"),i));
    int y_c=(par(("targ%dy"),i));
}
}
Coord p;
p.x=x_c;
p.y=y_c;

return p;
}
//Handles observations
void agent::handleMessage(cMessage *msg){
    if (msg->isSelfMessage()){
       //Implement action choosing here
            delete msg;
           customMessage *actionSelection = new customMessage("actionSelection");
           actionSelection->setGate_num(this->id);
           mobility->setCoordinates(getCoordForTarget());
           scheduleAt(SimTime()+decisionInterval,actionSelection);

    }
    else{

        customMessage newmsg = check_and_cast<*customMessage> msg;
        gate_num = newmsg->getGate_num();
        target_uncertainties.at(gate_num) = newmsg->getUncertainty();

    }

}
//




