
#include "TypeRegistration.h"

#include "Object.h"
#include "ObjectSystem.h"
#include "ObjectBatchSource.h"
#include "ObjectManager.h"

#include <Inscription/OutputBinaryArchive.h>
#include <Inscription/InputBinaryArchive.h>

namespace Atmos
{
    TypeRegistration::TypeRegistration()
    {}

    void TypeRegistration::PushAllTo(ObjectManager& manager)
    {
        for (auto& loop : groups)
            loop.PushTo(manager);
    }

    void TypeRegistration::PushAllTo(ObjectTypeGraph& graph)
    {
        for (auto& loop : groups)
            loop.PushTo(graph);
    }

    void TypeRegistration::PushAllTo(::Inscription::BinaryArchive& archive)
    {
        for (auto& loop : groups)
            loop.PushTo(archive);
    }

    TypeRegistration::Entry::~Entry()
    {}

    void TypeRegistration::Group::PushTo(ObjectManager& manager)
    {
        for (auto& loop : entryList)
            loop->PushTo(manager);
    }

    void TypeRegistration::Group::PushTo(ObjectTypeGraph& graph)
    {
        for (auto& loop : entryList)
            loop->PushTo(graph);
    }

    void TypeRegistration::Group::PushTo(::Inscription::BinaryArchive& archive)
    {
        for (auto& loop : entryList)
            loop->PushTo(archive);
    }

    TypeRegistration::Group* TypeRegistration::CreateGroup()
    {
        groups.push_back(Group());
        return &groups.back();
    }
}