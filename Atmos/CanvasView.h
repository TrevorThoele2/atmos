#pragma once

#include "RenderFragment.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class Canvas;
    class CanvasView : public RenderFragment
    {
    public:
        typedef StoredProperty<const Canvas*> CanvasProperty;
        CanvasProperty source;
    public:
        CanvasView(ObjectManager& manager, const Canvas* source);
        CanvasView(const CanvasView& arg) = default;
        CanvasView(const ::Inscription::Table<CanvasView>& table);
        CanvasView& operator=(const CanvasView& arg) = default;

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<CanvasView> : ObjectTraitsBase<CanvasView>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<RenderFragment> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::CanvasView)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}