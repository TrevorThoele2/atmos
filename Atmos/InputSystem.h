#pragma once

#include "UniqueProviderSystem.h"
#include "InputManager.h"

namespace Atmos::Input
{
    class InputSystem : public UniqueProviderSystem<Manager>
    {
    public:
        InputSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Input::InputSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Input::InputSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}