#pragma once

#include "UniqueProviderSystem.h"
#include "WindowBase.h"

namespace Atmos
{
    class WindowSystem : public UniqueProviderSystem<WindowBase>
    {
    public:
        WindowSystem(ObjectManager& manager);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::WindowSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::WindowSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}