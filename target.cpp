#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <customMessage_m.h>

using namespace omnetpp;
class target : public cSimpleModule {

public:
int uncertainty=0;
int num_agents;
int id=0;
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

void Target::increaseUncertainty(double amount) 
{
    double u = par("uncertainty").doubleValue();
    u += amount;

    par("uncertainty").setDoubleValue(u);
}

void Target::decreaseUncertainty(double amount) {
    double u = par("uncertainty").doubleValue();
    u -= amount;
    if (u < 0) u = 0;
    par("uncertainty").setDoubleValue(u);
}

void checkDistance(){
    delta = simTime() - lastCheckTime;
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
                t->decreaseUncertainty(reductionRate * delta.dbl());
            }
        }

    }


}

void target :: handleMessage(cMessage *msg){
if (msg->isSelfMessage()){

    uncertainty++;
    scheduleAfter(delta,msg);

    simtime_t delta = simTime() - msg->getSendingTime();
    checkDistance(delta);
    scheduleAt(simTime() + 1.0, msg);
}
else{
    customMessage *msg = new customMessage();
    msg->setUncertainty(uncertainty);
    msg->setGate_num(gate_num);
    msg->messageType(1);
    customMessage reply = new customMessage;
    reply->setUncertainty(uncertainty);
    reply->setGate_num(this->id);

    send(reply,"out",msg->getGate_num());
}
}

