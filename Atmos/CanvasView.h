#pragma once

#include "RenderFragment.h"

#include "ObjectScribe.h"

namespace Atmos::Render
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
        CanvasView(const ::Inscription::BinaryTableData<CanvasView>& data);
        CanvasView& operator=(const CanvasView& arg) = default;

        ObjectTypeDescription TypeDescription() const override;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Render::CanvasView> : ObjectTraitsBase<Render::CanvasView>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Render::RenderFragment> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Render::CanvasView, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Render::CanvasView, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Render::CanvasView, BinaryArchive> :
        public ObjectScribe<::Atmos::Render::CanvasView, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}