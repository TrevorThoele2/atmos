#pragma once

#include "OutputArchiveBase.h"

#include "WorldStart.h"

#include "FilePath.h"

#include "SkipFileSaver.h"
#include <Inscription/OutputBinaryArchive.h>

namespace Atmos
{
    class ObjectManager;
}

namespace Atmos::World
{
    class Field;
}

namespace Atmos::World::Serialization
{
    class OutputWorldArchive : public OutputArchiveBase
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
        OutputWorldArchive(
            const File::Path& filePath,
            ::Inscription::ContainerSize fieldCount,
            ObjectManager& globalObjectManager,
            OpenMode openMode = OpenMode::FORCE_EXTENSION);

        void Save(Field& field) override;

        void OverwriteFieldCount(::Inscription::ContainerSize set) override;

        const File::Path& GetFilePath() const;

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
        Underlying underlyingArchive;
        File::Path filePath;
    private:
        ObjectManager* globalObjectManager;
        bool hasOutputHeader;

        void OutputHeader();
    private:
        static File::Path GetForcedFilePath(const File::Path& filePath, OpenMode openMode);
    };
}