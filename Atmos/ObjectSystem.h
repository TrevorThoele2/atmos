#pragma once

#include "ObjectSystemTraits.h"
#include "ObjectSystemScribe.h"

#include "Serialization.h"

namespace Atmos
{
    class ObjectManager;

    class ObjectSystem
    {
    public:
        ObjectSystem(ObjectManager& manager);
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
    template<>
    class Scribe<::Atmos::ObjectSystem, BinaryArchive> : public CompositeScribe<::Atmos::ObjectSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}