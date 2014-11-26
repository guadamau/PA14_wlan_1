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

#ifndef LISTENERKENNUNG_H_
#define LISTENERKENNUNG_H_

#include <omnetpp.h>
#include "MACAddress.h"
#include "MessageList.h"
#include "hsrDefines.h"
#include "EtherFrame_m.h"

typedef struct
{
    //Inhalt
    MACAddress source;
    unsigned int  sequencenumber;

    //Verwaltungsinformationen
    long addcount;
    unsigned long laufnummer;
    unsigned long usecount;
    simtime_t addTime;
    simtime_t lastReadTime;
    simtime_t lastWriteTime;
} ListItemData;

class ListenErkennung  : public cSimpleModule {

private:
    ListItemData* dupplikatListe;
    long dupplikatListenPointer;
    int listSize;

    MessageList* backup;

    cOutVector duplicateLifeTimeAddToLastWrite;
    cOutVector duplicateLifeTimeAddToLastRead;
    cOutVector duplicateTimeLastWriteToNow;
    cOutVector duplicateTimeLastReadToNow;
    cOutVector duplicateAddIndex;
    cOutVector duplicateAccessIndex;
    cOutVector duplicateAddIndexMinusAccessIndex;

    void registerDuplicateEntry (ListItemData entry, bool finish);
    long searchMessage(MACAddress& source,unsigned int sequencenumber);



protected:
  virtual void initialize();
  virtual void handleMessage(cMessage *msg);
  virtual void finish();


public:
    ListenErkennung();
    virtual ~ListenErkennung();

    void addMessage(MACAddress& source, unsigned int sequencenumber);
    bool limitCheck(MACAddress& source, unsigned int sequencenumber, int limit);
    //bool duplicateCheck(MACAddress& source, unsigned int sequencenumber);
    
    int  checkFrame(EthernetIIFrame *ethTag);
    
};

#endif /* LISTENERKENNUNG_H_ */
