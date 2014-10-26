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

#include "HsrSwitch.h"



HsrSwitch::HsrSwitch()
{
    /*
     * have a look at initialize method.
     * Omnet++ modules are mainly created by the initialize-method.
     */
}


HsrSwitch::~HsrSwitch()
{
    /*
     * Don't know what the hell omnet++ does in the background with its modules.
     * Therefore just to be sure, delete referenced members to prevent memleaks.
     */
    delete macAddress;

    /* constructor and destructor of cGate are protected: only cModule is allowed
     * to create and delete gates. Therefore the following lines are obsolete.

    delete gateAOut;
    delete gateBIn;
    delete gateBOut;
    delete gateCpuIn;
    delete gateCpuOut;

    */

    delete sched;
}


/* Getters */
cGate* HsrSwitch::getGateAIn( void ) {
    return gateAIn;
}

cGate* HsrSwitch::getGateAOut( void ) {
    return gateAOut;
}

cGate* HsrSwitch::getGateBIn( void ) {
    return gateBIn;
}

cGate* HsrSwitch::getGateBOut( void ) {
    return gateBOut;
}

cGate* HsrSwitch::getGateCpuIn( void ) {
    return gateCpuIn;
}

cGate* HsrSwitch::getGateCpuOut( void ) {
    return gateCpuOut;
}

MACAddress* HsrSwitch::getMacAddress( void ) {
    return macAddress;
}

unsigned int HsrSwitch::getRingId( void ) {
    return ringID;
}

Scheduler* HsrSwitch::getSched( void ) {
    return sched;
}

schedulerMode HsrSwitch::getSchedmode( void ) {
    return schedmode;
}

unsigned int HsrSwitch::getSequenceNum( void ) {
    return sequenceNum;
}


/* Setters */
void HsrSwitch::setSched( Scheduler* sched ) {
    this->sched = sched;
}

void HsrSwitch::setSequenceNum( unsigned int sequenceNum ) {
    this->sequenceNum = sequenceNum;
}


void HsrSwitch::initialize()
{
    /* Initialize Scheduler */
    schedmode = static_cast<schedulerMode>( atoi( par( "schedulerMode" ).stringValue() ) );

    macAddress = new MACAddress();

    macAddress->setAddress( par("macAddress").stringValue() );

    if( ( macAddress->isBroadcast() ) || ( macAddress->isMulticast() ) )
    {
        throw cRuntimeError( "Illegal device address ! \n" );
        endSimulation();
    }

    ringID = par( "ringID" );
    if( ( ringID < 1 ) || ( ringID > 6 ) )
    {
        throw cRuntimeError( "invalid Ring ID" );
    }

    sequenceNum = 0;

    gateAIn = gate("gateA$i");
    gateAOut = gate("gateA$o");
    gateBIn = gate("gateB$i");
    gateBOut = gate("gateB$o");
    gateCpuIn = gate("gateCPU$i");
    gateCpuOut = gate("gateCPU$o");
}
