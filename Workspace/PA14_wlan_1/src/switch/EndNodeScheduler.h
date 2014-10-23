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

class EndNodeScheduler: public Scheduler
{

public:
    EndNodeScheduler( schedulerMode schedmode );
    virtual ~EndNodeScheduler();

    void enqueueMessage( cMessage *msg, HsrSwitch* parentModule );
    void processQueues( HsrSwitch* parentModule );
};

#endif /* ENDNODESCHEDULER_H_ */
