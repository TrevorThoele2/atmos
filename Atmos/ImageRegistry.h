
#pragma once

#include <unordered_map>

#include "AssetRegistry.h"
#include "ImageAsset.h"

#include "Join.h"

#include "Serialization.h"

namespace Atmos
{
    template<>
    class AssetRegistry<ImageAsset> : public AssetRegistryBase<ImageAsset, ImageAsset, FileName, AssetRegistry<ImageAsset>>
    {
    private:
        typedef AssetRegistryBase<ImageAsset, ImageAsset, FileName, AssetRegistry<ImageAsset>> BaseT;
        friend AssetRegistryBase<ImageAsset, ImageAsset, FileName, AssetRegistry<ImageAsset>>;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        void BeforeRemove(Map::iterator itr) override final;
        void BeforeClear() override final;
    private:
        ReferenceT Register(ID id, void *buffer, std::int32_t size, const FileName &name, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows);
        static Join2<ImageAsset::GridDimension> SetupGrid(ImageAsset::GridDimension cols, ImageAsset::GridDimension rows);

        static AssetT& UnpackStore(StoreT &unpack);
        static const AssetT& UnpackStore(const StoreT &unpack);
    public:
        // Returns true if the asset was unique
        static std::pair<bool, ReferenceT> Register(const FilePath &filePath, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows);
        // Returns true if the asset/ID was unique
        static std::pair<bool, ReferenceT> Register(ID id, const FilePath &filePath, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows);

        static ReferenceT Find(const KeyT &key);
        static ReferenceT Find(ID id);
    };

    typedef AssetRegistry<ImageAsset> ImageRegistry;
}