#pragma once

#include "UnownedProviderSystem.h"

#include "Engine.h"

#include "Event.h"

namespace Atmos
{
    class EngineSystem : public UnownedProviderSystem<Engine>
    {
    public:
        Event<> onFocusLost;
        Event<> onFocusRegained;
    public:
        EngineSystem(ObjectManager& manager);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::EngineSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::EngineSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}