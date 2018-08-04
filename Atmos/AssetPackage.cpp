
#include <wx\zipstrm.h>
#include <wx\txtstrm.h>
#include <wx\wfstream.h>
#include <wx/log.h>
#include <unordered_map>

#include "AssetPackage.h"
#include "Script.h"

#include "Environment.h"
#include "SimpleFile.h"
#include "StringUtility.h"
#include "Logger.h"

namespace Atmos
{
    String ProcessWorldFilePath(const FilePath &worldFilePath)
    {
        return ReplaceFileExtension(worldFilePath, AssetPackage::GetFileExtension());
    }

    const char* AssetPackage::imageSub = "images";
    const char* AssetPackage::shaderSub = "shaders";
    const char* AssetPackage::audioSub = "audio";
    const char* AssetPackage::scriptSub = "scripts";

    AssetPackage::~AssetPackage()
    {
        ClearAll();
    }

    void AssetPackage::AddToMap(BufferMap &map, const FilePath &path)
    {
        if (path.IsEmpty())
            return;

        map.emplace(path, ReadFileIntoBuffer(path));
    }

    bool AssetPackage::RemoveFromMap(BufferMap &map, const FileName &name)
    {
        return map.erase(name) != 0;
    }

    AssetPackage::Buffer* AssetPackage::RetrieveAsset(BufferMap &map, const FileName &name)
    {
        auto found = map.find(name);
        if (found == map.end())
            return nullptr;

        return &found->second;
    }

    void AssetPackage::ClearMap(BufferMap &map)
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
        ClearMap(Instance().images);
        ClearMap(Instance().shaders);
        ClearMap(Instance().audio);
        ClearMap(Instance().scripts);
    }

    AssetPackage& AssetPackage::Instance()
    {
        static AssetPackage instance;
        return instance;
    }

    void AssetPackage::AddImage(const FilePath &path)
    {
        AddToMap(Instance().images, path);
    }

    void AssetPackage::AddShader(const FilePath &path)
    {
        AddToMap(Instance().shaders, path);
    }

    void AssetPackage::AddAudio(const FilePath &path)
    {
        AddToMap(Instance().audio, path);
    }

    void AssetPackage::AddScript(const FilePath &path)
    {
        FilePath usePath(path);
        ScriptModuleBase::SetupModuleExtension(usePath);

        AddToMap(Instance().scripts, usePath);
    }

    bool AssetPackage::RemoveImage(const FileName &name)
    {
        return RemoveFromMap(Instance().images, name);
    }

    bool AssetPackage::RemoveShader(const FileName &name)
    {
        return RemoveFromMap(Instance().shaders, name);
    }
    
    bool AssetPackage::RemoveAudio(const FileName &name)
    {
        return RemoveFromMap(Instance().audio, name);
    }

    bool AssetPackage::RemoveScript(const FileName &name)
    {
        return RemoveFromMap(Instance().scripts, name);
    }

    AssetPackage::Buffer* AssetPackage::RetrieveImage(const FileName &name)
    {
        auto buffer = RetrieveAsset(Instance().images, name);
        if (!buffer)
            Logger::Log("A requested image was not found in the asset package. Discarding.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return buffer;
    }

    AssetPackage::Buffer* AssetPackage::RetrieveShader(const FileName &name)
    {
        auto buffer = RetrieveAsset(Instance().shaders, name);
        if (!buffer)
            Logger::Log("A requested shader was not found in the asset package. Discarding.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return buffer;
    }

    AssetPackage::Buffer* AssetPackage::RetrieveAudio(const FileName &name)
    {
        auto buffer = RetrieveAsset(Instance().audio, name);
        if (!buffer)
            Logger::Log("A requested audio was not found in the asset package. Discarding.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return buffer;
    }

    AssetPackage::Buffer* AssetPackage::RetrieveScript(const FileName &name)
    {
        auto buffer = RetrieveAsset(Instance().scripts, name);
        if (!buffer)
            Logger::Log("A requested script was not found in the asset package. Discarding.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return buffer;
    }

    const AssetPackage::BufferMap& AssetPackage::GetImages()
    {
        return Instance().images;
    }

    const AssetPackage::BufferMap& AssetPackage::GetShaders()
    {
        return Instance().shaders;
    }

    const AssetPackage::BufferMap& AssetPackage::GetAudio()
    {
        return Instance().audio;
    }

    const AssetPackage::BufferMap& AssetPackage::GetScripts()
    {
        return Instance().scripts;
    }

    void AssetPackage::Clear()
    {
        ClearAll();
    }

    void AssetPackage::Save(const FilePath &filePath)
    {
        wxFileOutputStream stream(ProcessWorldFilePath(filePath));
        wxZipOutputStream zip(stream);

        auto saver = [&](const char *sub, BufferMap &map)
        {
            String nextEntry(sub);
            nextEntry.append(Environment::GetFileSystem()->GetFileSeparator());
            if (map.empty())
            {
                zip.PutNextEntry(nextEntry);
                return;
            }

            for (auto &loop : map)
            {
                zip.PutNextEntry(nextEntry + loop.first.GetValue());
                zip.Write(loop.second.first, loop.second.second);
            }
        };

        saver(imageSub, Instance().images);
        saver(shaderSub, Instance().shaders);
        saver(audioSub, Instance().audio);
        saver(scriptSub, Instance().scripts);
    }

    void AssetPackage::SaveWorldFolder(const FileName &name)
    {
        Save(FilePath(String(worldFolder) +  '\\' + name.GetValue()));
    }

    void AssetPackage::Load(const FilePath &path)
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
                auto &directoryName = path.GetDirectoryName(0);
                if (directoryName == imageSub)
                    Instance().images.emplace(name, Buffer(buffer, size));
                else if (directoryName == shaderSub)
                    Instance().shaders.emplace(name, Buffer(buffer, size));
                else if (directoryName == audioSub)
                    Instance().audio.emplace(name, Buffer(buffer, size));
                else if (directoryName == scriptSub)
                    Instance().scripts.emplace(name, Buffer(buffer, size));
            }

            entry = zip.GetNextEntry();
        }
    }

    void AssetPackage::LoadWorldFolder(const FileName &name)
    {
        Load(FilePath(String(worldFolder) + '\\' + name.GetValue()));
    }

    String AssetPackage::GetFileExtension()
    {
        return "dat";
    }
}