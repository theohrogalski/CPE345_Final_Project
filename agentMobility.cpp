#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <algorithm> // Required for std::clamp
#include "agentMobility.h"

namespace inet {

Define_Module(agentMobility);

agentMobility::agentMobility() {
    nextMoveIsWait = false;
}

void agentMobility::initialize(int stage) {
    LineSegmentsMobilityBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
        num_targets = par("num_targets");
        updateInterval = par("updateInterval");
        // Ensure we don't start out of bounds
        setInitialPosition();
    }

    // Only schedule the loop once at the final stage
    if (stage == INITSTAGE_LAST) {
        cMessage *msg = new cMessage("actionSelection");
        scheduleAt(simTime() + updateInterval, msg);
    }
}

void agentMobility::setTargetPosition() {
    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue();
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    }
    else {
        // SAFETY: Use a small epsilon instead of direct != for double comparison
        if (lastPosition.distance(targetPosition) > 0.001) {
            double speed = speedParameter->doubleValue();
            if (speed <= 0) {
                nextChange = simTime() + 1.0;
                return;
            }
            double distance = lastPosition.distance(targetPosition);
            simtime_t travelTime = distance / speed;
            nextChange = simTime() + travelTime;
            nextMoveIsWait = hasWaitTime;
        }
        else {
            nextChange = simTime() + 1.0;
        }
    }
}

void agentMobility::move()
{
    Coord currentPos = getCurrentPosition();
    EV << "Agent " << getParentModule()->getId() << " is first at " << currentPos.x << ", " << currentPos.y << "\n";
    EV_WARN<<"I am moving";
    LineSegmentsMobilityBase::move();


    // FIXED: Clamp current position to prevent floating point "negative" errors
    // before calling raiseErrorIfOutside()
    lastPosition.x = std::max(0.0, std::min(500.0, lastPosition.x));
    lastPosition.y = std::max(0.0, std::min(500.0, lastPosition.y));

    raiseErrorIfOutside();
    currentPos = getCurrentPosition();
    EV << "Agent " << getParentModule()->getId() << " is now at " << currentPos.x << ", " << currentPos.y << "\n";
    
}

double agentMobility::getMaxSpeed() const
{    //EV_WARN<<"get max speed"<<endl;

    return speedParameter->isExpression() ? NaN : speedParameter->doubleValue();
}
void agentMobility::setCoordinates(Coord input){
    //EV_WARN<<"set coord"<<endl;

targetPosition=input;
}
void agentMobility::sendAllUncertainties(){
    //EV_WARN<<"send all unc"<<endl;

    cModule *network = getParentModule();

    for (cModule::SubmoduleIterator it(network); !it.end(); ++it) {
        cModule *submodule = *it;

        if (std::string(submodule->getName()).find("target") != std::string::npos) {
            int id_for_gate=-1;
        for(int i=0; i<num_targets; i++){

            if (std::string(submodule->getName()).find(i) != std::string::npos){
                   id_for_gate=i;


            }
        }
        cMessage *getUnc = new cMessage("Not_empty");

      sendDirect(getUnc,submodule,"in",id_for_gate);

    }

}}

Coord agentMobility::getCoordForTarget() {
    Coord max_target_coord = {0, 0, 0};
    double max_uncertainty = -1.0;

    cModule *network = getParentModule();
    targetUncertainties.clear();

    for (cModule::SubmoduleIterator it(network); !it.end(); ++it) {
        cModule *sub = *it;
        if (!sub || std::string(sub->getName()).find("target") == std::string::npos)
            continue;

        cModule *mobModule = sub->getSubmodule("mobility");
        if (!mobModule) continue;

        auto *mob = check_and_cast<inet::IMobility *>(mobModule);
        inet::Coord pos = mob->getCurrentPosition();

        double u = sub->par("uncertainty").doubleValue();
        targetUncertainties.push_back(u);

        if (u > max_uncertainty) {
            max_uncertainty = u;
            bestCoord.x = std::max(0.1, std::min(499.9, pos.x));
            bestCoord.y = std::max(0.1, std::min(499.9, pos.y));
            bestCoord.z = 0;
        }
    }
    return bestCoord;
}

void agentMobility::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        this->targetPosition = getCoordForTarget();
        this->setTargetPosition();
        scheduleAt(simTime() + updateInterval, msg);
    } else {
        delete msg; // Clean up direct messages if they aren't self-messages
    }
}
void agentMobility::setCoordinates(Coord input){


}
} // namespace inet

