
#include "InputWorldScribe.h"

#include "AssetPackageSystem.h"

#include "ObjectTypeNameSerializer.h"

namespace Atmos
{
    const char* const InputWorldScribe::worldExtension = "gaia";

    InputWorldScribe::InputWorldScribe(
        const FilePath& filePath,
        ObjectManager& globalObjectManager,
        OpenMode openMode) :

        InputScribeBase([this]() { return _worldStart; }),
        globalObjectManager(&globalObjectManager),
        filePath(GetForcedFilePath(filePath, openMode)), underlyingScribe(GetForcedFilePath(filePath, openMode), "ATMOS GAIA")
    {}

    bool InputWorldScribe::Load(LoadType load)
    {
        underlyingScribe.MovePositionToStart();

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
            RegistrySkipHandle skipper(underlyingScribe);

            LoadGlobal(&skipper);
            underlyingScribe.SeekStream(skipper.SkipPosition());
            LoadAfterGlobal();

            break;
        }

        return true;
    }

    bool InputWorldScribe::WillLoad()
    {
        return true;
    }

    Optional<Field> InputWorldScribe::ExtractField(FieldID id)
    {
        typedef Optional<Field> Ret;
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return Ret();

        Field out(id);
        FillField(out, found);
        return Ret(std::move(out));
    }

    std::unique_ptr<Field> InputWorldScribe::ExtractFieldAsHeap(FieldID id)
    {
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return nullptr;

        auto out = std::make_unique<Field>(id);
        FillField(*out, found);
        return out;
    }

    ::Inscription::Buffer InputWorldScribe::ExtractFieldAsBuffer(FieldID id)
    {
        auto found = fieldHandles.find(id);
        return found->second.LoadBuffer(found->second.size);
    }

    InputWorldScribe::FieldIDList InputWorldScribe::AllFieldIDs() const
    {
        FieldIDList list;
        for (auto& loop : fieldHandles)
            list.push_back(loop.first);
        return list;
    }

    bool InputWorldScribe::HasField(FieldID fieldID) const
    {
        return fieldHandles.find(fieldID) != fieldHandles.end();
    }

    size_t InputWorldScribe::FieldCount() const
    {
        return fieldHandles.size();
    }

    const FilePath& InputWorldScribe::GetFilePath() const
    {
        return filePath;
    }

    ::Inscription::BinaryScribe& InputWorldScribe::UnderlyingScribe()
    {
        return underlyingScribe;
    }

    const ::Inscription::BinaryScribe& InputWorldScribe::UnderlyingScribe() const
    {
        return underlyingScribe;
    }

    InputWorldScribe::FieldHandle::FieldHandle(FocusedScribe& scribe) : SkipFileHandle(scribe), size(0)
    {}

    void InputWorldScribe::FieldHandle::LoadExtra()
    {
        scribe.ReadNumeric(fieldID);
        scribe.ReadNumeric(size);
    }

    void InputWorldScribe::LoadGlobal(RegistrySkipHandle* registrySkipper)
    {
        if (registrySkipper)
        {
            registrySkipper->LoadPosition();
            return;
        }
        else
        {
            RegistrySkipHandle notUsed(underlyingScribe);
            notUsed.LoadPosition();
        }

        // Load asset package
        auto assetPackageSystem = globalObjectManager->FindSystem<AssetPackageSystem>();
        assetPackageSystem->Load(filePath);

        // Load object type names
        ObjectTypeNameSerializer objectTypeNameSerializer;
        objectTypeNameSerializer.LoadAll(underlyingScribe);

        // Load global object manager
        underlyingScribe.Load(*globalObjectManager);
    }

    void InputWorldScribe::LoadAfterGlobal()
    {
        // Load WorldStart
        underlyingScribe.Load(_worldStart);

        // Retrieve field handles
        ::Inscription::ContainerSize size;
        underlyingScribe.Load(size);
        while (size-- > 0)
        {
            FieldHandle handle(underlyingScribe);
            handle.LoadPosition();
            fieldHandles.emplace(handle.fieldID, handle);
        }
    }

    void InputWorldScribe::FillField(Field& fill, FieldHandleMap::iterator handle)
    {
        // Load the field while setting up a section to clear immediately after
        underlyingScribe.StartTrackingSection();
        handle->second.LoadObject(fill);
        underlyingScribe.StopTrackingSection(true);
    }

    FilePath InputWorldScribe::GetForcedFilePath(const FilePath& filePath, OpenMode openMode)
    {
        FilePath ret(filePath);
        if (openMode == OpenMode::FORCE_EXTENSION)
            ret.SetExtension(worldExtension);
        return ret;
    }
}