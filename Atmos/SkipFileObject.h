#pragma once

namespace Atmos
{
    template<class Archive>
    class SkipFileObject
    {
    public:
        using FocusedArchive = Archive;
    public:
        FocusedArchive& archive;
    public:
        SkipFileObject(FocusedArchive& scribe);
    protected:
        using Position = typename FocusedArchive::StreamPosition;
    protected:
        Position position;
    };

    template<class Archive>
    SkipFileObject<Archive>::SkipFileObject(FocusedArchive& archive) : archive(archive), position(0)
    {}
}