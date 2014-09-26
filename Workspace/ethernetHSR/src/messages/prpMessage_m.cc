//
// Generated file, do not edit! Created by opp_msgc 4.5 from messages/prpMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "prpMessage_m.h"

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

Register_Class(prpMessage);

prpMessage::prpMessage(const char *name, int kind) : ::EthernetIIFrame(name,kind)
{
    this->sequencenumber_var = 0;
    this->lanIdentifier_var = 0;
    this->lsdu_size_var = 0;
}

prpMessage::prpMessage(const prpMessage& other) : ::EthernetIIFrame(other)
{
    copy(other);
}

prpMessage::~prpMessage()
{
}

prpMessage& prpMessage::operator=(const prpMessage& other)
{
    if (this==&other) return *this;
    ::EthernetIIFrame::operator=(other);
    copy(other);
    return *this;
}

void prpMessage::copy(const prpMessage& other)
{
    this->sequencenumber_var = other.sequencenumber_var;
    this->lanIdentifier_var = other.lanIdentifier_var;
    this->lsdu_size_var = other.lsdu_size_var;
}

void prpMessage::parsimPack(cCommBuffer *b)
{
    ::EthernetIIFrame::parsimPack(b);
    doPacking(b,this->sequencenumber_var);
    doPacking(b,this->lanIdentifier_var);
    doPacking(b,this->lsdu_size_var);
}

void prpMessage::parsimUnpack(cCommBuffer *b)
{
    ::EthernetIIFrame::parsimUnpack(b);
    doUnpacking(b,this->sequencenumber_var);
    doUnpacking(b,this->lanIdentifier_var);
    doUnpacking(b,this->lsdu_size_var);
}

unsigned int prpMessage::getSequencenumber() const
{
    return sequencenumber_var;
}

void prpMessage::setSequencenumber(unsigned int sequencenumber)
{
    this->sequencenumber_var = sequencenumber;
}

unsigned int prpMessage::getLanIdentifier() const
{
    return lanIdentifier_var;
}

void prpMessage::setLanIdentifier(unsigned int lanIdentifier)
{
    this->lanIdentifier_var = lanIdentifier;
}

unsigned int prpMessage::getLsdu_size() const
{
    return lsdu_size_var;
}

void prpMessage::setLsdu_size(unsigned int lsdu_size)
{
    this->lsdu_size_var = lsdu_size;
}

class prpMessageDescriptor : public cClassDescriptor
{
  public:
    prpMessageDescriptor();
    virtual ~prpMessageDescriptor();

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

Register_ClassDescriptor(prpMessageDescriptor);

prpMessageDescriptor::prpMessageDescriptor() : cClassDescriptor("prpMessage", "EthernetIIFrame")
{
}

prpMessageDescriptor::~prpMessageDescriptor()
{
}

bool prpMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<prpMessage *>(obj)!=NULL;
}

const char *prpMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int prpMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int prpMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *prpMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sequencenumber",
        "lanIdentifier",
        "lsdu_size",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int prpMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequencenumber")==0) return base+0;
    if (fieldName[0]=='l' && strcmp(fieldName, "lanIdentifier")==0) return base+1;
    if (fieldName[0]=='l' && strcmp(fieldName, "lsdu_size")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *prpMessageDescriptor::getFieldTypeString(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *prpMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int prpMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    prpMessage *pp = (prpMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string prpMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    prpMessage *pp = (prpMessage *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getSequencenumber());
        case 1: return ulong2string(pp->getLanIdentifier());
        case 2: return ulong2string(pp->getLsdu_size());
        default: return "";
    }
}

bool prpMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    prpMessage *pp = (prpMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setSequencenumber(string2ulong(value)); return true;
        case 1: pp->setLanIdentifier(string2ulong(value)); return true;
        case 2: pp->setLsdu_size(string2ulong(value)); return true;
        default: return false;
    }
}

const char *prpMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *prpMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    prpMessage *pp = (prpMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


