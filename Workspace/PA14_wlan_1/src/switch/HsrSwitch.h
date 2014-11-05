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

/* Abstract class as a construction plan for the specific HSR-switches, baby! */

class HsrSwitch : public cSimpleModule
{
private:

    MACAddress* macAddress;

    unsigned int ringID;
    unsigned int sequenceNum;

    cGate* gateAIn;
    cGate* gateAOut;
    cGate* gateBIn;
    cGate* gateBOut;
    cGate* gateCpuIn;
    cGate* gateCpuOut;

    Scheduler* schedGateAOut;
    Scheduler* schedGateBOut;
    Scheduler* schedGateCpuOut;

    schedulerMode schedmode;


protected:

      virtual void initialize(const char* schedcoice);

      /* This behavior must be implemented by derived classes. */
      virtual void handleMessage( cMessage *msg ) = 0;

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

    MACAddress* getMacAddress( void );

    unsigned int getRingId( void );

    Scheduler* getSchedGateAOut( void );

    Scheduler* getSchedGateBOut( void );

    Scheduler* getSchedGateCpuOut( void );

    schedulerMode getSchedmode( void );

    unsigned int getSequenceNum( void );


    /* Setters */

    void setSequenceNum( unsigned int sequenceNum );

    void setSchedGateAOut( Scheduler* schedGateAOut );

    void setSchedGateBOut( Scheduler* schedGateBOut );

    void setSchedGateCpuOut( Scheduler* schedGateCpuOut );
};

#endif /* HSRSWITCH_H_ */
