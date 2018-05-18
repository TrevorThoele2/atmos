#pragma once

#include <string>
#include <vector>

#include "FilePath.h"

namespace Atmos
{
    class AssetPackage
    {
    public:
        typedef std::pair<void*, std::int32_t> Buffer;
        typedef std::unordered_map<FileName, Buffer> BufferMap;
    private:
        static const char *imageSub;
        static const char *shaderSub;
        static const char *audioSub;
        static const char *scriptSub;

        BufferMap images;
        BufferMap shaders;
        BufferMap audio;
        BufferMap scripts;

        AssetPackage() = default;
        ~AssetPackage();
        AssetPackage(const AssetPackage &arg) = delete;
        AssetPackage& operator=(const AssetPackage &arg) = delete;

        static void AddToMap(BufferMap &map, const FilePath &path);
        static bool RemoveFromMap(BufferMap &map, const FileName &name);
        static Buffer* RetrieveAsset(BufferMap &map, const FileName &name);
        static void ClearMap(BufferMap &map);
        static void ClearAll();
    public:
        static AssetPackage& Instance();

        static void AddImage(const FilePath &path);
        static void AddShader(const FilePath &path);
        static void AddAudio(const FilePath &path);
        static void AddScript(const FilePath &path);

        static bool RemoveImage(const FileName &name);
        static bool RemoveShader(const FileName &name);
        static bool RemoveAudio(const FileName &name);
        static bool RemoveScript(const FileName &name);

        static Buffer* RetrieveImage(const FileName &name);
        static Buffer* RetrieveShader(const FileName &name);
        static Buffer* RetrieveAudio(const FileName &name);
        static Buffer* RetrieveScript(const FileName &name);

        static const BufferMap& GetImages();
        static const BufferMap& GetShaders();
        static const BufferMap& GetAudio();
        static const BufferMap& GetScripts();

        static void Clear();

        // The path given should be for the world file
        static void Save(const FilePath &path);
        // The name given should be for the world file
        static void SaveWorldFolder(const FileName &name);

        // The path given should be for the world file
        static void Load(const FilePath &path);
        // The name given should be for the world file
        // Searches the world folder
        static void LoadWorldFolder(const FileName &name);

        static String GetFileExtension();
    };
}