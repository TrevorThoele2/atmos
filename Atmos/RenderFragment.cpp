#include "RenderFragment.h"

#include <Inscription\Scribe.h>

namespace Atmos::Render
{
    RenderFragment::RenderFragment(ObjectManager& manager) : AxisAlignedObject(manager)
    {
        SubscribeToProperties();
    }

    RenderFragment::RenderFragment(const ::Inscription::BinaryTableData<RenderFragment>& data) :
        AxisAlignedObject(std::get<0>(data.bases))
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
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Render::RenderFragment>::typeName = "RenderFragment";
}