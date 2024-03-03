
#include "ObjectManager.h"

#include "GameEnvironment.h"
#include "Assert.h"

#include <Inscription/Scribe.h>
#include <Inscription/Memory.h>
#include <Inscription/Multimap.h>

namespace Atmos
{
    ObjectManager::ObjectManager()
    {}

    void ObjectManager::Initialize()
    {
        for (auto& loop : systems)
            loop.second->Initialize();
    }

    void ObjectManager::Work()
    {
        for (auto& loop : systems)
            loop.second->Work();
    }

    ObjectReference ObjectManager::FindObject(ObjectID id)
    {
        auto found = objects.Find(id);
        if (found == objects.end())
            return ObjectReference(nullptr);

        return ObjectReference(found->get());
    }

    void ObjectManager::DestroyObject(ObjectID id)
    {
        for (auto loop = objects.begin(); loop != objects.end(); ++loop)
        {
            if ((*loop)->ID() != id)
                continue;

            NotifyDestruction(**loop);
            NotifyBatchSourcesDestruction(**loop);
            objects.Remove(loop.ID());
            return;
        }
    }

    void ObjectManager::DestroyObject(ObjectReference reference)
    {
        if (!reference.IsOccupied())
            return;

        DestroyObject(reference->ID());
    }

    ObjectManager::SizeT ObjectManager::Size() const
    {
        return objects.Size();
    }

    ObjectTypeGraph ObjectManager::TypeGraph() const
    {
        return typeGraph;
    }

    ObjectTypeDescriptionGroup ObjectManager::DescriptionGroupFor(const ObjectTypeName& typeName)
    {
        return typeGraph.GroupFor(typeName);
    }

    void ObjectManager::NotifyCreation(Object& object)
    {
        onAdded(ObjectReference(object));
    }

    void ObjectManager::NotifyDestruction(Object& object)
    {
        onBeforeDestroyed(object);
    }

    ObjectBatchSourceBase* ObjectManager::FindBatchSource(const ObjectTypeName& typeName)
    {
        auto &found = batchSources.find(typeName);
        if (found == batchSources.end())
            return nullptr;

        return found->second.get();
    }

    void ObjectManager::NotifyBatchSourcesCreation(Object& object)
    {
        for (auto& loop : batchSources)
            loop.second->NotifyCreated(object);
    }

    void ObjectManager::NotifyBatchSourcesDestruction(Object& object)
    {
        for (auto& loop : batchSources)
            loop.second->NotifyDestroyed(object);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ObjectManager)
    {
        if (scribe.IsOutput())
        {
            scribe.Save(objects);
            scribe.Save(batchSources);
            scribe.Save(systems);
        }
        else // INPUT
        {
            scribe.Load(objects);
            auto loop = objects.begin();
            auto end = objects.end();
            auto test = loop == end;
            for (auto loop = objects.begin(); loop != objects.end(); ++loop)
            {
                (*loop)->id = loop.ID();
                (*loop)->manager = this;
            }

            scribe.Load(batchSources);
            scribe.Load(systems);
        }
    }
}