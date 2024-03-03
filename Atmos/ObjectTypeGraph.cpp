
#include "ObjectTypeGraph.h"

namespace Atmos
{
    ObjectTypeGraph::ObjectTypeGraph()
    {}

    void ObjectTypeGraph::AddDescription(const ObjectTypeDescription& add)
    {
        auto created = nodes.emplace(add.typeName, Node(add.typeName));

        for (auto& loop : add.baseTypes)
        {
            auto found = nodes.emplace(loop, Node(loop));
            found.first->second.children.push_back(&created.first->second);
        }
    }

    ObjectTypeDescriptionGroup ObjectTypeGraph::AllDescriptions() const
    {
        typedef std::vector<ObjectTypeDescription> DescriptionList;

        DescriptionList descriptions;

        for (auto& loop : nodes)
            descriptions.push_back(ObjectTypeDescription(loop.second.typeName));

        return ObjectTypeDescriptionGroup(std::move(descriptions));
    }

    ObjectTypeDescriptionGroup ObjectTypeGraph::GroupFor(const ObjectTypeName& typeName) const
    {
        typedef std::vector<ObjectTypeDescription> DescriptionList;

        DescriptionList descriptions;

        const auto extractGroup = [&](const Node* from) -> void
        {
            auto extractGroupImpl = [&](const Node* from, auto& extractGroupRef) mutable -> void
            {
                descriptions.push_back(from->typeName);
                for (auto& loop : from->children)
                    extractGroupRef(loop, extractGroupRef);
            };

            return extractGroupImpl(from, extractGroupImpl);
        };

        for (auto& loop : nodes)
        {
            if (loop.first != typeName)
                continue;

            extractGroup(&loop.second);
            break;
        }

        return ObjectTypeDescriptionGroup(std::move(descriptions));
    }

    ObjectTypeGraph::Node::Node(const ObjectTypeName& typeName) : typeName(typeName)
    {}
}