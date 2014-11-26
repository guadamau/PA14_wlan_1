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

#include "nodeTable.h"
#include "nodeTableTimeoutMessage_m.h"

Define_Module(nodeTable);

nodeTable::nodeTable()
{
  nodeTableArray = NULL;
}

void nodeTable::initialize()
{
    if(nodeTableArray != NULL)
    {
        free(nodeTableArray);
    }
    nodeTableArray = NULL;
    nodeTableArrayMaxEntries = par("nodeTableMaxEntries");
    nodeTableArrayEntryTimeout = par("nodeTableEntryTimeout").doubleValue();
    unsigned long securityPointer = nodeTableArrayMaxEntries*sizeof(NodesTableEntry);

    nodeTableArray = (NodesTableEntry*)malloc(nodeTableArrayMaxEntries*sizeof(NodesTableEntry));

    EV << "init nodeTableArray";
    for(int c = 0; c < nodeTableArrayMaxEntries; c++)
    {
        if(((&(nodeTableArray[c]))+sizeof(NodesTableEntry)) >= (securityPointer + nodeTableArray))
            throw cRuntimeError("pointer error! Panik ! \n");
        nodeTableArray[c].inUse = false;

    }
}

void
nodeTable::finish()
{
//    for(int c = 0; c < nodeTableArrayMaxEntries; c++)
//    {
//        if(nodeTableArray[c].inUse == true)
//        {
//            throw cRuntimeError("timeout error! Panik ! \n");
//        }
//    }

    if(nodeTableArray != NULL)
    {
        free(nodeTableArray);
    }
    nodeTableArray = NULL;
}

long nodeTable::searchNode(MACAddress& nodeMacAddress)
{
    if(nodeTableArray != NULL)
    {
        for(int c = 0; c < nodeTableArrayMaxEntries; c++)
        {
            if(nodeTableArray[c].inUse == true)
            {
                if(nodeTableArray[c].nodeMacAddress == nodeMacAddress)
                {
                    return c;
                }
            }
        }
    }
    return -1;
}

long nodeTable::getNextFreeIndex()
{
    if(nodeTableArray != NULL)
    {
        for(int c = 0; c < nodeTableArrayMaxEntries; c++)
        {
            if(nodeTableArray[c].inUse == false)
            {
                return c;
            }
        }
    }
    return -1;
}

bool nodeTable::pushNode(MACAddress& nodeMacAddress, PortName pn, NodeType nodeType)
{
//    Enter_Method("pushNode(MACAddress& nodeMacAddress, PortName pn, NodeType nodeType)");

//    //EV << "nodeTableArray pushNode: ";
//    if(nodeTableArray != NULL)
//    {
//      long nodeIndex = searchNode(nodeMacAddress);
//      //EV << "searchNode: " << nodeIndex << " \n";
//      if(nodeIndex == -1)
//      {
//          nodeIndex = getNextFreeIndex();
//          if(nodeIndex == -1)
//          {
//              return false;
//          }
//          nodeTableArray[nodeIndex].inUse = true;
//          nodeTableArray[nodeIndex].cntReceivedA = 0;
//          nodeTableArray[nodeIndex].cntReceivedB = 0;
//          nodeTableArray[nodeIndex].timeLastSeenA = 0;
//          nodeTableArray[nodeIndex].timeLastSeenB = 0;
//          nodeTableArray[nodeIndex].timeFirstSeen = simTime();
//          cntNodes++;
//      }
//      nodeTableArray[nodeIndex].nodeMacAddress = nodeMacAddress;
//      nodeTableArray[nodeIndex].nodeType = nodeType;
//      if(pn == PORTNAME_A)
//      {
//          nodeTableArray[nodeIndex].timeLastSeenA = simTime();
//          nodeTableArray[nodeIndex].cntReceivedA++;
//      }
//      if(pn == PORTNAME_B)
//      {
//          nodeTableArray[nodeIndex].timeLastSeenB = simTime();
//          nodeTableArray[nodeIndex].cntReceivedB++;
//      }
//
//      NodeTableTimeoutMessage *timeoutMessage = new NodeTableTimeoutMessage();
//      timeoutMessage->setNode(nodeMacAddress);
//      scheduleAt(simTime() + nodeTableArrayEntryTimeout, timeoutMessage);
//    }
    return true;

}

NodeType nodeTable::getNodeMode(MACAddress& nodeMacAddress)
{
//    Enter_Method("getNodeMode(MACAddress& nodeMacAddress)");
    NodeType result = NODETYPE_HSR;
//    if(nodeTableArray != NULL)
//    {
//      long nodeIndex = searchNode(nodeMacAddress);
//      if(nodeIndex != -1)
//      {
//          result = nodeTableArray[nodeIndex].nodeType;
//      }
//    }
    return result;
}

void nodeTable::handleMessage(cMessage *msg)
{
    //EV << "nodeTableArray handleMessage: \n";
    if(nodeTableArray != NULL)
    {
        if (msg->isSelfMessage())
        {
            NodeTableTimeoutMessage *timeoutMessage = check_and_cast<NodeTableTimeoutMessage *> (msg);
            long nodeIndex = searchNode(timeoutMessage->getNode());
            if (nodeIndex != -1)
             {
                 if((nodeTableArray[nodeIndex].timeLastSeenA+nodeTableArrayEntryTimeout <= simTime()) && (nodeTableArray[nodeIndex].timeLastSeenB+nodeTableArrayEntryTimeout <= simTime()))
                 {
                     nodeTableArray[nodeIndex].inUse = false;
                     //EV << "Node: " << timeoutMessage->getNode() << " aged out at "<< simTime() <<"s \n";
                 }
             }
         }
    }
    delete msg;
}

