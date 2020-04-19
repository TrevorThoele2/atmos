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
        file.Seek(0, std::ios::end);
        const auto length = file.Tell();
        file.Seek(0, std::ios::beg);

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