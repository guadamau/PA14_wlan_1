/*
 * hsrDefines.h
 *
 *  Created on: 02.03.2009
 *      Author: gemp
 */

#ifndef HSRDEFINES_H_
#define HSRDEFINES_H_

#include <omnetpp.h>
#include "MACAddress.h"



typedef enum
{
	TYPE_ERROR = 0,
	TYPE_STD,
	TYPE_HSR,
} SendType;

typedef enum
{
	BEHAVIOR_ERROR = 0,
	BEHAVIOR_STD,
	BEHAVIOR_CONSTANT_LOADGEN,
	BEHAVIOR_UNIFORM_LOADGEN,
	BEHAVIOR_NORMAL_LOADGEN,
} SendBehavior;

typedef struct
{
    MACAddress destination;
    SendType  sendType;
    SendBehavior sendBehavior;
    simtime_t startTime;
    simtime_t stopTime;

    double last;
    double maxLast;
    double interval;
    double maxInterval;

    double standardabweichung ;
    unsigned short vlan;

    unsigned short paketgroesse;

} SendData;







#endif /* HSRDEFINES_H_ */
