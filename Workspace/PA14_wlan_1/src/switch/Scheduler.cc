/*
 * Scheduler.cc
 *
 *  Created on: Oct 9, 2014
 *      Author: guadagnini
 */

#include "Scheduler.h"


Scheduler::Scheduler( schedulerMode schedmode )
{
    this->queues = new cArray();
    this->schedmode = schedmode;

    initScheduler();
}

Scheduler::Scheduler()
{
}

cArray* Scheduler::getQueues( void ) {
    return queues;
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


/* private methods */
void Scheduler::initScheduler( void )
{
    queues->setCapacity( 3 );
    queues->addAt( EXPRESS_INTERNAL, new cQueue() );
    queues->addAt( HIGH_INTERNAL, new cQueue() );
    queues->addAt( LOW_INTERNAL, new cQueue() );

    switch( schedmode )
    {
        case RING_FIRST:
        case ZIPPER:
        case TOKENS:
        {
            queues->setCapacity( 6 );
            queues->addAt( EXPRESS_RING, new cQueue() );
            queues->addAt( HIGH_RING, new cQueue() );
            queues->addAt( LOW_RING, new cQueue() );
            break;
        }

        default:
            break;
    }
}
