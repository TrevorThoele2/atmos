
#include "ShaderAsset.h"
#include <fstream>

namespace Atmos
{
    ShaderAsset::ShaderAsset(ObjectManager& manager, const FileName& fileName, DataPtr&& data) :
        FileAsset(manager, fileName), data(std::move(data))
    {}

    ShaderAsset::ShaderAsset(const ShaderAsset& arg) : FileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(ShaderAsset) : INSCRIPTION_TABLE_GET_BASE(FileAsset)
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
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::ShaderAsset)
    {

    }
}