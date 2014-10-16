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

#include "ListenErkennung.h"
#include "hsrMessage_m.h"
#include "prpMessage_m.h"
#include "MessagePacker.h"

Define_Module(ListenErkennung)
;

ListenErkennung::ListenErkennung()
{
}
ListenErkennung::~ListenErkennung()
{
}

void
ListenErkennung::initialize()
{
    duplicateLifeTimeAddToLastWrite.setName("duplicateLifeTime Add To LastWrite");
    duplicateLifeTimeAddToLastRead.setName("duplicateLifeTime Add To LastRead");
    duplicateTimeLastWriteToNow.setName("duplicateTime LastWrite To Now");
    duplicateTimeLastReadToNow.setName("duplicateTime LastRead To Now");
    duplicateAddIndex.setName("duplicate Add Index");
    duplicateAccessIndex.setName("duplicate Access Index");
    duplicateAddIndexMinusAccessIndex.setName("duplicate Add Index - Access Index");

    listSize = par("ListSize");
    if(listSize < 1)
    {
        throw cRuntimeError("Anzahl erkennbarer Duplikate kann nicht negativ oder 0 sein ! \n");
        listSize = 0;
    }

    dupplikatListe = (ListItemData*) malloc(sizeof(ListItemData) * listSize);
    //[ListSize]

    for (int c = 0; c < listSize; c++)
    {
        //Inhalt
        dupplikatListe[c].source.setAddress("00:00:00:00:00:00");
        dupplikatListe[c].sequencenumber = 0;

        //Verwaltungsinformationen
        dupplikatListe[c].addcount = 0;
        dupplikatListe[c].laufnummer = c;
        dupplikatListe[c].usecount = 0;
        dupplikatListe[c].addTime = SimTime(0);
        dupplikatListe[c].lastReadTime = SimTime(0);
        dupplikatListe[c].lastWriteTime = SimTime(0);
    }

    dupplikatListenPointer = 0;
    backup = new MessageList();
}

void
ListenErkennung::finish()
{
    //Zeitverlauf einhalten
    for (int c = dupplikatListenPointer; c < listSize; c++)
    {
        registerDuplicateEntry(dupplikatListe[c], true);
    }

    for (int c = 0; c < dupplikatListenPointer; c++)
    {
        registerDuplicateEntry(dupplikatListe[c], true);
    }

    //Sorgt daf�r, das die Zeitskala stimmt
    duplicateTimeLastWriteToNow.record(0);
    duplicateTimeLastReadToNow.record(0);

    if (backup != NULL)
    {
        delete backup;
        backup = NULL;
    }

    free(dupplikatListe);
}

int
ListenErkennung::checkFrame(EthernetIIFrame *ethTag)
{
    int result = 0;
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;

    MessagePacker::decapsulateMessage(&ethTag, &vlanTag, &hsrTag, &messageData);

    //EV << "DUP DETECT: " << ethTag->getSrc();
    if(hsrTag == NULL)
    {
        //Wenn das Paket kein HSR Paket ist wird es einfach durchgereicht
    	throw cRuntimeError("Paket hat kein HSR Tag! Panik ! \n");
        result = 1;

    }
    else if (limitCheck(ethTag->getSrc(), hsrTag->getSequencenumber(), 1) == false)
    {
        addMessage(ethTag->getSrc(), hsrTag->getSequencenumber());
        result = 1;
        //EV << " SEQ NUM " << hsrTag->getSequencenumber();
    }
    //EV << " Result: " << result << endl;
    //EV << "DUP DETECT: " << ethTag->getSrc() << " " << hsrTag->getSequencenumber() << " " << result << endl;
    MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
    return result;
}

