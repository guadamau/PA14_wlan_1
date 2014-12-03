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

#include <SendingStatus.h>

#include "MessagePacker.h"
#include "hsrMessage_m.h"
#include "EtherFrame_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"

SendingStatus::SendingStatus()
{}

SendingStatus::~SendingStatus()
{}

framePriority SendingStatus::getFramePrio( void )
{
    return framePrio;
}

cMessage* SendingStatus::getMessage( void )
{
    return message;
}

simtime_t SendingStatus::getSendingTime( void )
{
    return sendingTime;
}

EthernetIIFrame* SendingStatus::getEthTag( void )
{
    return ethTag;
}

void SendingStatus::attachFrame( cMessage* message )
{
    /* duplicate message first ... */
    this->message = message->dup();

    MessagePacker::decapsulateMessage( &this->ethTag, &this->vlanTag, &this->hsrTag, &this->dataStream );

    this->framePrio = static_cast<framePriority>( this->vlanTag->getUser_priority() );
    this->sendingTime = simTime();
}

void SendingStatus::detachFrame( void )
{
    MessagePacker::deleteMessage( &this->ethTag, &this->vlanTag, &this->hsrTag, &this->dataStream );

    this->sendingTime = SIMTIME_ZERO;

    if( this->message != NULL )
    {
        delete this->message;
    }
}

unsigned char SendingStatus::hasAttachedFrame( void )
{
    unsigned char retVal = 0x00;

    if( this->sendingTime != SIMTIME_ZERO && this->message != NULL )
    {
        retVal = 0x01;
    }

    return retVal;
}



