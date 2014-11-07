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


void Scheduler::initScheduler( schedulerMode schedmode, cGate* schedOutGate, cGate* schedOutGateExp )
{
    this->schedmode = schedmode;
    setQueueSizeLowInt( 0 );

    queueLowIntVector = new cOutVector();
    queueLowIntVector->setName("QueueSize Low Internal");

    queues->addAt( EXPRESS_RING, new cQueue() );
    queues->addAt( EXPRESS_INTERNAL, new cQueue() );
    queues->addAt( HIGH_RING, new cQueue() );
    queues->addAt( HIGH_INTERNAL, new cQueue() );
    queues->addAt( LOW_RING, new cQueue() );
    queues->addAt( LOW_INTERNAL, new cQueue() );

    this->schedOutGate = schedOutGate;
    this->schedOutGateExp = schedOutGateExp;

}

unsigned long int Scheduler::getQueueSizeLowInt()
{
    return queuesize_low_int;
}

cOutVector* Scheduler::getQueueLowIntVector( void ) {
    return queueLowIntVector;
}

void Scheduler::setQueueSizeLowInt( unsigned long int nr )
{
    queuesize_low_int = nr;
}


void Scheduler::handleMessage( cMessage *msg )
{
    if( msg->isSelfMessage() )
    {
        delete msg;
        processQueues();
    }
}

void Scheduler::enqueueMessage( cMessage *msg, queueName queue )
{
    ( check_and_cast<cQueue*>( queues->get( queue ) ) )->insert( msg );

    switch( queue )
    {
        case LOW_INTERNAL:
        {
            setQueueSizeLowInt( getQueueSizeLowInt() + 1 );
            getQueueLowIntVector()->record( getQueueSizeLowInt() );
            EV << "Queue Size (" << simTime() << "): " << getQueueSizeLowInt() << endl;
            break;
        }

        default:
        {
            break;
        }
    }
}


void Scheduler::processQueues( void )
{

    switch( schedmode )
    {
        case FCFS:
        {
//            ( ( cQueue* )queues->get(EXPRESS_RING) );
//            ( ( cQueue* )queues->get(EXPRESS_INTERNAL) );
//            ( ( cQueue* )queues->get(HIGH_RING) );
//            ( ( cQueue* )queues->get(HIGH_INTERNAL) );
//            ( ( cQueue* )queues->get(LOW_RING) );
//            ( ( cQueue* )queues->get(LOW_INTERNAL) );

            for( int i = 0; i < queues->size(); i++ )
            {
                cQueue* currentQueue = ( ( cQueue* )queues->get( static_cast<schedulerMode>( i ) ) );

                while( !currentQueue->isEmpty() )
                {
                    if( !( schedOutGate->getChannel()->isBusy() ) )
                    {
                        cMessage* msg = check_and_cast<cMessage*>( currentQueue->pop() );

                        /* Channel is free. Send the frame.
                           If Express Prio send via ExpressGate */
                        if( i == EXPRESS_RING || i == EXPRESS_INTERNAL )
                        {
                            getParentModule()->send( msg, schedOutGateExp );
                        }
                        else
                        {
                            getParentModule()->send( msg, schedOutGate );
                        }


                        /* Some logging shizzle */
                        setQueueSizeLowInt(getQueueSizeLowInt()-1);
                        getQueueLowIntVector()->record(getQueueSizeLowInt());
                        EV << "Queue Size (" << simTime() << "): " << getQueueSizeLowInt() << endl;
                    }
                    else
                    {
                        /* Channel is currently busy. Have to reschedule the frame. */
                        simtime_t finishTime = schedOutGate->getChannel()->getTransmissionFinishTime();
                        scheduleAt( finishTime, new SchedulerSelfMessage() );
                    }
                }
            }
            break;
        }

        case RING_FIRST:
        {
            break;
        }

        case ZIPPER:
        {
            break;
        }

        case TOKENS:
        {
            break;
        }

        default:
        {
            break;
        }
    }
}
