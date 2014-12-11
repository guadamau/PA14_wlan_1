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

#ifndef DELAYLOGGER_H_
#define DELAYLOGGER_H_

#include <omnetpp.h>

class DelayLogger : public cSimpleModule
{

private:

    cOutVector* endToEndDelayVecExpress;
    cOutVector* endToEndDelayVecHigh;
    cOutVector* endToEndDelayVecLow;

    cDoubleHistogram* endToEndDelayStatsExpress;
    cDoubleHistogram* endToEndDelayStatsHigh;
    cDoubleHistogram* endToEndDelayStatsLow;

    cOutVector* preemptionTimeAdditionVecHigh;
    cOutVector* preemptionTimeAdditionVecLow;

    cDoubleHistogram* preemptionTimeAdditionStatsHigh;
    cDoubleHistogram* preemptionTimeAdditionStatsLow;

protected:
    virtual void initialize( void );

public:
    DelayLogger();
    virtual ~DelayLogger();
    void addDelay( cMessage* msg );
    void finish( void );

};

#endif /* DELAYLOGGER_H_ */
