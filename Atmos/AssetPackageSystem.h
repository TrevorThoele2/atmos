#pragma once

#include <vector>
#include <unordered_map>

#include "ObjectSystem.h"

#include "AudioAsset.h"
#include "ImageAsset.h"
#include "ScriptAsset.h"
#include "ShaderAsset.h"

#include "LoggingSystem.h"

#include "FilePath.h"
#include "String.h"
#include "Optional.h"
#include "SimpleFile.h"

namespace Atmos::File
{
    class FileSystem;
}

namespace Atmos::Asset
{
    class AssetPackageSystem : public ObjectSystem
    {
    public:
        typedef std::pair<void*, std::int32_t> Buffer;
        typedef std::unordered_map<File::Name, Buffer> BufferMap;
    public:
        AssetPackageSystem(ObjectManager& manager);

        template<class AssetT, typename std::enable_if<std::is_base_of_v<FileAsset, AssetT>, int>::type = 0>
        void Add(const File::Path& path);
        template<class AssetT, typename std::enable_if<std::is_base_of_v<FileAsset, AssetT>, int>::type = 0>
        void Remove(const File::Name& name);
        template<class AssetT, typename std::enable_if<std::is_base_of_v<FileAsset, AssetT>, int>::type = 0>
        Optional<Buffer> RetrieveBuffer(const File::Name& name);
        template<class AssetT, typename std::enable_if<std::is_base_of_v<FileAsset, AssetT>, int>::type = 0>
        BufferMap RetrieveBufferMap();

        void Clear();

        // The path given should be for the world file
        void Save(const File::Path& path);
        // The name given should be for the world file
        void SaveWorldFolder(const File::Name& name);

        // The path given should be for the world file
        void Load(const File::Path& path);
        // The name given should be for the world file
        // Searches the world folder
        void LoadWorldFolder(const File::Name& name);

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
        template<class AssetT, typename std::enable_if<std::is_same_v<AudioAsset, AssetT>, int>::type = 0>
        BufferMap& ModifiableBufferMap();
        template<class AssetT, typename std::enable_if<std::is_same_v<ImageAsset, AssetT>, int>::type = 0>
        BufferMap& ModifiableBufferMap();
        template<class AssetT, typename std::enable_if<std::is_same_v<ScriptAsset, AssetT>, int>::type = 0>
        BufferMap& ModifiableBufferMap();
        template<class AssetT, typename std::enable_if<std::is_same_v<ShaderAsset, AssetT>, int>::type = 0>
        BufferMap& ModifiableBufferMap();
        template<class AssetT, typename std::enable_if<std::is_same_v<AudioAsset, AssetT>, int>::type = 0>
        String StringFor();
        template<class AssetT, typename std::enable_if<std::is_same_v<ImageAsset, AssetT>, int>::type = 0>
        String StringFor();
        template<class AssetT, typename std::enable_if<std::is_same_v<ScriptAsset, AssetT>, int>::type = 0>
        String StringFor();
        template<class AssetT, typename std::enable_if<std::is_same_v<ShaderAsset, AssetT>, int>::type = 0>
        String StringFor();

        void ClearMap(BufferMap& map);
        void ClearAll();
    private:
        File::Path WorldFilePath(const File::Name& fileName) const;
    private:
        LoggingSystem* FindLoggingSystem();
        File::FileSystem* FindFileSystem();
    };

    template<class AssetT, typename std::enable_if<std::is_base_of_v<FileAsset, AssetT>, int>::type>
    void AssetPackageSystem::Add(const File::Path& path)
    {
        if (path.IsEmpty())
            return;

        auto& map = ModifiableBufferMap<AssetT>();
        map.emplace(path, ReadFileIntoBuffer(path));
    }

    template<class AssetT, typename std::enable_if<std::is_base_of_v<FileAsset, AssetT>, int>::type>
    void AssetPackageSystem::Remove(const File::Name& name)
    {
        auto& map = ModifiableBufferMap<AssetT>();
        return map.erase(name) != 0;
    }

    template<class AssetT, typename std::enable_if<std::is_base_of_v<FileAsset, AssetT>, int>::type>
    Optional<AssetPackageSystem::Buffer> AssetPackageSystem::RetrieveBuffer(const File::Name& name)
    {
        typedef Optional<Buffer> RetT;

        auto& map = ModifiableBufferMap<AssetT>();

        auto found = map.find(name);
        if (found != map.end())
            return RetT(found->second);

        FindLoggingSystem()->Log("A requested " + StringFor<AssetT>() + " was not found in the asset package. Discarding.",
            LogType::ERROR_SEVERE,
            LogNameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return RetT();
    }

    template<class AssetT, typename std::enable_if<std::is_base_of_v<FileAsset, AssetT>, int>::type>
    AssetPackageSystem::BufferMap AssetPackageSystem::RetrieveBufferMap()
    {
        return ModifiableBufferMap<AssetT>();
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<AudioAsset, AssetT>, int>::type>
    AssetPackageSystem::BufferMap& AssetPackageSystem::ModifiableBufferMap()
    {
        return audio;
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ImageAsset, AssetT>, int>::type>
    AssetPackageSystem::BufferMap& AssetPackageSystem::ModifiableBufferMap()
    {
        return images;
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ScriptAsset, AssetT>, int>::type>
    AssetPackageSystem::BufferMap& AssetPackageSystem::ModifiableBufferMap()
    {
        return scripts;
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ShaderAsset, AssetT>, int>::type>
    AssetPackageSystem::BufferMap& AssetPackageSystem::ModifiableBufferMap()
    {
        return shaders;
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<AudioAsset, AssetT>, int>::type>
    String AssetPackageSystem::StringFor()
    {
        return "audio";
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ImageAsset, AssetT>, int>::type>
    String AssetPackageSystem::StringFor()
    {
        return "image";
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ScriptAsset, AssetT>, int>::type>
    String AssetPackageSystem::StringFor()
    {
        return "script";
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ShaderAsset, AssetT>, int>::type>
    String AssetPackageSystem::StringFor()
    {
        return "shader";
    }
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::AssetPackageSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Asset::AssetPackageSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}