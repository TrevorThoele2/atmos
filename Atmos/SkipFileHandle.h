#pragma once

#include "SkipFileObject.h"

namespace Atmos
{
    template<class ScribeT>
    class SkipFileHandle : public SkipFileObject<ScribeT>
    {
    public:
        typedef typename SkipFileObject<ScribeT>::Position Position;
        typedef typename SkipFileObject<ScribeT>::FocusedScribe FocusedScribe;
    public:
        SkipFileHandle(FocusedScribe& scribe);

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

    template<class ScribeT>
    SkipFileHandle<ScribeT>::SkipFileHandle(FocusedScribe& scribe) : SkipFileObject(scribe)
    {}

    template<class ScribeT>
    typename SkipFileHandle<ScribeT>::Position SkipFileHandle<ScribeT>::LoadPosition()
    {
        scribe.Load(position);
        LoadExtra();
        return position;
    }

    template<class ScribeT>
    template<class T>
    void SkipFileHandle<ScribeT>::LoadObject(T& object)
    {
        auto currentPosition = scribe.TellStream();
        scribe.SeekStream(position);

        OnBeforeObjectLoad();
        scribe.Load(object);
        OnAfterObjectLoad();

        scribe.SeekStream(currentPosition);
    }

    template<class Scribe>
    ::Inscription::Buffer SkipFileHandle<Scribe>::LoadBuffer(::Inscription::Buffer::SizeT size)
    {
        auto currentPosition = scribe.TellStream();
        scribe.SeekStream(position);

        OnBeforeObjectLoad();
        ::Inscription::Buffer buffer(size);
        scribe.ReadBuffer(buffer);
        OnAfterObjectLoad();

        scribe.SeekStream(currentPosition);

        return buffer;
    }

    template<class Scribe>
    typename SkipFileHandle<Scribe>::Position SkipFileHandle<Scribe>::SkipPosition() const
    {
        return position;
    }

    template<class Scribe>
    void SkipFileHandle<Scribe>::OnBeforeObjectLoad()
    {}

    template<class Scribe>
    void SkipFileHandle<Scribe>::OnAfterObjectLoad()
    {}

    template<class Scribe>
    void SkipFileHandle<Scribe>::LoadExtra()
    {}
}