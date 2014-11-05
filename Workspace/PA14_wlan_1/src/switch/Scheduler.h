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

typedef enum
{
    EXPRESS_RING,
    EXPRESS_INTERNAL,
    HIGH_RING,
    HIGH_INTERNAL,
    LOW_RING,
    LOW_INTERNAL
} queueName;

typedef enum
{
    FCFS,
    RING_FIRST,
    ZIPPER,
    TOKENS
} schedulerMode;


class Scheduler : public cSimpleModule {

private:
    cArray* queues;
    schedulerMode schedmode;

    cGate* schedOutGate;

    /* Members to record statistics. */
    cOutVector* queueLowIntVector;
    unsigned long int queuesize_low_int;

public:
    Scheduler();
    virtual ~Scheduler();

    void initScheduler( schedulerMode schedmode, cGate* schedOutGate );

    /* Main sched methods ... */
    void enqueueMessage( cMessage* msg );
    void processQueues( void );

    /* Getters */
    cArray* getQueues( void );
    schedulerMode getSchedmode( void );

    unsigned long int getQueueSizeLowInt( void );
    cOutVector* getQueueLowIntVector( void );

    /* Setters */
    void setSchedmode( schedulerMode schedmode );
    void setQueueSizeLowInt( unsigned long nr );

};

#endif /* SCHEDULER_H_ */
