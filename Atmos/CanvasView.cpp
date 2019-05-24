
#include "CanvasView.h"

namespace Atmos
{
    CanvasView::CanvasView(ObjectManager& manager, const Canvas* canvas) : RenderFragment(manager), source(canvas)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(CanvasView) : INSCRIPTION_TABLE_GET_BASE(RenderFragment)
    {}

    ObjectTypeDescription CanvasView::TypeDescription() const
    {
        return ObjectTraits<CanvasView>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<CanvasView>::typeName = "CanvasView";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::CanvasView)
    {

    }
}