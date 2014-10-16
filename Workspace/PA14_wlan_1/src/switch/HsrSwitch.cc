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

Define_Module( HsrSwitch );

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

    delete gateAIn;
    delete gateAOut;
    delete gateBIn;
    delete gateBOut;
    delete gateCpuIn;
    delete gateCpuOut;

    delete sched;
}


/* Getters */
const cGate*& HsrSwitch::getGateAIn() const {
    return gateAIn;
}

const cGate*& HsrSwitch::getGateAOut() const {
    return gateAOut;
}

const cGate*& HsrSwitch::getGateBIn() const {
    return gateBIn;
}

const cGate*& HsrSwitch::getGateBOut() const {
    return gateBOut;
}

const cGate*& HsrSwitch::getGateCpuIn() const {
    return gateCpuIn;
}

const cGate*& HsrSwitch::getGateCpuOut() const {
    return gateCpuOut;
}

const MACAddress*& HsrSwitch::getMacAddress() const {
    return macAddress;
}

unsigned int HsrSwitch::getRingId() const {
    return ringID;
}

const Scheduler*& HsrSwitch::getSched() const {
    return sched;
}

schedulerMode HsrSwitch::getSchedmode() const {
    return schedmode;
}

unsigned int HsrSwitch::getSequenceNum() const {
    return sequenceNum;
}


void HsrSwitch::initialize()
{
    /* Initialize Scheduler */
    schedmode = par( "schedulerMode" ).stringValue();
    sched = new Scheduler( schedmode );

    macAddress->setAddress( par("macAddress").stringValue() );

    if( ( macAddress->isBroadcast() ) || ( macAddress->isMulticast() ) )
    {
        throw cRuntimeError( "invalid Addr" );
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
