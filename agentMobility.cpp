//
// Copyright (C) 2005 Georg Lutz, Institut fuer Telematik, University of Karlsruhe
// Copyright (C) 2005 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
// Derived from aboves
//
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "agentMobility.h"
#include "customMessage_m.h"
namespace inet {

Define_Module(agentMobility);

agentMobility::agentMobility()
{
    nextMoveIsWait = false;
}

void agentMobility::initialize(int stage)
{
    //EV_WARN<<"Agent initialized";

    //EV_WARN<<"Agent initialized2";

    LineSegmentsMobilityBase::initialize(stage);
    //EV_WARN<<"Agent initialized3";

    if (stage == INITSTAGE_LOCAL) {
        waitTimeParameter = &par("waitTime");
        hasWaitTime = waitTimeParameter->isExpression() || waitTimeParameter->doubleValue() != 0;
        speedParameter = &par("speed");
        stationary = !speedParameter->isExpression() && speedParameter->doubleValue() == 0;
        num_targets=par("num_targets");
           updateInterval=par("updateInterval");
    }
    for(int i=0; i<num_targets; i++){
            targetUncertainties.push_back(0);

        }
    //EV_WARN<<"Agent initialized4";

}

void agentMobility::setTargetPosition()
{
    EV_WARN<<"set  targ"<<endl;

    if (nextMoveIsWait) {
        simtime_t waitTime = waitTimeParameter->doubleValue();
        nextChange = simTime() + waitTime;
        nextMoveIsWait = false;
    }
    else {
        if (lastPosition != targetPosition){
        //Make change here vvvvv

        double speed = speedParameter->doubleValue();
        double distance = lastPosition.distance(targetPosition);

        simtime_t travelTime = distance / speed;
        nextChange = simTime() + travelTime;
        nextMoveIsWait = hasWaitTime;}
        else{nextChange = simTime() + 1.0;}
    }
}

void agentMobility::move()
{
    Coord currentPos = getCurrentPosition();
    EV << "Agent " << getParentModule()->getId() << " is first at " << currentPos.x << ", " << currentPos.y << "\n";
    EV_WARN<<"I am moving";
    LineSegmentsMobilityBase::move();
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

    cModule::SubmoduleIterator it(network);
    while (!it.end()) {
        cModule *sub = *it;

        if (sub && sub->hasPar("uncertainty")) {
            double u = sub->par("uncertainty").doubleValue();
            targetUncertainties.push_back(u);

            if (u > max_uncertainty) {
                max_uncertainty = u;

                if (sub->hasPar("x") && sub->hasPar("y")) {
                    max_target_coord.x = sub->par("x").intValue();
                    max_target_coord.y = sub->par("y").intValue();
                }
            }
        }
        it++;
    }

    return max_target_coord;
}
void agentMobility::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage() && strcmp(msg->getName(), "actionSelection") == 0) {
        // 1. Get the new coord
        inet::Coord nextTarget = getCoordForTarget();

        // 2. Assign it to the PROTECTED member variable from the base class
        // Use 'this->' to be absolutely sure you aren't hitting a local variable
        this->targetPosition = nextTarget;

        // 3. Trigger the INET math to start the move
        this->setTargetPosition();

        // 4. Schedule next check
        scheduleAt(simTime() + updateInterval, msg);
    }

    if (msg->isSelfMessage() && strcmp(msg->getName(), "actionSelection") == 0) {
        // 1. Calculate the new destination
        inet::Coord nextTarget = getCoordForTarget();

        // 2. Update the internal member variable directly
        this->targetPosition = nextTarget;

        // 3. Trigger the base class logic to recalculate the trajectory
        this->setTargetPosition();

        scheduleAt(simTime() + updateInterval, msg);
    }
    // ... rest of your message handling
}}

