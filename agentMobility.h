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
#include "inet/mobility/base/MobilityBase.h"

namespace inet {

/**
 * Random Waypoint mobility model. See NED file for more info.
 *
 * @author Georg Lutz (georglutz AT gmx DOT de), Institut fuer Telematik,
 *  Universitaet Karlsruhe, http://www.tm.uka.de, 2004-2005
 */
class INET_API agentMobility : public MobilityBase
{
private:
    agentMobility *mobility;

  protected:
    bool nextMoveIsWait;
    cPar *speedParameter=nullptr;
    simtime_t lastUpdate;
    cPar *waitTimeParameter=nullptr;
    bool hasWaitTime;
    double updateInterval;
    int num_targets;
    virtual void handleMessage(cMessage *msg) override;
       void actionSelection();

       std::vector<double> targetUncertainties;
      Coord dummyCoord = Coord::ZERO;
      Quaternion dummyQuaternion = Quaternion::IDENTITY;

      // IMobility requirements
      virtual const Coord& getCurrentPosition() override { return lastPosition; }
      virtual const Coord& getCurrentVelocity() override { return dummyCoord; }
      virtual const Coord& getCurrentAcceleration() override { return dummyCoord; }
      virtual const Quaternion& getCurrentAngularPosition() override { return dummyQuaternion; }
      virtual const Quaternion& getCurrentAngularVelocity() override { return dummyQuaternion; }
      virtual const Quaternion& getCurrentAngularAcceleration() override { return dummyQuaternion; }

      // MobilityBase requirement
      virtual void handleSelfMessage(cMessage *msg) override { handleMessage(msg); }
      virtual double getMaxSpeed() const override { return 100.0; }
       void sendAllUncertainties();
       Coord getCoordForTarget();
    /** @brief Initializes mobility model parameters.*/
    virtual void initialize() override;

  public:
    agentMobility();

};

} // namespace inet

#endif

