#pragma once

#include <fstream>
#include "FilePath.h"
#include "Buffer.h"

namespace Atmos
{
    template<class Stream>
    class SimpleFile
    {
    public:
        using StreamT = Stream;
        using Position = typename StreamT::pos_type;
        using OpenMode = std::ios_base::openmode;
    public:
        SimpleFile(const File::Path& path, OpenMode mode);
        virtual ~SimpleFile() = 0;
        // Returns the position before the seek
        virtual void Seek(Position position) = 0;
        Position Tell();
        std::streamsize Size();
    protected:
        Stream stream;

        virtual Position DoTell() = 0;
    private:
        File::Path path;
    };

    template<class Stream>
    SimpleFile<Stream>::SimpleFile(const File::Path& path, OpenMode mode) : path(path)
    {
        stream.open(path.c_str(), mode);
    }

    template<class Stream>
    SimpleFile<Stream>::~SimpleFile() = default;

    template<class Stream>
    auto SimpleFile<Stream>::Tell() -> Position
    {
        return Tell(stream);
    }

    template<class Stream>
    std::streamsize SimpleFile<Stream>::Size()
    {
        Stream checkStream(path.c_str(), std::ios::binary | std::ios::ate);
        return Tell(checkStream);
    }

    class SimpleInFile final : public SimpleFile<std::ifstream>
    {
    public:
        explicit SimpleInFile(const File::Path& path);
        template<class T>
        void operator>>(T& arg);
        void Seek(Position position) override;
        [[nodiscard]] Buffer ReadBuffer(std::streamsize size = std::numeric_limits<std::streamsize>::max());
    private:
        Position DoTell() override;
    };

    template<class T>
    void SimpleInFile::operator>>(T& arg)
    {
        stream.read(reinterpret_cast<char*>(&arg), sizeof(arg));
    }

    class SimpleOutFile final : public SimpleFile<std::ofstream>
    {
    public:
        explicit SimpleOutFile(const File::Path& path);
        template<class T>
        void operator<<(const T& arg);
        void Seek(Position position) override;
    private:
        Position DoTell() override;
    };

    template<class T>
    void SimpleOutFile::operator<<(const T& arg)
    {
        stream.write(reinterpret_cast<const char*>(&arg), sizeof(arg));
    }

    size_t FileSize(const File::Path& path);
}