
#include "RenderFragment.h"
#include "CurrentField.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    nRenderFragment::nRenderFragment()
    {
        SubscribeToProperties();
    }

    nRenderFragment::nRenderFragment(const ::Inscription::Table<nRenderFragment>& table) : INSCRIPTION_TABLE_GET_BASE(nSense)
    {
        SubscribeToProperties();
    }

    void nRenderFragment::Draw()
    {
        DrawImpl();
    }

    ObjectTypeDescription nRenderFragment::TypeDescription() const
    {
        return ObjectTraits<nRenderFragment>::TypeDescription();
    }

    void nRenderFragment::SubscribeToProperties()
    {
        enabled.onValueChanged.Subscribe(&nRenderFragment::OnEnabledChanged, *this);
    }

    void nRenderFragment::OnEnabledChanged(bool newValue)
    {

    }

    const ObjectTypeName ObjectTraits<nRenderFragment>::typeName = "RenderFragment";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nRenderFragment)
    {

    }
}