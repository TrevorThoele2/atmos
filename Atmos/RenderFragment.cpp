#include "RenderFragment.h"

#include <Inscription\Scribe.h>

namespace Atmos::Render
{
    Fragment::Fragment(ObjectManager& manager) : Sense(manager)
    {
        SubscribeToProperties();
    }

    Fragment::Fragment(const ::Inscription::BinaryTableData<Fragment>& data) :
        Sense(std::get<0>(data.bases))
    {
        SubscribeToProperties();
    }

    void Fragment::Draw()
    {
        DrawImpl();
    }

    ObjectTypeDescription Fragment::TypeDescription() const
    {
        return ObjectTraits<Fragment>::TypeDescription();
    }

    void Fragment::SubscribeToProperties()
    {
        enabled.onValueChanged.Subscribe(&Fragment::OnEnabledChanged, *this);
    }

    void Fragment::OnEnabledChanged(bool newValue)
    {

    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Render::Fragment>::typeName = "RenderFragment";
}