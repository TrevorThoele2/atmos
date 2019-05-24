
#include "ObjectSystem.h"

#include "ObjectManager.h"

namespace Atmos
{
    ObjectSystem::ObjectSystem(ObjectManager& manager) : manager(&manager), isInitialized(false), isStarted(false)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(ObjectSystem) : INSCRIPTION_TABLE_GET_MEM(manager)
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
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::ObjectSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_UNOWNING_POINTER(manager);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}