#pragma once

#include "AssetRegistry.h"
#include "ShaderAsset.h"

#include "Serialization.h"

namespace Atmos
{
    template<>
    class AssetRegistry<ShaderAsset> : public AssetRegistryBase<ShaderAsset, ShaderAsset, FileName, AssetRegistry<ShaderAsset>>
    {
    private:
        typedef AssetRegistryBase<ShaderAsset, ShaderAsset, FileName, AssetRegistry<ShaderAsset>> BaseT;
        friend AssetRegistryBase<ShaderAsset, ShaderAsset, FileName, AssetRegistry<ShaderAsset>>;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ReferenceT Register(ID id, void *buffer, size_t size, const FileName &name);
        static AssetT& UnpackStore(StoreT &unpack);
        static const AssetT& UnpackStore(const StoreT &unpack);
    public:
        static std::pair<bool, ReferenceT> Register(const FilePath &filePath);
        static std::pair<bool, ReferenceT> Register(ID id, const FilePath &filePath);

        static ReferenceT Find(const KeyT &key);
        static ReferenceT Find(ID id);
    };

    typedef AssetRegistry<ShaderAsset> ShaderRegistry;
}