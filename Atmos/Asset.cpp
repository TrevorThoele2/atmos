
#include "Asset.h"

namespace Atmos
{
    nAsset::nAsset(const Name& name) : name(name)
    {}

    nAsset::nAsset(const nAsset& arg)
    {}

    nAsset::nAsset(const ::Inscription::Table<nAsset>& table) : INSCRIPTION_TABLE_GET_MEM(name)
    {}

    ObjectTypeDescription nAsset::TypeDescription() const
    {
        return ObjectTraits<nAsset>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<nAsset>::typeName = "Asset";

    nFileAsset::~nFileAsset()
    {}

    ObjectTypeDescription nFileAsset::TypeDescription() const
    {
        return ObjectTraits<nFileAsset>::TypeDescription();
    }

    nFileAsset::nFileAsset(const FileName& fileName) :
        nAsset(fileName), fileName(fileName)
    {}

    nFileAsset::nFileAsset(const nFileAsset& arg) :
        nAsset(arg), fileName(arg.fileName)
    {}

    nFileAsset::nFileAsset(const ::Inscription::Table<nFileAsset>& table) :
        INSCRIPTION_TABLE_GET_BASE(nAsset), INSCRIPTION_TABLE_GET_MEM(fileName)
    {}

    const ObjectTypeName ObjectTraits<nFileAsset>::typeName = "FileAsset";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nAsset)
    {
        INSCRIPTION_TABLE_ADD(name);
    }

    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nFileAsset)
    {
        INSCRIPTION_TABLE_ADD(fileName);
    }
}