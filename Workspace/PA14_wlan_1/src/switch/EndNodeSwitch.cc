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

#include "EndNodeSwitch.h"
#include "schedulerSelfMessage_m.h"

Define_Module( EndNodeSwitch );


nodeTable* EndNodeSwitch::getNodeTable()
{
    cModule *calleeModule = getParentModule();

    if( calleeModule != NULL )
    {
        /* TODO: Namen als Parameter übergeben */
        calleeModule = calleeModule->getSubmodule( "nodeTable" );
        return check_and_cast<nodeTable*>( calleeModule );
    }

    return NULL;
}


void EndNodeSwitch::initialize()
{
    /* Call initialize of the base class. */
    HsrSwitch::initialize();

    this->endNodeTable = new nodeTable();
}

EndNodeSwitch::EndNodeSwitch()
{
}

EndNodeSwitch::~EndNodeSwitch()
{
    /*
     * Don't know what the hell omnet++ does in the background with its modules.
     * "Generally" the omnet++ kernel cleans up a network after running.
     * So we do not manually delete referenced mebers here as we should in "normal c++".
     * Otherwise a failure will be thrown in omnet++s "network-cleanup".
     * This is wtf.
     */
}

void EndNodeSwitch::handleMessage( cMessage* msg )
{
    /* Schedulers */
    Scheduler* schedGateAOut = HsrSwitch::getSchedGateAOut();
    Scheduler* schedGateBOut = HsrSwitch::getSchedGateBOut();
    Scheduler* schedGateCpuOut = HsrSwitch::getSchedGateCpuOut();

    if( msg->isSelfMessage() )
    {
        if( typeid( *msg ) == typeid( HsrSwitchSelfMessage ) )
        {
            HsrSwitchSelfMessage* selfmsg = check_and_cast<HsrSwitchSelfMessage*>(msg);
            unsigned char schedname = selfmsg->getSchedulerName();
            unsigned char type = selfmsg->getType();

            if( schedname == 'A' ) {
                if( type == 'o' )
                {
                    schedGateAOut->resetNotifiedToken();
                }
                else if ( type == 'i' )
                {
                    schedGateAOut->resetNotifiedTimeslot();
                }
                schedGateAOut->processQueues();
            }
            else if( schedname == 'B' ) {
                if( type == 'o' )
                {
                    schedGateBOut->resetNotifiedToken();
                }
                else if ( type == 'i' )
                {
                    schedGateBOut->resetNotifiedTimeslot();
                }
                schedGateBOut->processQueues();
            }
            else if( schedname == 'C' ) {
                if( type == 'o' )
                {
                    schedGateCpuOut->resetNotifiedToken();
                }
                else if ( type == 'i' )
                {
                    schedGateCpuOut->resetNotifiedTimeslot();
                }
                schedGateCpuOut->processQueues();
            }
            else {
                delete selfmsg;
                throw cRuntimeError( "Scheduler not found for reprocessing! \n" );
            }

            delete selfmsg;
            return;
        }
    }

    /* Gates */
    cGate* gateAIn = HsrSwitch::getGateAIn();
    cGate* gateBIn = HsrSwitch::getGateBIn();
    cGate* gateCpuIn = HsrSwitch::getGateCpuIn();

    cGate* gateAInExp = HsrSwitch::getGateAInExp();
    cGate* gateBInExp = HsrSwitch::getGateBInExp();
    cGate* gateCpuInExp = HsrSwitch::getGateCpuInExp();


    /* Make a clone of the frame and take ownership.
     * Then we have to downcast the Message as an ethernet frame. */
    cMessage* switchesMsg = msg;
    this->take( switchesMsg );


    if( typeid( *msg ) == typeid( SchedulerSelfMessage ) )
    {
        /*
         * Check if endIFGMsg
         */
        if( msg->getArrivalGate() == gateAIn )
        {
            schedGateAOut->unlock();
            schedGateAOut->processQueues();
        }
        else if ( msg->getArrivalGate() == gateAInExp )
        {
            schedGateAOut->unlockExp();
            schedGateAOut->getSendingStatus()->updateSendtime( simTime() );
            schedGateAOut->processQueues();
        }
        else if ( msg->getArrivalGate() == gateBIn )
        {
            schedGateBOut->unlock();
            schedGateBOut->processQueues();
        }
        else if ( msg->getArrivalGate() == gateBInExp )
        {
            schedGateBOut->unlockExp();
            schedGateBOut->getSendingStatus()->updateSendtime( simTime() );
            schedGateBOut->processQueues();
        }
        else
        {
            throw cRuntimeError( "Could not find a valid arrival gate for Scheduler-Self-Message.\n" );
        }
        delete msg;
        return;
    }



    /* Arrival Gate */
    cGate* arrivalGate = switchesMsg->getArrivalGate();

    /* Switch mac address. */
    MACAddress switchMacAddress = *( HsrSwitch::getMacAddress() );

    EthernetIIFrame* ethernetFrame = check_and_cast<EthernetIIFrame*>( switchesMsg );

    /* Source and destination mac addresses */
    MACAddress frameDestination = ethernetFrame->getDest();
    MACAddress frameSource = ethernetFrame->getSrc();

    EthernetIIFrame* ethTag = NULL;
    vlanMessage* vlanTag = NULL;
    hsrMessage* hsrTag = NULL;
    dataMessage* messageData = NULL;

    EthernetIIFrame* frameToDeliver = NULL;
    EthernetIIFrame* frameToDeliverClone = NULL;

    framePriority frameprio = LOW;

    MessagePacker::decapsulateMessage( &ethernetFrame, &vlanTag, &hsrTag, &messageData );

    /* After decapsulating the whole ethernet frame becomes a ethernet tag
     * (only the header of an ethernet frame)
     * ethTag variable just there for a better understanding. */
    ethTag = ethernetFrame;

    /* determine package prio and set enum */
    if( vlanTag != NULL )
    {
        if( vlanTag->getUser_priority() == EXPRESS )
        {
            frameprio = EXPRESS;
        }
        else if( vlanTag->getUser_priority() == HIGH )
        {
            frameprio = HIGH;
        }
    }
    else
    {
        EV << "No vlan for this frame. Unable to determine frames priority." << endl;
    }



    /* If the following criteria is not checked,
     * the ring can be flooded with
     * broadcast / multicast traffic.
     * Also circulating unicast frames will
     * never be destroyed. */
    if( switchMacAddress == frameSource &&
      ( arrivalGate != gateCpuIn && arrivalGate != gateCpuInExp ) )
    {
        /*
         * TODO: What to do with broadcast frames?
         */

        /* If it is a circulating multicast frame,
         * we have to send it to the cpu and log it. */
        if( frameDestination.isMulticast() || /* Multicast frame circulated once in the ring */
            ( switchMacAddress == frameSource || /* Circulating frame is unicast or unspecified */
              ( !frameDestination.isBroadcast() && !frameDestination.isMulticast() ) ) )
        {
            frameToDeliver = hsrTagReceiveFromRingRoutine( ethTag, vlanTag, hsrTag, messageData, arrivalGate );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateCpuOut->enqueueMessage( frameToDeliver, EXPRESS_INTERNAL );
                    break;
                }
                case HIGH:
                {
                    schedGateCpuOut->enqueueMessage( frameToDeliver, HIGH_INTERNAL );
                    break;
                }
                default:
                {
                    schedGateCpuOut->enqueueMessage( frameToDeliver, LOW_INTERNAL );
                    break;
                }
            }
            scheduleProcessQueues('C');
        }
        else
        {
            /* Should never come here ... */

            /* Break the circulation and drop the frame at this point. */
            EV << "ATTENTION: Circulating Frame in the HSR-Ring. Frame is going to be dropped." << endl;
            delete msg;
        }
    }

    /* UNICAST TRAFFIC TO ME
     *
     * unicast traffic. frame only for me. */
    else if( switchMacAddress == frameDestination )
    {
        if( arrivalGate == gateAIn || arrivalGate == gateAInExp ||
            arrivalGate == gateBIn || arrivalGate == gateBInExp )
        {
            frameToDeliver = hsrTagReceiveFromRingRoutine( ethTag, vlanTag, hsrTag, messageData, arrivalGate );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateCpuOut->enqueueMessage( frameToDeliver, EXPRESS_INTERNAL );
                    break;
                }
                case HIGH:
                {
                    schedGateCpuOut->enqueueMessage( frameToDeliver, HIGH_INTERNAL );
                    break;
                }
                default:
                {
                    schedGateCpuOut->enqueueMessage( frameToDeliver, LOW_INTERNAL );
                    break;
                }
            }
            scheduleProcessQueues('C');
        }
        else if( arrivalGate == gateCpuIn || arrivalGate == gateCpuInExp )
        {
            EV << "Trying to send a self message within the HSR-Switch. Deleting frame ..." << endl;
            delete msg;
        }
        else
        {
            /* wrong gate */
            delete msg;
            throw cRuntimeError( "Got message from unknown gate! P A N I C ! \n" );
            endSimulation();
        }
    } /* End of handling unicast traffic to me. */

    /* BROADCAST AND MULTICAST TRAFFIC
     *
     * handle broadcast or multicast traffic */
    else if( frameDestination.isBroadcast() || frameDestination.isMulticast() )
    {
        if( arrivalGate == gateCpuIn || arrivalGate == gateCpuInExp )
        {
            frameToDeliver = hsrTagSendToRingRoutine( ethTag, vlanTag, hsrTag, messageData );
            frameToDeliverClone = frameToDeliver->dup();

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, EXPRESS_INTERNAL );
                    schedGateBOut->enqueueMessage( frameToDeliverClone, EXPRESS_INTERNAL );
                    break;
                }
                case HIGH:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, HIGH_INTERNAL );
                    schedGateBOut->enqueueMessage( frameToDeliverClone, HIGH_INTERNAL );
                    break;
                }
                default:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, LOW_INTERNAL );
                    schedGateBOut->enqueueMessage( frameToDeliverClone, LOW_INTERNAL );
                    break;
                }
            }
            scheduleProcessQueues('A');
            scheduleProcessQueues('B');
        }
        else if( arrivalGate == gateAIn || arrivalGate == gateAInExp )
        {
            frameToDeliver = hsrTagReceiveFromRingRoutine( ethTag, vlanTag, hsrTag, messageData, arrivalGate );
            frameToDeliverClone = frameToDeliver->dup();

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateBOut->enqueueMessage( frameToDeliver, EXPRESS_RING );
                    schedGateCpuOut->enqueueMessage( frameToDeliverClone, EXPRESS_RING );
                    break;
                }
                case HIGH:
                {
                    schedGateBOut->enqueueMessage( frameToDeliver, HIGH_RING );
                    schedGateCpuOut->enqueueMessage( frameToDeliverClone, HIGH_RING );
                    break;
                }
                default:
                {
                    schedGateBOut->enqueueMessage( frameToDeliver, LOW_RING );
                    schedGateCpuOut->enqueueMessage( frameToDeliverClone, LOW_RING );
                    break;
                }
            }
            scheduleProcessQueues('B');
            scheduleProcessQueues('C');
        }
        else if( arrivalGate == gateBIn || arrivalGate == gateBInExp )
        {
            frameToDeliver = hsrTagReceiveFromRingRoutine( ethTag, vlanTag, hsrTag, messageData, arrivalGate );
            frameToDeliverClone = frameToDeliver->dup();

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, EXPRESS_RING );
                    schedGateCpuOut->enqueueMessage( frameToDeliverClone, EXPRESS_RING );
                    break;
                }
                case HIGH:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, HIGH_RING );
                    schedGateCpuOut->enqueueMessage( frameToDeliverClone, HIGH_RING );
                    break;
                }
                default:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, LOW_RING );
                    schedGateCpuOut->enqueueMessage( frameToDeliverClone, LOW_RING );
                    break;
                }
            }
            scheduleProcessQueues('A');
            scheduleProcessQueues('C');
        }
        else
        {
            /* wrong gate */
            delete msg;
            throw cRuntimeError( "Got message from unknown gate! P A N I C ! \n" );
            endSimulation();
        }
    } /* End of handle broadcast or multicast traffic */

    /* FORWARD IN THE RING / SEND TO RING
     * UNICAST FRAMES
     *
     * Non multicast, non broadcast traffic.
     * Frame is not for me, so we have to forward it. */
    else
    {
        if( arrivalGate == gateCpuIn || arrivalGate == gateCpuInExp )
        {
            frameToDeliver = hsrTagSendToRingRoutine( ethTag, vlanTag, hsrTag, messageData );
            frameToDeliverClone = frameToDeliver->dup();

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, EXPRESS_INTERNAL );
                    schedGateBOut->enqueueMessage( frameToDeliverClone, EXPRESS_INTERNAL );
                    break;
                }
                case HIGH:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, HIGH_INTERNAL );
                    schedGateBOut->enqueueMessage( frameToDeliverClone, HIGH_INTERNAL );
                    break;
                }
                default:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, LOW_INTERNAL );
                    schedGateBOut->enqueueMessage( frameToDeliverClone, LOW_INTERNAL );
                    break;
                }
            }
            scheduleProcessQueues('A');
            scheduleProcessQueues('B');

        }
        else if( arrivalGate == gateAIn || arrivalGate == gateAInExp )
        {
            frameToDeliver = hsrTagReceiveFromRingRoutine( ethTag, vlanTag, hsrTag, messageData, arrivalGate );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateBOut->enqueueMessage( frameToDeliver, EXPRESS_RING );
                    break;
                }
                case HIGH:
                {
                    schedGateBOut->enqueueMessage( frameToDeliver, HIGH_RING );
                    break;
                }
                default:
                {
                    schedGateBOut->enqueueMessage( frameToDeliver, LOW_RING );
                    break;
                }
            }
            scheduleProcessQueues('B');
        }
        else if( arrivalGate == gateBIn || arrivalGate == gateBInExp )
        {
            frameToDeliver = hsrTagReceiveFromRingRoutine( ethTag, vlanTag, hsrTag, messageData, arrivalGate );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, EXPRESS_RING );
                    break;
                }
                case HIGH:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, HIGH_RING );
                    break;
                }
                default:
                {
                    schedGateAOut->enqueueMessage( frameToDeliver, LOW_RING );
                    break;
                }
            }
            scheduleProcessQueues('A');
        }
        else
        {
            /* wrong gate */
            delete msg;
            throw cRuntimeError( "Got message from unknown gate! P A N I C ! \n" );
            endSimulation();
        }
    } /* End of handling frames to forward in the ring. */
}


