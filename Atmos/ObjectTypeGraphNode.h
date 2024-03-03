#pragma once

#include <vector>

#include "ObjectTypeName.h"

namespace Atmos
{
    class ObjectTypeGraphNode
    {
    public:
        const ObjectTypeName typeName;
    public:
        ObjectTypeGraphNode(const ObjectTypeName& typeName);

        void CreateChild(const ObjectTypeName& typeName);
    private:
        ObjectTypeGraphNode* parent;

        typedef std::unique_ptr<ObjectTypeGraphNode> NodePtr;
        typedef std::vector<NodePtr> Children;
        Children children;
    private:
        friend class ObjectTypeGraph;
        template<class T>
        friend class ObjectTypeGraphIteratorBase;
    };
}