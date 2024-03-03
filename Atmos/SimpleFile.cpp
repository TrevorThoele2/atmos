
#include "SimpleFile.h"

namespace Atmos
{
    SimpleInFile::Pos SimpleInFile::Tell(StreamT &tell)
    {
        return tell.tellg();
    }

    SimpleInFile::SimpleInFile(const FilePath &path) : SimpleFile(path, std::ios::in | std::ios::binary)
    {}

    void SimpleInFile::Seek(Pos pos)
    {
        stream.seekg(pos);
    }

    void SimpleInFile::FillBuffer(char *buf, StreamSize size)
    {
        stream.read(buf, size);
    }

    SimpleOutFile::Pos SimpleOutFile::Tell(StreamT &tell)
    {
        return tell.tellp();
    }

    SimpleOutFile::SimpleOutFile(const FilePath &path) : SimpleFile(path, std::ios::out | std::ios::binary)
    {}

    void SimpleOutFile::Seek(Pos pos)
    {
        stream.seekp(pos);
    }

    size_t GetFileSize(const FilePath &path)
    {
        std::ifstream stream(path.GetValue(), std::ios::in | std::ios::app | std::ios::ate);
        return static_cast<size_t>(stream.tellg());
    }

    std::pair<void*, size_t> ReadFileIntoBuffer(const FilePath &path)
    {
        std::ifstream file(path, std::ios::in | std::ios::ate | std::ios::binary);
        size_t fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0);

        auto buffer = new char[fileSize];
        file.read(buffer, fileSize);

        return std::pair<void*, size_t>(buffer, fileSize);
    }
}