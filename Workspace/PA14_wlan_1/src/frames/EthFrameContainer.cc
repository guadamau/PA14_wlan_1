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

#include <frames/EthFrameContainer.h>
#include "hsrMessage_m.h"
#include "prpMessage_m.h"
#include "dataMessage_m.h"
#include "vlanMessage_m.h"

EthFrameContainer::EthFrameContainer()
{
    this->frameTransmissionTime = 0.0;
    this->frameTransmissionTimeVector = new cOutVector();

    frameTransmissionTimeVector->setName( "---- noname frame ----" );
    frameTransmissionTimeVector->record( frameTransmissionTime );
}

EthFrameContainer::~EthFrameContainer()
{
    delete this->frameTransmissionTimeVector;
}

void EthFrameContainer::calcAndRecordTransmissionTime( void )
{
    //simtime_t creationTime = EthernetIIFrame::getCreationTime();
    simtime_t arrivalTime = simTime();

    EthernetIIFrame* tempFrame = check_and_cast<EthernetIIFrame*>( this->dup() );
    vlanMessage** vlanTag = NULL;
    hsrMessage** hsrTag = NULL;
    dataMessage** messageData = NULL;

    //MessagePacker::decapsulateMessage( EthernetIIFrame **tempFrame, vlanMessage **vlanTag, hsrMessage **hsrTag, dataMessage **messageData );

    // frameTransmissionTime = arrivalTime - creationTime;

//    for(int i = 0; i < QUEUES_COUNT; i++)
//            {
//                queueSizes[i] = 0;
//
//                cOutVector* queueVector = new cOutVector();
//
//                std::stringstream ss;
//                ss << schedID << ": " << queueNamesStr[i];
//                queueVector->setName( ss.str().c_str() );
//
//                queueVector->record( queueSizes[ i ] );
//                queueVectors->addAt( i, queueVector );
//            }

}

const char* EthFrameContainer::getDisplayString( void ) const
{
    return NULL;
}

