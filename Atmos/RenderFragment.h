#pragma once

#include "Sense.h"

#include "Position3D.h"
#include "StoredProperty.h"

#include "Serialization.h"

namespace Atmos::Render
{
    class Fragment : public Sense
    {
    public:
        Fragment(ObjectManager& manager);
        Fragment(const Fragment& arg) = default;
        Fragment(const ::Inscription::BinaryTableData<Fragment>& data);

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
    struct ObjectTraits<Render::Fragment> : ObjectTraitsBase<Render::Fragment>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Sense> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Render::Fragment, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Render::Fragment, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Render::Fragment, BinaryArchive> :
        public ObjectScribe<::Atmos::Render::Fragment, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}