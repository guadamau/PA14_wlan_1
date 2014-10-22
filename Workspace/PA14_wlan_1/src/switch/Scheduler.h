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

/* Enum type definitions */
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

private:
    cArray* queues;
    schedulerMode schedmode;

    void initScheduler( void );

public:
    Scheduler( schedulerMode schedmode );
    virtual ~Scheduler();

    /* Abstract methods */
    virtual void enqueueMessage( cMessage *msg ) = 0;
    virtual void processQueues() = 0;

    /* Getters */
    const cArray*& getQueues() const;
    schedulerMode getSchedmode() const;
};

#endif /* SCHEDULER_H_ */
