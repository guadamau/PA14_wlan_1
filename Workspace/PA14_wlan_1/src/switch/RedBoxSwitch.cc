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

#include "RedBoxSwitch.h"

Define_Module(RedBoxSwitch)
;

void
RedBoxSwitch::initialize()
{
    myAddr.setAddress(par("myAddress").stringValue());

    if ((myAddr.isBroadcast()) || (myAddr.isMulticast()))
    {
        throw cRuntimeError("Illegal device address ! \n");
        endSimulation();
    }

    ringID = par("ringID");
    if ((ringID < 1) || (ringID > 6))
    {
        throw cRuntimeError("invalid Ring ID");
    }

    sequenceNum = 0;    
    
    int temp = par("redBoxCfg");
    switch (temp)
    {
    case 0:
        redBoxCfg = REDBOX_REGULAR;
    break;
    case 1:
        redBoxCfg = REDBOX_HSR;
    break;
    case 2:
        redBoxCfg = REDBOX_PRP_A;
    break;
    case 3:
        redBoxCfg = REDBOX_PRP_B;
    break;
    default:
        throw cRuntimeError("Illegal redbox configuration ! \n");
        endSimulation();
    }

    gateAIn  = gate("gateA$i");
    gateAOut = gate("gateA$o");
    gateBIn  = gate("gateB$i");
    gateBOut = gate("gateB$o");
    gateInterlinkIn  = gate("gateInterlink$i");
    gateInterlinkOut = gate("gateInterlink$o");
    gateCpuIn  = gate("gateCPU$i");
    gateCpuOut = gate("gateCPU$o");

}

void RedBoxSwitch::forwardToInterlink(EthernetIIFrame *ethTag, vlanMessage *vlanTag, hsrMessage *hsrTag, dataMessage *messageData)
{
    switch (redBoxCfg)
    {
        case REDBOX_REGULAR:
        {
            //TODO: Adressen lernen
            send(MessagePacker::generateEthMessage(ethTag, vlanTag, NULL, messageData), gateInterlinkOut);
        }
        break;
        case REDBOX_HSR:
        {
            send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gateInterlinkOut);
        }
        break;
        case REDBOX_PRP_A:
        {
            //TODO: PRP
        }
        case REDBOX_PRP_B:
        {
            //TODO: PRP
        }
    }
}

void
RedBoxSwitch::handleMessage(cMessage *msg)
{
    cGate* arrivalGate = msg->getArrivalGate();

    EthernetIIFrame *ethTag = check_and_cast<EthernetIIFrame *> (msg);
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;

    MessagePacker::decapsulateMessage(&ethTag, &vlanTag, &hsrTag, &messageData);

    if ((arrivalGate == gateAIn) || (arrivalGate == gateBIn))
    {
        cGate* tempOutGate;
        if (arrivalGate == gateAIn)
        {
            tempOutGate = gateBOut;
        }
        else
        {
            tempOutGate = gateAOut;
        }

        //HSR verarbeiten (an CPU und/oder an gate weiterleiten)
        //remove the HSR tagging and pass the modified frame to its higher protocol layer, if this is the first frame of a pair, otherwise:

        if((myAddr == ethTag->getDest()) && (myAddr != ethTag->getSrc()))
        {
            // EV << "CPU \n";
            send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gateCpuOut);
        }
        else
        {
            if ((ethTag->getDest().isBroadcast()) && (myAddr != ethTag->getSrc()))
            {
                //Broadcast -> CPU
                //EV << "Broadcast \n";
                send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gateCpuOut);
            }
            //Ring
            //EV << "Ring weiterleiten \n";
            forwardToInterlink(ethTag, vlanTag, hsrTag, messageData);
            send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), tempOutGate);
        }
    }
    else if (arrivalGate == gateCpuIn)
    {
		if((hsrTag != NULL))
		{
		    //Do not modify the frame;
		}
		else //(non-HSR frame)
		{
		    //Insert the HSR tag with the sequence number of the host;
		    hsrTag = MessagePacker::createHSRTag("HSR", ringID, sequenceNum);
		    //Increment the sequence number, wrapping through 0
		    sequenceNum++;
		}	    
	    
        //Broadcast zum  Ring
        send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gateAOut);
        send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gateBOut);
        forwardToInterlink(ethTag, vlanTag, hsrTag, messageData);
    }
    else if (arrivalGate == gateInterlinkIn)
    {
        //TODO: Adressen lernen

        if((myAddr == ethTag->getDest()) && (myAddr != ethTag->getSrc()))
        {
            // EV << "CPU \n";
            send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gateCpuOut);
        }
        else
        {
            if ((ethTag->getDest().isBroadcast()) && (myAddr != ethTag->getSrc()))
            {
                //Broadcast -> CPU
                //EV << "Broadcast \n";
                send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gateCpuOut);
            }
            //Ring
            //EV << "Ring weiterleiten \n";
            send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gateAOut);
            send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gateBOut);
        }
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
