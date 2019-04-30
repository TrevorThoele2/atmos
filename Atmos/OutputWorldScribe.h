#pragma once

#include "OutputScribeBase.h"

#include "WorldStart.h"

#include "FilePath.h"

#include "SkipFileSaver.h"
#include <Inscription/BinaryScribe.h>

namespace Atmos
{
    class Field;
    class ObjectManager;

    class OutputWorldScribe : public OutputScribeBase
    {
    public:
        enum class OpenMode
        {
            NONE,
            FORCE_EXTENSION
        };
    public:
        static const char* const worldExtension;
    public:
        WorldStart worldStart;
    public:
        OutputWorldScribe(
            const FilePath& filePath,
            ::Inscription::ContainerSize::ValueT fieldCount,
            ObjectManager& globalObjectManager,
            OpenMode openMode = OpenMode::FORCE_EXTENSION);

        void Save(Field& field) override;

        void OverwriteFieldCount(::Inscription::ContainerSize::ValueT set) override;

        const FilePath& GetFilePath() const;

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
            FieldSaver(FocusedScribe& cribe);
        private:
            typedef ::Inscription::Buffer::SizeT Position;
            Position startPosition;
        private:
            void OnBeforeObjectSave() override;
            void OnAfterObjectSave() override;
            void SavePlaceholderExtra() override;
            void SaveExtra() override;
        };

        typedef std::vector<FieldSaver> FieldSaversVector;
        FieldSaversVector fieldSavers;
        FieldSaversVector::iterator curSaver;
    private:
        Underlying underlyingScribe;
        FilePath filePath;
    private:
        ObjectManager* globalObjectManager;
        bool hasOutputHeader;

        void OutputHeader();
    private:
        static FilePath GetForcedFilePath(const FilePath& filePath, OpenMode openMode);
    };
}