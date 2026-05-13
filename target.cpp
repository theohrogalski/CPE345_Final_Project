#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <customMessage_m.h>
#include "agentMobility.h"
#include "inet/common/ModuleAccess.h"
using namespace omnetpp;
class Target : public cSimpleModule {

public:
double sensingRange;
double reductionRate;

double decayRate;
cOutVector* uncertaintyStore = cOutVector("Target Uncertainties");

mutable double uncertainty;
int num_agents;
int id;
simtime_t delta;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void checkDistance(simtime_t deltaTime);
    virtual void increaseUncertainty(double amount);
    virtual void decreaseUncertainty(double amount);
};

Define_Module(Target);


void Target::initialize() {
    // Initialize parameters from NED
    id = par("id"); 
    uncertainty = par("uncertainty").doubleValue();
    delta = 5;

    int num_agents=par("num_agents");

    // Create the initial self-message for the sensing loop
    //EV<<"here3";
    cMessage *timer = new cMessage("sensingTimer");
    //EV<<"here2";
    scheduleAt(simTime() + delta, timer);
    //EV<<"here4";

}

void Target::increaseUncertainty(double amount) {
    uncertainty += amount;
    par("uncertainty").setDoubleValue(uncertainty);
}

void Target::decreaseUncertainty(double amount) {
    uncertainty -= amount;
    if (uncertainty < 0) uncertainty = 0;
    par("uncertainty").setDoubleValue(uncertainty);
}

void Target::checkDistance(simtime_t deltaTime) {
    double sensingRange = par("sensingRange").doubleValue();
    double reductionRate = par("reductionRate").doubleValue();
    double decayRate = par("decayRate").doubleValue();
    //Get this target's position
    bool agentPresent = false;
    cModule *network = getParentModule();
    cModule *host = this;

    // 2. Get the graphical position (x, y) directly from the simulation engine
    // This works even if the @display string is empty/defaulted
    auto x = atof(host->getDisplayString().getTagArg("p",0));
    auto y = atof(host->getDisplayString().getTagArg("p",1));
    inet::Coord TargetPos;
    TargetPos.x=x;
    TargetPos.y=y;
    inet::Coord pos(x, y);    // Look for agents in the network
    // Fix submodules problem
    for (cModule::SubmoduleIterator it(network); !it.end(); ++it) {
        cModule * sub = *it;
        if (std::string(sub->getName()).find("agent") !=std::string::npos) {
            //Another possibility
            auto* am = check_and_cast<inet::agentMobility *>(sub);
            //EV<<"Got past am";
            if (TargetPos.distance(am->getCurrentPosition()) <= sensingRange) {
                agentPresent = true;
                break; 
            }

    }

    if (agentPresent) {
        decreaseUncertainty(reductionRate * deltaTime.dbl());
        uncertaintyStore.collect(uncertainty);

    } else {
        increaseUncertainty(decayRate * deltaTime.dbl());
       uncertaintyStore.collect(uncertainty);

    }
    }}

void Target::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        // Calculate the actual time elapsed since the last tick
        simtime_t elapsed = simTime() - msg->getSendingTime();
        //EV<<"Here";
        checkDistance(elapsed);

        // Reschedule the timer
        scheduleAt(simTime() + delta, msg);
    } 
    else {
        // Handling incoming customMessage from Agents
        customMessage *incoming = check_and_cast<customMessage *>(msg);
        
        customMessage *reply = new customMessage("uncertaintyReply");
        reply->setUncertainty(uncertainty);
        reply->setGate_num(this->id);
        
        // Send back through the gate the message arrived on
        send(reply, "out", incoming->getArrivalGate()->getIndex());
        
        delete incoming; // Clean up the incoming message
    }

}
void Target::finish(){
  uncertaintyStore.record();


}


