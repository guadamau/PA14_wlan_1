//
// Generated file, do not edit! Created by opp_msgc 4.5 from messages/hsrMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "hsrMessage_m.h"

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

Register_Class(hsrMessage);

hsrMessage::hsrMessage(const char *name, int kind) : ::cPacket(name,kind)
{
    this->path_var = 0;
    this->sequencenumber_var = 0;
    this->lsdu_size_var = 0;
    this->nextEtherType_var = 0;
    this->ownEtherType_var = 0x88FB;
}

hsrMessage::hsrMessage(const hsrMessage& other) : ::cPacket(other)
{
    copy(other);
}

hsrMessage::~hsrMessage()
{
}

hsrMessage& hsrMessage::operator=(const hsrMessage& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void hsrMessage::copy(const hsrMessage& other)
{
    this->path_var = other.path_var;
    this->sequencenumber_var = other.sequencenumber_var;
    this->lsdu_size_var = other.lsdu_size_var;
    this->nextEtherType_var = other.nextEtherType_var;
    this->ownEtherType_var = other.ownEtherType_var;
}

void hsrMessage::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->path_var);
    doPacking(b,this->sequencenumber_var);
    doPacking(b,this->lsdu_size_var);
    doPacking(b,this->nextEtherType_var);
    doPacking(b,this->ownEtherType_var);
}

void hsrMessage::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->path_var);
    doUnpacking(b,this->sequencenumber_var);
    doUnpacking(b,this->lsdu_size_var);
    doUnpacking(b,this->nextEtherType_var);
    doUnpacking(b,this->ownEtherType_var);
}

unsigned int hsrMessage::getPath() const
{
    return path_var;
}

void hsrMessage::setPath(unsigned int path)
{
    this->path_var = path;
}

unsigned int hsrMessage::getSequencenumber() const
{
    return sequencenumber_var;
}

void hsrMessage::setSequencenumber(unsigned int sequencenumber)
{
    this->sequencenumber_var = sequencenumber;
}

unsigned int hsrMessage::getLsdu_size() const
{
    return lsdu_size_var;
}

void hsrMessage::setLsdu_size(unsigned int lsdu_size)
{
    this->lsdu_size_var = lsdu_size;
}

int hsrMessage::getNextEtherType() const
{
    return nextEtherType_var;
}

void hsrMessage::setNextEtherType(int nextEtherType)
{
    this->nextEtherType_var = nextEtherType;
}

int hsrMessage::getOwnEtherType() const
{
    return ownEtherType_var;
}

void hsrMessage::setOwnEtherType(int ownEtherType)
{
    this->ownEtherType_var = ownEtherType;
}

class hsrMessageDescriptor : public cClassDescriptor
{
  public:
    hsrMessageDescriptor();
    virtual ~hsrMessageDescriptor();

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

Register_ClassDescriptor(hsrMessageDescriptor);

hsrMessageDescriptor::hsrMessageDescriptor() : cClassDescriptor("hsrMessage", "cPacket")
{
}

hsrMessageDescriptor::~hsrMessageDescriptor()
{
}

bool hsrMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<hsrMessage *>(obj)!=NULL;
}

const char *hsrMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int hsrMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int hsrMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *hsrMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "path",
        "sequencenumber",
        "lsdu_size",
        "nextEtherType",
        "ownEtherType",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int hsrMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "path")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequencenumber")==0) return base+1;
    if (fieldName[0]=='l' && strcmp(fieldName, "lsdu_size")==0) return base+2;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextEtherType")==0) return base+3;
    if (fieldName[0]=='o' && strcmp(fieldName, "ownEtherType")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *hsrMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "int",
        "int",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *hsrMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int hsrMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    hsrMessage *pp = (hsrMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string hsrMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    hsrMessage *pp = (hsrMessage *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPath());
        case 1: return ulong2string(pp->getSequencenumber());
        case 2: return ulong2string(pp->getLsdu_size());
        case 3: return long2string(pp->getNextEtherType());
        case 4: return long2string(pp->getOwnEtherType());
        default: return "";
    }
}

bool hsrMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    hsrMessage *pp = (hsrMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setPath(string2ulong(value)); return true;
        case 1: pp->setSequencenumber(string2ulong(value)); return true;
        case 2: pp->setLsdu_size(string2ulong(value)); return true;
        case 3: pp->setNextEtherType(string2long(value)); return true;
        case 4: pp->setOwnEtherType(string2long(value)); return true;
        default: return false;
    }
}

const char *hsrMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *hsrMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    hsrMessage *pp = (hsrMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


