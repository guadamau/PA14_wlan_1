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
    delete queueLowIntVector;
}


void Scheduler::initScheduler( schedulerMode schedmode )
{
    this->schedmode = schedmode;
    setQueueSizeLowInt(0);

    queueLowIntVector = new cOutVector();
    queueLowIntVector->setName("QueueSize Low Internal");

    queues->addAt( EXPRESS_RING, new cQueue() );
    queues->addAt( EXPRESS_INTERNAL, new cQueue() );
    queues->addAt( HIGH_RING, new cQueue() );
    queues->addAt( HIGH_INTERNAL, new cQueue() );
    queues->addAt( LOW_RING, new cQueue() );
    queues->addAt( LOW_INTERNAL, new cQueue() );

}

int Scheduler::getQueueSizeLowInt()
{
    return queuesize_low_int;
}

cOutVector* Scheduler::getQueueLowIntVector( void ) {
    return queueLowIntVector;
}

void Scheduler::setQueueSizeLowInt( unsigned long nr )
{
    queuesize_low_int = nr;
}
