
#include "ImageAsset.h"

namespace Atmos
{
    ImageAsset::ImageAsset(const FileName& fileName, DataPtr&& data) :
        nFileAsset(fileName), data(std::move(data)), width(0), height(0)
    {}

    ImageAsset::ImageAsset(const ImageAsset& arg) :
        nFileAsset(arg), data((arg.data) ? arg.data->Clone() : nullptr), width(arg.width), height(arg.height)
    {}

    ImageAsset::ImageAsset(const ::Inscription::Table<ImageAsset>& table) :
        INSCRIPTION_TABLE_GET_BASE(nFileAsset)
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
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::ImageAsset)
    {

    }
}