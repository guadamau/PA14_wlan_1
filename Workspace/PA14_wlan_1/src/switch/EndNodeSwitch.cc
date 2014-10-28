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
#include "EndNodeScheduler.h"

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

    HsrSwitch::setSched( new EndNodeScheduler() );
    EndNodeScheduler* sched = ( EndNodeScheduler* )HsrSwitch::getSched();
    sched->initScheduler( HsrSwitch::getSchedmode() );
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

void EndNodeSwitch::DANH_receiving_from_its_link_layer_interface(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData)
{
    unsigned int ringID = HsrSwitch::getRingId();
    unsigned int sequenceNum = HsrSwitch::getSequenceNum();

    cGate* gateAOut = HsrSwitch::getGateAOut();
    cGate* gateBOut = HsrSwitch::getGateBOut();


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
        HsrSwitch::setSequenceNum( sequenceNum++ );
    }

    //Duplicate the frame, enqueue it for sending into both HSR ports
    send(MessagePacker::generateEthMessage(*ethTag, *vlanTag, *hsrTag, *messageData), gateAOut);
    send(MessagePacker::generateEthMessage(*ethTag, *vlanTag, *hsrTag, *messageData), gateBOut);
}


void EndNodeSwitch::DANH_receiving_from_an_HSR_port(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData)
{
    cGate* arrivalGate = (*ethTag)->getArrivalGate();
    cGate* tempOutGate;
    cGate* gateAIn = HsrSwitch::getGateAIn();
    cGate* gateAOut = HsrSwitch::getGateAOut();
    cGate* gateBOut = HsrSwitch::getGateBOut();
    cGate* gateCpuOut = HsrSwitch::getGateCpuOut();
    MACAddress macAddress = *( HsrSwitch::getMacAddress() );

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
            send(MessagePacker::generateEthMessage((*ethTag), (*vlanTag), (*hsrTag), (*messageData)), gateCpuOut);
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
	        send(MessagePacker::generateEthMessage((*ethTag), (*vlanTag), (*hsrTag), (*messageData)), gateCpuOut);
        }
        else //(if this node is not a destination)
        {
            //Do not send it over the link layer interface
        }
        //If this node is not the only destination (multicast or unicast for another node)
        if( macAddress != (*ethTag)->getDest() )
        {
	        send(MessagePacker::generateEthMessage((*ethTag), (*vlanTag), (*hsrTag), (*messageData)), tempOutGate);
        }
        else//(If this node is the only (unicast) destination)
        {
            //Discard the frame
            delete *ethTag;
        }
    }
}



void EndNodeSwitch::handleMessage(cMessage *msg)
{
    ///////////////////////////////////////////////////////////////
    // Will be implemented as soon as the function itself is implemented
    ///////////////////////////////////////////////////////////////

    Scheduler* sched = HsrSwitch::getSched();

    if( msg->isSelfMessage() )
    {
        sched->processQueues();
    }
    else
    {
        sched->enqueueMessage( msg );
    }


    /* Folgender Inhalt wird künftig in der send-methode abgehandelt. */

    cGate* arrivalGate = msg->getArrivalGate();

    cGate* gateAIn = HsrSwitch::getGateAIn();
    cGate* gateBIn = HsrSwitch::getGateBIn();
    cGate* gateCpuIn = HsrSwitch::getGateCpuIn();

    EthernetIIFrame *ethTag = check_and_cast<EthernetIIFrame *> (msg);
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;

    MessagePacker::decapsulateMessage(&ethTag, &vlanTag, &hsrTag, &messageData);

    if ((arrivalGate == gateAIn) || (arrivalGate == gateBIn))
    {
        //5.3.3 DANH receiving from an HSR port
        DANH_receiving_from_an_HSR_port(&ethTag, &vlanTag, &hsrTag, &messageData);
    }
    else if (arrivalGate == gateCpuIn)
    {
        //5.3.2 DANH receiving from its link layer interface
        DANH_receiving_from_its_link_layer_interface(&ethTag, &vlanTag, &hsrTag, &messageData);
    }
    else
    {
        //from somewhere else
        MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
        throw cRuntimeError("Get message from unknown gate! Panik ! \n");
        endSimulation();
    }

    MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
}
