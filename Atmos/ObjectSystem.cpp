
#include "ObjectSystem.h"

#include "ObjectManager.h"

namespace Atmos
{
    ObjectSystem::ObjectSystem(ObjectManager& manager) : manager(&manager), isInitialized(false), isStarted(false)
    {}

    ObjectSystem::~ObjectSystem()
    {}

    void ObjectSystem::Initialize()
    {
        InitializeImpl();
        isInitialized = true;
    }

    void ObjectSystem::StartStep()
    {
        isStarted = StartStepImpl();
    }

    void ObjectSystem::Work()
    {
        WorkImpl();
    }

    void ObjectSystem::StopStep()
    {
        StopStepImpl();
        isStarted = false;
    }

    void ObjectSystem::Finalize()
    {
        FinalizeImpl();
    }

    bool ObjectSystem::IsInitialized() const
    {
        return isInitialized;
    }

    bool ObjectSystem::IsStarted() const
    {
        return isStarted;
    }

    ObjectManager* ObjectSystem::Manager()
    {
        return manager;
    }

    const ObjectManager* ObjectSystem::Manager() const
    {
        return manager;
    }

    void ObjectSystem::InitializeImpl()
    {}

    bool ObjectSystem::StartStepImpl()
    {
        return true;
    }

    void ObjectSystem::WorkImpl()
    {}

    void ObjectSystem::StopStepImpl()
    {}

    void ObjectSystem::FinalizeImpl()
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::ObjectSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.manager);
    }

    void Scribe<::Atmos::ObjectSystem, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}