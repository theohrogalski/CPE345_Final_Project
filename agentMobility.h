//
// Copyright (C) 2005 Georg Lutz, Institut fuer Telematik, University of Karlsruhe
// Copyright (C) 2005 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//
//derived from above

#ifndef __INET_RANDOMWAYPOINTMOBILITY_H
#define __INET_RANDOMWAYPOINTMOBILITY_H
//TODO: Fix the below broken include
#include "inet/mobility/base/LineSegmentsMobilityBase.h"

namespace inet {

/**
 * Random Waypoint mobility model. See NED file for more info.
 *
 * @author Georg Lutz (georglutz AT gmx DOT de), Institut fuer Telematik,
 *  Universitaet Karlsruhe, http://www.tm.uka.de, 2004-2005
 */
class INET_API agentMobility : public LineSegmentsMobilityBase
{
private:
    agentMobility *mobility;

  protected:
    bool nextMoveIsWait;
    cPar *speedParameter=nullptr;

    cPar *waitTimeParameter=nullptr;
    bool hasWaitTime;
    int num_targets;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void handleMessage(cMessage *msg) override;
       void actionSelection();
       std::vector<double> targetUncertainties;

       void sendAllUncertainties();
       Coord getCoordForTarget();
    /** @brief Initializes mobility model parameters.*/
    virtual void initialize(int stage) override;

    /** @brief Overridden from LineSegmentsMobilityBase.*/
    virtual void setTargetPosition() override;

    /** @brief Overridden from LineSegmentsMobilityBase.*/
    virtual void move() override;

  public:
    agentMobility();
    virtual void setCoordinates(Coord input);
    virtual double getMaxSpeed() const override;

};

} // namespace inet

#endif

