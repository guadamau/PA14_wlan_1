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

void EndNodeScheduler::forwardFrame(cMessage* msg) {
    cGate* arrivalGate = msg->getArrivalGate();

    EndNodeSwitch* parentSwitch = check_and_cast<EndNodeSwitch*> (getParentModule());

    cGate* gateAIn = parentSwitch->getGateAIn();
    cGate* gateBIn = parentSwitch->getGateBIn();
    cGate* gateCpuIn = parentSwitch->getGateCpuIn();

    EthernetIIFrame *ethTag = check_and_cast<EthernetIIFrame *> (msg);
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;

    MessagePacker::decapsulateMessage(&ethTag, &vlanTag, &hsrTag, &messageData);

    if ((arrivalGate == gateAIn) || (arrivalGate == gateBIn))
    {
        //5.3.3 DANH receiving from an HSR port
        recieveFromRing(&ethTag, &vlanTag, &hsrTag, &messageData);
    }
    else if (arrivalGate == gateCpuIn)
    {
        //5.3.2 DANH receiving from its link layer interface
        sendToRing(&ethTag, &vlanTag, &hsrTag, &messageData);
    }
    else
    {
        //from somewhere else
        MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
        throw cRuntimeError("Get message from unknown gate! Panik ! \n");
        endSimulation();
    }

    // MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
}

void EndNodeScheduler::sendToRing(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData)
{
    EndNodeSwitch* parentSwitch = check_and_cast<EndNodeSwitch*> (getParentModule());

    unsigned int ringID = parentSwitch->getRingId();
    unsigned int sequenceNum = parentSwitch->getSequenceNum();
    nodeTable* endNodeTable = parentSwitch->getNodeTable();

    cGate* gateAOut = parentSwitch->getGateAOut();
    cGate* gateBOut = parentSwitch->getGateBOut();


    //If this frame is HSR or the destination node has been registered as non-HSR
    if((*hsrTag != NULL)    or (endNodeTable->getNodeMode((*ethTag)->getSrc()) != NODETYPE_HSR))
    {
        //Do not modify the frame;
    }
    else //(non-HSR frame and destination node not registered as non-HSR)
    {
        //Insert the HSR tag with the sequence number of the host;
        *hsrTag = MessagePacker::createHSRTag("HSR", ringID, sequenceNum);
        //Increment the sequence number, wrapping through 0
        EndNodeSwitch* parentSwitch = check_and_cast<EndNodeSwitch*> (getParentModule());
        parentSwitch->setSequenceNum( sequenceNum++ );
    }

    //Duplicate the frame, enqueue it for sending into both HSR ports
    parentSwitch->send(MessagePacker::generateEthMessage(*ethTag, *vlanTag, *hsrTag, *messageData), gateAOut);
    parentSwitch->send(MessagePacker::generateEthMessage(*ethTag, *vlanTag, *hsrTag, *messageData), gateBOut);
}

void EndNodeScheduler::recieveFromRing(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData)
{
    cGate* arrivalGate = (*ethTag)->getArrivalGate();
    cGate* tempOutGate;
    EndNodeSwitch* parentSwitch = check_and_cast<EndNodeSwitch*> (getParentModule());
    cGate* gateAIn = parentSwitch->getGateAIn();
    cGate* gateAOut = parentSwitch->getGateAOut();
    cGate* gateBOut = parentSwitch->getGateBOut();
    cGate* gateCpuOut = parentSwitch->getGateCpuOut();
    MACAddress macAddress = *( parentSwitch->getMacAddress() );
    nodeTable* endNodeTable = parentSwitch->getNodeTable();

     if (arrivalGate == gateAIn)
     {
         tempOutGate = gateBOut;
     }
     else
     {
         tempOutGate = gateAOut;
     }


    //If this frame is not HSR-tagged:
    if((*hsrTag) == NULL)
    {
        //Register the source in its node table as non-HSR node;
        if (arrivalGate == gateAIn)
        {
            endNodeTable->pushNode((*ethTag)->getSrc(), PORTNAME_A, NODETYPE_SAN);
        }
        else
        {
            endNodeTable->pushNode((*ethTag)->getSrc(), PORTNAME_B, NODETYPE_SAN);
        }

        if(((macAddress == (*ethTag)->getDest()) || ((*ethTag)->getDest().isBroadcast()) || ((*ethTag)->getDest().isMulticast())) && (macAddress != (*ethTag)->getSrc()))
        {
            parentSwitch->send(MessagePacker::generateEthMessage((*ethTag), (*vlanTag), (*hsrTag), (*messageData)), gateCpuOut);
        }
        //(the frame is not forwarded)
    }
    else //(HSR-tagged frame)
    {
        //Register the source in its node table as HSR node
        if (arrivalGate == gateAIn)
            endNodeTable->pushNode((*ethTag)->getSrc(), PORTNAME_A, NODETYPE_HSR);
        else
            endNodeTable->pushNode((*ethTag)->getSrc(), PORTNAME_B, NODETYPE_HSR);
        //If this node is the (unicast or multicast) destination
        if(((macAddress == (*ethTag)->getDest()) || ((*ethTag)->getDest().isBroadcast()) || ((*ethTag)->getDest().isMulticast())) && (macAddress != (*ethTag)->getSrc()))
        {
            parentSwitch->send(MessagePacker::generateEthMessage((*ethTag), (*vlanTag), (*hsrTag), (*messageData)), gateCpuOut);
        }
        else //(if this node is not a destination)
        {
            //Do not send it over the link layer interface
        }
        //If this node is not the only destination (multicast or unicast for another node)
        if( macAddress != (*ethTag)->getDest() )
        {
            parentSwitch->send(MessagePacker::generateEthMessage((*ethTag), (*vlanTag), (*hsrTag), (*messageData)), tempOutGate);
        }
        else//(If this node is the only (unicast) destination)
        {
            //Discard the frame
            delete *ethTag;
        }
    }
}

