//
// Generated file, do not edit! Created by opp_msgc 4.5 from messages/vlanMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "vlanMessage_m.h"

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

Register_Class(vlanMessage);

vlanMessage::vlanMessage(const char *name, int kind) : ::cPacket(name,kind)
{
    this->user_priority_var = 0;
    this->canonical_format_indicator_var = 0;
    this->vlan_identifier_var = 0;
    this->nextEtherType_var = 0;
    this->ownEtherType_var = 0x8100;
    this->preemptionDelay_var = SIMTIME_ZERO;
}

vlanMessage::vlanMessage(const vlanMessage& other) : ::cPacket(other)
{
    copy(other);
}

vlanMessage::~vlanMessage()
{
}

vlanMessage& vlanMessage::operator=(const vlanMessage& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void vlanMessage::copy(const vlanMessage& other)
{
    this->user_priority_var = other.user_priority_var;
    this->canonical_format_indicator_var = other.canonical_format_indicator_var;
    this->vlan_identifier_var = other.vlan_identifier_var;
    this->nextEtherType_var = other.nextEtherType_var;
    this->ownEtherType_var = other.ownEtherType_var;
    this->preemptionDelay_var = other.preemptionDelay_var;
}

void vlanMessage::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->user_priority_var);
    doPacking(b,this->canonical_format_indicator_var);
    doPacking(b,this->vlan_identifier_var);
    doPacking(b,this->nextEtherType_var);
    doPacking(b,this->ownEtherType_var);
    doPacking(b,this->preemptionDelay_var);
}

void vlanMessage::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->user_priority_var);
    doUnpacking(b,this->canonical_format_indicator_var);
    doUnpacking(b,this->vlan_identifier_var);
    doUnpacking(b,this->nextEtherType_var);
    doUnpacking(b,this->ownEtherType_var);
    doUnpacking(b,this->preemptionDelay_var);
}

int vlanMessage::getUser_priority() const
{
    return user_priority_var;
}

void vlanMessage::setUser_priority(int user_priority)
{
    this->user_priority_var = user_priority;
}

int vlanMessage::getCanonical_format_indicator() const
{
    return canonical_format_indicator_var;
}

void vlanMessage::setCanonical_format_indicator(int canonical_format_indicator)
{
    this->canonical_format_indicator_var = canonical_format_indicator;
}

int vlanMessage::getVlan_identifier() const
{
    return vlan_identifier_var;
}

void vlanMessage::setVlan_identifier(int vlan_identifier)
{
    this->vlan_identifier_var = vlan_identifier;
}

int vlanMessage::getNextEtherType() const
{
    return nextEtherType_var;
}

void vlanMessage::setNextEtherType(int nextEtherType)
{
    this->nextEtherType_var = nextEtherType;
}

int vlanMessage::getOwnEtherType() const
{
    return ownEtherType_var;
}

void vlanMessage::setOwnEtherType(int ownEtherType)
{
    this->ownEtherType_var = ownEtherType;
}

simtime_t vlanMessage::getPreemptionDelay() const
{
    return preemptionDelay_var;
}

void vlanMessage::setPreemptionDelay(simtime_t preemptionDelay)
{
    this->preemptionDelay_var = preemptionDelay;
}

class vlanMessageDescriptor : public cClassDescriptor
{
  public:
    vlanMessageDescriptor();
    virtual ~vlanMessageDescriptor();

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

Register_ClassDescriptor(vlanMessageDescriptor);

vlanMessageDescriptor::vlanMessageDescriptor() : cClassDescriptor("vlanMessage", "cPacket")
{
}

vlanMessageDescriptor::~vlanMessageDescriptor()
{
}

bool vlanMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<vlanMessage *>(obj)!=NULL;
}

const char *vlanMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int vlanMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int vlanMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *vlanMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "user_priority",
        "canonical_format_indicator",
        "vlan_identifier",
        "nextEtherType",
        "ownEtherType",
        "preemptionDelay",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int vlanMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='u' && strcmp(fieldName, "user_priority")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "canonical_format_indicator")==0) return base+1;
    if (fieldName[0]=='v' && strcmp(fieldName, "vlan_identifier")==0) return base+2;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextEtherType")==0) return base+3;
    if (fieldName[0]=='o' && strcmp(fieldName, "ownEtherType")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "preemptionDelay")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *vlanMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "simtime_t",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *vlanMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int vlanMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    vlanMessage *pp = (vlanMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string vlanMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    vlanMessage *pp = (vlanMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getUser_priority());
        case 1: return long2string(pp->getCanonical_format_indicator());
        case 2: return long2string(pp->getVlan_identifier());
        case 3: return long2string(pp->getNextEtherType());
        case 4: return long2string(pp->getOwnEtherType());
        case 5: return double2string(pp->getPreemptionDelay());
        default: return "";
    }
}

bool vlanMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    vlanMessage *pp = (vlanMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setUser_priority(string2long(value)); return true;
        case 1: pp->setCanonical_format_indicator(string2long(value)); return true;
        case 2: pp->setVlan_identifier(string2long(value)); return true;
        case 3: pp->setNextEtherType(string2long(value)); return true;
        case 4: pp->setOwnEtherType(string2long(value)); return true;
        case 5: pp->setPreemptionDelay(string2double(value)); return true;
        default: return false;
    }
}

const char *vlanMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *vlanMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    vlanMessage *pp = (vlanMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


