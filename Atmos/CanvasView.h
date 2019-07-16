#pragma once

#include "RenderFragment.h"

#include "ObjectScribe.h"

namespace Atmos::Render
{
    class Canvas;

    class CanvasView : public Fragment
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

    template<>
    struct ObjectTraits<CanvasView> : ObjectTraitsBase<CanvasView>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Fragment> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::CanvasView, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::CanvasView, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::CanvasView, BinaryArchive> :
        public ObjectScribe<::Atmos::CanvasView, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}