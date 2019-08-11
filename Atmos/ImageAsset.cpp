#include "ImageAsset.h"

namespace Atmos::Asset
{
    ImageAsset::ImageAsset(ObjectManager& manager, const File::Name& fileName, DataPtr&& data) :
        FileAsset(manager, fileName), data(std::move(data)), width(0), height(0)
    {}

    ImageAsset::ImageAsset(const ImageAsset& arg) :
        FileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr), width(arg.width), height(arg.height)
    {}

    ImageAsset::ImageAsset(const ::Inscription::BinaryTableData<ImageAsset>& data) :
        FileAsset(std::get<0>(data.bases))
    {}

    ImageAsset::DataT* ImageAsset::Data()
    {
        return data.get();
    }

    const ImageAsset::DataT* ImageAsset::Data() const
    {
        return data.get();
    }

    ObjectTypeDescription ImageAsset::TypeDescription() const
    {
        return ObjectTraits<ImageAsset>::TypeDescription();
    }

    ImageAssetData::~ImageAssetData()
    {}
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Asset::ImageAsset>::typeName = "ImageAsset";
}