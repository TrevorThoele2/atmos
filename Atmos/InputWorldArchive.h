#pragma once

#include "InputArchiveBase.h"

#include "WorldStart.h"

#include "ReadonlyProperty.h"

#include "FilePath.h"

#include "SkipFileHandle.h"
#include <Inscription/InputBinaryArchive.h>

namespace Atmos::World::Serialization
{
    class InputWorldArchive : public InputArchiveBase
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
        InputWorldArchive(
            const File::Path& filePath,
            ObjectManager& globalObjectManager,
            OpenMode openMode = OpenMode::FORCE_EXTENSION);

        bool Load(LoadType load = LoadType::LOAD_ALL) override;
        bool WillLoad() override;

        Optional<Field> ExtractField(FieldID id) override;
        std::unique_ptr<Field> ExtractFieldAsHeap(FieldID id) override;
        ::Inscription::Buffer ExtractFieldAsBuffer(FieldID id) override;

        FieldIDList AllFieldIDs() const override;
        bool HasField(FieldID fieldID) const override;
        size_t FieldCount() const override;

        const File::Path& GetFilePath() const;
    protected:
        ::Inscription::BinaryArchive& UnderlyingArchive() override;
        const ::Inscription::BinaryArchive& UnderlyingArchive() const override;
    private:
        typedef ::Inscription::InputBinaryArchive Underlying;

        class FieldHandle : public SkipFileHandle<Underlying>
        {
        public:
            typedef ::Inscription::Buffer::SizeT Size;
        public:
            FieldID fieldID;
            Size size;
        public:
            FieldHandle(FocusedArchive& archive);
        private:
            void LoadExtra() override;
        };

        typedef std::map<FieldID, FieldHandle> FieldHandleMap;
    private:
        Underlying underlyingArchive;

        File::Path filePath;
        FieldHandleMap fieldHandles;
        WorldStart _worldStart;
    private:
        ObjectManager* globalObjectManager;
    private:
        typedef SkipFileHandle<Underlying> RegistrySkipHandle;
        // If the skipper is given, then it skips loading registries
        void LoadGlobal(RegistrySkipHandle* registrySkipper = nullptr);
        void LoadAfterGlobal();

        void FillField(Field& fill, FieldHandleMap::iterator handle);

        static File::Path GetForcedFilePath(const File::Path& filePath, OpenMode openMode);
    };
}