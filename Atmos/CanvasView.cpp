
#include "CanvasView.h"

#include "Environment.h"

namespace Atmos
{
    CanvasView::CanvasView() : nRenderFragment()
    {}

    CanvasView::CanvasView(const Canvas& canvas) : nRenderFragment()
    {}

    CanvasView::CanvasView(const ::Inscription::Table<CanvasView>& table) : INSCRIPTION_TABLE_GET_BASE(nRenderFragment)
    {}

    ObjectTypeDescription CanvasView::TypeDescription() const
    {
        return ObjectTraits<CanvasView>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<CanvasView>::typeName = "CanvasView";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::CanvasView)
    {

    }
}