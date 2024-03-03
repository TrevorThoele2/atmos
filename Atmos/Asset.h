#pragma once

#include "Object.h"

#include "Name.h"
#include "FilePath.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class nAsset : public Object
    {
    public:
        const Name name;
    public:
        nAsset(const Name& name);
        nAsset(const nAsset& arg);
        nAsset(const ::Inscription::Table<nAsset>& table);

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<nAsset> : ObjectTraitsBase<nAsset>
    {
        static const ObjectTypeName typeName;
    };

    class nFileAssetData;

    class nFileAsset : public nAsset
    {
    public:
        const FileName fileName;
    public:
        virtual ~nFileAsset() = 0;

        ObjectTypeDescription TypeDescription() const override;
    protected:
        nFileAsset(const FileName& fileName);
        nFileAsset(const nFileAsset& arg);
        nFileAsset(const ::Inscription::Table<nFileAsset>& table);
    };

    template<>
    struct ObjectTraits<nFileAsset> : ObjectTraitsBase<nFileAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<nAsset> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::nAsset)
    {
    public:
        static void AddMembers(TableT& table);
    };

    DECLARE_OBJECT_INSCRIPTER(::Atmos::nFileAsset)
    {
    public:
        static void AddMembers(TableT& table);
    };
}