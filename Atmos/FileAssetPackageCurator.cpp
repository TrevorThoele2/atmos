#include <wx/zipstrm.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "FileAssetPackageCurator.h"

#include <Arca/Reliquary.h>
#include "Log.h"

namespace Atmos::Asset
{
    String ProcessWorldFilePath(const File::Path& worldFilePath)
    {
        return ReplaceFileExtension(worldFilePath, FileAssetPackageCurator::FileExtension());
    }

    void FileAssetPackageCurator::Clear()
    {
        ClearAll();
    }

    void FileAssetPackageCurator::Save(const File::Path& filePath)
    {
        wxFileOutputStream stream(ProcessWorldFilePath(filePath));
        wxZipOutputStream zip(stream);

        const auto saver = [&](const char* sub, BufferMap& map)
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

    void FileAssetPackageCurator::SaveWorldFolder(const File::Name& name)
    {
        Save(WorldFilePath(name));
    }

    void FileAssetPackageCurator::Load(const File::Path& filePath)
    {
        ClearAll();

        const File::Path processedPath(ProcessWorldFilePath(filePath));
        wxFFileInputStream stream(processedPath.GetValue());
        if (!stream)
        {
            Owner().Raise<Log::Log>
            (
                "Could not open the asset file.",
                Log::Severity::SevereError,
                Log::NameValuePairs{ NameValuePair("File Path", processedPath.GetValue()) }
            );
            return;
        }

        wxZipInputStream zip(stream);
        if (zip.GetTotalEntries() == 0)
            return;

        if (!zip.CanRead())
        {
            Owner().Raise<Log::Log>
            (
                "The asset file cannot be read.",
                Log::Severity::SevereError,
                Log::NameValuePairs{ NameValuePair("File Path", processedPath.GetValue()) }
            );
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
                Owner().Raise<Log::Log>
                (
                    "The asset package cannot be read. There probably isn't any data available.",
                    Log::Severity::SevereError,
                    Log::NameValuePairs{ NameValuePair("File Name", name.GetValue()) }
                );
                continue;
            }

            zip.Read(buffer, size);
            if (zip.LastRead() < size)
            {
                Owner().Raise<Log::Log>
                (
                    "Read too little data in the asset package.",
                    Log::Severity::SevereError,
                    Log::NameValuePairs{ NameValuePair("File Name", name.GetValue()) }
                );
                continue;
            }

            // Create the asset
            {
                auto directoryName = path.GetDirectoryName(0);
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

    void FileAssetPackageCurator::LoadWorldFolder(const File::Name& name)
    {
        Load(WorldFilePath(name));
    }

    String FileAssetPackageCurator::FileExtension()
    {
        return "dat";
    }

    const char* FileAssetPackageCurator::audioSub = "audio";
    const char* FileAssetPackageCurator::imageSub = "images";
    const char* FileAssetPackageCurator::scriptSub = "scripts";
    const char* FileAssetPackageCurator::shaderSub = "shaders";

    void FileAssetPackageCurator::ClearMap(BufferMap& map)
    {
        auto loop = map.begin();
        while (loop != map.end())
        {
            delete[] loop->second.first;
            loop = map.erase(loop);
        }
    }

    void FileAssetPackageCurator::ClearAll()
    {
        ClearMap(audio);
        ClearMap(images);
        ClearMap(scripts);
        ClearMap(shaders);
    }

    File::Path FileAssetPackageCurator::WorldFilePath(const File::Name& fileName) const
    {
        auto filePath = File::Path("Worlds");
        filePath.AppendSeparator();
        filePath.Append(fileName);
        return filePath;
    }
}