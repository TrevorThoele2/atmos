#pragma once

#include "SimpleFile.h"
#include <fstream>

namespace Atmos
{
    class OutputSimpleFile final : public SimpleFile<std::ofstream>
    {
    public:
        explicit OutputSimpleFile(const File::Path& path);
        template<class T>
        void operator<<(const T& arg);
        void Seek(Position position) override;
    private:
        Position DoTell(StreamT& stream) override;
    };

    template<class T>
    void OutputSimpleFile::operator<<(const T& arg)
    {
        stream.write(reinterpret_cast<const char*>(&arg), sizeof(arg));
    }
}