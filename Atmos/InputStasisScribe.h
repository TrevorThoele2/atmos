#pragma once

#include "InputScribeBase.h"

#include "WorldStart.h"

#include "ReadonlyProperty.h"

#include "FilePath.h"
#include "FileName.h"

#include "SkipFileHandle.h"
#include <Inscription/InputBinaryScribe.h>

namespace Atmos
{
    class InputStasisScribe : public InputScribeBase
    {
    public:
        enum class OpenMode
        {
            NONE,
            FORCE_EXTENSION
        };
    public:
        static const char* const defaultExtension;
    public:
        InputStasisScribe(
            const FilePath& filePath,
            const FilePath& worldFilePath,
            ObjectManager& globalObjectManager,
            OpenMode openMode = OpenMode::FORCE_EXTENSION);

        // Returns false if the world path does not match the file name in the stasis file
        // This call will setup the registries from the world file set to this stasis
        // Copying/moving trackers after this will give the registry trackers
        bool Load(LoadType type = LoadType::LOAD_ALL) override;
        bool WillLoad() override;

        Optional<Field> ExtractField(FieldID fieldID) override;
        std::unique_ptr<Field> ExtractFieldAsHeap(FieldID id) override;
        ::Inscription::Buffer ExtractFieldAsBuffer(FieldID id) override;

        FieldIDList AllFieldIDs() const override;
        bool HasField(FieldID fieldID) const override;
        size_t FieldCount() const override;

        const FileName& GetFileName() const;
    protected:
        ::Inscription::BinaryScribe& UnderlyingScribe() override;
        const ::Inscription::BinaryScribe& UnderlyingScribe() const override;
    private:
        typedef ::Inscription::InputBinaryScribe Underlying;
        Underlying underlyingScribe;
    private:
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

        typedef std::map<FieldID, FieldHandle> FieldHandles;
        FieldHandles fieldHandles;
    private:
        FileName fileName;
        FilePath worldFilePath;
        WorldStart _worldStart;
    private:
        ObjectManager* globalObjectManager;
    private:
        // Returns if this is still valid
        bool LoadBeforeGlobal();
        void LoadGlobal();
        void LoadAfterGlobal();

        void FillField(Field& fill, FieldHandles::iterator handle);
    };
}