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

class HsrSwitch;

typedef enum
{
    FCFS,
    RING_FIRST,
    ZIPPER,
    TOKENS
} schedulerMode;

class Scheduler {

private:
    cArray* queues;
    unsigned long int queueSizes[QUEUES_COUNT];
    cArray* queueVectors;
    schedulerMode schedmode;
    HsrSwitch* parentSwitch;

    simtime_t finishTime;

    cGate* schedOutGate;
    cGate* schedOutGateExp;

    NetworkInterfaceCard* schedNic;
    NetworkInterfaceCard* schedNicExp;

    unsigned char schedID; // GateA = A, GateB = B, GateCPU = C, GateInterlink = I

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

    /* Setters */
    void setSchedmode( schedulerMode schedmode );

protected:
    virtual void handleMessage( cMessage *msg );
    void sendMessage( cMessage* msg, cGate* outGate );

};

#endif /* SCHEDULER_H_ */
