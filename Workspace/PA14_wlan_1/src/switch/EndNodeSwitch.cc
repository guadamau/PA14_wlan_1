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
#include "Scheduler.h"

Define_Module(EndNodeSwitch)
;


nodeTable *EndNodeSwitch::getNodeTable()
{
    cModule *calleeModule = getParentModule();

    if(calleeModule != NULL)
    {
        calleeModule = calleeModule->getSubmodule("nodeTable"); //TODO: Namen als Parameter �bergeben
        return check_and_cast<nodeTable *>(calleeModule);
    }

    return NULL;
}


void
EndNodeSwitch::initialize()
{
    //Initialize Scheduler
    schedmode = par("schedulerMode").stringValue();
    sched = new Scheduler(schedmode);

    myAddr.setAddress(par("myAddress").stringValue());

    if ((myAddr.isBroadcast()) || (myAddr.isMulticast()))
    {
        throw cRuntimeError("invalid Addr");
    }

    ringID = par("ringID");
    if ((ringID < 1) || (ringID > 6))
    {
        throw cRuntimeError("invalid Ring ID");
    }

    sequenceNum = 0;

    endNodeTable = getNodeTable();
    if(endNodeTable == NULL)
    {
        throw cRuntimeError("can't load node table");
    }

    gateAIn = gate("gateA$i");
    gateAOut = gate("gateA$o");
    gateBIn = gate("gateB$i");
    gateBOut = gate("gateB$o");
    gateCpuIn = gate("gateCPU$i");
    gateCpuOut = gate("gateCPU$o");
}

EndNodeSwitch::~EndNodeSwitch() {
    delete sched;
}

void EndNodeSwitch::DANH_receiving_from_its_link_layer_interface(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData)
{
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
        sequenceNum++;
    }

    //Duplicate the frame, enqueue it for sending into both HSR ports
    send(MessagePacker::generateEthMessage(*ethTag, *vlanTag, *hsrTag, *messageData), gateAOut);
    send(MessagePacker::generateEthMessage(*ethTag, *vlanTag, *hsrTag, *messageData), gateBOut);
}


void EndNodeSwitch::DANH_receiving_from_an_HSR_port(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData)
{
    cGate* arrivalGate = (*ethTag)->getArrivalGate();
    cGate* tempOutGate;
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
            endNodeTable->pushNode((*ethTag)->getSrc(), PORTNAME_A, NODETYPE_SAN);
        else
            endNodeTable->pushNode((*ethTag)->getSrc(), PORTNAME_B, NODETYPE_SAN);
        //Enqueue the unchanged frame for sending to its link layer interface.
        if(((myAddr == (*ethTag)->getDest()) || ((*ethTag)->getDest().isBroadcast()) || ((*ethTag)->getDest().isMulticast())) && (myAddr != (*ethTag)->getSrc()))
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
        if(((myAddr == (*ethTag)->getDest()) || ((*ethTag)->getDest().isBroadcast()) || ((*ethTag)->getDest().isMulticast())) && (myAddr != (*ethTag)->getSrc()))
        {
	        send(MessagePacker::generateEthMessage((*ethTag), (*vlanTag), (*hsrTag), (*messageData)), gateCpuOut);
        }
        else //(if this node is not a destination)
        {
            //Do not send it over the link layer interface
        }
        //If this node is not the only destination (multicast or unicast for another node)
        if(myAddr != (*ethTag)->getDest())
        {
	        send(MessagePacker::generateEthMessage((*ethTag), (*vlanTag), (*hsrTag), (*messageData)), tempOutGate);
        }
        else//(If this node is the only (unicast) destination)
        {
            //Discard the frame
        }
    }
}



void
EndNodeSwitch::handleMessage(cMessage *msg)
{
    ///////////////////////////////////////////////////////////////
    // Will be implemented as soon as the function itself is implemented
    ///////////////////////////////////////////////////////////////
    // sched->enqueueMessage(msg);

    cGate* arrivalGate = msg->getArrivalGate();

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
