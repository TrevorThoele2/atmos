#pragma once

#include <Arca/Curator.h>

#include <vector>
#include <unordered_map>

#include "AudioAsset.h"
#include "ImageAsset.h"
#include "ScriptAsset.h"
#include "ShaderAsset.h"

#include "Log.h"

#include "FilePath.h"
#include "String.h"
#include "SimpleFile.h"

namespace Atmos::File
{
    class ManagerProvider;
}

namespace Atmos::Asset
{
    class FileAssetPackageCurator final : public Arca::Curator
    {
    public:
        using Buffer = std::pair<void*, std::int32_t>;
        using BufferMap = std::unordered_map<File::Name, Buffer>;
    public:
        using Curator::Curator;

        template<class AssetT, std::enable_if_t<std::is_base_of_v<FileAsset, AssetT>, int> = 0>
        void Add(const File::Path& filePath);
        template<class AssetT, std::enable_if_t<std::is_base_of_v<FileAsset, AssetT>, int> = 0>
        void Remove(const File::Name& fileName);
        template<class AssetT, std::enable_if_t<std::is_base_of_v<FileAsset, AssetT>, int> = 0>
        std::optional<Buffer> RetrieveBuffer(const File::Name& fileName);
        template<class AssetT, std::enable_if_t<std::is_base_of_v<FileAsset, AssetT>, int> = 0>
        BufferMap RetrieveBufferMap();

        void Clear();

        // The path given should be for the world file
        void Save(const File::Path& filePath);
        // The name given should be for the world file
        void SaveWorldFolder(const File::Name& fileName);

        // The path given should be for the world file
        void Load(const File::Path& filePath);
        // The name given should be for the world file
        // Searches the world folder
        void LoadWorldFolder(const File::Name& fileName);

        static String FileExtension();
    private:
        static const char* audioSub;
        static const char* imageSub;
        static const char* scriptSub;
        static const char* shaderSub;
    private:
        BufferMap audio;
        BufferMap images;
        BufferMap scripts;
        BufferMap shaders;
    private:
        template<class AssetT, std::enable_if_t<std::is_same_v<AudioAsset, AssetT>, int> = 0>
        BufferMap& ModifiableBufferMap();
        template<class AssetT, std::enable_if_t<std::is_same_v<ImageAsset, AssetT>, int> = 0>
        BufferMap& ModifiableBufferMap();
        template<class AssetT, std::enable_if_t<std::is_same_v<ScriptAsset, AssetT>, int> = 0>
        BufferMap& ModifiableBufferMap();
        template<class AssetT, std::enable_if_t<std::is_same_v<ShaderAsset, AssetT>, int> = 0>
        BufferMap& ModifiableBufferMap();
        template<class AssetT, std::enable_if_t<std::is_same_v<AudioAsset, AssetT>, int> = 0>
        static String StringFor();
        template<class AssetT, std::enable_if_t<std::is_same_v<ImageAsset, AssetT>, int> = 0>
        static String StringFor();
        template<class AssetT, std::enable_if_t<std::is_same_v<ScriptAsset, AssetT>, int> = 0>
        static String StringFor();
        template<class AssetT, std::enable_if_t<std::is_same_v<ShaderAsset, AssetT>, int> = 0>
        static String StringFor();

        void ClearMap(BufferMap& map);
        void ClearAll();
    private:
        [[nodiscard]] File::Path WorldFilePath(const File::Name& fileName) const;
    };

    template<class AssetT, typename std::enable_if<std::is_base_of_v<FileAsset, AssetT>, int>::type>
    void FileAssetPackageCurator::Add(const File::Path& filePath)
    {
        if (filePath.IsEmpty())
            return;

        auto& map = ModifiableBufferMap<AssetT>();
        map.emplace(filePath, ReadFileIntoBuffer(filePath));
    }

    template<class AssetT, std::enable_if_t<std::is_base_of_v<FileAsset, AssetT>, int>>
    void FileAssetPackageCurator::Remove(const File::Name& fileName)
    {
        auto& map = ModifiableBufferMap<AssetT>();
        return map.erase(fileName) != 0;
    }

    template<class AssetT, std::enable_if_t<std::is_base_of_v<FileAsset, AssetT>, int>>
    std::optional<FileAssetPackageCurator::Buffer> FileAssetPackageCurator::RetrieveBuffer(const File::Name& fileName)
    {
        auto& map = ModifiableBufferMap<AssetT>();

        auto found = map.find(fileName);
        if (found != map.end())
            return Optional<Buffer>(found->second);

        const Log::Log log
        {
            "A requested " + StringFor<AssetT>() + " was not found in the asset package. Discarding.",
            Log::Severity::SevereError,
            Log::NameValuePairs{ NameValuePair("File Name", fileName.GetValue()) }
        };

        Owner().Raise(log);

        return {};
    }

    template<class AssetT, std::enable_if_t<std::is_base_of_v<FileAsset, AssetT>, int>>
    FileAssetPackageCurator::BufferMap FileAssetPackageCurator::RetrieveBufferMap()
    {
        return ModifiableBufferMap<AssetT>();
    }

    template<class AssetT, std::enable_if_t<std::is_same_v<AudioAsset, AssetT>, int>>
    FileAssetPackageCurator::BufferMap& FileAssetPackageCurator::ModifiableBufferMap()
    {
        return audio;
    }

    template<class AssetT, std::enable_if_t<std::is_same_v<ImageAsset, AssetT>, int>>
    FileAssetPackageCurator::BufferMap& FileAssetPackageCurator::ModifiableBufferMap()
    {
        return images;
    }

    template<class AssetT, std::enable_if_t<std::is_same_v<ScriptAsset, AssetT>, int>>
    FileAssetPackageCurator::BufferMap& FileAssetPackageCurator::ModifiableBufferMap()
    {
        return scripts;
    }

    template<class AssetT, std::enable_if_t<std::is_same_v<ShaderAsset, AssetT>, int>>
    FileAssetPackageCurator::BufferMap& FileAssetPackageCurator::ModifiableBufferMap()
    {
        return shaders;
    }

    template<class AssetT, std::enable_if_t<std::is_same_v<AudioAsset, AssetT>, int>>
    String FileAssetPackageCurator::StringFor()
    {
        return "audio";
    }

    template<class AssetT, std::enable_if_t<std::is_same_v<ImageAsset, AssetT>, int>>
    String FileAssetPackageCurator::StringFor()
    {
        return "image";
    }

    template<class AssetT, std::enable_if_t<std::is_same_v<ScriptAsset, AssetT>, int>>
    String FileAssetPackageCurator::StringFor()
    {
        return "script";
    }

    template<class AssetT, std::enable_if_t<std::is_same_v<ShaderAsset, AssetT>, int>>
    String FileAssetPackageCurator::StringFor()
    {
        return "shader";
    }
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::AssetPackageCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Asset::AssetPackageCurator, BinaryArchive>
    {};
}