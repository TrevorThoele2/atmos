#pragma once

#include "ImageAssetManager.h"
#include "FreeImageIncludes.h"
#include "Logger.h"

namespace Atmos::Asset
{
    class FreeImageManager final : public ImageManager
    {
    public:
        explicit FreeImageManager(Logging::Logger& logger);
        ~FreeImageManager();

        Resource::Loaded<Resource::Image> Load(const File::Path& filePath) override;
        Resource::Loaded<Resource::Image> Load(const Buffer& memory) override;
    private:
        static Logging::Logger* logger;
    private:
        static void ErrorHandler(FREE_IMAGE_FORMAT format, const char* message);

        static Resource::Loaded<Resource::Image> ProcessBitmap(FIBITMAP* loadedBitmap, FREE_IMAGE_FORMAT format);

        static FREE_IMAGE_FORMAT FIFFor(const String& filePath);
        static FREE_IMAGE_FORMAT FIFFor(FIMEMORY& memory, int size);
        static std::optional<ImageFileType> TypeFromFIF(FREE_IMAGE_FORMAT format);
    };
}