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

#include "EndNodeSwitch.h"
#include "EndNodeScheduler.h"

Define_Module( EndNodeSwitch );


nodeTable* EndNodeSwitch::getNodeTable()
{
    cModule *calleeModule = getParentModule();

    if( calleeModule != NULL )
    {
        /* TODO: Namen als Parameter übergeben */
        calleeModule = calleeModule->getSubmodule( "nodeTable" );
        return check_and_cast<nodeTable*>( calleeModule );
    }

    return NULL;
}


void EndNodeSwitch::initialize()
{
    /* Call initialize of the base class. */
    HsrSwitch::initialize("FCFS");

    endNodeTable = getNodeTable();
    if( endNodeTable == NULL )
    {
        throw cRuntimeError( "can't load node table" );
    }

    HsrSwitch::setSched( new EndNodeScheduler() );
    EndNodeScheduler* sched = ( EndNodeScheduler* )HsrSwitch::getSched();
    sched->initScheduler( HsrSwitch::getSchedmode() );
    scheduleAt( SIMTIME_ZERO,  new HsrSwitchSelfMessage() );
}

EndNodeSwitch::EndNodeSwitch()
{
}

EndNodeSwitch::~EndNodeSwitch()
{
    if( endNodeTable != NULL )
    {
        delete endNodeTable;
    }
}


void EndNodeSwitch::handleMessage(cMessage *msg)
{
    Scheduler* sched = HsrSwitch::getSched();

    if( msg->isSelfMessage() )
    {
        delete msg;
        sched->processQueues();
        scheduleAt(simTime()+0.001, new HsrSwitchSelfMessage());
    }
    else
    {
        sched->enqueueMessage( msg );
    }
}
