#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <customMessage_m.h>

using namespace omnetpp;

class agent : public cSimpleModule
{
private:
    double decisionInterval;
    cMessage *decisionMsg;
    int num_targets;
    int id;
    std::vector<int> target_positions;
    int max_uncertainty;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void actionSelection();
    void takeAction();
public:
    void observeEnvironment();

};

Define_Module(agent);

void agent::initialize(){
    double decisionInterval= par("decisionInterval");
    id = 0;
    //This gets the x, y values for each target to be later accessed by target_position[targ_idx*2]
    num_targets = par("num_targets");
    for(int l=0; l<num_targets; l++){
        target_positions.push_back(par(("targx%d",l)));
        target_positions.push_back(par(("targy%d",l)));
    }
    }
}

//Handles observations
void agent::handleMessage(customMessage *msg){
customMessage *msg = check_and_cast<cMessage*> msg;
int uncertainty_vals[num_targets];
for(int k=0; k<num_targets;k++){
if (msg->uncertainty > max_uncertainty) {
}
}}


//
void agent::actionSelection(){

}
void agent::takeAction(){

}

void agent::observeEnvironment(){
for(int i=0; i<num_targets;i++){
    customMessage *info = new customMessage("info");

    info->setUncertainty(0);
    info->setMessage_type(0);
    info->setGate_num(this->id);

    send(info,"out",i);

}
}


