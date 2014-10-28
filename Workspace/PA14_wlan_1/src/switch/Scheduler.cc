/*
 * Scheduler.cc
 *
 *  Created on: Oct 9, 2014
 *      Author: guadagnini
 */

#include "Scheduler.h"
#include "HsrSwitch.h"
#include "hsrSwitchSelfMessage_m.h"


Scheduler::Scheduler()
{
    this->queues = new cArray();
}

cArray* Scheduler::getQueues( void ) {
    return queues;
}

void Scheduler::setSchedmode(schedulerMode schedmode) {
    this->schedmode = schedmode;
}

schedulerMode Scheduler::getSchedmode( void ) {
    return schedmode;
}


Scheduler::~Scheduler()
{
    /* do some garbage collection. */
    int i;

    for( i = 0; i < queues->getCapacity(); i++ )
    {
        delete queues->get( i );
    }

    delete queues;
}


void Scheduler::initScheduler( schedulerMode schedmode )
{
    this->schedmode = schedmode;


    HsrSwitch* parentModule = ( HsrSwitch* )getParentModule();

    HsrSwitchSelfMessage* switchSelfMessage = parentModule->generateSelfMessage();

    unsigned int i;

    queues->addAt( EXPRESS_RING, new cQueue() );
    queues->addAt( EXPRESS_INTERNAL, new cQueue() );
    queues->addAt( HIGH_RING, new cQueue() );
    queues->addAt( HIGH_INTERNAL, new cQueue() );
    queues->addAt( LOW_RING, new cQueue() );
    queues->addAt( LOW_INTERNAL, new cQueue() );

    /* set the times, where the scheduler should process its queues ... */
    for( i = 0; i < 100000; i++ )
    {
        /* Schedule a self message every millisecond, with an offset of 0.2 seconds */
        scheduleAt( ( 0.2 + i * 0.001 ), switchSelfMessage );
    }
}
