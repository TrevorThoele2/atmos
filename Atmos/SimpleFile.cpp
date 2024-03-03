#include "SimpleFile.h"

namespace Atmos
{
    SimpleInFile::SimpleInFile(const File::Path &path) : SimpleFile(path, std::ios::in | std::ios::binary)
    {}

    void SimpleInFile::Seek(Position position)
    {
        stream.seekg(position);
    }

    Buffer SimpleInFile::ReadBuffer(std::streamsize size)
    {
        Buffer buffer;
        buffer.reserve(static_cast<const unsigned int>(size));
        stream.read(&buffer[0], size);
        return buffer;
    }

    auto SimpleInFile::DoTell() -> Position
    {
        return stream.tellg();
    }

    SimpleOutFile::SimpleOutFile(const File::Path& path) : SimpleFile(path, std::ios::out | std::ios::binary)
    {}

    void SimpleOutFile::Seek(Position position)
    {
        stream.seekp(position);
    }

    auto SimpleOutFile::DoTell() -> Position
    {
        return stream.tellp();
    }

    size_t FileSize(const File::Path& path)
    {
        std::ifstream stream(path.GetValue(), std::ios::in | std::ios::app | std::ios::ate);
        return static_cast<size_t>(stream.tellg());
    }
}