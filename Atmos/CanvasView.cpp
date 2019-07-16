#include "CanvasView.h"

namespace Atmos::Render
{
    CanvasView::CanvasView(ObjectManager& manager, const Canvas* canvas) : Fragment(manager), source(canvas)
    {}

    CanvasView::CanvasView(const ::Inscription::BinaryTableData<CanvasView>& data) :
        Fragment(std::get<0>(data.bases))
    {}

    ObjectTypeDescription CanvasView::TypeDescription() const
    {
        return ObjectTraits<CanvasView>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<CanvasView>::typeName = "CanvasView";
}