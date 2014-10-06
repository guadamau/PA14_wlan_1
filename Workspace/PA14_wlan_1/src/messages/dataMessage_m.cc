//
// Generated file, do not edit! Created by opp_msgc 4.5 from messages/dataMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "dataMessage_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(dataMessage);

dataMessage::dataMessage(const char *name, int kind) : ::cPacket(name,kind)
{
    this->counter_var = 0;
    this->sendTime_var = 0;
    for (unsigned int i=0; i<500; i++)
        this->randomData_var[i] = 0;
    this->ownEtherType_var = 0x0800;
}

dataMessage::dataMessage(const dataMessage& other) : ::cPacket(other)
{
    copy(other);
}

dataMessage::~dataMessage()
{
}

dataMessage& dataMessage::operator=(const dataMessage& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void dataMessage::copy(const dataMessage& other)
{
    this->counter_var = other.counter_var;
    this->sendTime_var = other.sendTime_var;
    for (unsigned int i=0; i<500; i++)
        this->randomData_var[i] = other.randomData_var[i];
    this->ownEtherType_var = other.ownEtherType_var;
}

void dataMessage::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->counter_var);
    doPacking(b,this->sendTime_var);
    doPacking(b,this->randomData_var,500);
    doPacking(b,this->ownEtherType_var);
}

void dataMessage::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->counter_var);
    doUnpacking(b,this->sendTime_var);
    doUnpacking(b,this->randomData_var,500);
    doUnpacking(b,this->ownEtherType_var);
}

unsigned long dataMessage::getCounter() const
{
    return counter_var;
}

void dataMessage::setCounter(unsigned long counter)
{
    this->counter_var = counter;
}

double dataMessage::getSendTime() const
{
    return sendTime_var;
}

void dataMessage::setSendTime(double sendTime)
{
    this->sendTime_var = sendTime;
}

unsigned int dataMessage::getRandomDataArraySize() const
{
    return 500;
}

char dataMessage::getRandomData(unsigned int k) const
{
    if (k>=500) throw cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return randomData_var[k];
}

void dataMessage::setRandomData(unsigned int k, char randomData)
{
    if (k>=500) throw cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->randomData_var[k] = randomData;
}

int dataMessage::getOwnEtherType() const
{
    return ownEtherType_var;
}

void dataMessage::setOwnEtherType(int ownEtherType)
{
    this->ownEtherType_var = ownEtherType;
}

class dataMessageDescriptor : public cClassDescriptor
{
  public:
    dataMessageDescriptor();
    virtual ~dataMessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(dataMessageDescriptor);

dataMessageDescriptor::dataMessageDescriptor() : cClassDescriptor("dataMessage", "cPacket")
{
}

dataMessageDescriptor::~dataMessageDescriptor()
{
}

bool dataMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<dataMessage *>(obj)!=NULL;
}

const char *dataMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int dataMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int dataMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *dataMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "counter",
        "sendTime",
        "randomData",
        "ownEtherType",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int dataMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "counter")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sendTime")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "randomData")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "ownEtherType")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *dataMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned long",
        "double",
        "char",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *dataMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int dataMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    dataMessage *pp = (dataMessage *)object; (void)pp;
    switch (field) {
        case 2: return 500;
        default: return 0;
    }
}

std::string dataMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    dataMessage *pp = (dataMessage *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getCounter());
        case 1: return double2string(pp->getSendTime());
        case 2: return long2string(pp->getRandomData(i));
        case 3: return long2string(pp->getOwnEtherType());
        default: return "";
    }
}

bool dataMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    dataMessage *pp = (dataMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setCounter(string2ulong(value)); return true;
        case 1: pp->setSendTime(string2double(value)); return true;
        case 2: pp->setRandomData(i,string2long(value)); return true;
        case 3: pp->setOwnEtherType(string2long(value)); return true;
        default: return false;
    }
}

const char *dataMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *dataMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    dataMessage *pp = (dataMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


