#pragma once

#include "UniqueProviderSystem.h"
#include "WindowBase.h"

namespace Atmos::Window
{
    class WindowSystem : public UniqueProviderSystem<WindowBase>
    {
    public:
        WindowSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Window::WindowSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Window::WindowSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}