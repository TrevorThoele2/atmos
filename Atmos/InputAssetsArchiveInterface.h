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
    class InputAssetsArchiveInterface final
    {
    public:
        struct ExtractedAsset
        {
            Name name;
            DataBuffer memory;
            ExtractedAsset() = default;
            ExtractedAsset(const Name& name, DataBuffer&& memory);
        };

        using ExtractedAssets = std::vector<ExtractedAsset>;

        struct Extracted
        {
            ExtractedAssets images;
            ExtractedAssets shaders;
            ExtractedAssets scripts;
        };

        using ToExtract = std::set<Name>;

        struct AllToExtract
        {
            ToExtract images;
            ToExtract shaders;
            ToExtract scripts;
            AllToExtract() = default;
            AllToExtract(const ToExtract& images, const ToExtract& shaders, const ToExtract& scripts);
        };
    public:
        explicit InputAssetsArchiveInterface(const File::Path& filePath, Logging::Logger& logger);

        [[nodiscard]] Extracted ExtractAssets(const AllToExtract& toExtract);
    private:
        std::unique_ptr<wxFileInputStream> stream;
        File::Path filePath;
    private:
        [[nodiscard]] static DataBuffer ExtractData(wxZipInputStream& zip);
    private:
        Logging::Logger* logger;
    };
}