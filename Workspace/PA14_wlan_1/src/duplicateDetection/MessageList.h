/*
 * MessageList.h
 *
 *  Created on: 07.01.2009
 *      Author: gemp
 */

#ifndef MESSAGELIST_H_
#define MESSAGELIST_H_

#include <omnetpp.h>
#include "hsrMessage_m.h"
#include <map>
#include <keyMSG.h>
#include "MACAddress.h"

typedef std::map<unsigned int, int> MyMessageInfoMap;

typedef std::map<unsigned int, int> PathInfoMap;
typedef std::map<unsigned int, PathInfoMap> SequenceInfoMap;
typedef std::map<unsigned int, SequenceInfoMap> DestinationInfoMap;
typedef std::map<unsigned int, DestinationInfoMap> SourceInfoMap;

typedef std::pair<keyMSG, int> key_Data;
typedef std::map<keyMSG, int> MapTable;

class MessageList {
private:
	SourceInfoMap messageMap2;

	MapTable table;

	unsigned long long MacToLonglong(MACAddress &mac);

public:
	MessageList();
	virtual ~MessageList();

	void addMessage(MACAddress& source, short sequencenumber);
	bool duplicateCheck(MACAddress& source, short sequencenumber);
    bool limitCheck(MACAddress& source, short sequencenumber, int limit);

};


#endif /* MESSAGELIST_H_ */
