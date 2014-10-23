//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "EndNodeScheduler.h"

#include <omnetpp.h>
#include "MessagePacker.h"
#include "hsrMessage_m.h"
#include "prpMessage_m.h"
#include "EtherFrame_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"


EndNodeScheduler::EndNodeScheduler( schedulerMode schedmode ) : Scheduler( schedmode )
{

}


EndNodeScheduler::~EndNodeScheduler()
{
}


void EndNodeScheduler::enqueueMessage( cMessage *msg, HsrSwitch* parentModule )
{
    EthernetIIFrame *ethernetFrame = check_and_cast<EthernetIIFrame *> (msg);
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;
    cGate* arrivalGate = msg->getArrivalGate();

    framePriority frameprio = LOW;

    schedulerMode schedmode = Scheduler::getSchedmode();
    cArray* queues = Scheduler::getQueues();

    MessagePacker::decapsulateMessage(&ethernetFrame, &vlanTag, &hsrTag, &messageData);



    // determine package prio and set enum
    if(ethernetFrame->getEtherType() == 0x8500)
    {
        frameprio = EXPRESS;
    }
    else if (vlanTag->getUser_priority() == HIGH)
    {
        frameprio = HIGH;
    }

    switch( schedmode )
    {
        ////////////////////////////////////////
        // FIRST COME FIRST SERVE
        ////////////////////////////////////////
        case FCFS:
        {
            /* Enqueue a message according its priority. */
            switch( frameprio )
            {

                case EXPRESS:
                {
                    queues->get(EXPRESS_INTERNAL)->insert(msg);
                    break;
                }

                case HIGH:
                {
                    queues->get(HIGH_INTERNAL)->insert(msg);
                    break;
                }

                default:
                {
                    queues->get(LOW_INTERNAL)->insert(msg);
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
            switch( frameprio )
            {
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


void EndNodeScheduler::processQueues()
{
    schedulerMode schedmode = Scheduler::getSchedmode();

    switch( schedmode )
    {
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

