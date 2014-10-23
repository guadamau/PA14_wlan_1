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

#ifndef __RedBoxSwitch_H__
#define __RedBoxSwitch_H__

#include <omnetpp.h>
#include "HsrSwitch.h"

#include "MessagePacker.h"

#include "hsrMessage_m.h"
#include "prpMessage_m.h"
#include "EtherFrame_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"


enum rbr
{
    REDBOX_REGULAR,
    REDBOX_HSR,
    REDBOX_PRP_A,
    REDBOX_PRP_B,
};
typedef enum rbr redBoxConfiguration;


class RedBoxSwitch : public HsrSwitch
{

private:

  redBoxConfiguration redBoxCfg;

  cGate* gateInterlinkIn;
  cGate* gateInterlinkOut;

protected:

  virtual void initialize();
  virtual void handleMessage(cMessage *msg);

public:

    RedBoxSwitch();
    virtual ~RedBoxSwitch();

    cGate* getGateInterlinkIn();
    cGate* getGateInterlinkOut();


    void forwardToInterlink(EthernetIIFrame *ethTag, vlanMessage *vlanTag, hsrMessage *hsrTag, dataMessage *messageData);

};

#endif
