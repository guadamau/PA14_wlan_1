/*
 * Scheduler.cc
 *
 *  Created on: Oct 9, 2014
 *      Author: guadagnini
 */

#include <Ethernet.h>
#include <sstream>
#include <string>
#include "Scheduler.h"
#include "HsrSwitch.h"
#include "hsrSwitchSelfMessage_m.h"
#include "schedulerSelfMessage_m.h"


Scheduler::Scheduler()
{
    this->queues = new cArray();
    this->queueVectors = new cArray();
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
    delete queueVectors;
}


void Scheduler::initScheduler( unsigned char schedID, HsrSwitch* parentSwitch, schedulerMode schedmode,
                               cGate* schedOutGate, cGate* schedOutGateExp,
                               NetworkInterfaceCard* schedNic, NetworkInterfaceCard* schedNicExp )
{
    this->schedID = schedID;
    this->finishTime = SIMTIME_ZERO;

    this->parentSwitch = parentSwitch;

    this->schedmode = schedmode;

    queues->addAt( EXPRESS_RING, new cQueue() );
    queues->addAt( EXPRESS_INTERNAL, new cQueue() );
    queues->addAt( HIGH_RING, new cQueue() );
    queues->addAt( HIGH_INTERNAL, new cQueue() );
    queues->addAt( LOW_RING, new cQueue() );
    queues->addAt( LOW_INTERNAL, new cQueue() );

    this->schedOutGate = schedOutGate;
    this->schedOutGateExp = schedOutGateExp;

    this->schedNic = schedNic;
    this->schedNicExp = schedNicExp;

    /* Members to record statistics. */
    const char* queueNamesStr[ QUEUES_COUNT ]={
            "QueueSize EXPRESS from Ring",
            "QueueSize EXPRESS (from Internal)",
            "QueueSize HIGH from Ring",
            "QueueSize HIGH (from Internal)",
            "QueueSize LOW from Ring",
            "QueueSize LOW (from Internal)"
    };

    if(schedID != 'C') {
        for(int i = 0; i < QUEUES_COUNT; i++)
        {
            queueSizes[i] = 0;

            cOutVector* queueVector = new cOutVector();

            std::stringstream ss;
            ss << schedID << ": " << queueNamesStr[i];
            queueVector->setName( ss.str().c_str() );

            queueVector->record( queueSizes[i] );
            queueVectors->addAt(i, queueVector);
        }
    }

    fcfsSortOrder[ 0 ] = EXPRESS_RING;
    fcfsSortOrder[ 1 ] = EXPRESS_INTERNAL;
    fcfsSortOrder[ 2 ] = HIGH_RING;
    fcfsSortOrder[ 3 ] = HIGH_INTERNAL;
    fcfsSortOrder[ 4 ] = LOW_RING;
    fcfsSortOrder[ 5 ] = LOW_INTERNAL;

    ringFirstSortOrder[ 0 ] = EXPRESS_RING;
    ringFirstSortOrder[ 1 ] = HIGH_RING;
    ringFirstSortOrder[ 2 ] = LOW_RING;
    ringFirstSortOrder[ 3 ] = EXPRESS_INTERNAL;
    ringFirstSortOrder[ 4 ] = HIGH_INTERNAL;
    ringFirstSortOrder[ 5 ] = LOW_INTERNAL;

    zipperSortOrder[ 0 ] = EXPRESS_RING;
    zipperSortOrder[ 1 ] = HIGH_RING;
    zipperSortOrder[ 2 ] = LOW_RING;
    zipperSortOrder[ 3 ] = EXPRESS_INTERNAL;
    zipperSortOrder[ 4 ] = HIGH_INTERNAL;
    zipperSortOrder[ 5 ] = LOW_INTERNAL;

    queueName ringFirstSortOrder[ QUEUES_COUNT ];
    queueName zipperSortOrder[ QUEUES_COUNT ];
    queueName tokenizerSortOrder[ QUEUES_COUNT ];
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
    if(schedmode == FCFS)
    {
        if(queue == EXPRESS_RING) { queue = EXPRESS_INTERNAL; }
        else if(queue == HIGH_RING) { queue = HIGH_INTERNAL; }
        else if(queue == LOW_RING) { queue = LOW_INTERNAL; }
    }

    ( check_and_cast<cQueue*>( queues->get( queue ) ) )->insert( msg );

    if(schedID != 'C')
    {
        /* LOGGING */
        cOutVector* currentQueueSizeVector = check_and_cast<cOutVector*>( queueVectors->get( queue ) );
        currentQueueSizeVector->record( queueSizes[queue]++ );
    }
}

