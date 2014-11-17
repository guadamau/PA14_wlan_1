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
#include "NetworkInterfaceCard.h"


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
    if( macAddress != NULL )
    {
        delete macAddress;
    }

    /*
     * Constructor and destructor of cGate are protected: only cModule is allowed
     * to create and delete gates. Therefore the following lines are obsolete.
     * The gates are deleted automatically so they don't have to be deleted here.
     */

    delete schedGateAOut;
    delete schedGateAOutExp;
    delete schedGateBOut;
    delete schedGateBOutExp;
    delete schedGateCpuOut;
    delete schedGateCpuOutExp;

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

cGate* HsrSwitch::getGateAInExp() {
    return gateAInExp;
}

cGate* HsrSwitch::getGateAOutExp() {
    return gateAOutExp;
}

cGate* HsrSwitch::getGateBInExp() {
    return gateBInExp;
}

cGate* HsrSwitch::getGateBOutExp() {
    return gateBOutExp;
}

cGate* HsrSwitch::getGateCpuInExp() {
    return gateCpuInExp;
}

cGate* HsrSwitch::getGateCpuOutExp() {
    return gateCpuOutExp;
}

MACAddress* HsrSwitch::getMacAddress( void ) {
    return macAddress;
}

unsigned int HsrSwitch::getRingId( void ) {
    return ringID;
}


schedulerMode HsrSwitch::getSchedmode( void ) {
    return schedmode;
}

unsigned int HsrSwitch::getSequenceNum( void ) {
    return sequenceNum;
}


Scheduler* HsrSwitch::getSchedGateAOut( void ) {
    return schedGateAOut;
}

Scheduler* HsrSwitch::getSchedGateBOut( void ) {
    return schedGateBOut;
}

Scheduler* HsrSwitch::getSchedGateCpuOut( void ) {
    return schedGateCpuOut;
}

Scheduler* HsrSwitch::getSchedGateAOutExp() {
    return schedGateAOutExp;
}

Scheduler* HsrSwitch::getSchedGateBOutExp() {
    return schedGateBOutExp;
}

Scheduler* HsrSwitch::getSchedGateCpuOutExp() {
    return schedGateCpuOutExp;
}


/* Setters */
void HsrSwitch::setSequenceNum( unsigned int sequenceNum ) {
    this->sequenceNum = sequenceNum;
}

void HsrSwitch::setSchedGateAOut( Scheduler* schedGateAOut ) {
    this->schedGateAOut = schedGateAOut;
}

void HsrSwitch::setSchedGateBOut( Scheduler* schedGateBOut ) {
    this->schedGateBOut = schedGateBOut;
}

void HsrSwitch::setSchedGateCpuOut( Scheduler* schedGateCpuOut ) {
    this->schedGateCpuOut = schedGateCpuOut;
}

void HsrSwitch::setSchedGateAOutExp( Scheduler* schedGateAOutExp ) {
    this->schedGateAOutExp = schedGateAOutExp;
}

void HsrSwitch::setSchedGateBOutExp( Scheduler* schedGateBOutExp ) {
    this->schedGateBOutExp = schedGateBOutExp;
}

void HsrSwitch::setSchedGateCpuOutExp( Scheduler* schedGateCpuOutExp ) {
    this->schedGateCpuOutExp = schedGateCpuOutExp;
}


void HsrSwitch::initialize( const char* schedchoice )
{
    /* Initialize Scheduler */
    //schedmode = static_cast<schedulerMode>( par( "schedulerMode" ).stringValue() );

    if (strncasecmp("FCFS",schedchoice,4) == 0)
    {
       schedmode = FCFS;
    }
    else if (strncasecmp("RING_FIRST",schedchoice,10) == 0)
    {
        schedmode = RING_FIRST;
    }
    else if (strncasecmp("ZIPPER",schedchoice,6) == 0)
    {
        schedmode = ZIPPER;
    }
    else if (strncasecmp("TOKENS",schedchoice,6) == 0)
    {
        schedmode = TOKENS;
    }
    else
    {
        throw cRuntimeError( "Illegal scheduler mode ! \n" );
    }

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

    gateAIn = gate( "gateA$i" );
    gateAInExp = gate( "gateAExp$i" );
    gateAOut = gate( "gateA$o" );
    gateAOutExp = gate( "gateAExp$o" );
    gateBIn = gate( "gateB$i" );
    gateBInExp = gate( "gateBExp$i" );
    gateBOut = gate( "gateB$o" );
    gateBOutExp = gate( "gateBExp$o" );
    gateCpuIn = gate( "gateCPU$i" );
    gateCpuInExp = gate( "gateCPUExp$i" );
    gateCpuOut = gate( "gateCPU$o" );
    gateCpuOut = gate( "gateCPUExp$o" );

    NetworkInterfaceCard* eth0 = check_and_cast<NetworkInterfaceCard*>( getModuleByPath( "^.eth0" ) );
    NetworkInterfaceCard* eth0Exp = check_and_cast<NetworkInterfaceCard*>( getModuleByPath( "^.eth0Exp" ) );
    NetworkInterfaceCard* eth1 = check_and_cast<NetworkInterfaceCard*>( getModuleByPath( "^.eth1" ) );
    NetworkInterfaceCard* eth1Exp = check_and_cast<NetworkInterfaceCard*>( getModuleByPath( "^.eth1Exp" ) );

    schedGateAOut = new Scheduler();
    schedGateAOut->initScheduler( schedmode, gateAOut, gateAOutExp, eth0, eth0Exp );
    schedGateBOut = new Scheduler();
    schedGateBOut->initScheduler( schedmode, gateBOut, gateBOutExp, eth1, eth1Exp );
    schedGateCpuOut = new Scheduler();
    /* cpu has no external transmission gate, so we pass the internal gates twice here. */
    schedGateCpuOut->initScheduler( schedmode, gateCpuOut, gateCpuOutExp, NULL, NULL );
}


