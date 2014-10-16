/*
 * Scheduler.h
 *
 *  Created on: Oct 9, 2014
 *      Author: guadagnini
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

typedef enum
{
    EXPRESS,
    HIGH,
    LOW
} framePriority;

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
public:
    Scheduler( schedulerMode schedmode );
    virtual ~Scheduler();

protected:
    virtual void enqueueMessage( cMessage *msg );
    virtual void processQueues();

private:
    cArray* queues;
    schedulerMode schedmode;
};

#endif /* SCHEDULER_H_ */