void
ListenErkennung::handleMessage(cMessage *msg)
{
    EthernetIIFrame *ethTag = check_and_cast<EthernetIIFrame *> (msg);
    vlanMessage *vlanTag = NULL;
    hsrMessage *hsrTag = NULL;
    dataMessage *messageData = NULL;


    if (checkFrame(check_and_cast<EthernetIIFrame *>(ethTag->dup())) == 1)
    {
    	send(ethTag,    gate("gate$o"));
    }
    else
    {
	    MessagePacker::decapsulateMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
	    MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
    	//delete msg;
    }

//    MessagePacker::decapsulateMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
//
//    if(hsrTag == NULL)
//    {
//        //Wenn das Paket kein HSR Paket ist wird es einfach durchgereicht
//        send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gate("gate$o"));
//    }
//    else if (limitCheck(ethTag->getSrc(), hsrTag->getSequencenumber(), 1) == false)
//    {
//        addMessage(ethTag->getSrc(), hsrTag->getSequencenumber());
//        send(MessagePacker::generateEthMessage(ethTag, vlanTag, hsrTag, messageData), gate("gate$o"));
//    }
//    MessagePacker::deleteMessage(&ethTag, &vlanTag, &hsrTag, &messageData);
}

long
ListenErkennung::searchMessage(MACAddress& source, short sequencenumber)
{
    //EV << "ListenErkennung searchMessage1 " << source << " " << destination << " " << sequencenumber << " " << ((int)(path)) << " \n";
    //EV << "dupplikatListenPointer " << dupplikatListenPointer << " \n";
    for (int c = dupplikatListenPointer - 1; c >= 0; c--)
    {

        //EV << "ListenErkennung searchMessage2 " << dupplikatListe[c].source << " " << dupplikatListe[c].destination << " " << dupplikatListe[c].sequencenumber << " " << ((int)(dupplikatListe[c].path)) << " " << c  << " " <<  dupplikatListe[c].laufnummer << " " << " \n";
        if ((dupplikatListe[c].source.equals(source)) && (dupplikatListe[c].sequencenumber == sequencenumber))
        {
            //EV << " JA \n";
            duplicateAccessIndex.record(c);
            if (c > dupplikatListenPointer)
                duplicateAddIndexMinusAccessIndex.record(dupplikatListenPointer - c + listSize);
            else
                duplicateAddIndexMinusAccessIndex.record(dupplikatListenPointer - c);
            return c;
        }
    }
    for (int c = listSize - 1; c >= dupplikatListenPointer; c--)
    {
        //EV << "ListenErkennung searchMessage2 " << dupplikatListe[dupplikatListenPointer].source << " " << dupplikatListe[dupplikatListenPointer].destination << " " << dupplikatListe[dupplikatListenPointer].sequencenumber << " " << ((int)(dupplikatListe[dupplikatListenPointer].path)) << " " << c << " \n";
        if ((dupplikatListe[c].source.equals(source)) && (dupplikatListe[c].sequencenumber == sequencenumber))
        {
            //EV << " JA \n";
            duplicateAccessIndex.record(c);
            if (c > dupplikatListenPointer)
                duplicateAddIndexMinusAccessIndex.record(dupplikatListenPointer - c + listSize);
            else
                duplicateAddIndexMinusAccessIndex.record(dupplikatListenPointer - c);
            return c;
        }
    }
    //EV << " NEIN \n";
    return -1;
}

void
ListenErkennung::addMessage(MACAddress& source, short sequencenumber)
{
    //EV << "ListenErkennung add " << source << " " << destination << " " << sequencenumber << " " << ((int)(path)) << " \n";
    Enter_Method("addMessage(MACAddress& source, short sequencenumber)");
    long pointer = searchMessage(source, sequencenumber);

    if (pointer != -1)
    {
        dupplikatListe[pointer].addcount++;
        dupplikatListe[pointer].lastWriteTime = simTime();
        //EV << "ListenErkennung add2 " << dupplikatListe[pointer].addcount << " \n";
    }
    else
    {
        registerDuplicateEntry(dupplikatListe[dupplikatListenPointer], false);
        dupplikatListe[dupplikatListenPointer].source = source;
        dupplikatListe[dupplikatListenPointer].sequencenumber = sequencenumber;
        dupplikatListe[dupplikatListenPointer].addcount = 1;

        dupplikatListe[dupplikatListenPointer].usecount = 0;
        dupplikatListe[dupplikatListenPointer].addTime = simTime();
        dupplikatListe[dupplikatListenPointer].lastWriteTime = simTime();
        dupplikatListe[dupplikatListenPointer].lastReadTime = SimTime(0);

        duplicateAddIndex.record(dupplikatListenPointer);
        dupplikatListenPointer++;
        if (listSize == dupplikatListenPointer)
        {
            dupplikatListenPointer = 0;
        }
        //EV << "ListenErkennung add2 " << dupplikatListe[dupplikatListenPointer-1].source << " " << dupplikatListe[dupplikatListenPointer-1].destination << " " << dupplikatListe[dupplikatListenPointer-1].sequencenumber << " " << ((int)(dupplikatListe[dupplikatListenPointer-1].path)) << " " << dupplikatListenPointer << " \n";
    }

    //EV << "ListenErkennung add  end \n";

    backup->addMessage(source, sequencenumber);
}

