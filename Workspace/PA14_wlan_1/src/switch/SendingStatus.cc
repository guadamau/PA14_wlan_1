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
    return this->framePrio;
}

cMessage* SendingStatus::getMessage( void )
{
    EthernetIIFrame* ethMsg = MessagePacker::generateEthMessage( this->ethTag, this->vlanTag, this->hsrTag, this->dataStream );
    cMessage* cMsg = check_and_cast<cMessage*>( ethMsg );
    return cMsg;
}

simtime_t SendingStatus::getSendingTime( void )
{
    return this->sendingTime;
}

int64_t SendingStatus::getMessageSize( void )
{
    return this->messageSize;
}

void SendingStatus::setMessageSize ( int64_t messageSize )
{
    this->messageSize = messageSize;
}

void SendingStatus::attachFrame( cMessage* message )
{
    /* duplicate message first ... */
    this->message = message->dup();


    this->ethTag = check_and_cast<EthernetIIFrame*>( this->message );

    this->messageSize = this->ethTag->getByteLength();


    MessagePacker::decapsulateMessage( &this->ethTag, &this->vlanTag, &this->hsrTag, &this->dataStream );

    this->framePrio = static_cast<framePriority>( this->vlanTag->getUser_priority() );
    this->sendingTime = simTime();
}

void SendingStatus::detachFrame( void )
{
    this->messageSize = 0;

    MessagePacker::deleteMessage( &this->ethTag, &this->vlanTag, &this->hsrTag, &this->dataStream );

    this->sendingTime = SIMTIME_ZERO;
}

unsigned char SendingStatus::hasAttachedFrame( void )
{
    unsigned char retVal = 0x00;

    if( this->sendingTime != SIMTIME_ZERO && this->message != NULL && messageSize > 0 )
    {
        retVal = 0x01;
    }

    return retVal;
}

void SendingStatus::updateSendtime( simtime_t sendtime )
{
    this->sendingTime = sendtime;
}

simtime_t SendingStatus::getPreemptionDelay( void )
{
    return this->vlanTag->getPreemptionDelay();
}



