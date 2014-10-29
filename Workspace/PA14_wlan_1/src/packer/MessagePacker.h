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

#ifndef MESSAGEPACKER_H_
#define MESSAGEPACKER_H_

#include <omnetpp.h>
#include "MACAddress.h"
#include "EtherFrame_m.h"
#include "hsrMessage_m.h"
#include "prpMessage_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"

class MessagePacker
{
public:
    static dataMessage *createDataMessage(const char *name, int64 length, int messageCounter);
    static EthernetIIFrame *createETHTag(const char *name, MACAddress dest, MACAddress src);
    static vlanMessage* createVLANTag( const char *name,
                                       int user_priority_var,
                                       int canonical_format_indicator_var,
                                       int vlan_identifier_var,
                                       int nextEtherType_var );
    static hsrMessage* createHSRTag( const char *name, unsigned int ringID, unsigned int sequenceNum );
    static hsrMessage* generateHSRTag(const char *name, int etherType, unsigned int ringID, unsigned int sequenceNum);

    static EthernetIIFrame *generateEthMessage(EthernetIIFrame *ethTag, vlanMessage *vlanTag, hsrMessage  *hsrTag, dataMessage *messageData);
    static void deleteMessage(EthernetIIFrame **ethMessage, vlanMessage **vlanTag, hsrMessage  **hsrTag, dataMessage **messageData);
    static void decapsulateMessage(EthernetIIFrame **ethMessage, vlanMessage **vlanTag, hsrMessage  **hsrTag, dataMessage **messageData);
    static void openMessage(EthernetIIFrame **ethMessage, vlanMessage **vlanTag, hsrMessage  **hsrTag, dataMessage **messageData);


    MessagePacker();
    virtual ~MessagePacker();
};

#endif /* MESSAGEPACKER_H_ */
