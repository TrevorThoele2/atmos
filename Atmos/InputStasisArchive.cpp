#include "InputStasisArchive.h"

#include "FileSystem.h"

#include "InputWorldArchive.h"

#include <Inscription/FileNameScribe.h>

namespace Atmos
{
    const char* const InputStasisArchive::defaultExtension = "stasis";

    InputStasisArchive::InputStasisArchive(
        const FilePath& filePath,
        const FilePath& worldFilePath,
        ObjectManager& globalObjectManager,
        OpenMode openMode) :

        InputArchiveBase([this]() { return _worldStart; }),
        fileName(filePath), worldFilePath(worldFilePath), underlyingArchive(filePath, "ATMOS GAIA STASIS"),
        globalObjectManager(&globalObjectManager)
    {
        if (openMode == OpenMode::FORCE_EXTENSION)
            this->fileName.SetExtension(defaultExtension);
    }

    bool InputStasisArchive::Load(LoadType load)
    {
        underlyingArchive.MovePositionToStart();
        if (!LoadBeforeGlobal())
            return false;

        switch (load)
        {
        case LoadType::LOAD_ALL:
            LoadGlobal();
            LoadAfterGlobal();
            break;
        case LoadType::LOAD_GLOBAL:
            LoadGlobal();
            break;
        case LoadType::LOAD_FIELD_PLACEHOLDERS:
            LoadAfterGlobal();
            break;
        }

        return true;
    }

    bool InputStasisArchive::WillLoad()
    {
        underlyingArchive.MovePositionToStart();
        return LoadBeforeGlobal();
    }

    Optional<Field> InputStasisArchive::ExtractField(FieldID fieldID)
    {
        typedef Optional<Field> Ret;

        auto found = fieldHandles.find(fieldID);
        if (found == fieldHandles.end())
            return Ret();

        Field out(fieldID);
        FillField(out, found);
        return Ret(std::move(out));
    }

    std::unique_ptr<Field> InputStasisArchive::ExtractFieldAsHeap(FieldID id)
    {
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return nullptr;

        auto out = std::make_unique<Field>(id);
        FillField(*out, found);
        return out;
    }

    ::Inscription::Buffer InputStasisArchive::ExtractFieldAsBuffer(FieldID id)
    {
        auto found = fieldHandles.find(id);
        return found->second.LoadBuffer(found->second.size);
    }

    InputStasisArchive::FieldIDList InputStasisArchive::AllFieldIDs() const
    {
        FieldIDList list;
        for (auto& loop : fieldHandles)
            list.push_back(loop.first);
        return list;
    }

    bool InputStasisArchive::HasField(FieldID fieldID) const
    {
        return fieldHandles.find(fieldID) != fieldHandles.end();
    }

    size_t InputStasisArchive::FieldCount() const
    {
        return fieldHandles.size();
    }

    const FileName& InputStasisArchive::GetFileName() const
    {
        return fileName;
    }

    ::Inscription::BinaryArchive& InputStasisArchive::UnderlyingArchive()
    {
        return underlyingArchive;
    }

    const ::Inscription::BinaryArchive& InputStasisArchive::UnderlyingArchive() const
    {
        return underlyingArchive;
    }

    InputStasisArchive::FieldHandle::FieldHandle(FocusedArchive& archive) : SkipFileHandle(archive), size(0)
    {}

    void InputStasisArchive::FieldHandle::LoadExtra()
    {
        archive(fieldID);
        archive(size);
    }

    bool InputStasisArchive::LoadBeforeGlobal()
    {
        FileName worldFileName;

        // Load base world name
        underlyingArchive(worldFileName);

        return worldFileName == worldFilePath.GetFileName();
    }

    void InputStasisArchive::LoadGlobal()
    {
        InputWorldArchive world(worldFilePath, *globalObjectManager);
        world.Load(InputWorldArchive::LOAD_GLOBAL);

        world.MoveTrackersTo(underlyingArchive);
    }

    void InputStasisArchive::LoadAfterGlobal()
    {
        // Load WorldStart
        underlyingArchive(_worldStart);

        // Load field handles
        ::Inscription::ContainerSize size;
        underlyingArchive(size);
        while (size-- > 0)
        {
            FieldHandle handle(underlyingArchive);
            handle.LoadPosition();
            fieldHandles.emplace(handle.fieldID, handle);
        }
    }

    void InputStasisArchive::FillField(Field& fill, FieldHandles::iterator handle)
    {
        // Load the field while setting up a section to clear immediately after
        underlyingArchive.StartTrackingSection();
        handle->second.LoadObject(fill);
        underlyingArchive.StopTrackingSection(true);
    }
}