
#include "ImageRegistry.h"

#include "Environment.h"
#include "Logger.h"

#include <AGUI\System.h>

#include "AssetPackage.h"
#include <Inscription\Scribe.h>
#include <Inscription\ContainerSize.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AssetRegistry<ImageAsset>)
    {
        ::Inscription::TrackingChangerStack tracking(scribe, false);

        if (scribe.IsOutput())
        {
            ::Inscription::ContainerSize size(map.size());
            scribe.Save(size);

            for (auto &loop : map)
            {
                scribe.Save(loop.second.GetID());
                scribe.Save(::Inscription::RemoveConst(loop.second.GetFileName()));
                scribe.Save(loop.second.GetColumns());
                scribe.Save(loop.second.GetRows());
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

                FileName fileName;
                scribe.Load(fileName);

                ImageAsset::GridDimension cols;
                scribe.Load(cols);

                ImageAsset::GridDimension rows;
                scribe.Load(rows);

                auto buffer = AssetPackage::RetrieveImage(fileName);
                if (buffer)
                {
                    if (!ImageAsset::CanMake(buffer->first, buffer->second))
                    {
                        Logger::Log("An image file was not loadable. It is probably corrupt or invalid.",
                            Logger::Type::ERROR_SEVERE,
                            Logger::NameValueVector{ NameValuePair("File Name", fileName.GetValue()) });
                        continue;
                    }

                    Register(id, buffer->first, buffer->second, fileName, rows, cols);
                }
                else
                    Logger::Log("An image file was not found in the asset package.",
                        Logger::Type::ERROR_SEVERE,
                        Logger::NameValueVector{ NameValuePair("File Name", fileName.GetValue()) });
            }
        }
    }

    void AssetRegistry<ImageAsset>::BeforeRemove(Map::iterator itr)
    {
        ::Agui::System::DestroyImageResource(itr->first.c_str());
    }

    void AssetRegistry<ImageAsset>::BeforeClear()
    {
        ::Agui::System::ClearImageResources();
    }

    AssetRegistry<ImageAsset>::ReferenceT AssetRegistry<ImageAsset>::Register(ID id, void *buffer, std::int32_t size, const FileName &name, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        Agui::System::CreateImageResource(buffer, size, Agui::FileName(name.GetValue()), cols, rows);
        return Emplace(id, name, std::move(Environment::GetGraphics()->CreateImage(buffer, size, name, rows, cols)));
    }

    Join2<ImageAsset::GridDimension> AssetRegistry<ImageAsset>::SetupGrid(ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        Join2<ImageAsset::GridDimension> ret;
        ret.first = (cols == 0) ? 1 : cols;
        ret.second = (rows == 0) ? 1 : rows;
        return ret;
    }

    AssetRegistry<ImageAsset>::AssetT& AssetRegistry<ImageAsset>::UnpackStore(StoreT &unpack)
    {
        return unpack;
    }

    const AssetRegistry<ImageAsset>::AssetT& AssetRegistry<ImageAsset>::UnpackStore(const StoreT &unpack)
    {
        return unpack;
    }

    std::pair<bool, AssetRegistry<ImageAsset>::ReferenceT> AssetRegistry<ImageAsset>::Register(const FilePath &filePath, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        return Register(AssetT::nullID, filePath, cols, rows);
    }

    std::pair<bool, AssetRegistry<ImageAsset>::ReferenceT> AssetRegistry<ImageAsset>::Register(ID id, const FilePath &filePath, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        typedef std::pair<bool, ReferenceT> Ret;
        auto &map = Instance().map;

        // Check if can make this bitmap
        {
            if (!ImageAsset::CanMake(filePath))
                return Ret(false, ReferenceT());
        }

        // Check if this file name is unique
        {
            auto found = map.find(filePath.GetFileName());
            if (found != map.end())
                return Ret(false, ReferenceT(&found->second));
        }

        // Check if ID is unique
        {
            auto found = idMap.find(id);
            if (found != idMap.end())
                return Ret(false, ReferenceT(&found->second->second));
        }

        auto &grid = SetupGrid(cols, rows);
        Agui::System::CreateImageResource(filePath.c_str(), grid.first, grid.second);
        auto &asset = Environment::GetGraphics()->CreateImage(filePath, grid.first, grid.second);
        return Ret(true, Emplace(id, asset.GetFileName(), std::move(asset)));
    }

    AssetRegistry<ImageAsset>::ReferenceT AssetRegistry<ImageAsset>::Find(const KeyT &key)
    {
        return FindImpl(key);
    }

    AssetRegistry<ImageAsset>::ReferenceT AssetRegistry<ImageAsset>::Find(ID id)
    {
        return FindImpl(id);
    }
}