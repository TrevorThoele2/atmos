#pragma once

#include "AxisAlignedObject.h"

#include "Position3D.h"
#include "StoredProperty.h"

#include "Serialization.h"

namespace Atmos::Render
{
    class RenderFragment : public AxisAlignedObject
    {
    public:
        StoredProperty<bool> enabled;
    public:
        RenderFragment(ObjectManager& manager);
        RenderFragment(const RenderFragment& arg) = default;
        RenderFragment(const ::Inscription::BinaryTableData<RenderFragment>& data);

        void Draw();

        ObjectTypeDescription TypeDescription() const override;
    private:
        virtual void DrawImpl() = 0;
    private:
        void SubscribeToProperties();
    private:
        void OnEnabledChanged(bool newValue);
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Render::RenderFragment> : ObjectTraitsBase<Render::RenderFragment>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<AxisAlignedObject> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Render::RenderFragment, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Render::RenderFragment, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Render::RenderFragment, BinaryArchive> :
        public ObjectScribe<::Atmos::Render::RenderFragment, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}