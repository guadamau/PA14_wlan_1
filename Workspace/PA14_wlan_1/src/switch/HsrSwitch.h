//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef HSRSWITCH_H_
#define HSRSWITCH_H_

#include <omnetpp.h>
#include "MACAddress.h"
#include "Scheduler.h"
#include "hsrSwitchSelfMessage_m.h"
#include <float.h>

/* Abstract class as a construction plan for the specific HSR-switches, baby! */

class HsrSwitch : public cSimpleModule
{
private:

    MACAddress* macAddress;

    unsigned int ringID;
    short sequenceNum;

    cGate* gateAIn;
    cGate* gateAInExp;
    cGate* gateAOut;
    cGate* gateAOutExp;
    cGate* gateBIn;
    cGate* gateBInExp;
    cGate* gateBOut;
    cGate* gateBOutExp;
    cGate* gateCpuIn;
    cGate* gateCpuInExp;
    cGate* gateCpuOut;
    cGate* gateCpuOutExp;

    Scheduler* schedGateAOut;
    Scheduler* schedGateAOutExp;
    Scheduler* schedGateBOut;
    Scheduler* schedGateBOutExp;
    Scheduler* schedGateCpuOut;
    Scheduler* schedGateCpuOutExp;

    schedulerMode schedmode;


protected:

      virtual void initialize(const char* schedcoice);

      virtual void handleMessage( cMessage *msg ) = 0;

      virtual void scheduleProcessQueues( unsigned char schedID );

public:

    HsrSwitch();
    virtual ~HsrSwitch();


    /* Getters */
    cGate* getGateAIn( void );

    cGate* getGateAOut( void );

    cGate* getGateBIn( void );

    cGate* getGateBOut( void );

    cGate* getGateCpuIn( void );

    cGate* getGateCpuOut( void );

    cGate* getGateAInExp( void );

    cGate* getGateAOutExp( void );

    cGate* getGateBInExp( void );

    cGate* getGateBOutExp( void );

    cGate* getGateCpuInExp( void );

    cGate* getGateCpuOutExp( void );

    MACAddress* getMacAddress( void );

    unsigned int getRingId( void );

    Scheduler* getSchedGateAOut( void );

    Scheduler* getSchedGateBOut( void );

    Scheduler* getSchedGateCpuOut( void );

    Scheduler* getSchedGateAOutExp( void );

    Scheduler* getSchedGateBOutExp( void );

    Scheduler* getSchedGateCpuOutExp( void ) ;

    schedulerMode getSchedmode( void );

    unsigned int getSequenceNum( void );


    /* Setters */

    void setSequenceNum( unsigned int sequenceNum );

    void setSchedGateAOut( Scheduler* schedGateAOut );

    void setSchedGateBOut( Scheduler* schedGateBOut );

    void setSchedGateCpuOut( Scheduler* schedGateCpuOut );

    void setSchedGateAOutExp( Scheduler* schedGateAOutExp );

    void setSchedGateBOutExp( Scheduler* schedGateBOutExp );

    void setSchedGateCpuOutExp( Scheduler* schedGateCpuOutExp );


    void scheduleMessage( simtime_t finishTime, unsigned char schedID );

};

#endif /* HSRSWITCH_H_ */
