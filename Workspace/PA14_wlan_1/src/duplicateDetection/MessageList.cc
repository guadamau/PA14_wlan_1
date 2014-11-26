/*
 * MessageList.cc
 *
 *  Created on: 07.01.2009
 *      Author: gemp
 */

#include "MessageList.h"
#include <utility> // make_pair


MessageList::MessageList() {
}

MessageList::~MessageList() {
}

unsigned long long MessageList::MacToLonglong(MACAddress &mac)
{
	unsigned long long temp = 0;
	for(unsigned int c = 0; c < mac.getAddressSize(); c++)
	{
		temp += mac.getAddressByte(c) << c*8;
	}
	return temp;
}

void MessageList::addMessage(MACAddress& source, unsigned int sequencenumber)
{
    //SequenceInfoMap::iterator seqIter;
    //SourceInfoMap::iterator sourceIter;

	keyMSG temp(MacToLonglong(source),sequencenumber);

	MapTable::iterator tableIter = table.find(temp);
	if(tableIter != table.end()){
			tableIter -> second += 1;
	}else{
		table.insert(key_Data(temp,1));
	}
}

bool MessageList::limitCheck(MACAddress& source, unsigned int sequencenumber, int limit)
{

	keyMSG templ(MacToLonglong(source),sequencenumber);
	int dublikat = 0;

	MapTable::iterator tableIter = table.find(templ);
	if(tableIter != table.end()){
			dublikat = tableIter -> second;
	}else{
		dublikat = 0;
	}

	if (dublikat < limit){
    	//EV << "Message ist KEIN Duplikat\n";
    	return false;
    }else{
        //EV << "Message ist  ein Duplikat\n";
    	return true;
    }
}

bool MessageList::duplicateCheck(MACAddress& source, unsigned int sequencenumber)
{
	return limitCheck(source, sequencenumber, 1);
}
