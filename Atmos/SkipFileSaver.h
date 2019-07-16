#pragma once

#include "SkipFileObject.h"

#include <Inscription/BufferScribe.h>

namespace Atmos
{
    template<class Archive>
    class SkipFileSaver : public SkipFileObject<Archive>
    {
    public:
        using FocusedArchive = typename SkipFileObject<Archive>::FocusedArchive;
    public:
        SkipFileSaver(FocusedArchive& archive);

        void SavePlaceholder();
        void SaveNothing();
        template<class T>
        void SaveObject(T& object);
        void SaveBuffer(const ::Inscription::Buffer& buffer);
    protected:
        typedef typename SkipFileObject<Archive>::Position Position;
    private:
        void OverwritePlaceholder(Position overwrite);
    private:
        virtual void OnBeforeObjectSave();
        virtual void OnAfterObjectSave();

        virtual void SavePlaceholderExtra();
        virtual void SaveExtra();
    };

    template<class Archive>
    SkipFileSaver<Archive>::SkipFileSaver(FocusedArchive& archive) : SkipFileObject(archive)
    {}

    template<class Archive>
    void SkipFileSaver<Archive>::SavePlaceholder()
    {
        position = archive.TellStream();
        Position placeholder;
        archive(placeholder);
        SavePlaceholderExtra();
    }

    template<class Archive>
    void SkipFileSaver<Archive>::SaveNothing()
    {
        Position currentPosition = archive.TellStream();

        OnBeforeObjectSave();
        OnAfterObjectSave();

        OverwritePlaceholder(currentPosition);
    }

    template<class Archive>
    template<class T>
    void SkipFileSaver<Archive>::SaveObject(T& object)
    {
        Position currentPosition = archive.TellStream();

        OnBeforeObjectSave();
        archive(object);
        OnAfterObjectSave();

        OverwritePlaceholder(currentPosition);
    }

    template<class Archive>
    void SkipFileSaver<Archive>::SaveBuffer(const ::Inscription::Buffer& buffer)
    {
        Position currentPosition = archive.TellStream();

        OnBeforeObjectSave();
        archive(buffer);
        OnAfterObjectSave();

        OverwritePlaceholder(currentPosition);
    }

    template<class Archive>
    void SkipFileSaver<Archive>::OverwritePlaceholder(Position overwrite)
    {
        Position currentPosition = archive.TellStream();
        archive.SeekStream(position);
        archive(overwrite);
        SaveExtra();
        archive.SeekStream(currentPosition);
    }

    template<class Archive>
    void SkipFileSaver<Archive>::OnBeforeObjectSave()
    {}

    template<class Archive>
    void SkipFileSaver<Archive>::OnAfterObjectSave()
    {}

    template<class Archive>
    void SkipFileSaver<Archive>::SavePlaceholderExtra()
    {}

    template<class Archive>
    void SkipFileSaver<Archive>::SaveExtra()
    {}
}