
#include <wx/zipstrm.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/log.h>
#include <unordered_map>

#include "AssetPackage.h"

#include "Environment.h"
#include "StringUtility.h"
#include "Logger.h"

namespace Atmos
{
    String ProcessWorldFilePath(const FilePath& worldFilePath)
    {
        return ReplaceFileExtension(worldFilePath, AssetPackage::FileExtension());
    }

    AssetPackage::~AssetPackage()
    {
        ClearAll();
    }

    void AssetPackage::Clear()
    {
        ClearAll();
    }

    void AssetPackage::Save(const FilePath& filePath)
    {
        wxFileOutputStream stream(ProcessWorldFilePath(filePath));
        wxZipOutputStream zip(stream);

        auto saver = [&](const char *sub, BufferMap& map)
        {
            String nextEntry(sub);
            nextEntry.append(Environment::GetFileSystem()->GetFileSeparator());
            if (map.empty())
            {
                zip.PutNextEntry(nextEntry);
                return;
            }

            for (auto& loop : map)
            {
                zip.PutNextEntry(nextEntry + loop.first.GetValue());
                zip.Write(loop.second.first, loop.second.second);
            }
        };

        saver(audioSub, audio);
        saver(imageSub, images);
        saver(scriptSub, scripts);
        saver(shaderSub, shaders);
    }

    void AssetPackage::SaveWorldFolder(const FileName& name)
    {
        Save(FilePath(String(worldFolder) + '\\' + name.GetValue()));
    }

    void AssetPackage::Load(const FilePath& path)
    {
        ClearAll();

        Atmos::FilePath processedPath(ProcessWorldFilePath(path));
        wxFFileInputStream stream(processedPath.GetValue());
        if (!stream)
        {
            Logger::Log("Could not open the asset file.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Path", processedPath.GetValue()) });
            return;
        }

        wxZipInputStream zip(stream);
        if (zip.GetTotalEntries() == 0)
            return;

        if (!zip.CanRead())
        {
            Logger::Log("The asset file cannot be read.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Path", processedPath.GetValue()) });
            return;
        }
        
        auto entry = zip.GetNextEntry();
        while (entry)
        {
            // Get size
            auto size = entry->GetSize();
            Atmos::FilePath path(entry->GetName());
            Atmos::FileName name(path);

            // Read buffer
            auto buffer = new char[size];
            if (!zip.CanRead())
            {
                Logger::Log("The asset package cannot be read. There probably isn't any data available.",
                    Logger::Type::ERROR_SEVERE,
                    Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });
                continue;
            }

            zip.Read(buffer, size);
            if (zip.LastRead() < size)
            {
                Logger::Log("Read too little data in the asset package.",
                    Logger::Type::ERROR_SEVERE,
                    Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });
                continue;
            }

            // Create the asset
            {
                auto& directoryName = path.GetDirectoryName(0);
                if (directoryName == audioSub)
                    audio.emplace(name, Buffer(buffer, size));
                else if (directoryName == imageSub)
                    images.emplace(name, Buffer(buffer, size));
                else if (directoryName == scriptSub)
                    scripts.emplace(name, Buffer(buffer, size));
                else if (directoryName == shaderSub)
                    shaders.emplace(name, Buffer(buffer, size));
            }

            entry = zip.GetNextEntry();
        }
    }

    void AssetPackage::LoadWorldFolder(const FileName& name)
    {
        Load(FilePath(String(worldFolder) + '\\' + name.GetValue()));
    }

    String AssetPackage::FileExtension()
    {
        return "dat";
    }

    const char* AssetPackage::audioSub = "audio";
    const char* AssetPackage::imageSub = "images";
    const char* AssetPackage::scriptSub = "scripts";
    const char* AssetPackage::shaderSub = "shaders";

    void AssetPackage::ClearMap(BufferMap& map)
    {
        auto loop = map.begin();
        while (loop != map.end())
        {
            delete[] loop->second.first;
            loop = map.erase(loop);
        }
    }

    void AssetPackage::ClearAll()
    {
        ClearMap(audio);
        ClearMap(images);
        ClearMap(scripts);
        ClearMap(shaders);
    }
}