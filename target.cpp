#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <customMessage_m.h>
using namespace omnetpp;

class target : public cSimpleModule {

public:
int uncertainty=0;
int num_agents;
simtime_t delta;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) override;
    virtual void checkDistance();
};

Define_Module(target);


void target :: initialize ()
{
    num_agents=par("num_agents");
    delta=1;
    customMessage *msg = new customMessage(0,1,uncertainty);

    scheduleAfter(delta, msg);

}
void checkDistance(){
    for(int j=0;j<num_agents;j++){
        double total_distance=0;
        getCur

        if(){

        }

    }


}
void target :: handleMessage(cMessage *msg){
if (msg->isSelfMessage()){

    uncertainty++;
    scheduleAfter(delta,msg);
}
else{
    customMessage *msg = new customMessage();
    msg->setUncertainty(uncertainty);
    msg->setGate_num(gate_num);
    msg->messageType(1);

    send("msg","in",msg->getGate_num());

}
}

