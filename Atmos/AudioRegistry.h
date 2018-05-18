#pragma once

#include "AudioAsset.h"
#include "Sound.h"
#include "Music.h"

#include "FilePath.h"
#include "AssetRegistry.h"
#include "Optional.h"

#include "Serialization.h"

namespace Atmos
{
    class Sound;
    class Music;

    template<>
    class AssetRegistry<AudioAsset> : public AssetRegistryBase<AudioAsset, AudioAsset, FileName, AssetRegistry<AudioAsset>>
    {
    private:
        typedef AssetRegistryBase<AudioAsset, AudioAsset, FileName, AssetRegistry<AudioAsset>> BaseT;
        friend AssetRegistryBase<AudioAsset, AudioAsset, FileName, AssetRegistry<AudioAsset>>;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        AssetRegistry() = default;

        template<class T, class... Args>
        static Optional<T> CreateImpl(const AssetReference<AudioAsset> &asset, Args && ... args);
        template<class T, class... Args>
        static Optional<T> CreateImpl(const FileName &fileName, Args && ... args);

        static AssetT& UnpackStore(StoreT &unpack);
        static const AssetT& UnpackStore(const StoreT &unpack);
    public:
        // Returns true if the asset was unique
        static std::pair<bool, ReferenceT> Register(const FilePath &path);
        // Returns true if the asset/ID was unique
        static std::pair<bool, ReferenceT> Register(ID id, const FilePath &path);

        static Optional<Sound> CreateSound(const AssetReference<AudioAsset> &asset, const Position3D &pos);
        static Optional<Sound> CreateSound(const FileName &fileName, const Position3D &pos);
        static Optional<Music> CreateMusic(const AssetReference<AudioAsset> &asset);
        static Optional<Music> CreateMusic(const FileName &fileName);

        static ReferenceT Find(const KeyT &key);
        static ReferenceT Find(ID id);
    };

    template<class T, class... Args>
    Optional<T> AssetRegistry<AudioAsset>::CreateImpl(const AssetReference<AudioAsset> &asset, Args && ... args)
    {
        if (!asset.IsValid())
            return Optional<T>();

        return Optional<T>(T(*asset, std::forward<Args>(args)...));
    }

    template<class T, class... Args>
    Optional<T> AssetRegistry<AudioAsset>::CreateImpl(const FileName &name, Args && ... args)
    {
        auto found = map.find(name);
        if (found == map.end())
            return Optional<T>();

        return Optional<T>(T(found->second, std::forward<Args>(args)...));
    }

    typedef AssetRegistry<AudioAsset> AudioRegistry;
}