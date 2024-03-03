
#include "ShaderAsset.h"
#include <fstream>

namespace Atmos
{
    ShaderAsset::ShaderAsset(const FileName& fileName, DataPtr&& data) : nFileAsset(fileName), data(std::move(data))
    {}

    ShaderAsset::ShaderAsset(const ShaderAsset& arg) : nFileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr)
    {}

    ShaderAsset::ShaderAsset(const ::Inscription::Table<ShaderAsset>& table) : INSCRIPTION_TABLE_GET_BASE(nFileAsset)
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

    const ObjectTypeName ObjectTraits<ShaderAsset>::typeName = "ShaderAsset";

    ShaderAssetData::~ShaderAssetData()
    {}
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::ShaderAsset)
    {

    }
}