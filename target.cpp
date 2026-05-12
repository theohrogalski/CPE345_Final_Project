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
    virtual void increaseUncertainty(double amount);
    virtual void decreaseUncertainty(double amount);
};

Define_Module(target);


void target::initialize() {
    // Initialize parameters from NED
    num_agents = par("num_agents");
    id = par("id"); 
    uncertainty = par("uncertainty").doubleValue();
    delta = 1.0; 

    // Create the initial self-message for the sensing loop
    cMessage *timer = new cMessage("sensingTimer");
    scheduleAt(simTime() + delta, timer);
}

void target::increaseUncertainty(double amount) {
    uncertainty += amount;
    if (uncertainty > 100) uncertainty = 100;
    par("uncertainty").setDoubleValue(uncertainty);
}

void target::decreaseUncertainty(double amount) {
    uncertainty -= amount;
    if (uncertainty < 0) uncertainty = 0;
    par("uncertainty").setDoubleValue(uncertainty);
}

void target::checkDistance(simtime_t deltaTime) {
    double sensingRange = par("sensingRange").doubleValue();
    double reductionRate = par("reductionRate").doubleValue();
    double decayRate = par("decayRate").doubleValue();

    // Get this target's position
    auto* tm = check_and_cast<inet::IMobility *>(getSubmodule("mobility"));
    inet::Coord targetPos = tm->getCurrentPosition();

    bool agentPresent = false;
    cModule *network = getParentModule();

    // Look for agents in the network
    for (int i = 0; i < network->submoduleCount(); ++i) {
        cModule *sub = network->getSubmodule(i);
        if (std::string(sub->getName()).find("agent") != std::string::npos) {
            auto* am = check_and_cast<inet::IMobility *>(sub->getSubmodule("mobility"));
            if (targetPos.distance(am->getCurrentPosition()) <= sensingRange) {
                agentPresent = true;
                break; 
            }
        }
    }

    if (agentPresent) {
        decreaseUncertainty(reductionRate * deltaTime.dbl());
    } else {
        increaseUncertainty(decayRate * deltaTime.dbl());
    }
}

void target::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        // Calculate the actual time elapsed since the last tick
        simtime_t elapsed = simTime() - msg->getSendingTime();
        
        checkDistance(elapsed);

        // Reschedule the timer
        scheduleAt(simTime() + delta, msg);
    } 
    else {
        // Handling incoming customMessage from Agents
        customMessage *incoming = check_and_cast<customMessage *>(msg);
        
        // Create a reply (using your customMessage structure)
        customMessage *reply = new customMessage("uncertaintyReply");
        reply->setUncertainty((int)uncertainty); // Casting back to int if your msg requires it
        reply->setGate_num(this->id);
        
        // Send back through the gate the message arrived on
        send(reply, "out", incoming->getArrivalGate()->getIndex());
        
        delete incoming; // Clean up the incoming message
    }
}

