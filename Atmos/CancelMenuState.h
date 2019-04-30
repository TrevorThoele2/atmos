
#pragma once

#include "StateWithGui.h"

#include "InputKey.h"

namespace Atmos
{
    class CancelMenuStateGui : public StateGui
    {
    public:
        CancelMenuStateGui(ObjectManager& objectManager);
    private:
        void DoInitialize() override;
    };

    class CancelMenuState : public StateWithGui<CancelMenuStateGui>
    {
    public:
        CancelMenuState() = default;
    private:
        void DoInitialize() override;

        void OnKeyPressed(const Input::Key& args);
    };

    template<>
    struct ObjectTraits<CancelMenuState> : ObjectTraitsBase<CancelMenuState>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<State> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::CancelMenuState)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}