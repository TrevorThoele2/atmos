#pragma once

#include "AssetResourceManager.h"
#include "FreeImageIncludes.h"

#include "Logger.h"

#undef LoadImage

namespace Atmos::Asset::Resource
{
    class RealManager final : public Manager
    {
    public:
        RealManager(Logging::Logger& logger);

        LoadedData<Image> LoadImageData(const File::Path& filePath) override;
        LoadedData<Image> LoadImageData(const Buffer& memory) override;

        LoadedData<Audio> LoadAudioData(const File::Path& filePath) override;
        LoadedData<Audio> LoadAudioData(const Buffer& memory) override;
    private:
        class ImageManager
        {
        public:
            explicit ImageManager(Logging::Logger& logger);
            ~ImageManager();

            LoadedData<Image> Load(const File::Path& filePath);
            LoadedData<Image> Load(const Buffer& memory);
        private:
            static Logging::Logger* logger;
        private:
            static void ErrorHandler(FREE_IMAGE_FORMAT format, const char* message);

            static LoadedData<Image> ProcessBitmap(FIBITMAP* loadedBitmap, FREE_IMAGE_FORMAT format);

            static FREE_IMAGE_FORMAT FIFFor(const String& filePath);
            static FREE_IMAGE_FORMAT FIFFor(FIMEMORY& memory, int size);
            static std::optional<ImageFileType> TypeFromFIF(FREE_IMAGE_FORMAT format);
        } freeImageManager;
    private:
        class AudioManager
        {
        public:
            LoadedData<Audio> Load(const File::Path& filePath);
            LoadedData<Audio> Load(const Buffer& memory);
        } audioManager;
    };
}