//
// Generated file, do not edit! Created by opp_msgc 4.5 from nodeTable/nodeTableTimeoutMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "nodeTableTimeoutMessage_m.h"

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

Register_Class(NodeTableTimeoutMessage);

NodeTableTimeoutMessage::NodeTableTimeoutMessage(const char *name, int kind) : ::cMessage(name,kind)
{
}

NodeTableTimeoutMessage::NodeTableTimeoutMessage(const NodeTableTimeoutMessage& other) : ::cMessage(other)
{
    copy(other);
}

NodeTableTimeoutMessage::~NodeTableTimeoutMessage()
{
}

NodeTableTimeoutMessage& NodeTableTimeoutMessage::operator=(const NodeTableTimeoutMessage& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void NodeTableTimeoutMessage::copy(const NodeTableTimeoutMessage& other)
{
    this->node_var = other.node_var;
}

void NodeTableTimeoutMessage::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->node_var);
}

void NodeTableTimeoutMessage::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->node_var);
}

MACAddress& NodeTableTimeoutMessage::getNode()
{
    return node_var;
}

void NodeTableTimeoutMessage::setNode(const MACAddress& node)
{
    this->node_var = node;
}

class NodeTableTimeoutMessageDescriptor : public cClassDescriptor
{
  public:
    NodeTableTimeoutMessageDescriptor();
    virtual ~NodeTableTimeoutMessageDescriptor();

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

Register_ClassDescriptor(NodeTableTimeoutMessageDescriptor);

NodeTableTimeoutMessageDescriptor::NodeTableTimeoutMessageDescriptor() : cClassDescriptor("NodeTableTimeoutMessage", "cMessage")
{
}

NodeTableTimeoutMessageDescriptor::~NodeTableTimeoutMessageDescriptor()
{
}

bool NodeTableTimeoutMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NodeTableTimeoutMessage *>(obj)!=NULL;
}

const char *NodeTableTimeoutMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NodeTableTimeoutMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int NodeTableTimeoutMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *NodeTableTimeoutMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "node",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int NodeTableTimeoutMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "node")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NodeTableTimeoutMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "MACAddress",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *NodeTableTimeoutMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NodeTableTimeoutMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NodeTableTimeoutMessage *pp = (NodeTableTimeoutMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NodeTableTimeoutMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NodeTableTimeoutMessage *pp = (NodeTableTimeoutMessage *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getNode(); return out.str();}
        default: return "";
    }
}

bool NodeTableTimeoutMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NodeTableTimeoutMessage *pp = (NodeTableTimeoutMessage *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *NodeTableTimeoutMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(MACAddress));
        default: return NULL;
    };
}

void *NodeTableTimeoutMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NodeTableTimeoutMessage *pp = (NodeTableTimeoutMessage *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getNode()); break;
        default: return NULL;
    }
}


