#pragma once

#include <unordered_map>

#include "Asset.h"
#include "AssetReference.h"

#include "IDSequencer.h"

#include "Serialization.h"
#include <Inscription/Scribe.h>
#include <Inscription/ContainerSize.h>

namespace Atmos
{
    template<class T, class Store, class Key, class Mixin>
    class AssetRegistryBase
    {
    public:
        typedef T AssetT;
        typedef Key KeyT;
        typedef AssetReference<AssetT> ReferenceT;
        typedef typename AssetT::ID ID;

        static const ID nullID = AssetT::nullID;
        static const ID startID = nullID + 1;
    protected:
        typedef Store StoreT;
    private:
        typedef std::unordered_map<Key, StoreT> Map;
    public:
        typedef typename Map::size_type size_type;
        typedef typename Map::const_iterator const_iterator;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        AssetRegistryBase(const AssetRegistryBase &arg) = delete;
        AssetRegistryBase& operator=(const AssetRegistryBase &arg) = delete;
    protected:
        static Map map;

        static IDSequencer<ID> idSequencer;
        typedef std::unordered_map<ID, typename Map::iterator> IDMap;
        static IDMap idMap;

        AssetRegistryBase();

        static StoreT* Emplace(const KeyT &key, StoreT &&emplace);
        static StoreT* Emplace(KeyT &&key, StoreT &&emplace);
        static StoreT* Emplace(ID id, const KeyT &key, StoreT &&emplace);
        static StoreT* Emplace(ID id, KeyT &&key, StoreT &&emplace);

        static ReferenceT FindImpl(const KeyT &key);
        static ReferenceT FindImpl(ID id);

        static void SetID(AssetT &set, ID id);
    public:
        static Mixin& Instance();

        // Returns true if the asset was here
        static bool Remove(const KeyT &key);
        // Returns true if the asset was here
        static bool Remove(ID id);
        static const_iterator Remove(const_iterator itr);
        static void Clear();

        static size_type Size();
        static bool Empty();
        static const_iterator Begin();
        static const_iterator End();
        size_type size() const;
        bool empty() const;
        const_iterator begin() const;
        const_iterator end() const;
    };

    template<class T, class Store, class Key, class Mixin>
    void AssetRegistryBase<T, Store, Key, Mixin>::Serialize(::inscription::Scribe &scribe)
    {
        if (scribe.IsOutput())
        {
            ::inscription::ContainerSize size(map.size());
            scribe.Save(size);

            for (auto &loop : map)
            {
                scribe.Save(::inscription::RemoveConst(loop.first));
                scribe.Save(loop.second.GetID());
                scribe.Save(loop.second);
            }
        }
        else
        {
            map.clear();
            idMap.clear();

            ::inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                FileName fileName;
                scribe.Load(fileName);

                ID id;
                scribe.Load(id);

                AssetT asset;
                scribe.Load(asset);

                asset.id = id;

                auto emplaced = map.emplace(std::move(fileName), std::move(asset)).first;
                idMap.emplace(id, emplaced);
                idSequencer.Reserve(id);
            }
        }
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::Map AssetRegistryBase<T, Store, Key, Mixin>::map;
    template<class T, class Store, class Key, class Mixin>
    IDSequencer<typename AssetRegistryBase<T, Store, Key, Mixin>::ID> AssetRegistryBase<T, Store, Key, Mixin>::idSequencer(startID);
    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::IDMap AssetRegistryBase<T, Store, Key, Mixin>::idMap;

