//
// Generated file, do not edit! Created by opp_msgc 4.5 from messages/hsrSwitchSelfMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "hsrSwitchSelfMessage_m.h"

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

Register_Class(HsrSwitchSelfMessage);

HsrSwitchSelfMessage::HsrSwitchSelfMessage(const char *name, int kind) : ::cMessage(name,kind)
{
    this->schedulerName_var = 0;
    this->type_var = 0;
}

HsrSwitchSelfMessage::HsrSwitchSelfMessage(const HsrSwitchSelfMessage& other) : ::cMessage(other)
{
    copy(other);
}

HsrSwitchSelfMessage::~HsrSwitchSelfMessage()
{
}

HsrSwitchSelfMessage& HsrSwitchSelfMessage::operator=(const HsrSwitchSelfMessage& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void HsrSwitchSelfMessage::copy(const HsrSwitchSelfMessage& other)
{
    this->schedulerName_var = other.schedulerName_var;
    this->type_var = other.type_var;
}

void HsrSwitchSelfMessage::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->schedulerName_var);
    doPacking(b,this->type_var);
}

void HsrSwitchSelfMessage::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->schedulerName_var);
    doUnpacking(b,this->type_var);
}

unsigned char HsrSwitchSelfMessage::getSchedulerName() const
{
    return schedulerName_var;
}

void HsrSwitchSelfMessage::setSchedulerName(unsigned char schedulerName)
{
    this->schedulerName_var = schedulerName;
}

unsigned char HsrSwitchSelfMessage::getType() const
{
    return type_var;
}

void HsrSwitchSelfMessage::setType(unsigned char type)
{
    this->type_var = type;
}

class HsrSwitchSelfMessageDescriptor : public cClassDescriptor
{
  public:
    HsrSwitchSelfMessageDescriptor();
    virtual ~HsrSwitchSelfMessageDescriptor();

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

Register_ClassDescriptor(HsrSwitchSelfMessageDescriptor);

HsrSwitchSelfMessageDescriptor::HsrSwitchSelfMessageDescriptor() : cClassDescriptor("HsrSwitchSelfMessage", "cMessage")
{
}

HsrSwitchSelfMessageDescriptor::~HsrSwitchSelfMessageDescriptor()
{
}

bool HsrSwitchSelfMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<HsrSwitchSelfMessage *>(obj)!=NULL;
}

const char *HsrSwitchSelfMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int HsrSwitchSelfMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int HsrSwitchSelfMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *HsrSwitchSelfMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "schedulerName",
        "type",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int HsrSwitchSelfMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "schedulerName")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *HsrSwitchSelfMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned char",
        "unsigned char",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *HsrSwitchSelfMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int HsrSwitchSelfMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    HsrSwitchSelfMessage *pp = (HsrSwitchSelfMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string HsrSwitchSelfMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    HsrSwitchSelfMessage *pp = (HsrSwitchSelfMessage *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getSchedulerName());
        case 1: return ulong2string(pp->getType());
        default: return "";
    }
}

bool HsrSwitchSelfMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    HsrSwitchSelfMessage *pp = (HsrSwitchSelfMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setSchedulerName(string2ulong(value)); return true;
        case 1: pp->setType(string2ulong(value)); return true;
        default: return false;
    }
}

const char *HsrSwitchSelfMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *HsrSwitchSelfMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    HsrSwitchSelfMessage *pp = (HsrSwitchSelfMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


