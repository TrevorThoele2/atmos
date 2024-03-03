#include "OutputAssetsArchiveInterface.h"

#include "ImageAsset.h"

#include <wx/mstream.h>

namespace Atmos::World::Serialization
{
    OutputAssetsArchiveInterface::SaveAsset::SaveAsset(const Name& name, DataBuffer&& memory) :
        name(name), memory(std::move(memory))
    {}

    OutputAssetsArchiveInterface::OutputAssetsArchiveInterface(const File::Path& filePath) :
        stream(filePath.c_str())
    {}

    void OutputAssetsArchiveInterface::Save(const Saves& saves)
    {
        wxZipOutputStream zip(stream);

        Save("images", saves.images, zip);
        Save("shaders", saves.shaders, zip);
    }

    void OutputAssetsArchiveInterface::Save(const String& folder, const SaveAssets& saves, wxZipOutputStream& zip)
    {
        for(auto& save : saves)
        {
            zip.PutNextEntry(folder + "/" + save.name);
            zip.Write(&save.memory[0], save.memory.size());
        }
    }
}