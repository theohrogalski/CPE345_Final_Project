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

void target :: handleMessage(cMessage *msg){
    customMessage reply = new customMessage;
    reply->setUncertainty(uncertainty);
    reply->setGate_num(this->id);

    send(reply,"out",msg->getGate_num());
}
}

