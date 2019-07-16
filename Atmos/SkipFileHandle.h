#pragma once

#include "SkipFileObject.h"

namespace Atmos
{
    template<class Archive>
    class SkipFileHandle : public SkipFileObject<Archive>
    {
    public:
        using Position = typename SkipFileObject<Archive>::Position;
        using FocusedArchive = typename SkipFileObject<Archive>::FocusedArchive;
    public:
        SkipFileHandle(FocusedArchive& archive);

        Position LoadPosition();
        template<class T>
        void LoadObject(T& object);
        ::Inscription::Buffer LoadBuffer(::Inscription::Buffer::SizeT size);

        Position SkipPosition() const;
    private:
        virtual void OnBeforeObjectLoad();
        virtual void OnAfterObjectLoad();

        virtual void LoadExtra();
    };

    template<class Archive>
    SkipFileHandle<Archive>::SkipFileHandle(FocusedArchive& archive) : SkipFileObject(archive)
    {}

    template<class Archive>
    typename SkipFileHandle<Archive>::Position SkipFileHandle<Archive>::LoadPosition()
    {
        archive(position);
        LoadExtra();
        return position;
    }

    template<class Archive>
    template<class T>
    void SkipFileHandle<Archive>::LoadObject(T& object)
    {
        auto currentPosition = archive.TellStream();
        archive.SeekStream(position);

        OnBeforeObjectLoad();
        archive(object);
        OnAfterObjectLoad();

        archive.SeekStream(currentPosition);
    }

    template<class Archive>
    ::Inscription::Buffer SkipFileHandle<Archive>::LoadBuffer(::Inscription::Buffer::SizeT size)
    {
        auto currentPosition = archive.TellStream();
        archive.SeekStream(position);

        OnBeforeObjectLoad();
        ::Inscription::Buffer buffer(size);
        archive(buffer);
        OnAfterObjectLoad();

        archive.SeekStream(currentPosition);

        return buffer;
    }

    template<class Archive>
    typename SkipFileHandle<Archive>::Position SkipFileHandle<Archive>::SkipPosition() const
    {
        return position;
    }

    template<class Archive>
    void SkipFileHandle<Archive>::OnBeforeObjectLoad()
    {}

    template<class Archive>
    void SkipFileHandle<Archive>::OnAfterObjectLoad()
    {}

    template<class Archive>
    void SkipFileHandle<Archive>::LoadExtra()
    {}
}