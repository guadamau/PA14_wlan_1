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

    this->curQueueState = RING;

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

            queueVector->record( queueSizes[ i ] );
            queueVectors->addAt( i, queueVector );
        }
    }

    ringFirstSortOrder[ 0 ] = EXPRESS_RING;
    ringFirstSortOrder[ 1 ] = EXPRESS_INTERNAL;
    ringFirstSortOrder[ 2 ] = HIGH_RING;
    ringFirstSortOrder[ 3 ] = HIGH_INTERNAL;
    ringFirstSortOrder[ 4 ] = LOW_RING;
    ringFirstSortOrder[ 5 ] = LOW_INTERNAL;

    internalFirstSortOrder[ 0 ] = EXPRESS_INTERNAL;
    internalFirstSortOrder[ 1 ] = EXPRESS_RING;
    internalFirstSortOrder[ 2 ] = HIGH_INTERNAL;
    internalFirstSortOrder[ 3 ] = HIGH_RING;
    internalFirstSortOrder[ 4 ] = LOW_INTERNAL;
    internalFirstSortOrder[ 5 ] = LOW_RING;
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
    queueName* currentSortOrder;


    switch( schedmode )
    {
        /*
         * FCFS: we have no distinction between Ring- and Internal frames.
         * See the function above "enqueueMessage". Frames ordered to the ring queues
         * are redirected to the internal queues.
         * Due to the above description, the behaviour FCFS and RING_FIRST have not to
         * be distinguished here.
         *
         * The Ring queues of each level are processed first,
         * which is part of the RING_FIRST policy.
         *
         * In case of FCFS-scheduling-mode the ring queues are just always empty,
         * see enqueueMessage-function.
         *
         * The processing order of the queues is as follows:
         *
         * EXPRESS_RING,
         * EXPRESS_INTERNAL,
         * HIGH_RING,
         * HIGH_INTERNAL,
         * LOW_RING,
         * LOW_INTERNAL
         * */
        case FCFS:
        case RING_FIRST:
        {
            currentSortOrder = ringFirstSortOrder;
            loopQueues( currentSortOrder );
            break;
        }

        /*
         * The Internal queues of each level are processed first,
         * which is part of the INTERNAL_FIRST policy.
         */
        case INTERNAL_FIRST:
        {
            currentSortOrder = internalFirstSortOrder;
            loopQueues( currentSortOrder );
            break;
        }

        /*
         * ZIPPER-Policy
         * Alternates the queues between ring and internal.
         *
         * Processing order of the queues:
         *
         * [2n] even
         * EXPRESS_RING,
         * EXPRESS_INTERNAL,
         * HIGH_RING,
         * HIGH_INTERNAL,
         * LOW_RING,
         * LOW_INTERNAL
         *
         * [2n-1] odd
         * EXPRESS_INTERNAL,
         * EXPRESS_RING,
         * HIGH_INTERNAL,
         * HIGH_RING,
         * LOW_INTERNAL,
         * LOW_RING
         * */
        case ZIPPER:
        {
            switch( curQueueState )
            {
                case RING:
                {
                    currentSortOrder = ringFirstSortOrder;
                    loopQueues( currentSortOrder );
                    curQueueState = INTERNAL;
                    break;
                }
                case INTERNAL:
                {
                    currentSortOrder = internalFirstSortOrder;
                    loopQueues( currentSortOrder );
                    curQueueState = RING;
                    break;
                }
                default:
                {
                    break;
                }
            }
            /* break to case ZIPPER */
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

void Scheduler::loopQueues( queueName* currentSortOrder )
{
    cQueue* currentQueue = NULL;
    queueName currentQueueName;

    for( unsigned char i = 0; i < QUEUES_COUNT; i++ )
    {
        currentQueueName = currentSortOrder[ i ];
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
