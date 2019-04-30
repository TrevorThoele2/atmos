#pragma once

#include "SkipFileObject.h"

namespace Atmos
{
    template<class ScribeT>
    class SkipFileSaver : public SkipFileObject<ScribeT>
    {
    public:
        typedef typename SkipFileObject<ScribeT>::FocusedScribe FocusedScribe;
    public:
        SkipFileSaver(FocusedScribe& scribe);

        void SavePlaceholder();
        void SaveNothing();
        template<class T>
        void SaveObject(T& object);
        void SaveBuffer(const ::Inscription::Buffer& buffer);
    protected:
        typedef typename SkipFileObject<ScribeT>::Position Position;
    private:
        void OverwritePlaceholder(Position overwrite);
    private:
        virtual void OnBeforeObjectSave();
        virtual void OnAfterObjectSave();

        virtual void SavePlaceholderExtra();
        virtual void SaveExtra();
    };

    template<class ScribeT>
    SkipFileSaver<ScribeT>::SkipFileSaver(FocusedScribe& scribe) : SkipFileObject(scribe)
    {}

    template<class ScribeT>
    void SkipFileSaver<ScribeT>::SavePlaceholder()
    {
        position = scribe.TellStream();
        scribe.Save(Position());
        SavePlaceholderExtra();
    }

    template<class ScribeT>
    void SkipFileSaver<ScribeT>::SaveNothing()
    {
        Position currentPosition = scribe.TellStream();

        OnBeforeObjectSave();
        OnAfterObjectSave();

        OverwritePlaceholder(currentPosition);
    }

    template<class ScribeT>
    template<class T>
    void SkipFileSaver<ScribeT>::SaveObject(T& object)
    {
        Position currentPosition = scribe.TellStream();

        OnBeforeObjectSave();
        scribe.Save(object);
        OnAfterObjectSave();

        OverwritePlaceholder(currentPosition);
    }

    template<class ScribeT>
    void SkipFileSaver<ScribeT>::SaveBuffer(const ::Inscription::Buffer& buffer)
    {
        Position currentPosition = scribe.TellStream();

        OnBeforeObjectSave();
        scribe.WriteBuffer(buffer);
        OnAfterObjectSave();

        OverwritePlaceholder(currentPosition);
    }

    template<class ScribeT>
    void SkipFileSaver<ScribeT>::OverwritePlaceholder(Position overwrite)
    {
        Position currentPosition = scribe.TellStream();
        scribe.SeekStream(position);
        scribe.Save(overwrite);
        SaveExtra();
        scribe.SeekStream(currentPosition);
    }

    template<class ScribeT>
    void SkipFileSaver<ScribeT>::OnBeforeObjectSave()
    {}

    template<class ScribeT>
    void SkipFileSaver<ScribeT>::OnAfterObjectSave()
    {}

    template<class ScribeT>
    void SkipFileSaver<ScribeT>::SavePlaceholderExtra()
    {}

    template<class ScribeT>
    void SkipFileSaver<ScribeT>::SaveExtra()
    {}
}