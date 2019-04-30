#pragma once

#include "OutputScribeBase.h"

#include "WorldStart.h"
#include "FieldID.h"

#include "FilePath.h"

#include "SkipFileSaver.h"
#include <Inscription/BinaryScribe.h>

namespace Atmos
{
    class OutputStasisScribe : public OutputScribeBase
    {
    public:
        enum class OpenMode
        {
            NONE,
            FORCE_EXTENSION
        };
    public:
        WorldStart worldStart;
    public:
        static const char* const defaultExtension;
    public:
        OutputStasisScribe(
            const FilePath& filePath,
            const FileName& worldFileName,
            ::Inscription::ContainerSize::ValueT fieldCount,
            OpenMode openMode = OpenMode::FORCE_EXTENSION);

        void Save(Field& save) override;
        void Save(FieldID id, const ::Inscription::Buffer& buffer);

        void OverwriteFieldCount(::Inscription::ContainerSize::ValueT set) override;

        const FileName& GetFileName() const;

        static ::Inscription::Version CurrentVersion();
    protected:
        ::Inscription::Scribe& UnderlyingScribe() override;
        const ::Inscription::Scribe& UnderlyingScribe() const override;
    private:
        typedef ::Inscription::BinaryScribe Underlying;

        class FieldSaver : public SkipFileSaver<Underlying>
        {
        public:
            typedef ::Inscription::Buffer::SizeT Size;
        public:
            FieldID fieldID;
            Size size;
        public:
            FieldSaver(FocusedScribe& scribe);
        private:
            Size positionStart;
        private:
            void OnBeforeObjectSave() override;
            void OnAfterObjectSave() override;
            void SavePlaceholderExtra() override;
            void SaveExtra() override;
        };

        typedef std::vector<FieldSaver> FieldSaverList;
        FieldSaverList fieldSaverList;
        FieldSaverList::iterator curSaver;
    private:
        Underlying underlyingScribe;

        FileName fileName;
        FileName worldFileName;
    private:
        bool hasOutputHeader;
        void OutputHeader();
    };
}