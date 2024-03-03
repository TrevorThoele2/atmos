
#include "RenderFragment.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    RenderFragment::RenderFragment(ObjectManager& manager) : Sense(manager)
    {
        SubscribeToProperties();
    }

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(RenderFragment) : INSCRIPTION_TABLE_GET_BASE(Sense)
    {
        SubscribeToProperties();
    }

    void RenderFragment::Draw()
    {
        DrawImpl();
    }

    ObjectTypeDescription RenderFragment::TypeDescription() const
    {
        return ObjectTraits<RenderFragment>::TypeDescription();
    }

    void RenderFragment::SubscribeToProperties()
    {
        enabled.onValueChanged.Subscribe(&RenderFragment::OnEnabledChanged, *this);
    }

    void RenderFragment::OnEnabledChanged(bool newValue)
    {

    }

    const ObjectTypeName ObjectTraits<RenderFragment>::typeName = "RenderFragment";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::RenderFragment)
    {

    }
}