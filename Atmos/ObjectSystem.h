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
    protected:
        virtual void InitializeImpl();
        virtual bool StartStepImpl();
        virtual void WorkImpl();
        virtual void StopStepImpl();
        virtual void FinalizeImpl();
    private:
        ObjectManager* manager;
    private:
        bool isInitialized;
        bool isStarted;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::ObjectSystem, BinaryArchive> : public CompositeScribe<::Atmos::ObjectSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}