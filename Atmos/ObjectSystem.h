#pragma once

#include "ObjectSystemTraits.h"

#include "Serialization.h"

namespace Atmos
{
    class ObjectManager;

    class ObjectSystem
    {
    public:
        ObjectSystem(ObjectManager& manager);
        ObjectSystem(const ::Inscription::Table<ObjectSystem>& table);
        virtual ~ObjectSystem() = 0;

        void Initialize();
        void StartStep();
        void Work();
        void StopStep();
        void Finalize();

        bool IsInitialized() const;
        bool IsStarted() const;
    protected:
        ObjectManager* Manager();
        const ObjectManager* Manager() const;
    private:
        ObjectManager* manager;
    private:
        bool isInitialized;
        bool isStarted;

        virtual void InitializeImpl();
        virtual bool StartStepImpl();
        virtual void WorkImpl();
        virtual void StopStepImpl();
        virtual void FinalizeImpl();
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::ObjectSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };
}