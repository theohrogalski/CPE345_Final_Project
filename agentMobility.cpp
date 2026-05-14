#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <algorithm>
#include "agentMobility.h"

namespace inet {

Define_Module(agentMobility);

agentMobility::agentMobility(){}

void agentMobility::initialize() {
     updateInterval = par("updateInterval").doubleValue();

        speedParameter = &par("speed");

        // Use INET's built-in tool to set the starting X/Y
        setInitialPosition();


        // Start the movement heart-beat
        cMessage *msg = new cMessage("moveStep");
        scheduleAt(simTime() + updateInterval, msg);

}

void agentMobility::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        lastPosition.x=uniform(10,490);
        lastPosition.y=uniform(10,490);
        // 1. Calculate new position
        inet::Coord target = getCoordForTarget();
        double speed = speedParameter->doubleValue();
        double dist = lastPosition.distance(target);

        if (dist > 0.1 && speed > 0) {
            double step = speed * updateInterval;
            if (step > dist) step = dist;

            // Move the internal variable
            lastPosition += (target - lastPosition) / dist * step;

            lastUpdate = simTime();

            // Signal the GUI that position has changed

            // Force the host module to sync its display string with lastPosition

            getParentModule()->getDisplayString().setTagArg("p", 0, uniform(10,490));
            getParentModule()->getDisplayString().setTagArg("p", 1,  uniform(10,490));
            emit(mobilityStateChangedSignal, this);


        scheduleAt(simTime() + updateInterval, msg);
    } else {
        delete msg;
    }
    }}
inet::Coord inet::agentMobility::getCoordForTarget() {
    inet::Coord bestCoord = lastPosition;
    double max_uncertainty = -1.0;

    cModule *network = getParentModule();
    for (cModule::SubmoduleIterator it(network); !it.end(); ++it) {
        cModule *sub = *it;
        if (!sub || std::string(sub->getName()).find("target") == std::string::npos) continue;

        cModule *mobModule = sub->getSubmodule("mobility");
        if (!mobModule) continue;

        auto *mob = check_and_cast<inet::IMobility *>(mobModule);
        double u = sub->par("uncertainty").doubleValue();

        if (u > max_uncertainty) {
            max_uncertainty = u;
            bestCoord = mob->getCurrentPosition();
        }
    }
    return bestCoord;
}


// 2. Implementation of getMaxSpeed (REQUIRED by the framework)

}
