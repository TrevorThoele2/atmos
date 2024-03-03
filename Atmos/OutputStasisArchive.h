#pragma once

#include "OutputArchiveBase.h"

#include "WorldStart.h"
#include "FieldID.h"

#include "FilePath.h"
#include "FileName.h"

#include "SkipFileSaver.h"
#include <Inscription/OutputBinaryArchive.h>

namespace Atmos
{
    class OutputStasisArchive : public OutputArchiveBase
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
        OutputStasisArchive(
            const FilePath& filePath,
            const FileName& worldFileName,
            ::Inscription::ContainerSize fieldCount,
            OpenMode openMode = OpenMode::FORCE_EXTENSION);

        void Save(Field& save) override;
        void Save(FieldID id, const ::Inscription::Buffer& buffer);

        void OverwriteFieldCount(::Inscription::ContainerSize set) override;

        const FileName& GetFileName() const;

        static ::Inscription::Version CurrentVersion();
    protected:
        ::Inscription::BinaryArchive& UnderlyingArchive() override;
        const ::Inscription::BinaryArchive& UnderlyingArchive() const override;
    private:
        typedef ::Inscription::OutputBinaryArchive Underlying;

        class FieldSaver : public SkipFileSaver<Underlying>
        {
        public:
            typedef ::Inscription::Buffer::SizeT Size;
        public:
            FieldID fieldID;
            Size size;
        public:
            FieldSaver(FocusedArchive& archive);
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
        Underlying underlyingArchive;

        FileName fileName;
        FileName worldFileName;
    private:
        bool hasOutputHeader;
        void OutputHeader();
    };
}