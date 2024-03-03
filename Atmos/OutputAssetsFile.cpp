#include "OutputAssetsFile.h"

#include "ImageAsset.h"

#include <wx/mstream.h>

namespace Atmos::World::Serialization
{
    OutputAssetsFile::SaveAsset::SaveAsset(const Name& name, Buffer&& memory) :
        name(name), memory(std::move(memory))
    {}

    OutputAssetsFile::OutputAssetsFile(const File::Path& filePath) :
        stream(filePath.c_str())
    {}

    void OutputAssetsFile::Save(const Saves& saves)
    {
        wxZipOutputStream zip(stream);

        Save("audio", saves.audio, zip);
        Save("images", saves.images, zip);
        Save("shaders", saves.shaders, zip);
        Save("fonts", saves.fonts, zip);
        Save("scripts", saves.scripts, zip);
    }

    void OutputAssetsFile::Save(const String& folder, const SaveAssets& saves, wxZipOutputStream& zip)
    {
        for(auto& save : saves)
        {
            zip.PutNextEntry(folder + "/" + save.name);
            if (save.memory.size() != 0)
                zip.Write(&save.memory[0], save.memory.size());
            else
            {
                unsigned char byte = 0;
                zip.Write(&byte, 1);
            }
        }
    }
}