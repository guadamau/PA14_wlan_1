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
    HsrSwitch::initialize("FCFS");

    endNodeTable = getNodeTable();
    if( endNodeTable == NULL )
    {
        throw cRuntimeError( "can't load node table" );
    }

    scheduleAt( SIMTIME_ZERO,  new HsrSwitchSelfMessage() );
}

EndNodeSwitch::EndNodeSwitch()
{
}

EndNodeSwitch::~EndNodeSwitch()
{
    if( endNodeTable != NULL )
    {
        delete endNodeTable;
    }
}


void EndNodeSwitch::handleMessage( cMessage *msg )
{
    /* Schedulers */
    Scheduler* schedGateAOut = HsrSwitch::getSchedGateAOut();
    Scheduler* schedGateBOut = HsrSwitch::getSchedGateBOut();
    Scheduler* schedGateCpuOut = HsrSwitch::getSchedGateCpuOut();

    /* Gates */
    cGate* gateAIn = HsrSwitch::getGateAIn();
    cGate* gateBIn = HsrSwitch::getGateBIn();
    cGate* gateCpuIn = HsrSwitch::getGateCpuIn();

    cGate* gateAInExp = HsrSwitch::getGateAInExp();
    cGate* gateBInExp = HsrSwitch::getGateBInExp();
    cGate* gateCpuInExp = HsrSwitch::getGateCpuInExp();

    /* Arrival Gate */
    cGate* arrivalGate = msg->getArrivalGate();

    /* Switch mac address. */
    MACAddress switchMacAddress = *( HsrSwitch::getMacAddress() );

    EthernetIIFrame* ethernetFrame = check_and_cast<EthernetIIFrame*>( msg );

    /* Source and destination mac addresses */
    MACAddress frameDestination = ethernetFrame->getDest();
    MACAddress frameSource = ethernetFrame->getSrc();


    vlanMessage* vlanTag = NULL;
    hsrMessage* hsrTag = NULL;
    dataMessage* messageData = NULL;

    framePriority frameprio = LOW;

    MessagePacker::openMessage( &ethernetFrame, &vlanTag, &hsrTag, &messageData );


    // determine package prio and set enum
    if( ethernetFrame->getEtherType() == 0x8500 )
    {
        frameprio = EXPRESS;
    }
    else if( vlanTag->getUser_priority() == HIGH )
    {
        frameprio = HIGH;
    }

    /* If the following criteria is not checked,
     * the ring can be flooded with
     * broadcast / multicast traffic.
     * Also circulating unicast frames will
     * never be destroyed. */
    if( switchMacAddress == frameSource )
    {
        EV << "ATTENTION: Circulating Frame in the HSR-Ring. Frame is going to be dropped." << endl;
        delete msg;
    }

    /* UNICAST TRAFFIC TO ME
     *
     * unicast traffic. frame only for me. */
    else if( switchMacAddress == frameDestination )
    {
        if( arrivalGate == gateAIn || arrivalGate == gateAInExp ||
            arrivalGate == gateBIn || arrivalGate == gateBInExp )
        {
            hsrTagReceiveFromRingRoutine( ethernetFrame, hsrTag, arrivalGate );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateCpuOut->enqueueMessage( msg, EXPRESS_INTERNAL );
                    break;
                }
                case HIGH:
                {
                    schedGateCpuOut->enqueueMessage( msg, HIGH_INTERNAL );
                    break;
                }
                default:
                {
                    schedGateCpuOut->enqueueMessage( msg, LOW_INTERNAL );
                    break;
                }
            }
            schedGateCpuOut->processQueues();
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
            hsrTagSendToRingRoutine( ethernetFrame, hsrTag );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateAOut->enqueueMessage( msg, EXPRESS_INTERNAL );
                    schedGateBOut->enqueueMessage( msg, EXPRESS_INTERNAL );
                    break;
                }
                case HIGH:
                {
                    schedGateAOut->enqueueMessage( msg, HIGH_INTERNAL );
                    schedGateBOut->enqueueMessage( msg, HIGH_INTERNAL );
                    break;
                }
                default:
                {
                    schedGateAOut->enqueueMessage( msg, LOW_INTERNAL );
                    schedGateBOut->enqueueMessage( msg, LOW_INTERNAL );
                    break;
                }
            }
            schedGateAOut->processQueues();
            schedGateBOut->processQueues();
        }
        else if( arrivalGate == gateAIn || arrivalGate == gateAInExp )
        {
            hsrTagReceiveFromRingRoutine( ethernetFrame, hsrTag, arrivalGate );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateBOut->enqueueMessage( msg, EXPRESS_RING );
                    schedGateCpuOut->enqueueMessage( msg, EXPRESS_RING );
                    break;
                }
                case HIGH:
                {
                    schedGateBOut->enqueueMessage( msg, HIGH_RING );
                    schedGateCpuOut->enqueueMessage( msg, HIGH_RING );
                    break;
                }
                default:
                {
                    schedGateBOut->enqueueMessage( msg, LOW_RING );
                    schedGateCpuOut->enqueueMessage( msg, LOW_RING );
                    break;
                }
            }
            schedGateBOut->processQueues();
            schedGateCpuOut->processQueues();
        }
        else if( arrivalGate == gateBIn || arrivalGate == gateBInExp )
        {
            hsrTagReceiveFromRingRoutine( ethernetFrame, hsrTag, arrivalGate );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateAOut->enqueueMessage( msg, EXPRESS_RING );
                    schedGateCpuOut->enqueueMessage( msg, EXPRESS_RING );
                    break;
                }
                case HIGH:
                {
                    schedGateAOut->enqueueMessage( msg, HIGH_RING );
                    schedGateCpuOut->enqueueMessage( msg, HIGH_RING );
                    break;
                }
                default:
                {
                    schedGateAOut->enqueueMessage( msg, LOW_RING );
                    schedGateCpuOut->enqueueMessage( msg, LOW_RING );
                    break;
                }
            }
            schedGateAOut->processQueues();
            schedGateCpuOut->processQueues();
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
            hsrTagSendToRingRoutine( ethernetFrame, hsrTag );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateAOut->enqueueMessage( msg, EXPRESS_INTERNAL );
                    schedGateBOut->enqueueMessage( msg, EXPRESS_INTERNAL );
                    break;
                }
                case HIGH:
                {
                    schedGateAOut->enqueueMessage( msg, HIGH_INTERNAL );
                    schedGateBOut->enqueueMessage( msg, HIGH_INTERNAL );
                    break;
                }
                default:
                {
                    schedGateAOut->enqueueMessage( msg, LOW_INTERNAL );
                    schedGateBOut->enqueueMessage( msg, LOW_INTERNAL );
                    break;
                }
            }
            schedGateAOut->processQueues();
            schedGateBOut->processQueues();

        }
        else if( arrivalGate == gateAIn || arrivalGate == gateAInExp )
        {
            hsrTagReceiveFromRingRoutine( ethernetFrame, hsrTag, arrivalGate );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateBOut->enqueueMessage( msg, EXPRESS_RING );
                    break;
                }
                case HIGH:
                {
                    schedGateBOut->enqueueMessage( msg, HIGH_RING );
                    break;
                }
                default:
                {
                    schedGateBOut->enqueueMessage( msg, LOW_RING );
                    break;
                }
            }
            schedGateBOut->processQueues();
        }
        else if( arrivalGate == gateBIn || arrivalGate == gateBInExp )
        {
            hsrTagReceiveFromRingRoutine( ethernetFrame, hsrTag, arrivalGate );

            switch( frameprio )
            {
                case EXPRESS:
                {
                    schedGateAOut->enqueueMessage( msg, EXPRESS_RING );
                    break;
                }
                case HIGH:
                {
                    schedGateAOut->enqueueMessage( msg, HIGH_RING );
                    break;
                }
                default:
                {
                    schedGateAOut->enqueueMessage( msg, LOW_RING );
                    break;
                }
            }
            schedGateAOut->processQueues();
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


void EndNodeSwitch::hsrTagSendToRingRoutine( EthernetIIFrame* ethernetFrame, hsrMessage* hsrTag )
{
    unsigned int ringID = HsrSwitch::getRingId();
    unsigned int sequenceNum = HsrSwitch::getSequenceNum();

    /* If this frame is HSR or
     * the destination node has been
     * registered as non-HSR */
    if( ( hsrTag != NULL ) ||
        ( endNodeTable->getNodeMode( ethernetFrame->getSrc() ) != NODETYPE_HSR ) )
    {
        /* Do not modify the frame. */
    }
    else /* (non-HSR frame and destination node not registered as non-HSR) */
    {
        /* Insert the HSR tag with the sequence number of the host */
        hsrTag = MessagePacker::createHSRTag( "HSR", ringID, sequenceNum );

        /* Increment the sequence number, wrapping through 0 */
        HsrSwitch::setSequenceNum( sequenceNum++ );
    }
}

void EndNodeSwitch::hsrTagReceiveFromRingRoutine( EthernetIIFrame* ethernetFrame, hsrMessage* hsrTag, cGate* arrivalGate )
{
    cGate* gateAIn = HsrSwitch::getGateAIn();
    cGate* gateAInExp = HsrSwitch::getGateAInExp();

    /* Non-HSR-tagged frame */
    if( hsrTag == NULL )
    {
        /* Register the source in its node table as non-HSR node */
        if( arrivalGate == gateAIn || arrivalGate == gateAInExp )
        {
            endNodeTable->pushNode( ethernetFrame->getSrc(), PORTNAME_A, NODETYPE_SAN );
        }
        else
        {
            endNodeTable->pushNode( ethernetFrame->getSrc(), PORTNAME_B, NODETYPE_SAN );
        }
    }
    /* HSR-tagged frame */
    else
    {
        /* Register the source in its node table as HSR node */
        if( arrivalGate == gateAIn || arrivalGate == gateAInExp )
        {
            endNodeTable->pushNode( ethernetFrame->getSrc(), PORTNAME_A, NODETYPE_HSR );
        }
        else
        {
            endNodeTable->pushNode( ethernetFrame->getSrc(), PORTNAME_B, NODETYPE_HSR );
        }
    }
}
