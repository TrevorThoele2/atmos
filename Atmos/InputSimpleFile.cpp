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
        const auto fileSize = Size();
        const auto useSize = size > fileSize ? fileSize : size;

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

    std::streamsize InputSimpleFile::Size()
    {
        const auto currentPosition = DoTell(stream);
        stream.ignore(std::numeric_limits<std::streamsize>::max());
        const auto size = stream.gcount();
        stream.clear();
        Seek(currentPosition);
        return size;
    }
}