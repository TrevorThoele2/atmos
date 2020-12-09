#include <wx/zipstrm.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "FileCurator.h"

#include <Inscription/InputSimpleFile.h>

namespace Atmos::File
{
    Curator::Curator(Init init) : Arca::Curator(init)
    {}

    Buffer Curator::Handle(const ExtractFile& command)
    {
        Inscription::InputSimpleFile file(command.filePath);
        file.SeekPositionFromEnd(0);
        const auto length = file.CurrentPosition();
        file.SeekPositionFromBegin(0);

        Buffer buffer;
        buffer.resize(length);
        file.FillBuffer(&buffer[0], length);
        return buffer;
    }

    Buffer Curator::Handle(const ExtractZippedFile& command)
    {
        return Buffer{};
    }
}