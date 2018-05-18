#pragma once

#include "ScribeBase.h"

#include "Field.h"
#include "WorldStart.h"
#include "FilePath.h"
#include "SkipFileHelper.h"

#include <Inscription\BinaryScribe.h>

namespace Atmos
{
    extern const char *stasisExtension;

    // Saves the savegame
    class StasisScribeOut : public OutScribeBase
    {
    public:
        typedef inscription::Buffer BufferT;

        enum class OpenMode
        {
            NONE,
            FORCE_EXTENSION
        };
    private:
        typedef inscription::BinaryScribe ScribeT;

        class FieldSaver : public SkipSaver<ScribeT>
        {
        public:
            typedef FieldID IDType;
            typedef inscription::Buffer::SizeT SizeT;
        private:
            SizeT positionStart;

            void OnBeforeObjectSave() override;
            void OnAfterObjectSave() override;
            void SavePlaceholderExtra() override;
            void SaveExtra() override;
        public:
            IDType fieldID;
            inscription::Buffer::SizeT size;

            FieldSaver(ScribeT &scribe);
        };

        typedef std::unordered_map<FieldID, BufferT> Fields;
    private:
        ScribeT basicScribe;

        FileName fileName;
        FileName worldFileName;
        StasisWorldStart worldStart;

        typedef std::vector<FieldSaver> FieldSaversVector;
        FieldSaversVector fieldSavers;
        FieldSaversVector::iterator curSaver;
        bool hasOutputHeader;

        void OutputHeader();
    protected:
        inscription::Scribe& GetBasicScribe() override;
        const inscription::Scribe& GetBasicScribe() const override;
    public:
        StasisScribeOut(const FileName &fileName, const FileName &worldFileName, inscription::ContainerSize::ValueT fieldCount, OpenMode openMode = OpenMode::FORCE_EXTENSION);
        StasisScribeOut(const FilePath &filePath, const FileName &worldFileName, inscription::ContainerSize::ValueT fieldCount, OpenMode openMode = OpenMode::FORCE_EXTENSION);

        void Save(Field &save) override;
        void Save(FieldID id, const inscription::Buffer &buffer);

        void OverwriteFieldCount(inscription::ContainerSize::ValueT set) override;
        void SetWorldStart(const StasisWorldStart &set);
        void SetWorldStartFromCurrent();

        const FileName& GetFileName() const;
        static FilePath MakePathFromName(const FileName &fileName);
        static FilePath GetPath();
    };

    // Loads the savegame
    class StasisScribeIn : public InScribeBase
    {
    public:
        enum class OpenMode
        {
            NONE,
            FORCE_EXTENSION
        };
    private:
        typedef inscription::BinaryScribe ScribeT;

        class FieldHandle : public SkipHandle<ScribeT>
        {
        public:
            typedef FieldID IDType;
            typedef inscription::Buffer::SizeT SizeT;
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

        FileName fileName;
        FieldHandles fieldHandles;
        StasisWorldStart worldStart;
        FilePath worldFilePath;
        
        // Returns if this is still valid
        bool LoadBeforeGlobal();
        void LoadGlobal();
        void LoadAfterGlobal();

        void FillField(Field &fill, FieldHandles::iterator handle);
    protected:
        inscription::Scribe& GetBasicScribe() override;
        const inscription::Scribe& GetBasicScribe() const override;
    public:
        StasisScribeIn(const FileName &fileName, const FilePath &worldPath, OpenMode openMode = OpenMode::FORCE_EXTENSION);
        StasisScribeIn(const FilePath &filePath, const FilePath &worldPath, OpenMode openMode = OpenMode::FORCE_EXTENSION);

        // Returns false if the world path does not match the file name in the stasis file
        // This call will setup the registries from the world file set to this stasis
        // Copying/moving trackers after this will give the registry trackers
        bool Load(LoadType type = LoadType::LOAD_ALL) override;
        bool WillLoad() override;

        Optional<Field> Get(FieldID fieldID) override;
        Field* GetAsHeap(FieldID id) override;
        inscription::Buffer GetAsBuffer(FieldID id) override;

        StasisWorldStart& GetWorldStart() override;

        void GetIDs(std::vector<FieldID> &ids) const override;
        bool HasField(FieldID fieldID) const override;
        size_t GetFieldCount() const override;

        const FileName& GetFileName() const;
        static FilePath MakePathFromName(const FileName &fileName);
        static FilePath GetPath();
    };
}