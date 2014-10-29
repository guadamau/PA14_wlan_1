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

#ifndef ENDNODESCHEDULER_H_
#define ENDNODESCHEDULER_H_

#include "Scheduler.h"
#include <omnetpp.h>
#include "HsrSwitch.h"

#include "MessagePacker.h"
#include "nodeTable.h"
#include "ListenErkennung.h"

#include "hsrMessage_m.h"
#include "prpMessage_m.h"
#include "EtherFrame_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"

class EndNodeScheduler: public Scheduler
{

public:
    EndNodeScheduler();
    virtual ~EndNodeScheduler();

    void enqueueMessage( cMessage *msg );
    void processQueues( void );

    void forwardFrame( cMessage *msg );
    void sendToRing(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData);
    void recieveFromRing(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData);
};

#endif /* ENDNODESCHEDULER_H_ */
