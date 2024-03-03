
#include "ImageAsset.h"

namespace Atmos
{
    ImageAsset::ImageAsset(ObjectManager& manager, const FileName& fileName, DataPtr&& data) :
        FileAsset(manager, fileName), data(std::move(data)), width(0), height(0)
    {}

    ImageAsset::ImageAsset(const ImageAsset& arg) :
        FileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr), width(arg.width), height(arg.height)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(ImageAsset) :
        INSCRIPTION_TABLE_GET_BASE(FileAsset)
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

    const ObjectTypeName ObjectTraits<ImageAsset>::typeName = "ImageAsset";

    ImageAssetData::~ImageAssetData()
    {}
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::ImageAsset)
    {

    }
}