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

#ifndef __NODETABLE_H__
#define __NODETABLE_H__

#include <omnetpp.h>
#include "MACAddress.h"

typedef enum
{
    PORTNAME_A,
    PORTNAME_B
} PortName;

typedef enum
{
    NODETYPE_SAN,
    NODETYPE_PRP_A,
    NODETYPE_PRP_B,
    NODETYPE_HSR
} NodeType;

typedef struct
{
   MACAddress nodeMacAddress;
   simtime_t timeLastSeenA;
   simtime_t timeLastSeenB;

   simtime_t timeFirstSeen;
   NodeType nodeType;
   unsigned long sendSeq; //Redbox only

   unsigned long cntReceivedA;
   unsigned long cntReceivedB;

   bool inUse;
} NodesTableEntry ;


class nodeTable : public cSimpleModule
{
  private:
    int nodeTableArrayMaxEntries;
    simtime_t nodeTableArrayEntryTimeout;
    unsigned long cntNodes; //number of nodes in the Nodes Table
    NodesTableEntry* nodeTableArray;

    long searchNode(MACAddress& nodeMacAddress);
    long getNextFreeIndex();

  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);

  public:
	nodeTable();
    bool pushNode(MACAddress& nodeMacAddress, PortName pn, NodeType nodeType);
    NodeType getNodeMode(MACAddress& nodeMacAddress);
};

#endif