bool
ListenErkennung::limitCheck(MACAddress& source, short sequencenumber, int limit)
{
    Enter_Method("limitCheck(MACAddress& source, short sequencenumber, int limit)");
    //EV << "ListenErkennung limitCheck " << source << " " << destination << " " << sequencenumber << " " << ((int)(path)) << " " << limit <<" \n";

    bool result = backup->limitCheck(source, sequencenumber, limit);

    long pointer = searchMessage(source, sequencenumber);

    if (pointer != -1)
    {
        dupplikatListe[pointer].usecount++;
        dupplikatListe[pointer].lastReadTime = simTime();
        if (dupplikatListe[pointer].addcount < limit)
        {

            if (result != false)
                throw cRuntimeError(" limitCheck error: limit falsch ! \n");

            return false;
        }

        if (result != true)
            throw cRuntimeError(" limitCheck error: treffer gefunden wo es gar keinen gibt ! \n");

        return true;
    }
    else
    {
        //EV << " nix gefunden \n";
        if (result != false)
        {
            //EV<< " limitCheck error: Duplikat verpasst ! \n";

        }
        //throw cRuntimeError(" limitCheck error: dupplikat verpasst ! \n");

        return false;
    }
}

void
ListenErkennung::registerDuplicateEntry(ListItemData entry, bool finish)
{
    if (entry.addcount != 0)
    {
        duplicateLifeTimeAddToLastWrite.recordWithTimestamp(entry.addTime, entry.lastWriteTime - entry.addTime);
        if (!finish)
            duplicateTimeLastWriteToNow.recordWithTimestamp(entry.addTime, simTime() - entry.lastWriteTime);

        //Wenn nie gelesen wurde, gibt es keine Lesezeit, was diese beiden Werte nutzlos macht
        if (entry.usecount != 0)
        {
            duplicateLifeTimeAddToLastRead.recordWithTimestamp(entry.addTime, entry.lastReadTime - entry.addTime);
            if (!finish)
                duplicateTimeLastReadToNow.recordWithTimestamp(entry.addTime, simTime() - entry.lastReadTime);
        }
    }
}
/*
 void ListenErkennung::addMessage(hsrMessage* msg)
 {
 if(msg == NULL)
 {
 EV << "pointer is null";
 throw cRuntimeError(" Panik ! \n");
 }
 addMessage(msg->getSrc(),msg->getDest(),msg->getSequencenumber(),msg->getPath());
 }

 bool ListenErkennung::limitCheck(hsrMessage *msg, int limit)
 {
 if(msg == NULL)
 {
 EV << "pointer is null";
 throw cRuntimeError(" Panik ! \n");

 return false;
 }
 return limitCheck(msg->getSrc(),msg->getDest(),msg->getSequencenumber(),msg->getPath(), limit);
 }

 bool ListenErkennung::duplicateCheck(hsrMessage *msg)
 {

 if(msg == NULL)
 {
 EV << "pointer is null";
 throw cRuntimeError(" Panik ! \n");

 return false;
 }

 return duplicateCheck(msg->getSrc(),msg->getDest(),msg->getSequencenumber(),msg->getPath());
 }

 bool ListenErkennung::duplicateCheck(MACAddress& source, MACAddress& destination,short sequencenumber,char path)
 {
 return limitCheck(source, destination, sequencenumber, path, 1);
 }
 */
