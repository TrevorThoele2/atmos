
#include "AudioRegistry.h"

#include "Buffer.h"
#include "AssetPackage.h"
#include "Logger.h"

#include "Environment.h"

#include <Inscription\Scribe.h>
#include <Inscription\ContainerSize.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AssetRegistry<AudioAsset>)
    {
        ::Inscription::TrackingChangerStack tracking(scribe, false);

        if (scribe.IsOutput())
        {
            ::Inscription::ContainerSize size(map.size());
            scribe.Save(size);

            for (auto &loop : map)
            {
                scribe.Save(loop.second.GetID());
                scribe.Save(::Inscription::RemoveConst(loop.first));
            }
        }
        else
        {
            Clear();

            ::Inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                ID id;
                scribe.Load(id);

                FileName fileName;
                scribe.Load(fileName);

                auto buffer = AssetPackage::RetrieveAudio(fileName);
                if (buffer)
                {
                    if (!AudioHandlerBase::CanMake(buffer->first, buffer->second))
                    {
                        Logger::Log("An audio file was not loadable. It is probably corrupt or invalid.",
                            Logger::Type::ERROR_SEVERE,
                            Logger::NameValueVector{ NameValuePair("File Name", fileName.GetValue()) });
                        continue;
                    }

                    Emplace(id, fileName, Environment::GetAudio()->CreateAudio(buffer->first, buffer->second, fileName));
                }
                else
                    Logger::Log("The audio file " + fileName.GetValue() + " was not found in the asset package.",
                        Logger::Type::ERROR_SEVERE,
                        Logger::NameValueVector{ NameValuePair("File Name", fileName.GetValue()) });
            }
        }
    }

    AssetRegistry<AudioAsset>::AssetT& AssetRegistry<AudioAsset>::UnpackStore(StoreT &unpack)
    {
        return unpack;
    }

    const AssetRegistry<AudioAsset>::AssetT& AssetRegistry<AudioAsset>::UnpackStore(const StoreT &unpack)
    {
        return unpack;
    }

    std::pair<bool, AssetRegistry<AudioAsset>::ReferenceT> AssetRegistry<AudioAsset>::Register(const FilePath &path)
    {
        return Register(AssetT::nullID, path);
    }

    std::pair<bool, AssetRegistry<AudioAsset>::ReferenceT> AssetRegistry<AudioAsset>::Register(ID id, const FilePath &path)
    {
        typedef std::pair<bool, ReferenceT> Ret;
        auto &map = Instance().map;

        // Check if this file path exists
        if (path.IsEmpty())
            return Ret(false, ReferenceT());

        // Check if this file name is unique
        {
            auto found = map.find(path.GetFileName());
            if (found != map.end())
                return Ret(false, ReferenceT(found->second));
        }

        // Check if ID is unique
        {
            auto found = idMap.find(id);
            if (found != idMap.end())
                return Ret(false, ReferenceT(&found->second->second));
        }

        auto &asset = Environment::GetAudio()->CreateAudio(path);
        return Ret(true, Emplace(id, asset.GetFileName(), std::move(asset)));
    }

    Optional<Sound> AssetRegistry<AudioAsset>::CreateSound(const AssetReference<AudioAsset> &asset, const Position3D &pos)
    {
        return CreateImpl<Sound>(asset, pos, false, true);
    }

    Optional<Sound> AssetRegistry<AudioAsset>::CreateSound(const FileName &fileName, const Position3D &pos)
    {
        return CreateImpl<Sound>(fileName, pos);
    }

    Optional<Music> AssetRegistry<AudioAsset>::CreateMusic(const AssetReference<AudioAsset> &asset)
    {
        return CreateImpl<Music>(asset);
    }

    Optional<Music> AssetRegistry<AudioAsset>::CreateMusic(const FileName &fileName)
    {
        return CreateImpl<Music>(fileName);
    }

    AssetRegistry<AudioAsset>::ReferenceT AssetRegistry<AudioAsset>::Find(const KeyT &key)
    {
        return FindImpl(key);
    }

    AssetRegistry<AudioAsset>::ReferenceT AssetRegistry<AudioAsset>::Find(ID id)
    {
        return FindImpl(id);
    }
}