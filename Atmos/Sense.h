#pragma once

#include "PositionalObject.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nSense : public PositionalObject
    {
    public:
        StoredProperty<bool> enabled;
    public:
        nSense();
        nSense(const nSense& arg) = default;
        nSense(const ::Inscription::Table<nSense>& table);
        virtual ~nSense() = 0;
    public:
        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<nSense> : ObjectTraitsBase<nSense>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<PositionalObject> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nSense)
    {
    public:
        static void AddMembers(TableT& table);
    };
}