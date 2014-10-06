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

#include "ForwardDelay.h"

Define_Module(ForwardDelay);

void ForwardDelay::initialize()
{
	// TODO - Generated method body
    delay = par("delay");
    storeAndForward = par("storeAndForward");
    //delay = 0.000005;
}

void ForwardDelay::handleMessage(cMessage *msg)
{

    cPacket *packet = check_and_cast<cPacket *> (msg);
    /*
    EV << "ForwardDelay BitLength: " << packet->getBitLength() << endl;
    */
    simtime_t correction = 0;
    if(storeAndForward)
    {
        correction = packet->getBitLength() * 0.00000001; //Paket wird erst voll empfangen
    }
    sendDelayed(msg, correction+delay, gate("out"));
}