EthernetIIFrame*
EndNodeSwitch::hsrTagSendToRingRoutine( EthernetIIFrame* ethTag, vlanMessage* vlanTag, hsrMessage* hsrTag, dataMessage* messageData )
{

    EthernetIIFrame* frameReadyToProcess = NULL;

    /*
     * Deleted some lines here. Check earlier revisions
     * for more information.
     * Lines became deprecated due to generating
     * a HSR-Tag for every frame created by a CPU.
     * */

    frameReadyToProcess = MessagePacker::generateEthMessage( ethTag, vlanTag, hsrTag, messageData );

    return frameReadyToProcess;
}

EthernetIIFrame*
EndNodeSwitch::hsrTagReceiveFromRingRoutine( EthernetIIFrame* ethTag, vlanMessage* vlanTag, hsrMessage* hsrTag, dataMessage* messageData, cGate* arrivalGate )
{
    cGate* gateAIn = HsrSwitch::getGateAIn();
    cGate* gateAInExp = HsrSwitch::getGateAInExp();

    EthernetIIFrame* frameReadyToProcess = NULL;

    /* Non-HSR-tagged frame */
    if( hsrTag == NULL )
    {
        /* Register the source in its node table as non-HSR node */
        if( arrivalGate == gateAIn || arrivalGate == gateAInExp )
        {
            endNodeTable->pushNode( ethTag->getSrc(), PORTNAME_A, NODETYPE_SAN );
        }
        else
        {
            endNodeTable->pushNode( ethTag->getSrc(), PORTNAME_B, NODETYPE_SAN );
        }
    }
    /* HSR-tagged frame */
    else
    {
        /* Register the source in its node table as HSR node */
        if( arrivalGate == gateAIn || arrivalGate == gateAInExp )
        {
            endNodeTable->pushNode( ethTag->getSrc(), PORTNAME_A, NODETYPE_HSR );
        }
        else
        {
            endNodeTable->pushNode( ethTag->getSrc(), PORTNAME_B, NODETYPE_HSR );
        }
    }

    frameReadyToProcess = MessagePacker::generateEthMessage( ethTag, vlanTag, hsrTag, messageData );

    return frameReadyToProcess;
}
