
#pragma once

#include <utility>
#include <fstream>
#include "FilePath.h"
#include "Buffer.h"

namespace Atmos
{
    template<class StreamT>
    class SimpleFile
    {
    public:
        typedef StreamT StreamT;
        typedef typename StreamT::pos_type Pos;
        typedef typename std::streamsize StreamSize;
        typedef std::ios_base::openmode Mode;
    private:
        FilePath path;

        virtual Pos Tell(StreamT &tell) = 0;
    protected:
        StreamT stream;
    public:
        SimpleFile(const FilePath &path, Mode mode);
        virtual ~SimpleFile() = 0 {}
        // Returns the position before the seek
        virtual void Seek(Pos pos) = 0;
        Pos Tell();
        StreamSize GetFileSize();
    };

    template<class StreamT>
    SimpleFile<StreamT>::SimpleFile(const FilePath &path, Mode mode) : path(path)
    {
        stream.open(path.c_str(), mode);
    }

    template<class StreamT>
    typename SimpleFile<StreamT>::Pos SimpleFile<StreamT>::Tell()
    {
        return Tell(stream);
    }

    template<class StreamT>
    typename SimpleFile<StreamT>::StreamSize SimpleFile<StreamT>::GetFileSize()
    {
        StreamT checkStream(path.c_str(), std::ios::binary | std::ios::ate);
        return Tell(checkStream);
    }

    class SimpleInFile : public SimpleFile<std::ifstream>
    {
    private:
        Pos Tell(StreamT &tell) override;
    public:
        SimpleInFile(const FilePath &path);
        template<class T>
        void operator>>(T &arg);
        void Seek(Pos pos) override;
        template<class SizeT>
        void FillBuffer(Buffer<SizeT> &fill, StreamSize size);
        void FillBuffer(char *buf, StreamSize size);
    };

    template<class T>
    void SimpleInFile::operator>>(T &arg)
    {
        stream.read(reinterpret_cast<char*>(&arg), sizeof(arg));
    }

    template<class SizeT>
    void SimpleInFile::FillBuffer(Buffer<SizeT> &fill, StreamSize size)
    {
        stream.read(fill.CastBytes<char*>(), size);
    }

    class SimpleOutFile : public SimpleFile<std::ofstream>
    {
    private:
        Pos Tell(StreamT &tell) override;
    public:
        SimpleOutFile(const FilePath &path);
        template<class T>
        void operator<<(const T &arg);
        void Seek(Pos pos) override;
    };

    template<class T>
    void SimpleOutFile::operator<<(const T &arg)
    {
        stream.write(reinterpret_cast<const char*>(&var), sizeof(var));
    }

    size_t GetFileSize(const FilePath &path);
    // The first parameter of the return value MUST be delete[]'d after you are done with it
    std::pair<void*, size_t> ReadFileIntoBuffer(const FilePath &path);
}