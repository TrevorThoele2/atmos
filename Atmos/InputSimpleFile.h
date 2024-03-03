#pragma once

#include "SimpleFile.h"
#include <fstream>
#include "Buffer.h"

namespace Atmos
{
    class InputSimpleFile final : public SimpleFile<std::ifstream>
    {
    public:
        explicit InputSimpleFile(const File::Path& path);
        template<class T>
        void operator>>(T& arg);
        void Seek(Position position) override;
        [[nodiscard]] DataBuffer ReadBuffer(std::streamsize size = std::numeric_limits<std::streamsize>::max());
        [[nodiscard]] std::streamsize Size();
    private:
        Position DoTell(StreamT& stream) override;
    };

    template<class T>
    void InputSimpleFile::operator>>(T& arg)
    {
        stream.read(reinterpret_cast<char*>(&arg), sizeof(arg));
    }
}