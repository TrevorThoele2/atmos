#pragma once

#include "UniqueProviderSystem.h"
#include "InputManager.h"

namespace Atmos::Input
{
    class System : public UniqueProviderSystem<Manager>
    {
    public:
        System(ObjectManager& manager);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Input::System, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Input::System, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}