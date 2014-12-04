/*
 * Scheduler.h
 *
 *  Created on: Oct 9, 2014
 *      Author: guadagnini
 *
 *  This class represents an
 *  "abstract class" of the scheduler.
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <omnetpp.h>

#include "hsrDefines.h"
#include "NetworkInterfaceCard.h"
#include "SendingStatus.h"

class HsrSwitch;

typedef enum
{
    FCFS,
    RING_FIRST,
    INTERNAL_FIRST,
    ZIPPER,
    TOKENS
} schedulerMode;

class Scheduler {

private:
    cArray* queues;
    unsigned long int queueSizes[ QUEUES_COUNT ];
    cArray* queueVectors;
    schedulerMode schedmode;
    HsrSwitch* parentSwitch;

    unsigned long int availableTokens;

    simtime_t finishTime;

    cGate* schedOutGate;
    cGate* schedOutGateExp;

    NetworkInterfaceCard* schedNic;
    NetworkInterfaceCard* schedNicExp;

    unsigned char schedID; // GateA = A, GateB = B, GateCPU = C, GateInterlink = I

    queueName ringFirstSortOrder[ QUEUES_COUNT ];
    queueName internalFirstSortOrder[ QUEUES_COUNT ];

    /* To alternate between Ring and Internal (Zipper mechanism) */
    queueState curQueueState;

    /* Sending Status to determine frame preemtion */
    SendingStatus* sendingStatus;

    void sendMessage( cMessage* msg, cGate* outGate );
    void processOneQueue( cQueue* currentQueue, queueName currentQueueName );
    void loopQueues( queueName* currentSortOrder );
    simtime_t getExpressSendTime( void );
    framePriority getMessagePriority( cMessage* msg );

public:
    Scheduler();
    virtual ~Scheduler();

    void initScheduler( unsigned char schedID, HsrSwitch* parentSwitch, schedulerMode schedmode,
                        cGate* schedOutGate, cGate* schedOutGateExp,
                        NetworkInterfaceCard* schedNic, NetworkInterfaceCard* schedNicExp );

    /* Main sched methods ... */
    void enqueueMessage( cMessage* msg, queueName queue );
    void processQueues( void );

    /* Getters */
    cArray* getQueues( void );
    schedulerMode getSchedmode( void );
    SendingStatus* getSendingStatus( void );

    /* Setters */
    void setSchedmode( schedulerMode schedmode );

protected:
    virtual void handleMessage( cMessage *msg );

};

#endif /* SCHEDULER_H_ */
