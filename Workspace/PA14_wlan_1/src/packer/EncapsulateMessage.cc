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

#include "EncapsulateMessage.h"

#include "MessagePacker.h"

#include "hsrMessage_m.h"
#include "prpMessage_m.h"
#include "EtherFrame_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"

Define_Module(EncapsulateMessage);

void EncapsulateMessage::initialize()
{
	// TODO - Generated method body
    ringID = par("ringID");
    if ((ringID < 1) || (ringID > 6))
    {
        throw cRuntimeError("invalid Ring ID");
    }

    sequenceNum = 0;
}

void EncapsulateMessage::handleMessage(cMessage *msg)
{
    EthernetIIFrame *ethTag = check_and_cast<EthernetIIFrame *> (msg); //Es muss ein ethernetframe sein

    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;

    //Frame in Einzelteile zerlegen
    MessagePacker::decapsulateMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
    if(hsrTag == NULL)
    {
        if(true == true) //TODO Welche Messages sollen getagt werden, und welche nicht?
        {
            // hsrTag = MessagePacker::generateHSRTag("HSR",0, ringID, sequenceNum);
            hsrTag = MessagePacker::createHSRTag( "HSR", ringID, sequenceNum );
            sequenceNum++;
        }
    }
    send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gate("gate$o"));
    MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
}
