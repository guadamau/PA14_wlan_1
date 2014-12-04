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

#ifndef SENDINGSTATUS_H_
#define SENDINGSTATUS_H_

#include <omnetpp.h>
#include "hsrDefines.h"

#include "hsrMessage_m.h"
#include "EtherFrame_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"

class SendingStatus
{

private:

    cMessage*        message;

    EthernetIIFrame* ethTag;
    vlanMessage*     vlanTag;
    hsrMessage*      hsrTag;
    dataMessage*     dataStream;


    simtime_t        sendingTime;
    framePriority    framePrio;

    int64_t          messageSize;

public:

    SendingStatus();
    virtual ~SendingStatus();

    cMessage* getMessage( void );
    simtime_t getSendingTime( void );
    framePriority getFramePrio( void );
    int64_t getMessageSize( void );

    void attachFrame( cMessage* message );
    void detachFrame( void );

    unsigned char hasAttachedFrame( void );

};

#endif /* SENDINGSTATUS_H_ */
