#include <wx/zipstrm.h>

#include "FileCurator.h"

#include <Inscription/InputBinaryFile.h>

namespace Atmos::File
{
    Curator::Curator(Init init) : Arca::Curator(init)
    {}

    Buffer Curator::Handle(const ExtractFile& command)
    {
        Inscription::File::InputBinary file(command.filePath);
        file.SeekFromEnd(0);
        const auto length = static_cast<size_t>(file.Position());
        file.SeekFromBeginning(0);

        Buffer buffer;
        buffer.resize(length);
        file.ReadData(buffer, length);
        return buffer;
    }

    Buffer Curator::Handle(const ExtractZippedFile& command)
    {
        return Buffer{};
    }
}