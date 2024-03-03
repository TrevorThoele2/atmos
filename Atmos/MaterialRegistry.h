#pragma once

#include "Material.h"
#include "Name.h"
#include "AssetRegistry.h"

#include "Serialization.h"

namespace Atmos
{
    template<>
    class AssetRegistry<Material> : public AssetRegistryBase<Material, Material, Name, AssetRegistry<Material>>
    {
    private:
        typedef AssetRegistryBase<Material, Material, Name, AssetRegistry<Material>> BaseT;
        friend AssetRegistryBase<Material, Material, Name, AssetRegistry<Material>>;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        AssetRegistry() = default;

        static AssetT& UnpackStore(StoreT &unpack);
        static const AssetT& UnpackStore(const StoreT &unpack);
    public:
        // Returns true if the name was unique
        static std::pair<bool, ReferenceT> Register(const Name &name);
        // Returns true if the name/ID was unique
        static std::pair<bool, ReferenceT> Register(ID id, const Name &name);

        static ReferenceT Find(const KeyT &key);
        static ReferenceT Find(ID id);
    };

    typedef AssetRegistry<Material> MaterialRegistry;
}