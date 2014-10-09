/*
 * Scheduler.cc
 *
 *  Created on: Oct 9, 2014
 *      Author: guadagnini
 */

#include "Scheduler.h"

Scheduler::Scheduler(schedulerMode schedmode) {
    // TODO Auto-generated constructor stub
    this->queues = new cArray();
    this->queues->setCapacity(6);
    this->schedmode = schedmode;
}

Scheduler::~Scheduler() {
    // TODO Auto-generated destructor stub
    delete queues;
}

void
Scheduler::processQueues() {
    switch (schedmode) {
        case FCFS:
        {
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

void
Scheduler::enqueueMessage(cMessage *msg)
{
    EthernetIIFrame *ethernetFrame = check_and_cast<EthernetIIFrame *> (msg);
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;
    cGate* arrivalGate = msg->getArrivalGate();

    framePriority frameprio = LOW;

    MessagePacker::decapsulateMessage(&ethernetFrame, &vlanTag, &hsrTag, &messageData);

    queues->addAt(queueName.EXPRESS_INTERNAL, new cQueue());
    queues->addAt(queueName.HIGH_INTERNAL, new cQueue());
    queues->addAt(queueName.LOW_INTERNAL, new cQueue());

    // determine package prio and set enum
    if(ethernetFrame->getEtherType() == 0x8500)
    {
        frameprio = EXPRESS;
    }
    else if (vlanTag->getUser_priority() == frameprio.HIGH)
    {
        frameprio = HIGH;
    }

    switch(schedmode) {

        ////////////////////////////////////////
        // FIRST COME FIRST SERVE
        ////////////////////////////////////////
        case FCFS:
        {

            switch(frameprio) {

                case EXPRESS:
                {
                    queues->get(queueName.EXPRESS_INTERNAL)->insert(msg);
                    break;
                }

                case HIGH:
                {
                    queues->get(queueName.HIGH_INTERNAL)->insert(msg);
                    break;
                }

                default:
                {
                    queues->get(queueName.LOW_INTERNAL)->insert(msg);
                    break;
                }

            }

            break;
        }

        ////////////////////////////////////////
        // RING FIRST, ZIPPER & TOKENS
        ////////////////////////////////////////
        case RING_FIRST:
        case ZIPPER:
        case TOKENS:
        {
            queues->addAt(queueName.EXPRESS_RING, new cQueue());
            queues->addAt(queueName.HIGH_RING, new cQueue());
            queues->addAt(queueName.LOW_RING, new cQueue());

            switch(frameprio) {

                case EXPRESS:
                {
                    if ((arrivalGate == getParentModule()->getGateAIn()) || (arrivalGate == getParentModule()->getGateBIn))
                    {
                        queues->get(queueName.EXPRESS_RING)->insert(msg);
                    }
                    else if (arrivalGate == gateCpuIn) {
                        queues->get(queueName.EXPRESS_INTERNAL)->insert(msg);
                    }
                    break;
                }

                case HIGH:
                {
                    if ((arrivalGate == getParentModule()->getGateAIn()) || (arrivalGate == getParentModule()->getGateBIn))
                    {
                        queues->get(queueName.HIGH_RING)->insert(msg);
                    }
                    else if (arrivalGate == gateCpuIn) {
                        queues->get(queueName.HIGH_INTERNAL)->insert(msg);
                    }
                    break;
                }

                case LOW:
                default:
                {
                    if ((arrivalGate == getParentModule()->getGateAIn()) || (arrivalGate == getParentModule()->getGateBIn))
                    {
                        queues->get(queueName.LOW_RING)->insert(msg);
                    }
                    else if (arrivalGate == gateCpuIn) {
                        queues->get(queueName.LOW_INTERNAL)->insert(msg);
                    }
                    break;
                }

            }

            break;
        }

        default:
        {
            throw cRuntimeError("No Schedulerbehavior set!\n");
            break;
        }

    }


}

