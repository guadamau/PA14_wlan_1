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

#define QUEUES_COUNT 6

typedef enum
{
    EXPRESS,
    HIGH,
    LOW
} framePriority;

typedef enum
{
    EXPRESS_RING,
    EXPRESS_INTERNAL,
    HIGH_RING,
    HIGH_INTERNAL,
    LOW_RING,
    LOW_INTERNAL
} queueName;

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
    framePriority frameprio;
    simtime_t startTime;
    simtime_t stopTime;

    double last;
    double maxLast;
    double interval;
    double epsilon;
    double maxInterval;

    double standardabweichung ;
    unsigned short vlan;

    unsigned short paketgroesse;

} SendData;







#endif /* HSRDEFINES_H_ */
