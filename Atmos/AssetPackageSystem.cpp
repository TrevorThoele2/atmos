
#include <wx/zipstrm.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/log.h>

#include "AssetPackageSystem.h"

#include "ObjectManager.h"
#include "FileSystem.h"

namespace Atmos
{
    AssetPackageSystem::AssetPackageSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(AssetPackageSystem) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

    String ProcessWorldFilePath(const FilePath& worldFilePath)
    {
        return ReplaceFileExtension(worldFilePath, AssetPackageSystem::FileExtension());
    }

    void AssetPackageSystem::Clear()
    {
        ClearAll();
    }

    void AssetPackageSystem::Save(const FilePath& filePath)
    {
        wxFileOutputStream stream(ProcessWorldFilePath(filePath));
        wxZipOutputStream zip(stream);

        auto saver = [&](const char *sub, BufferMap& map)
        {
            String nextEntry(sub);
            nextEntry.append(FilePath::fileSeparator);
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

    void AssetPackageSystem::SaveWorldFolder(const FileName& name)
    {
        Save(WorldFilePath(name));
    }

    void AssetPackageSystem::Load(const FilePath& path)
    {
        ClearAll();

        Atmos::FilePath processedPath(ProcessWorldFilePath(path));
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
            Atmos::FilePath path(entry->GetName());
            Atmos::FileName name(path);

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

    void AssetPackageSystem::LoadWorldFolder(const FileName& name)
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

    FilePath AssetPackageSystem::WorldFilePath(const FileName& fileName) const
    {
        auto filePath = FilePath("Worlds");
        filePath.AppendSeparator();
        filePath.Append(fileName);
        return filePath;
    }

    LoggingSystem* AssetPackageSystem::FindLoggingSystem()
    {
        return Manager()->FindSystem<LoggingSystem>();
    }

    FileSystem* AssetPackageSystem::FindFileSystem()
    {
        return Manager()->FindSystem<FileSystem>();
    }
}

namespace Inscription
{
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::AssetPackageSystem)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}