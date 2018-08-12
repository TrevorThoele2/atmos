
#include "MaterialRegistry.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AssetRegistry<Material>)
    {
        ::Inscription::TrackingChangerStack tracking(scribe, false);

        if (scribe.IsOutput())
        {
            ::Inscription::ContainerSize size(map.size());
            scribe.Save(size);

            for (auto &loop : map)
            {
                // Save ID
                scribe.Save(loop.second.GetID());
                // Save name
                scribe.Save(::Inscription::RemoveConst(loop.first));
                // Save asset
                scribe.Save(loop.second);
            }
        }
        else // INPUT
        {
            Clear();

            ::Inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                ID id;
                scribe.Load(id);

                Name name;
                scribe.Load(name);

                AssetT asset(name);
                scribe.Load(asset);

                SetID(asset, id);

                Emplace(id, std::move(name), std::move(asset));
            }
        }
    }

    AssetRegistry<Material>::AssetT& AssetRegistry<Material>::UnpackStore(StoreT &unpack)
    {
        return unpack;
    }

    const AssetRegistry<Material>::AssetT& AssetRegistry<Material>::UnpackStore(const StoreT &unpack)
    {
        return unpack;
    }

    std::pair<bool, AssetRegistry<Material>::ReferenceT> AssetRegistry<Material>::Register(const Name &name)
    {
        return Register(AssetT::nullID, name);
    }

    std::pair<bool, AssetRegistry<Material>::ReferenceT> AssetRegistry<Material>::Register(ID id, const Name &name)
    {
        typedef std::pair<bool, ReferenceT> Ret;
        auto &map = Instance().map;

        // Check if this file name is unique
        {
            auto found = map.find(name);
            if (found != map.end())
                return Ret(false, ReferenceT(found->second));
        }

        // Check if the ID is unique
        {
            auto found = idMap.find(id);
            if (found != idMap.end())
                return Ret(false, ReferenceT(found->second->second));
        }

        return Ret(true, Emplace(id, name, Material(name)));
    }

    AssetRegistry<Material>::ReferenceT AssetRegistry<Material>::Find(const KeyT &key)
    {
        return FindImpl(key);
    }

    AssetRegistry<Material>::ReferenceT AssetRegistry<Material>::Find(ID id)
    {
        return FindImpl(id);
    }
}