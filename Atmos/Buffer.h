#pragma once

#include <utility>

namespace Atmos
{
    // Return size read
    template<class T, class PositionT>
    size_t ReadFromBuffer(const void *buffer, T &obj, PositionT position);
    template<class PositionT>
    size_t ReadBufferFromBuffer(const void *from, void *to, size_t size, PositionT position);
    bool CompareBuffers(const void *first, const void *second, size_t size);

    template<class Size>
    class Buffer
    {
    public:
        typedef void* BytesT;
        typedef Size SizeT;
    private:
        BytesT bytes;
        SizeT size;

        bool Compare(BytesT pass) const;
    public:
        Buffer();
        Buffer(BytesT bytes, SizeT size);
        Buffer(const Buffer &arg);
        Buffer(Buffer &&arg);
        Buffer& operator=(const Buffer &arg);
        Buffer& operator=(Buffer &&arg);
        ~Buffer();
        bool operator==(const Buffer &arg) const;
        bool operator!=(const Buffer &arg) const;

        void Set(BytesT bytes, SizeT size);

        SizeT GetSize() const;
        BytesT GetBytes() const;
        template<class T>
        T GetBytes() const;
        // Removes the bytes out from the buffer
        std::pair<BytesT, SizeT> RecoverBytes();
        template<class T>
        std::pair<T, SizeT> RecoverBytes();

        // Returns the size read
        template<class T>
        SizeT ReadFrom(T &obj, SizeT position = 0) const;
        // Returns the size read
        SizeT ReadBufferFrom(void *to, size_t size, SizeT position = 0) const;
    };

    template<class Size>
    bool Buffer<Size>::Compare(BytesT pass) const
    {
        CompareBuffers(bytes, pass, size);
    }

    template<class Size>
    Buffer<Size>::Buffer() : bytes(nullptr), size(0)
    {}

    template<class Size>
    Buffer<Size>::Buffer(BytesT bytes, SizeT size) : bytes(bytes), size(size)
    {}

    template<class Size>
    Buffer<Size>::Buffer(const Buffer &arg) : bytes(arg.bytes), size(arg.size)
    {}

    template<class Size>
    Buffer<Size>::Buffer(Buffer &&arg) : bytes(arg.bytes), size(arg.size)
    {
        arg.bytes = nullptr;
    }

    template<class Size>
    Buffer<Size>& Buffer<Size>::operator=(const Buffer &arg)
    {
        bytes = arg.bytes;
        size = arg.size;
        return *this;
    }

    template<class Size>
    Buffer<Size>& Buffer<Size>::operator=(Buffer &&arg)
    {
        bytes = arg.bytes;
        arg.bytes = nullptr;
        size = arg.size;
        return *this;
    }

    template<class Size>
    Buffer<Size>::~Buffer()
    {
        if(bytes)
            delete[] reinterpret_cast<char*>(bytes);

        size = 0;
    }

    template<class Size>
    bool Buffer<Size>::operator==(const Buffer &arg) const
    {
        return Compare(arg.bytes) && size == arg.size;
    }

    template<class Size>
    bool Buffer<Size>::operator!=(const Buffer &arg) const
    {
        return !(*this == arg);
    }

    template<class Size>
    void Buffer<Size>::Set(BytesT bytes, SizeT size)
    {
        this->bytes = bytes;
        this->size = size;
    }

    
    template<class Size>
    typename Buffer<Size>::SizeT Buffer<Size>::GetSize() const
    {
        return size;
    }

    template<class Size>
    typename Buffer<Size>::BytesT Buffer<Size>::GetBytes() const
    {
        return bytes;
    }

    template<class Size>
    template<class T>
    T Buffer<Size>::GetBytes() const
    {
        return reinterpret_cast<T>(bytes);
    }

    template<class Size>
    std::pair<typename Buffer<Size>::BytesT, typename Buffer<Size>::SizeT> Buffer<Size>::RecoverBytes()
    {
        typedef std::pair<Buffer<Size>::BytesT, Buffer<Size>::SizeT> Ret;

        auto thisBytes = bytes;
        auto thisSize = size;
        bytes = nullptr;
        size = 0;

        return Ret(thisBytes, thisSize);
    }

    template<class Size>
    template<class T>
    std::pair<T, typename Buffer<Size>::SizeT> Buffer<Size>::RecoverBytes()
    {
        typedef std::pair<T, Buffer<Size>::SizeT> Ret;

        auto thisBytes = reinterpret_cast<T>(bytes);
        auto thisSize = size;
        bytes = nullptr;
        size = 0;

        return Ret(thisBytes, thisSize);
    }

    template<class Size>
    template<class T>
    typename Buffer<Size>::SizeT Buffer<Size>::ReadFrom(T &obj, SizeT position) const
    {
        return Atmos::ReadFromBuffer(bytes, obj, position);
    }

    template<class Size>
    typename Buffer<Size>::SizeT Buffer<Size>::ReadBufferFrom(void *to, size_t size, SizeT position) const
    {
        return Atmos::ReadBufferFromBuffer(bytes, to, size, position);
    }

    template<class T, class PositionT>
    size_t ReadFromBuffer(const void *buffer, T &obj, PositionT position)
    {
        memcpy(&obj, &reinterpret_cast<const char*>(buffer)[position], sizeof(T));
        return sizeof(T);
    }

    template<class PositionT>
    size_t ReadBufferFromBuffer(const void *from, void *to, size_t size, PositionT position)
    {
        memcpy(to, &reinterpret_cast<const char*>(from)[position], size);
        return size;
    }
}