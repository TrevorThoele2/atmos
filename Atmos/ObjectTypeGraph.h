#pragma once

#include <unordered_map>

#include "ObjectTypeGraphNode.h"
#include "ObjectTypeGraphIterator.h"
#include "ObjectTypeDescription.h"
#include "ObjectTypeDescriptionGroup.h"

namespace Atmos
{
    class ObjectTypeGraph
    {
    public:
        typedef ObjectTypeGraphIteratorBase<ObjectTypeGraphNode> iterator;
        typedef ObjectTypeGraphIteratorBase<const ObjectTypeGraphNode> const_iterator;
    public:
        ObjectTypeGraph();

        void AddDescription(const ObjectTypeDescription& add);

        ObjectTypeDescriptionGroup AllDescriptions() const;
        ObjectTypeDescriptionGroup GroupFor(const ObjectTypeName& typeName) const;
    private:
        struct Node
        {
            ObjectTypeName typeName;

            std::vector<Node*> children;

            Node(const ObjectTypeName& typeName);
        };

        std::unordered_map<ObjectTypeName, Node> nodes;
    };
}