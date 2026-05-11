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

void Target::decreaseUncertainty(double amount) 
{
    unceretainty -= amount;
    if (uncertainty < 0) {
        uncertainty = 0;
    }
}

void checkDistance(){
    simtime_t deltaTime = simTime() - lastCheckTime;
    lastCheckTime = simTime();
    
    //double tx = par("x").doubleValue();
    //double ty = par("y").doubleValue();

    double sensingRange = par("sensingRange").doubleValue();

    auto* agentPosition = check_and_cast<inet::IMobility *>(getSubModule("mobility"));
    inet::Coord agentPos = agentPosition->getCurrentPosition();

    cModule *parent = getParentModule();
    
    for(int j=0;j<parent->submoduleCount();j++){
       
        cModule *sub = parent->getTarget(j);
        
        if (std::string(sub->getName()).find("agent") != std string::npos) {

            auto* tm = check_and_cast<inet::IMobility *>(sub->getSubmodule("mobility"));
            double distance = agentPos.distance(tm->getCurrentPosition());
            
            //double ax = sub->par("x").doubleValue();
            //double ay = sub->par("y").doubleValue();
            //double dx = tx - ax;
            //double dy = ty - ay;
            //double distance = sqrt(dx * dx + dy * dy);

            if (distance <= sensingRange) {
                Target *t = check_and_cast<Target *>(sub);

                double reductionRate = 5.0;
                t->decreaseUncertainty(reductionRate * deltaTime.dbl());
            }
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

