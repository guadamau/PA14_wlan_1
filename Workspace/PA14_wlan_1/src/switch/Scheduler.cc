/*
 * Scheduler.cc
 *
 *  Created on: Oct 9, 2014
 *      Author: guadagnini
 */


#include "Scheduler.h"
#include "HsrSwitch.h"
#include "hsrSwitchSelfMessage_m.h"
#include "schedulerSelfMessage_m.h"


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

    EV << "[ OK ] Enqueued Message created at: " << msg->getCreationTime() << " to queue: " << static_cast<queueName>( queue ) << endl;

    switch( queue )
    {
        case LOW_INTERNAL:
        {
            setQueueSizeLowInt( getQueueSizeLowInt() + 1 );
            getQueueLowIntVector()->record( getQueueSizeLowInt() );
            /* EV << "Queue Size (" << simTime() << "): " << getQueueSizeLowInt() << endl; */
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
    HsrSwitch* parentModule = check_and_cast<HsrSwitch*>( getParentModule() );

    cQueue* currentQueue = NULL;
    queueName currentQueueName;
    cGate* selectedOutGate = NULL;

    switch( schedmode )
    {
        /*
         * FCFS Try to process all queues in the order of the queueName-enum.
         *
         * EXPRESS_RING,
         * EXPRESS_INTERNAL,
         * HIGH_RING,
         * HIGH_INTERNAL,
         * LOW_RING,
         * LOW_INTERNAL
         * */
        case FCFS:
        {
            for( int i = 0; i < queues->size(); i++ )
            {
                currentQueueName = static_cast<queueName>( i );
                currentQueue = check_and_cast<cQueue*>( queues->get( currentQueueName ) );

                while( !currentQueue->isEmpty() )
                {
                    /* Decide if the express channel has to be used. */
                    if( currentQueueName == EXPRESS_RING || currentQueueName == EXPRESS_INTERNAL )
                    {
                        selectedOutGate = schedOutGateExp;
                    }
                    else
                    {
                        selectedOutGate = schedOutGate;
                    }

                    if( !( selectedOutGate->getTransmissionChannel()->isBusy() ) )
                    {
                        cMessage* msg = check_and_cast<cMessage*>( currentQueue->pop() );

                        EV << "!!!!!!!!!!!!!!!!!!!!!!!!! TransmissionTime: " << selectedOutGate->getTransmissionChannel()->calculateDuration( msg ) << " s" << endl;

                        /* Channel is free. Send the frame.
                           If Express Prio send via ExpressGate */
                        EV << "[ OK ] Message created at: " << msg->getCreationTime() << " sent." << endl;

                        Enter_Method_Silent();
                        parentModule->send( msg, selectedOutGate );

                        /* Some logging shizzle */
                        setQueueSizeLowInt( getQueueSizeLowInt()-1 );
                        getQueueLowIntVector()->record( getQueueSizeLowInt() );
                        /* EV << "Queue Size (" << simTime() << "): " << getQueueSizeLowInt() << endl; */
                        EV << "SimTime: " << simTime() << " TransmissionFinishTime: " << selectedOutGate->getTransmissionChannel()->getTransmissionFinishTime() << endl;
                    }
                    else
                    {
                        EV << "[ !! ] Transmission channel: " << selectedOutGate->getTransmissionChannel()->getFullName() << " currently busy. Have to reschedule frame transmission." << endl;
                        /* Channel is currently busy. Have to reschedule the frame. */
                        simtime_t finishTime = selectedOutGate->getChannel()->getTransmissionFinishTime();
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
