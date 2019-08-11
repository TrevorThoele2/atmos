#include "CanvasView.h"

namespace Atmos::Render
{
    CanvasView::CanvasView(ObjectManager& manager, const Canvas* canvas) : RenderFragment(manager), source(canvas)
    {}

    CanvasView::CanvasView(const ::Inscription::BinaryTableData<CanvasView>& data) :
        RenderFragment(std::get<0>(data.bases))
    {}

    ObjectTypeDescription CanvasView::TypeDescription() const
    {
        return ObjectTraits<CanvasView>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Render::CanvasView>::typeName = "CanvasView";
}