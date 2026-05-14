#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "inet/mobility/contract/IMobility.h"
#include "inet/common/ModuleAccess.h"

using namespace omnetpp;

class Target : public cSimpleModule {
public:
    double sensingRange;
    double reductionRate;
    double decayRate;
    cOutVector uncertaintystore;
    double uncertainty;
    int num_agents;
    int id;
    simtime_t delta;

protected:
    virtual void initialize() override;
    virtual void checkDistance(simtime_t deltaTime);
    virtual void handleMessage(cMessage* msg) override;
    virtual void increaseUncertainty(double amount);
    virtual void decreaseUncertainty(double amount);
};

Define_Module(Target);

void Target::initialize() {
    EV_WARN << "Target init start" << endl;

    // Initialize parameters from NED
    id = par("id"); 

    // Setup uncertainty tracking for OMNeT++ analytics
    uncertaintystore.setName("Target_Uncertainty");
    uncertainty = par("uncertainty").doubleValue();
    uncertaintystore.record(uncertainty);

    delta = 5.0;
    num_agents = par("num_agents");

    // Create the initial self-message for the sensing loop
    cMessage *timer = new cMessage("sensingTimer");
    scheduleAt(simTime() + delta, timer);

    EV_WARN << "Target init end" << endl;
}

void Target::increaseUncertainty(double amount) {
    uncertainty += amount;
    par("uncertainty").setDoubleValue(uncertainty);
    uncertaintystore.record(uncertainty); // Log for graph
}

void Target::decreaseUncertainty(double amount) {
    uncertainty -= amount;
    if (uncertainty < 0) {
        uncertainty = 0; // Prevent negative uncertainty
    }
    par("uncertainty").setDoubleValue(uncertainty);
    uncertaintystore.record(uncertainty); // Log for graph
}

void Target::checkDistance(simtime_t deltaTime) {
    double sensingRange = par("sensingRange").doubleValue();
    double reductionRate = par("reductionRate").doubleValue();
    double decayRate = par("decayRate").doubleValue();

    bool agentPresent = false;

    // 1. Get Target's own position safely
    cModule *mobSub = getSubmodule("mobility");
    if (!mobSub) {
        // Fallback: If Target is a simple module inside a compound node,
        // mobility might be a sibling module.
        mobSub = getParentModule()->getSubmodule("mobility");
    }

    if (!mobSub) {
        EV_ERROR << "Target cannot find its mobility module!" << endl;
        return;
    }

    auto mobility = dynamic_cast<inet::IMobility *>(mobSub);
    if (!mobility) return; // Safety check

    inet::Coord targetPos = mobility->getCurrentPosition();

    // 2. Iterate through the network to find agents
    cModule *network = getParentModule();
    for (cModule::SubmoduleIterator it(network); !it.end(); ++it) {
        cModule *sub = *it;
        if (!sub) continue;

        // Check if the module is an agent
        if (std::string(sub->getName()).find("agent") != std::string::npos) {

            // As confirmed: The agent IS the mobility module.
            // dynamic_cast safely tests if 'sub' is actually an IMobility object.
            // If it is, it casts it. If it isn't, it returns nullptr.
            auto agentMob = dynamic_cast<inet::IMobility *>(sub);

            if (agentMob) {
                // Check distance
                if (targetPos.distance(agentMob->getCurrentPosition()) <= sensingRange) {
                    agentPresent = true;
                    break; // Found one agent, no need to check the rest
                }
            }
        }
    }

    // 3. Update uncertainty based on presence
    if (agentPresent) {
        decreaseUncertainty(reductionRate * deltaTime.dbl());
    } else {
        increaseUncertainty(decayRate * deltaTime.dbl());
    }
}

void Target::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        // Use the constant 'delta' to ensure consistent uncertainty math
        checkDistance(delta);

        // Reschedule for the next interval
        scheduleAt(simTime() + delta, msg);
    } else {
        // Cleanup direct messages so they don't leak memory
        delete msg;
    }
}
