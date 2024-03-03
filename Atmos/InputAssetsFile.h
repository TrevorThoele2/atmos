#pragma once

#include <set>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include "FilePath.h"
#include "Name.h"
#include "Buffer.h"
#include "Logger.h"

namespace Atmos::World::Serialization
{
    class InputAssetsFile final
    {
    public:
        struct ExtractedAsset
        {
            Name name;
            Buffer memory;
            ExtractedAsset() = default;
            ExtractedAsset(const Name& name, Buffer&& memory);
        };

        using ExtractedAssets = std::vector<ExtractedAsset>;

        struct Extracted
        {
            ExtractedAssets audio;
            ExtractedAssets images;
            ExtractedAssets shaders;
            ExtractedAssets fonts;
            ExtractedAssets scripts;
        };

        using ToExtract = std::set<Name>;

        struct AllToExtract
        {
            ToExtract audio;
            ToExtract images;
            ToExtract shaders;
            ToExtract fonts;
            ToExtract scripts;
            AllToExtract() = default;
            AllToExtract(
                const ToExtract& audio,
                const ToExtract& images,
                const ToExtract& shaders,
                const ToExtract& fonts,
                const ToExtract& scripts);
        };
    public:
        explicit InputAssetsFile(const File::Path& filePath, Logging::Logger& logger);

        [[nodiscard]] Extracted ExtractAssets(const AllToExtract& toExtract);
    private:
        std::unique_ptr<wxFileInputStream> stream;
        File::Path filePath;
    private:
        [[nodiscard]] static Buffer ExtractData(wxZipInputStream& zip);
    private:
        Logging::Logger* logger;
    };
}