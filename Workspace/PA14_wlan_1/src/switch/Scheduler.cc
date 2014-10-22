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


const cArray*& Scheduler::getQueues() const {
    return queues;
}


schedulerMode Scheduler::getSchedmode() const {
    return schedmode;
}


Scheduler::~Scheduler()
{
    /* do some garbage collection. */
    unsigned int i;

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
    queues->addAt( queueName.EXPRESS_INTERNAL, new cQueue() );
    queues->addAt( queueName.HIGH_INTERNAL, new cQueue() );
    queues->addAt( queueName.LOW_INTERNAL, new cQueue() );

    switch( schedmode )
    {
        case RING_FIRST:
        case ZIPPER:
        case TOKENS:
        {
            queues->setCapacity( 6 );
            queues->addAt( queueName.EXPRESS_RING, new cQueue() );
            queues->addAt( queueName.HIGH_RING, new cQueue() );
            queues->addAt( queueName.LOW_RING, new cQueue() );
            break;
        }

        default:
            break;
    }
}
