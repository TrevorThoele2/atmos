
#include <wx/zipstrm.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/log.h>

#include "AssetPackageSystem.h"

#include "ObjectManager.h"
#include "FileSystem.h"

namespace Atmos::Asset
{
    AssetPackageSystem::AssetPackageSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    String ProcessWorldFilePath(const File::Path& worldFilePath)
    {
        return ReplaceFileExtension(worldFilePath, AssetPackageSystem::FileExtension());
    }

    void AssetPackageSystem::Clear()
    {
        ClearAll();
    }

    void AssetPackageSystem::Save(const File::Path& filePath)
    {
        wxFileOutputStream stream(ProcessWorldFilePath(filePath));
        wxZipOutputStream zip(stream);

        auto saver = [&](const char *sub, BufferMap& map)
        {
            String nextEntry(sub);
            nextEntry.append(File::Path::fileSeparator);
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

    void AssetPackageSystem::SaveWorldFolder(const File::Name& name)
    {
        Save(WorldFilePath(name));
    }

    void AssetPackageSystem::Load(const File::Path& path)
    {
        ClearAll();

        Atmos::File::Path processedPath(ProcessWorldFilePath(path));
        wxFFileInputStream stream(processedPath.GetValue());
        if (!stream)
        {
            FindLoggingSystem()->Log("Could not open the asset file.",
                LogType::ERROR_SEVERE,
                LogNameValueVector{ NameValuePair("File Path", processedPath.GetValue()) });
            return;
        }

        wxZipInputStream zip(stream);
        if (zip.GetTotalEntries() == 0)
            return;

        if (!zip.CanRead())
        {
            FindLoggingSystem()->Log("The asset file cannot be read.",
                LogType::ERROR_SEVERE,
                LogNameValueVector{ NameValuePair("File Path", processedPath.GetValue()) });
            return;
        }

        auto entry = zip.GetNextEntry();
        while (entry)
        {
            // Get size
            auto size = entry->GetSize();
            Atmos::File::Path path(entry->GetName());
            Atmos::File::Name name(path);

            // Read buffer
            auto buffer = new char[size];
            if (!zip.CanRead())
            {
                FindLoggingSystem()->Log("The asset package cannot be read. There probably isn't any data available.",
                    LogType::ERROR_SEVERE,
                    LogNameValueVector{ NameValuePair("File Name", name.GetValue()) });
                continue;
            }

            zip.Read(buffer, size);
            if (zip.LastRead() < size)
            {
                FindLoggingSystem()->Log("Read too little data in the asset package.",
                    LogType::ERROR_SEVERE,
                    LogNameValueVector{ NameValuePair("File Name", name.GetValue()) });
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

    void AssetPackageSystem::LoadWorldFolder(const File::Name& name)
    {
        Load(WorldFilePath(name));
    }

    String AssetPackageSystem::FileExtension()
    {
        return "dat";
    }

    const char* AssetPackageSystem::audioSub = "audio";
    const char* AssetPackageSystem::imageSub = "images";
    const char* AssetPackageSystem::scriptSub = "scripts";
    const char* AssetPackageSystem::shaderSub = "shaders";

    void AssetPackageSystem::ClearMap(BufferMap& map)
    {
        auto loop = map.begin();
        while (loop != map.end())
        {
            delete[] loop->second.first;
            loop = map.erase(loop);
        }
    }

    void AssetPackageSystem::ClearAll()
    {
        ClearMap(audio);
        ClearMap(images);
        ClearMap(scripts);
        ClearMap(shaders);
    }

    File::Path AssetPackageSystem::WorldFilePath(const File::Name& fileName) const
    {
        auto filePath = File::Path("Worlds");
        filePath.AppendSeparator();
        filePath.Append(fileName);
        return filePath;
    }

    LoggingSystem* AssetPackageSystem::FindLoggingSystem()
    {
        return Manager()->FindSystem<LoggingSystem>();
    }

    File::FileSystem* AssetPackageSystem::FindFileSystem()
    {
        return Manager()->FindSystem<File::FileSystem>();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Asset::AssetPackageSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}