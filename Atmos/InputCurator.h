#pragma once

#include <Arca/Curator.h>

#include "MappedInputs.h"
#include "MappedActions.h"
#include "InputManager.h"

namespace Atmos::Input
{
    class Curator final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
        void WorkImplementation(Stage& stage) override;
    private:
        Arca::ComputedPtr<Manager*> manager;

        Arca::GlobalPtr<MappedInputs> mappedInputs;
        Arca::GlobalPtr<MappedActions> mappedActions;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Input::Curator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Input::Curator, BinaryArchive>
    {};
}