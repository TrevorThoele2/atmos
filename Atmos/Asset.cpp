
#include "Asset.h"

namespace Atmos
{
    Asset::Asset(ObjectManager& manager, const Name& name) : Object(manager), name(name)
    {}

    Asset::Asset(const Asset& arg) : Object(arg), name(arg.name)
    {}

    Asset::Asset(const ::Inscription::Table<Asset>& table) : INSCRIPTION_TABLE_GET_BASE(Object), INSCRIPTION_TABLE_GET_MEM(name)
    {}

    ObjectTypeDescription Asset::TypeDescription() const
    {
        return ObjectTraits<Asset>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<Asset>::typeName = "Asset";

    FileAsset::~FileAsset()
    {}

    ObjectTypeDescription FileAsset::TypeDescription() const
    {
        return ObjectTraits<FileAsset>::TypeDescription();
    }

    FileAsset::FileAsset(ObjectManager& manager, const FileName& fileName) :
        Asset(manager, fileName), fileName(fileName)
    {}

    FileAsset::FileAsset(const FileAsset& arg) :
        Asset(arg), fileName(arg.fileName)
    {}

    FileAsset::FileAsset(const ::Inscription::Table<FileAsset>& table) :
        INSCRIPTION_TABLE_GET_BASE(Asset), INSCRIPTION_TABLE_GET_MEM(fileName)
    {}

    const ObjectTypeName ObjectTraits<FileAsset>::typeName = "FileAsset";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Asset)
    {
        INSCRIPTION_TABLE_ADD(name);
    }

    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::FileAsset)
    {
        INSCRIPTION_TABLE_ADD(fileName);
    }
}