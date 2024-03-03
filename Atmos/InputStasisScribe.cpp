
#include "InputStasisScribe.h"

#include "FileSystem.h"

#include "InputWorldScribe.h"

namespace Atmos
{
    const char* const InputStasisScribe::defaultExtension = "stasis";

    InputStasisScribe::InputStasisScribe(
        const FilePath& filePath,
        const FilePath& worldFilePath,
        ObjectManager& globalObjectManager,
        OpenMode openMode) :

        InputScribeBase([this]() { return _worldStart; }),
        fileName(filePath), worldFilePath(worldFilePath), underlyingScribe(filePath, "ATMOS GAIA STASIS"),
        globalObjectManager(&globalObjectManager)
    {
        if (openMode == OpenMode::FORCE_EXTENSION)
            this->fileName.SetExtension(defaultExtension);
    }

    bool InputStasisScribe::Load(LoadType load)
    {
        underlyingScribe.Reset();
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

    bool InputStasisScribe::WillLoad()
    {
        underlyingScribe.Reset();
        return LoadBeforeGlobal();
    }

    Optional<Field> InputStasisScribe::ExtractField(FieldID fieldID)
    {
        typedef Optional<Field> Ret;

        auto found = fieldHandles.find(fieldID);
        if (found == fieldHandles.end())
            return Ret();

        Field out(fieldID);
        FillField(out, found);
        return Ret(std::move(out));
    }

    std::unique_ptr<Field> InputStasisScribe::ExtractFieldAsHeap(FieldID id)
    {
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return nullptr;

        auto out = std::make_unique<Field>(id);
        FillField(*out, found);
        return out;
    }

    ::Inscription::Buffer InputStasisScribe::ExtractFieldAsBuffer(FieldID id)
    {
        auto found = fieldHandles.find(id);
        return found->second.LoadBuffer(found->second.size);
    }

    InputStasisScribe::FieldIDList InputStasisScribe::AllFieldIDs() const
    {
        FieldIDList list;
        for (auto& loop : fieldHandles)
            list.push_back(loop.first);
        return list;
    }

    bool InputStasisScribe::HasField(FieldID fieldID) const
    {
        return fieldHandles.find(fieldID) != fieldHandles.end();
    }

    size_t InputStasisScribe::FieldCount() const
    {
        return fieldHandles.size();
    }

    const FileName& InputStasisScribe::GetFileName() const
    {
        return fileName;
    }

    ::Inscription::Scribe& InputStasisScribe::UnderlyingScribe()
    {
        return underlyingScribe;
    }

    const ::Inscription::Scribe& InputStasisScribe::UnderlyingScribe() const
    {
        return underlyingScribe;
    }

    InputStasisScribe::FieldHandle::FieldHandle(FocusedScribe& scribe) : SkipFileHandle(scribe), size(0)
    {}

    void InputStasisScribe::FieldHandle::LoadExtra()
    {
        scribe.ReadNumeric(fieldID);
        scribe.ReadNumeric(size);
    }

    bool InputStasisScribe::LoadBeforeGlobal()
    {
        FileName worldFileName;

        // Load base world name
        underlyingScribe.Load(worldFileName);

        return worldFileName == worldFilePath.GetFileName();
    }

    void InputStasisScribe::LoadGlobal()
    {
        InputWorldScribe world(worldFilePath, *globalObjectManager);
        world.Load(InputWorldScribe::LOAD_GLOBAL);

        world.MoveTrackersTo(underlyingScribe);
    }

    void InputStasisScribe::LoadAfterGlobal()
    {
        // Load WorldStart
        underlyingScribe.Load(_worldStart);

        // Load field handles
        ::Inscription::ContainerSize size;
        underlyingScribe.Load(size);
        while (size-- > 0)
        {
            FieldHandle handle(underlyingScribe);
            handle.LoadPosition();
            fieldHandles.emplace(handle.fieldID, handle);
        }
    }

    void InputStasisScribe::FillField(Field& fill, FieldHandles::iterator handle)
    {
        // Load the field while setting up a section to clear immediately after
        underlyingScribe.StartTrackingSection();
        handle->second.LoadObject(fill);
        underlyingScribe.StopTrackingSection(true);
    }
}