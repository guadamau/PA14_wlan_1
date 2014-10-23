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

#include "hsrDefines.h"
#include "HsrSwitch.h"

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


class Scheduler {

private:
    cArray* queues;
    schedulerMode schedmode;

    void initScheduler( void );

public:
    Scheduler( schedulerMode schedmode );
    virtual ~Scheduler();

    /* Abstract methods */
    virtual void enqueueMessage( cMessage* msg, HsrSwitch* parentModule ) = 0;
    virtual void processQueues( HsrSwitch* parentModule ) = 0;

    /* Getters */
    cArray* getQueues( void );
    schedulerMode getSchedmode( void );
};

#endif /* SCHEDULER_H_ */
