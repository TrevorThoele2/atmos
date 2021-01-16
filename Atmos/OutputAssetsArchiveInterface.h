#pragma once

#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include "FilePath.h"
#include "Name.h"
#include "Buffer.h"

namespace Atmos::World::Serialization
{
    class OutputAssetsArchiveInterface final
    {
    public:
        struct SaveAsset
        {
            Name name;
            Buffer memory;
            SaveAsset() = default;
            SaveAsset(const Name& name, Buffer&& memory);
        };

        using SaveAssets = std::vector<SaveAsset>;

        struct Saves
        {
            SaveAssets audio;
            SaveAssets images;
            SaveAssets shaders;
            SaveAssets scripts;
        };
    public:
        explicit OutputAssetsArchiveInterface(const File::Path& filePath);

        void Save(const Saves& saves);
    private:
        wxFileOutputStream stream;
    private:
        void Save(const String& folder, const SaveAssets& saves, wxZipOutputStream& zip);
    };
}