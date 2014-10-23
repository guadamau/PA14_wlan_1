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

#ifndef __ENDNODESWITCH_H__
#define __ENDNODESWITCH_H__

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



class EndNodeSwitch : public HsrSwitch
{
  public:

    EndNodeSwitch();
    virtual ~EndNodeSwitch();


  private:

    unsigned long cntTotalSentA; //number of frames sent over network interface A
    unsigned long cntTotalSentB; //number of frames sent over network interface B

    unsigned long cntErrorsA; //number of frames with errors received from network interface A
    unsigned long cntErrorsB; //number of frames with errors received from network interface B

    nodeTable* endNodeTable;

    nodeTable* getNodeTable();

    void DANH_receiving_from_its_link_layer_interface(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData);
    void DANH_receiving_from_an_HSR_port(EthernetIIFrame **ethTag, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData);


  protected:

    virtual void initialize();
    virtual void handleMessage( cMessage *msg );
};

#endif
