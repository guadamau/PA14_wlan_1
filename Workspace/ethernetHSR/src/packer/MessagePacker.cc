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

#include "MessagePacker.h"

MessagePacker::MessagePacker()
{
    // TODO Auto-generated constructor stub

}

MessagePacker::~MessagePacker()
{
    // TODO Auto-generated destructor stub
}

dataMessage *MessagePacker::createDataMessage(const char *name, int64 length, int messageCounter)
{
    dataMessage *result_messageData = new dataMessage(name);
    result_messageData->setCounter(messageCounter);
    result_messageData->setSendTime(simTime().dbl());
    result_messageData->setByteLength(length);
    for(int c = 0; c < 500; c++)
    {
        result_messageData->setRandomData(c,0);
    }
    return result_messageData;
}

EthernetIIFrame *MessagePacker::createETHTag(const char *name, MACAddress dest, MACAddress src)
{
    EthernetIIFrame *frm;
    frm = new EthernetIIFrame(name,0);
    frm->setByteLength(ETHER_MAC_FRAME_BYTES); //DEST + SRC + TYPE + CRC
    frm->setDest(dest);
    frm->setSrc(src);
    return frm;
}

vlanMessage* MessagePacker::createVLANTag( const char *name,
                                           int user_priority_var,
                                           int canonical_format_indicator_var,
                                           int vlan_identifier_var,
                                           int nextEtherType_var )
{
    vlanMessage* vlanmsg = NULL;
    vlanmsg = new vlanMessage( name, 0 );

    vlanmsg->setUser_priority( user_priority_var );
    vlanmsg->setCanonical_format_indicator( canonical_format_indicator_var );
    vlanmsg->setVlan_identifier( vlan_identifier_var );
    vlanmsg->setNextEtherType( nextEtherType_var );

    return vlanmsg;
}

hsrMessage * MessagePacker::createHSRTag(const char *name, unsigned int ringID, unsigned int sequenceNum)
{
    hsrMessage *result_hsrTag = new hsrMessage(name);
    result_hsrTag->setPath((ringID << 1)); //ringID ist fix
    result_hsrTag->setSequencenumber(sequenceNum);
    result_hsrTag->setByteLength(6); //Mix aus Ethernet und HSR: Die 6 HSR Bytes bestehen aus 2Byte Ethertype und 4Byte HSR Tag
    return result_hsrTag;
}

EthernetIIFrame *
MessagePacker::generateEthMessage(EthernetIIFrame *ethTag, vlanMessage *vlanTag, hsrMessage *hsrTag, dataMessage *messageData)
{
    EthernetIIFrame *result_ethTag = NULL;
    vlanMessage *result_vlanTag = NULL;
    hsrMessage *result_hsrTag = NULL;
/*
    if (ethTag != NULL)
    {
      EV << "ethTag  getBitLength " << ethTag->getBitLength() << endl;
    }
    if (vlanTag != NULL)
    {
        EV << "vlanTag  getBitLength " << vlanTag->getBitLength() << endl;
    }
    if (hsrTag != NULL)
    {
        EV << "hsrTag  getBitLength " << hsrTag->getBitLength() << endl;
    }
    if (messageData != NULL)
    {
        EV << "messageData  getBitLength " << messageData->getBitLength() << endl;
    }
*/
    if ((ethTag == NULL) || (messageData == NULL))
    {
        throw cRuntimeError("Can't generate Message without Address and Data ! \n");
        return NULL;
    }

    if (hsrTag != NULL)
    {
        result_hsrTag = hsrTag->dup();
        result_hsrTag->setOwnEtherType(0x0800); //TODO: Ethertype der daten
        result_hsrTag->encapsulate(messageData->dup());
        if (vlanTag != NULL)
        {
            result_vlanTag = vlanTag->dup();
            result_vlanTag->setOwnEtherType(0x88FB); //vlan zeigt auf hsr
            result_vlanTag->encapsulate(result_hsrTag);
            result_ethTag = ethTag->dup();
            result_ethTag->setEtherType(0x8100); //eth zeigt auf vlan
            result_ethTag->encapsulate(result_vlanTag);
        }
        else
        {
            result_ethTag = ethTag->dup();
            result_ethTag->setEtherType(0x88FB); //eth zeigt auf hsr
            result_ethTag->encapsulate(result_hsrTag);
        }
    }
    else
    {
        if (vlanTag != NULL)
        {
            result_vlanTag = vlanTag->dup();
            result_vlanTag->setOwnEtherType(0x0800); //TODO: Ethertype der daten
            result_vlanTag->encapsulate(messageData->dup());
            result_ethTag = ethTag->dup();
            result_ethTag->setEtherType(0x8100); //eth zeigt auf vlan
            result_ethTag->encapsulate(result_vlanTag);
        }
        else
        {
            result_ethTag = ethTag->dup();
            result_ethTag->setEtherType(0x0800); //TODO: Ethertype der daten
            result_ethTag->encapsulate(messageData->dup());
        }
    }

    return result_ethTag;
}

void
MessagePacker::deleteMessage(EthernetIIFrame **ethMessage, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData)
{
    if (*ethMessage != NULL)
    {
        delete *ethMessage;
    }
    *ethMessage = NULL;

    if (*vlanTag != NULL)
    {
        delete *vlanTag;
    }
    *vlanTag = NULL;

    if (*hsrTag != NULL)
    {
        delete *hsrTag;
    }
    *hsrTag = NULL;

    if (*messageData != NULL)
    {
        delete *messageData;
    }
    *messageData = NULL;
}

void
MessagePacker::decapsulateMessage(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData)
{
    if ((*ethTag)->getEtherType() == 0x8100) //if vlan
    {
        //VLAN TAG
        (*vlanTag) = check_and_cast<vlanMessage *> ((*ethTag)->decapsulate());
        if ((*vlanTag)->getOwnEtherType() == 0x88FB) //if hsr
        {
            (*hsrTag) = check_and_cast<hsrMessage *> ((*vlanTag)->decapsulate());
            (*messageData) = check_and_cast<dataMessage *> ((*hsrTag)->decapsulate());
        }
        else
        {
            (*messageData) = check_and_cast<dataMessage *> ((*vlanTag)->decapsulate());
        }
    }
    else if ((*ethTag)->getEtherType() == 0x88FB) //if hsr
    {
        //HSR TAG
        (*hsrTag) = check_and_cast<hsrMessage *> ((*ethTag)->decapsulate());
        (*messageData) = check_and_cast<dataMessage *> ((*hsrTag)->decapsulate());
    }
    else
    {
        //Alles anderere
        (*messageData) = check_and_cast<dataMessage *> ((*ethTag)->decapsulate());
    }
}
