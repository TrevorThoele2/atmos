#pragma once

#include "Object.h"

#include "Name.h"
#include "FilePath.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    class Asset : public Object
    {
    public:
        const Name name;
    public:
        Asset(ObjectManager& manager, const Name& name);
        Asset(const Asset& arg);
        Asset(const ::Inscription::Table<Asset>& table);

        ObjectTypeDescription TypeDescription() const override;
    };

    template<>
    struct ObjectTraits<Asset> : ObjectTraitsBase<Asset>
    {
        static const ObjectTypeName typeName;
    };

    class nFileAssetData;

    class FileAsset : public Asset
    {
    public:
        const FileName fileName;
    public:
        virtual ~FileAsset() = 0;

        ObjectTypeDescription TypeDescription() const override;
    protected:
        FileAsset(ObjectManager& manager, const FileName& fileName);
        FileAsset(const FileAsset& arg);
        FileAsset(const ::Inscription::Table<FileAsset>& table);
    };

    template<>
    struct ObjectTraits<FileAsset> : ObjectTraitsBase<FileAsset>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Asset> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Asset)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };

    DECLARE_OBJECT_INSCRIPTER(::Atmos::FileAsset)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}