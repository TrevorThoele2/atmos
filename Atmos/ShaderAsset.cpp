
#include "ShaderAsset.h"
#include <fstream>

namespace Atmos::Asset
{
    ShaderAsset::ShaderAsset(ObjectManager& manager, const File::Name& fileName, DataPtr&& data) :
        FileAsset(manager, fileName), data(std::move(data))
    {}

    ShaderAsset::ShaderAsset(const ShaderAsset& arg) : FileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr)
    {}

    ShaderAsset::ShaderAsset(const ::Inscription::BinaryTableData<ShaderAsset>& data) :
        FileAsset(std::get<0>(data.bases))
    {}

    ShaderAsset::DataT* ShaderAsset::Data()
    {
        return data.get();
    }

    const ShaderAsset::DataT* ShaderAsset::Data() const
    {
        return data.get();
    }

    ObjectTypeDescription ShaderAsset::TypeDescription() const
    {
        return ObjectTraits<ShaderAsset>::TypeDescription();
    }

    ShaderAssetData::~ShaderAssetData()
    {}
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Asset::ShaderAsset>::typeName = "ShaderAsset";
}