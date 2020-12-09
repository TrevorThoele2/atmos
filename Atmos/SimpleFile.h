#pragma once

#include "FilePath.h"

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
    protected:
        Stream stream;

        virtual Position DoTell(StreamT& stream) = 0;
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

    size_t FileSize(const File::Path& path);
}