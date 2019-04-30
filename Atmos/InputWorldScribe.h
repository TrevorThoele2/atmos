#pragma once

#include "InputScribeBase.h"

#include "WorldStart.h"

#include "ReadonlyProperty.h"

#include "FilePath.h"

#include "SkipFileHandle.h"
#include <Inscription/BinaryScribe.h>

namespace Atmos
{
    class InputWorldScribe : public InputScribeBase
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
        InputWorldScribe(
            const FilePath& filePath,
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

        const FilePath& GetFilePath() const;
    protected:
        ::Inscription::Scribe& UnderlyingScribe() override;
        const ::Inscription::Scribe& UnderlyingScribe() const override;
    private:
        typedef ::Inscription::BinaryScribe Underlying;

        class FieldHandle : public SkipFileHandle<Underlying>
        {
        public:
            typedef ::Inscription::Buffer::SizeT Size;
        public:
            FieldID fieldID;
            Size size;
        public:
            FieldHandle(FocusedScribe& scribe);
        private:
            void LoadExtra() override;
        };

        typedef std::map<FieldID, FieldHandle> FieldHandleMap;
    private:
        Underlying underlyingScribe;

        FilePath filePath;
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

        static FilePath GetForcedFilePath(const FilePath& filePath, OpenMode openMode);
    };
}