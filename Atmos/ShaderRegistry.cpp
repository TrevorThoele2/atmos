
#include "ShaderRegistry.h"

#include "Environment.h"
#include "AssetPackage.h"
#include "Error.h"

#include <Inscription\Scribe.h>
#include <Inscription\ContainerSize.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AssetRegistry<ShaderAsset>)
    {
        ::inscription::TrackingChangerStack tracking(scribe, false);

        if (scribe.IsOutput())
        {
            inscription::ContainerSize size(map.size());
            scribe.Save(size);

            for (auto &loop : map)
            {
                scribe.Save(loop.second.GetID());
                scribe.Save(inscription::RemoveConst(loop.second.GetFileName()));
            }
        }
        else
        {
            inscription::ContainerSize size;
            scribe.Load(size);

            map.clear();
            while (size-- > 0)
            {
                ID id = 0;
                scribe.Load(id);

                FileName fileName;
                scribe.Load(fileName);

                auto buffer = AssetPackage::RetrieveShader(fileName);
                if (buffer)
                {
                    //{
                        //ErrorHandler::Log("A shader file was not loadable. It is probably corrupt or invalid.",
                            //ErrorHandler::Severity::ERROR_SEVERE,
                            //ErrorHandler::NameValueVector{ NameValuePair("File Name", fileName.GetValue()) });
                        //continue;
                    //}

                    Register(id, buffer->first, buffer->second, fileName);
                }
                else
                    ErrorHandler::Log("A shader file was not found in the asset package.",
                        ErrorHandler::Severity::ERROR_SEVERE,
                        ErrorHandler::NameValueVector{ NameValuePair("File Name", fileName.GetValue()) });
            }
        }
    }

    AssetRegistry<ShaderAsset>::ReferenceT AssetRegistry<ShaderAsset>::Register(ID id, void *buffer, size_t size, const FileName &name)
    {
        return Emplace(id, name, std::move(Environment::GetGraphics()->CreateShader(buffer, size, name)));
    }

    AssetRegistry<ShaderAsset>::AssetT& AssetRegistry<ShaderAsset>::UnpackStore(StoreT &unpack)
    {
        return unpack;
    }

    const AssetRegistry<ShaderAsset>::AssetT& AssetRegistry<ShaderAsset>::UnpackStore(const StoreT &unpack)
    {
        return unpack;
    }

    std::pair<bool, AssetRegistry<ShaderAsset>::ReferenceT> AssetRegistry<ShaderAsset>::Register(const FilePath &filePath)
    {
        return Register(AssetT::nullID, filePath);
    }

    std::pair<bool, AssetRegistry<ShaderAsset>::ReferenceT> AssetRegistry<ShaderAsset>::Register(ID id, const FilePath &filePath)
    {
        typedef std::pair<bool, ReferenceT> Ret;

        // Check if this file name is unique
        {
            auto found = map.find(filePath);
            if (found != map.end())
                return Ret(false, ReferenceT(&found->second));
        }

        // Check if the ID is unique
        {
            auto found = idMap.find(id);
            if (found != idMap.end())
                return Ret(false, ReferenceT(&found->second->second));
        }

        auto &asset = Environment::GetGraphics()->CreateShader(filePath);
        return Ret(true, Emplace(id, asset.GetFileName(), std::move(asset)));
    }

    AssetRegistry<ShaderAsset>::ReferenceT AssetRegistry<ShaderAsset>::Find(const KeyT &key)
    {
        return FindImpl(key);
    }

    AssetRegistry<ShaderAsset>::ReferenceT AssetRegistry<ShaderAsset>::Find(ID id)
    {
        return FindImpl(id);
    }
}