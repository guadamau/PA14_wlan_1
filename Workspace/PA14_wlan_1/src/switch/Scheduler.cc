/*
 * Scheduler.cc
 *
 *  Created on: Oct 9, 2014
 *      Author: guadagnini
 */

#include <Ethernet.h>
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
            "QueueSize EXPRESS from Internal",
            "QueueSize HIGH from Ring",
            "QueueSize HIGH from Internal",
            "QueueSize LOW from Ring",
            "QueueSize LOW from Internal"
    };

    for(int i = 0; i < QUEUES_COUNT; i++)
    {
        queueSizes[i] = 0;

        cOutVector* queueVector = new cOutVector();
        queueVector->setName( queueNamesStr[i] );
        queueVector->record( queueSizes[i] );
        queueVectors->addAt(i, queueVector);
    }
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

//    EV << "[ Enqueue " << simTime() << " ] " << parentSwitch->getParentModule()->getFullName() << " - Message " << msg->getCreationTime() << " from Gate " << msg->getArrivalGate()->getFullName() << endl << endl;

    /* LOGGING */
    cOutVector* currentQueueSizeVector = check_and_cast<cOutVector*>( queueVectors->get( queue ) );
    currentQueueSizeVector->record( queueSizes[queue]++ );
}

void Scheduler::processQueues( void )
{
    cPacket* ifgmsg = new cPacket();
    ifgmsg->setBitLength(INTERFRAME_GAP_BITS);

    cQueue* currentQueue = NULL;
    queueName currentQueueName;
    cGate* selectedOutGate = NULL;
    NetworkInterfaceCard* selectedNic = NULL;
    unsigned char transmitLock = 1;

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


                        /* Channel is free. Send the frame.
                           If Express Prio send via ExpressGate */
//                        EV << "[ OK ] Message " << msg->getId() << " created at: " << msg->getCreationTime() << " sent." << endl;

                        if(schedID != 'C') {
                            selectedNic->lock();

                            //EV << "SimTime: " << simTime() << " TransmissionFinishTime: " << selectedNic->getPhysOutGate()->getTransmissionChannel()->getTransmissionFinishTime() << endl;
                            finishTime = simTime() + selectedNic->getPhysOutGate()->getTransmissionChannel()->calculateDuration( msg ) + selectedNic->getPhysOutGate()->getTransmissionChannel()->calculateDuration( ifgmsg );
//                            EV << "SimTime: " << simTime() << " TransmissionFinishTime: " << finishTime << endl;
                        }

//                        EV << "[ Sending " << simTime() << " ] " << parentSwitch->getParentModule()->getFullName() << " - Message " << msg->getCreationTime() << " to Gate " << schedID << " - Finished at " << finishTime << "  | Popping from Queue: " << currentQueueName << endl << endl;


                        /* Tell the switch to send the message now */
                        sendMessage( msg, selectedOutGate );

                        /* LOGGING */
                        cOutVector* currentQueueSizeVector = check_and_cast<cOutVector*>( queueVectors->get( i ) );
                        currentQueueSizeVector->record( queueSizes[i]-- );
                    }
                    else
                    {
//                        EV << "[ !! ] Transmission channel: " << selectedNic->getPhysOutGate()->getTransmissionChannel()->getFullName() << " currently busy. Have to reschedule frame transmission." << endl;
                        /* Channel is currently busy. Have to reschedule the frame. */
//                        simtime_t finishTime = selectedNic->getPhysOutGate()->getTransmissionChannel()->getTransmissionFinishTime();
//                        EV << "[ !! ] Rescheduling time is: " << finishTime << endl;

//                        EV << "[ Resched " << simTime() << " ] " << parentSwitch->getParentModule()->getFullName() << " at " << finishTime << " on Gate " << schedID << endl << endl;

                        /*
                         * EXPRESS HANDLING
                         */

//                        parentModule->scheduleMessage( finishTime, schedID );
                        //break;
                    }

                    /*
                     * processed one filled or all empty queues -> done
                     */
                    return;
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

void Scheduler::sendMessage( cMessage* msg, cGate* outGate )
{
    parentSwitch->send( msg, outGate );
    return;
}
