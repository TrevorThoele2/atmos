
#pragma once

#include <memory>
#include <map>

#include "ScribeBase.h"

#include "FieldID.h"
#include "WorldStart.h"
#include "FilePath.h"

#include "SkipFileHelper.h"
#include "Serialization.h"
#include <Inscription\ContainerSize.h>
#include <Inscription\BinaryScribe.h>

namespace Atmos
{
    extern const char *worldExtension;

    class Field;
    class WorldScribeOut : public OutScribeBase
    {
    public:
        enum class OpenMode
        {
            NONE,
            FORCE_EXTENSION
        };
    private:
        typedef ::Inscription::BinaryScribe ScribeT;

        class FieldSaver : public SkipSaver<ScribeT>
        {
        public:
            typedef FieldID IDType;
            typedef ::Inscription::Buffer::SizeT SizeT;
        private:
            SizeT positionStart;

            void OnBeforeObjectSave() override;
            void OnAfterObjectSave() override;
            void SavePlaceholderExtra() override;
            void SaveExtra() override;
        public:
            IDType fieldID;
            ::Inscription::Buffer::SizeT size;

            FieldSaver(ScribeT &cribe);
        };
    private:
        ScribeT basicScribe;
        FilePath filePath;
        WorldStart worldStart;

        typedef std::vector<FieldSaver> FieldSaversVector;
        FieldSaversVector fieldSavers;
        FieldSaversVector::iterator curSaver;
        bool hasOutputHeader;

        void OutputHeader();

        static FilePath GetForcedFilePath(const FilePath &filePath, OpenMode openMode);
    protected:
        ::Inscription::Scribe& GetBasicScribe() override;
        const ::Inscription::Scribe& GetBasicScribe() const override;
    public:
        WorldScribeOut(const FilePath &filePath, ::Inscription::ContainerSize::ValueT fieldCount, OpenMode openMode = OpenMode::FORCE_EXTENSION);

        void Save(Field &field) override;

        void OverwriteFieldCount(::Inscription::ContainerSize::ValueT set) override;
        void SetWorldStart(const WorldStart &set);

        const FilePath& GetFilePath() const;
    };

    class WorldScribeIn : public InScribeBase
    {
    public:
        enum class OpenMode
        {
            NONE,
            FORCE_EXTENSION
        };
    private:
        typedef ::Inscription::BinaryScribe ScribeT;

        class FieldHandle : public SkipHandle<ScribeT>
        {
        public:
            typedef FieldID IDType;
            typedef ::Inscription::Buffer::SizeT SizeT;
        private:
            void LoadExtra() override;
        public:
            FieldID fieldID;
            SizeT size;

            FieldHandle(ScribeT &scribe);
        };

        typedef std::map<FieldID, FieldHandle> FieldHandles;
    private:
        ScribeT basicScribe;

        FilePath filePath;
        FieldHandles fieldHandles;
        WorldStart worldStart;

        typedef SkipHandle<ScribeT> RegistrySkipHandle;
        // If the skipper is given, then it skips loading registries
        void LoadGlobal(RegistrySkipHandle *registrySkipper = nullptr);
        void LoadAfterGlobal();

        void FillField(Field &fill, FieldHandles::iterator handle);

        static FilePath GetForcedFilePath(const FilePath &filePath, OpenMode openMode);
    protected:
        ::Inscription::Scribe& GetBasicScribe() override;
        const ::Inscription::Scribe& GetBasicScribe() const override;
    public:
        // Loads from the world folder
        WorldScribeIn(const FileName &fileName, OpenMode openMode = OpenMode::FORCE_EXTENSION);
        WorldScribeIn(const FilePath &filePath, OpenMode openMode = OpenMode::FORCE_EXTENSION);

        bool Load(LoadType load = LoadType::LOAD_ALL) override;
        bool WillLoad() override;

        Optional<Field> Get(FieldID id) override;
        Field* GetAsHeap(FieldID id) override;
        ::Inscription::Buffer GetAsBuffer(FieldID id) override;

        WorldStart& GetWorldStart() override;

        void GetIDs(std::vector<FieldID> &ids) const override;
        bool HasField(FieldID fieldID) const override;
        size_t GetFieldCount() const override;

        const FilePath& GetFilePath() const;
    };
}