    template<class T, class Store, class Key, class Mixin>
    AssetRegistryBase<T, Store, Key, Mixin>::AssetRegistryBase()
    {}

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::StoreT* AssetRegistryBase<T, Store, Key, Mixin>::Emplace(const KeyT &key, StoreT &&emplace)
    {
        return Emplace(idSequencer.ReserveNext(), key, std::move(emplace));
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::StoreT* AssetRegistryBase<T, Store, Key, Mixin>::Emplace(KeyT &&key, StoreT &&emplace)
    {
        return Emplace(idSequencer.ReserveNext(), std::move(key), std::move(emplace));
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::StoreT* AssetRegistryBase<T, Store, Key, Mixin>::Emplace(ID id, const KeyT &key, StoreT &&emplace)
    {
        if (id == AssetT::nullID)
            id = idSequencer.ReserveNext();
        else
            idSequencer.Reserve(id);

        auto emplaced = map.emplace(key, std::move(emplace)).first;
        auto &unpacked = Mixin::UnpackStore(emplaced->second);
        unpacked.id = id;
        idMap.emplace(unpacked.GetID(), emplaced);
        return &emplaced->second;
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::StoreT* AssetRegistryBase<T, Store, Key, Mixin>::Emplace(ID id, KeyT &&key, StoreT &&emplace)
    {
        if (id == AssetT::nullID)
            id = idSequencer.ReserveNext();
        else
            idSequencer.Reserve(id);

        auto emplaced = map.emplace(std::move(key), std::move(emplace)).first;
        auto &unpacked = Mixin::UnpackStore(emplaced->second);
        unpacked.id = id;
        idMap.emplace(unpacked.GetID(), emplaced);
        return &emplaced->second;
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::ReferenceT AssetRegistryBase<T, Store, Key, Mixin>::FindImpl(const KeyT &key)
    {
        auto found = map.find(key);
        if (found == map.end())
            return ReferenceT();

        return ReferenceT(Mixin::UnpackStore(found->second));
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::ReferenceT AssetRegistryBase<T, Store, Key, Mixin>::FindImpl(ID id)
    {
        auto found = idMap.find(id);
        if (found == idMap.end())
            return ReferenceT();

        return ReferenceT(Mixin::UnpackStore(found->second->second));
    }

    template<class T, class Store, class Key, class Mixin>
    void AssetRegistryBase<T, Store, Key, Mixin>::SetID(AssetT &set, ID id)
    {
        set.id = id;
    }

    template<class T, class Store, class Key, class Mixin>
    Mixin& AssetRegistryBase<T, Store, Key, Mixin>::Instance()
    {
        static Mixin instance;
        return instance;
    }

    template<class T, class Store, class Key, class Mixin>
    bool AssetRegistryBase<T, Store, Key, Mixin>::Remove(const KeyT &key)
    {
        return Remove(map.find(key)) != map.end();
    }

    template<class T, class Store, class Key, class Mixin>
    bool AssetRegistryBase<T, Store, Key, Mixin>::Remove(ID id)
    {
        auto found = idMap.find(id);
        if (found == idMap.end())
            return false;

        idSequencer.IDRemoved(id);
        map.erase(found->second);
        idMap.erase(found);
        return true;
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::const_iterator AssetRegistryBase<T, Store, Key, Mixin>::Remove(const_iterator itr)
    {
        if (itr == map.end())
            return map.end();

        ID id = Mixin::UnpackStore(itr->second).GetID();
        idSequencer.IDRemoved(id);
        auto ret = map.erase(itr);
        idMap.erase(id);

        return ret;
    }

    template<class T, class Store, class Key, class Mixin>
    void AssetRegistryBase<T, Store, Key, Mixin>::Clear()
    {
        map.clear();
        idMap.clear();
        idSequencer.Clear();
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::size_type AssetRegistryBase<T, Store, Key, Mixin>::Size()
    {
        return map.size();
    }

    template<class T, class Store, class Key, class Mixin>
    bool AssetRegistryBase<T, Store, Key, Mixin>::Empty()
    {
        return map.empty();
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::const_iterator AssetRegistryBase<T, Store, Key, Mixin>::Begin()
    {
        return map.begin();
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::const_iterator AssetRegistryBase<T, Store, Key, Mixin>::End()
    {
        return map.end();
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::size_type AssetRegistryBase<T, Store, Key, Mixin>::size() const
    {
        return Size();
    }

    template<class T, class Store, class Key, class Mixin>
    bool AssetRegistryBase<T, Store, Key, Mixin>::empty() const
    {
        return Empty();
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::const_iterator AssetRegistryBase<T, Store, Key, Mixin>::begin() const
    {
        return Begin();
    }

    template<class T, class Store, class Key, class Mixin>
    typename AssetRegistryBase<T, Store, Key, Mixin>::const_iterator AssetRegistryBase<T, Store, Key, Mixin>::end() const
    {
        return End();
    }

    template<class T>
    class AssetRegistry;
}

namespace inscription
{
    template<class T>
    struct TrackPointer<::Atmos::AssetRegistry<T>>
    {
        static constexpr TrackEnum value = TRACK_NEVER;
    };
}