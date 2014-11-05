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
#include "EndNodeSwitch.h"
#include "hsrMessage_m.h"
#include "prpMessage_m.h"
#include "EtherFrame_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"

Define_Module( EndNodeScheduler );


EndNodeScheduler::EndNodeScheduler()
{
}

EndNodeScheduler::~EndNodeScheduler()
{
}


void EndNodeScheduler::enqueueMessage( cMessage *msg )
{
    EthernetIIFrame *ethernetFrame = check_and_cast<EthernetIIFrame *> (msg);
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;
    cGate* arrivalGate = msg->getArrivalGate();
    EndNodeSwitch* parentModule = ( EndNodeSwitch* )getParentModule();

    framePriority frameprio = LOW;

    schedulerMode schedmode = Scheduler::getSchedmode();
    cArray* queues = Scheduler::getQueues();

    MessagePacker::openMessage(&ethernetFrame, &vlanTag, &hsrTag, &messageData);

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
                    ( check_and_cast<cQueue *>(queues->get(EXPRESS_INTERNAL)) )->insert(msg);
                    break;
                }

                case HIGH:
                {
                    ( check_and_cast<cQueue *>(queues->get(HIGH_INTERNAL)) )->insert(msg);
                    break;
                }

                default:
                {
                    ( check_and_cast<cQueue *>(queues->get(LOW_INTERNAL)) )->insert(msg);
                    Scheduler::setQueueSizeLowInt(Scheduler::getQueueSizeLowInt()+1);
                    Scheduler::getQueueLowIntVector()->record(Scheduler::getQueueSizeLowInt());
                    EV << "Queue Size (" << simTime() << "): " << Scheduler::getQueueSizeLowInt() << endl;
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
                    if ((arrivalGate == parentModule->getGateAIn()) || (arrivalGate == parentModule->getGateBIn() ))
                    {
                        ( check_and_cast<cQueue *>(queues->get(EXPRESS_RING)) )->insert(msg);
                    }
                    else if (arrivalGate == parentModule->getGateCpuIn() ) {
                        ( check_and_cast<cQueue *>(queues->get(EXPRESS_INTERNAL)) )->insert(msg);
                    }
                    break;
                }

                case HIGH:
                {
                    if ((arrivalGate == parentModule->getGateAIn()) || (arrivalGate == parentModule->getGateBIn() ))
                    {
                        ( check_and_cast<cQueue *>(queues->get(HIGH_RING)) )->insert(msg);
                    }
                    else if (arrivalGate == parentModule->getGateCpuIn() ) {
                        ( check_and_cast<cQueue *>(queues->get(HIGH_INTERNAL)) )->insert(msg);
                    }
                    break;
                }

                case LOW:
                default:
                {
                    if ((arrivalGate == parentModule->getGateAIn()) || (arrivalGate == parentModule->getGateBIn() ))
                    {
                        ( check_and_cast<cQueue *>(queues->get(LOW_RING)) )->insert(msg);
                    }
                    else if (arrivalGate == parentModule->getGateCpuIn() ) {
                        ( check_and_cast<cQueue *>(queues->get(LOW_INTERNAL)) )->insert(msg);
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


void EndNodeScheduler::processQueues( void )
{
    schedulerMode schedmode = Scheduler::getSchedmode();
    cArray* queues = Scheduler::getQueues();

    switch( schedmode )
    {
        case FCFS:
        {
//            ( ( cQueue* )queues->get(EXPRESS_RING) );
//            ( ( cQueue* )queues->get(EXPRESS_INTERNAL) );
//            ( ( cQueue* )queues->get(HIGH_RING) );
//            ( ( cQueue* )queues->get(HIGH_INTERNAL) );
//            ( ( cQueue* )queues->get(LOW_RING) );
//            ( ( cQueue* )queues->get(LOW_INTERNAL) );

            for (int i=0; i<queues->size(); i++) {
                cQueue* currentQueue = ( ( cQueue* )queues->get(static_cast<schedulerMode>(i)) );
                while (!currentQueue->isEmpty())
                {
                    cMessage* msg = check_and_cast<cMessage*>(currentQueue->pop());
                    Scheduler::setQueueSizeLowInt(Scheduler::getQueueSizeLowInt()-1);
                    Scheduler::getQueueLowIntVector()->record(Scheduler::getQueueSizeLowInt());
                    EV << "Queue Size (" << simTime() << "): " << Scheduler::getQueueSizeLowInt() << endl;
                    forwardFrame(msg);
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

