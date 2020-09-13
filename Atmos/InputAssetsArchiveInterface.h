#pragma once

#include <set>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include "FilePath.h"
#include "Name.h"
#include "Buffer.h"

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
        };

        using ToExtract = std::set<Name>;

        struct AllToExtract
        {
            ToExtract images;
            ToExtract shaders;
            AllToExtract() = default;
            AllToExtract(const ToExtract& images, const ToExtract& shaders);
        };
    public:
        explicit InputAssetsArchiveInterface(const File::Path& filePath);

        [[nodiscard]] Extracted ExtractAssets(const AllToExtract& toExtract);
    private:
        std::unique_ptr<wxFileInputStream> stream;
        File::Path filePath;
    private:
        [[nodiscard]] static DataBuffer ExtractImage(wxZipInputStream& zip);
    };
}