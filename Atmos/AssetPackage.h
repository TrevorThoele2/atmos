#pragma once

#include <vector>
#include <unordered_map>

#include "AudioAsset.h"
#include "ImageAsset.h"
#include "ScriptAsset.h"
#include "ShaderAsset.h"

#include "Logger.h"
#include "FilePath.h"
#include "String.h"
#include "Optional.h"
#include "SimpleFile.h"

namespace Atmos
{
    class AssetPackage
    {
    public:
        typedef std::pair<void*, std::int32_t> Buffer;
        typedef std::unordered_map<FileName, Buffer> BufferMap;
    public:
        AssetPackage() = default;
        ~AssetPackage();
        AssetPackage(const AssetPackage& arg) = delete;
        AssetPackage& operator=(const AssetPackage& arg) = delete;
    public:
        template<class AssetT, typename std::enable_if<std::is_base_of_v<nFileAsset, AssetT>, int>::type = 0>
        void Add(const FilePath& path);
        template<class AssetT, typename std::enable_if<std::is_base_of_v<nFileAsset, AssetT>, int>::type = 0>
        void Remove(const FileName& name);
        template<class AssetT, typename std::enable_if<std::is_base_of_v<nFileAsset, AssetT>, int>::type = 0>
        Optional<Buffer> RetrieveBuffer(const FileName& name);
        template<class AssetT, typename std::enable_if<std::is_base_of_v<nFileAsset, AssetT>, int>::type = 0>
        BufferMap RetrieveBufferMap();

        void Clear();

        // The path given should be for the world file
        void Save(const FilePath& path);
        // The name given should be for the world file
        void SaveWorldFolder(const FileName& name);

        // The path given should be for the world file
        void Load(const FilePath& path);
        // The name given should be for the world file
        // Searches the world folder
        void LoadWorldFolder(const FileName& name);

        static String FileExtension();
    private:
        static const char *audioSub;
        static const char *imageSub;
        static const char *scriptSub;
        static const char *shaderSub;
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
    };

    template<class AssetT, typename std::enable_if<std::is_base_of_v<nFileAsset, AssetT>, int>::type>
    void AssetPackage::Add(const FilePath& path)
    {
        if (path.IsEmpty())
            return;

        auto& map = ModifiableBufferMap<AssetT>();
        map.emplace(path, ReadFileIntoBuffer(path));
    }

    template<class AssetT, typename std::enable_if<std::is_base_of_v<nFileAsset, AssetT>, int>::type>
    void AssetPackage::Remove(const FileName& name)
    {
        auto& map = ModifiableBufferMap<AssetT>();
        return map.erase(name) != 0;
    }

    template<class AssetT, typename std::enable_if<std::is_base_of_v<nFileAsset, AssetT>, int>::type>
    Optional<AssetPackage::Buffer> AssetPackage::RetrieveBuffer(const FileName& name)
    {
        typedef Optional<Buffer> RetT;

        auto& map = ModifiableBufferMap<AssetT>();

        auto found = map.find(name);
        if (found != map.end())
            return RetT(found->second);

        Logger::Log("A requested " + StringFor<AssetT>() + " was not found in the asset package. Discarding.",
            Logger::Type::ERROR_SEVERE,
            Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return RetT();
    }

    template<class AssetT, typename std::enable_if<std::is_base_of_v<nFileAsset, AssetT>, int>::type>
    AssetPackage::BufferMap AssetPackage::RetrieveBufferMap()
    {
        return ModifiableBufferMap<AssetT>();
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<AudioAsset, AssetT>, int>::type>
    AssetPackage::BufferMap& AssetPackage::ModifiableBufferMap()
    {
        return audio;
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ImageAsset, AssetT>, int>::type>
    AssetPackage::BufferMap& AssetPackage::ModifiableBufferMap()
    {
        return images;
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ScriptAsset, AssetT>, int>::type>
    AssetPackage::BufferMap& AssetPackage::ModifiableBufferMap()
    {
        return scripts;
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ShaderAsset, AssetT>, int>::type>
    AssetPackage::BufferMap& AssetPackage::ModifiableBufferMap()
    {
        return shaders;
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<AudioAsset, AssetT>, int>::type>
    String AssetPackage::StringFor()
    {
        return "audio";
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ImageAsset, AssetT>, int>::type>
    String AssetPackage::StringFor()
    {
        return "image";
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ScriptAsset, AssetT>, int>::type>
    String AssetPackage::StringFor()
    {
        return "script";
    }

    template<class AssetT, typename std::enable_if<std::is_same_v<ShaderAsset, AssetT>, int>::type>
    String AssetPackage::StringFor()
    {
        return "shader";
    }
}