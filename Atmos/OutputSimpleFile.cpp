#include "OutputSimpleFile.h"

namespace Atmos
{
    OutputSimpleFile::OutputSimpleFile(const File::Path& path) : SimpleFile(path, std::ios::out | std::ios::binary)
    {}

    void OutputSimpleFile::Seek(Position position)
    {
        stream.seekp(position);
    }

    auto OutputSimpleFile::DoTell(StreamT& stream) -> Position
    {
        return stream.tellp();
    }
}