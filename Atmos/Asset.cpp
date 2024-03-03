#include "Asset.h"

#include <Inscription/FileNameScribe.h>

namespace Atmos::Asset
{
    Asset::Asset(ObjectManager& manager, const Name& name) : Object(manager), name(name)
    {}

    Asset::Asset(const Asset& arg) : Object(arg), name(arg.name)
    {}

    Asset::Asset(const ::Inscription::BinaryTableData<Asset>& data) :
        Object(std::get<0>(data.bases)), name(data.name)
    {}

    ObjectTypeDescription Asset::TypeDescription() const
    {
        return ObjectTraits<Asset>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Asset::Asset>::typeName = "Asset";
}

namespace Inscription
{
    Scribe<::Atmos::Asset::Asset, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::name, &DataT::name) });
    }
}