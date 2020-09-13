#include "InputSimpleFile.h"

namespace Atmos
{
    InputSimpleFile::InputSimpleFile(const File::Path& path) : SimpleFile(path, std::ios::in | std::ios::binary)
    {}

    void InputSimpleFile::Seek(Position position)
    {
        stream.seekg(position);
    }

    DataBuffer InputSimpleFile::ReadBuffer(std::streamsize size)
    {
        const auto useSize = size > Size() ? Size() : size;

        Buffer buffer;
        buffer.resize(static_cast<size_t>(useSize));
        stream.read(&buffer[0], useSize);

        DataBuffer returnBuffer;
        returnBuffer.reserve(static_cast<size_t>(useSize));
        for (auto& byte : buffer)
            returnBuffer.push_back(static_cast<unsigned char>(byte));

        return returnBuffer;
    }

    auto InputSimpleFile::DoTell(StreamT& stream) -> Position
    {
        return stream.tellg();
    }
}