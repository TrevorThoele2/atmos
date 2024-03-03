
#include "ObjectTypeGraphNode.h"

namespace Atmos
{
    ObjectTypeGraphNode::ObjectTypeGraphNode(const ObjectTypeName& typeName) : typeName(typeName), parent(nullptr)
    {}

    void ObjectTypeGraphNode::CreateChild(const ObjectTypeName& typeName)
    {
        auto created = std::make_unique<ObjectTypeGraphNode>(typeName);

        created->parent = this;

        children.push_back(std::move(created));
    }
}