void Scheduler::processQueues( void )
{
    cQueue* currentQueue = NULL;
    queueName currentQueueName;

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
            for( unsigned char i = 0; i < queues->size(); i++ )
            {
                currentQueueName = static_cast<queueName>( i );
                currentQueue = check_and_cast<cQueue*>( queues->get( currentQueueName ) );

                if( currentQueue != NULL )
                {
                    processOneQueue( currentQueue, currentQueueName );
                }
                else
                {
                    throw cRuntimeError( " [ PANIC ] : current queue to process is NULL! exiting ... \n" );
                }
            }

            /* break belongs to case FCFS */
            break;
        }

        /*
         * RING_FIRST-Policy
         * The Ring-Queues are privileged here.
         * And are processed before the queues
         * containing Messages coming from the CPU.
         *
         * Processing order of the queues:
         * EXPRESS_RING,
         * HIGH_RING,
         * LOW_RING,
         * EXPRESS_INTERNAL,
         * HIGH_INTERNAL,
         * LOW_INTERNAL
         * */
        case RING_FIRST:
        {
            /* break belongs to case RING_FIRST */
            break;
        }

        /*
         * ZIPPER-Policy
         * Alternates the queues at their priority level.
         *
         * Processing order of the queues:
         * EXPRESS_RING,
         * HIGH_RING,
         * LOW_RING,
         * EXPRESS_INTERNAL,
         * HIGH_INTERNAL,
         * LOW_INTERNAL
         * */
        case ZIPPER:
        {
            /* break belongs to case ZIPPER */
            break;
        }

        /* TOKENS-Policy
         *
         * */
        case TOKENS:
        {
            /* break belongs to case TOKENS */
            break;
        }

        /* default to not fall through case statement ... */
        default:
        {
            break;
        }
    }
}

void Scheduler::sendMessage( cMessage* msg, cGate* outGate )
{
    parentSwitch->send( msg, outGate );
    return;
}

void Scheduler::processOneQueue( cQueue* currentQueue, queueName currentQueueName )
{
    cGate* selectedOutGate = NULL;
    NetworkInterfaceCard* selectedNic = NULL;
    unsigned char transmitLock = 1;

    if( !currentQueue->isEmpty() )
    {
        /* Decide if the express channel has to be used. */
        if( currentQueueName == EXPRESS_RING || currentQueueName == EXPRESS_INTERNAL )
        {
            selectedOutGate = schedOutGateExp;
            selectedNic = schedNicExp;
        }
        else
        {
            selectedOutGate = schedOutGate;
            selectedNic = schedNic;
        }

        /* check if the transmit state of the nic is idle.
         * refers to the enum MACTransmitState of EtherMACBase.h
         *
         * CPU has no NICs */
        if( schedID != 'C' )
        {
            transmitLock = selectedNic->isLocked();
        }

        if( transmitLock == 0 || schedID == 'C')
        {

            cMessage* msg = check_and_cast<cMessage*>( currentQueue->pop() );

            if(schedID != 'C')
            {
                selectedNic->lock();
            }


            /* Tell the switch to send the message now */
            sendMessage( msg, selectedOutGate );

            if(schedID != 'C')
            {
                /* LOGGING */
                cOutVector* currentQueueSizeVector = check_and_cast<cOutVector*>( queueVectors->get( currentQueueName ) );
                currentQueueSizeVector->record( queueSizes[ currentQueueName ]-- );
            }
        }
        else
        {
            /* Channel is currently busy. Have to reschedule the frame. */
            EV << "[ Channel blocked ] Node: " << selectedNic->getParentModule()->getFullName() << "  Network Interface Card: " << selectedNic->getPhysOutGate()->getFullName() <<  endl << endl;

            /*
             * EXPRESS HANDLING
             */

        }
    }
}
