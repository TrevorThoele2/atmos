#include "InputWorldArchive.h"

#include "AssetPackageSystem.h"

#include "ObjectTypeNameSerializer.h"

namespace Atmos::World::Serialization
{
    const char* const InputWorldArchive::worldExtension = "gaia";

    InputWorldArchive::InputWorldArchive(
        const File::Path& filePath,
        ObjectManager& globalObjectManager,
        OpenMode openMode) :

        InputArchiveBase([this]() { return _worldStart; }),
        globalObjectManager(&globalObjectManager),
        filePath(GetForcedFilePath(filePath, openMode)),
        underlyingArchive(GetForcedFilePath(filePath, openMode), "ATMOS GAIA")
    {}

    bool InputWorldArchive::Load(LoadType load)
    {
        underlyingArchive.MovePositionToStart();

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
            RegistrySkipHandle skipper(underlyingArchive);

            LoadGlobal(&skipper);
            underlyingArchive.SeekStream(skipper.SkipPosition());
            LoadAfterGlobal();

            break;
        }

        return true;
    }

    bool InputWorldArchive::WillLoad()
    {
        return true;
    }

    Optional<Field> InputWorldArchive::ExtractField(FieldID id)
    {
        typedef Optional<Field> Ret;
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return Ret();

        Field out(id);
        FillField(out, found);
        return Ret(std::move(out));
    }

    std::unique_ptr<Field> InputWorldArchive::ExtractFieldAsHeap(FieldID id)
    {
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return nullptr;

        auto out = std::make_unique<Field>(id);
        FillField(*out, found);
        return out;
    }

    ::Inscription::Buffer InputWorldArchive::ExtractFieldAsBuffer(FieldID id)
    {
        auto found = fieldHandles.find(id);
        return found->second.LoadBuffer(found->second.size);
    }

    InputWorldArchive::FieldIDList InputWorldArchive::AllFieldIDs() const
    {
        FieldIDList list;
        for (auto& loop : fieldHandles)
            list.push_back(loop.first);
        return list;
    }

    bool InputWorldArchive::HasField(FieldID fieldID) const
    {
        return fieldHandles.find(fieldID) != fieldHandles.end();
    }

    size_t InputWorldArchive::FieldCount() const
    {
        return fieldHandles.size();
    }

    const File::Path& InputWorldArchive::GetFilePath() const
    {
        return filePath;
    }

    ::Inscription::BinaryArchive& InputWorldArchive::UnderlyingArchive()
    {
        return underlyingArchive;
    }

    const ::Inscription::BinaryArchive& InputWorldArchive::UnderlyingArchive() const
    {
        return underlyingArchive;
    }

    InputWorldArchive::FieldHandle::FieldHandle(FocusedArchive& archive) : SkipFileHandle(archive), size(0)
    {}

    void InputWorldArchive::FieldHandle::LoadExtra()
    {
        archive(fieldID);
        archive(size);
    }

    void InputWorldArchive::LoadGlobal(RegistrySkipHandle* registrySkipper)
    {
        if (registrySkipper)
        {
            registrySkipper->LoadPosition();
            return;
        }
        else
        {
            RegistrySkipHandle notUsed(underlyingArchive);
            notUsed.LoadPosition();
        }

        // Load asset package
        auto assetPackageSystem = globalObjectManager->FindSystem<Asset::AssetPackageSystem>();
        assetPackageSystem->Load(filePath);

        // Load object type names
        ObjectTypeNameSerializer objectTypeNameSerializer;
        objectTypeNameSerializer.LoadAll(underlyingArchive);

        // Load global object manager
        underlyingArchive(*globalObjectManager);
    }

    void InputWorldArchive::LoadAfterGlobal()
    {
        // Load WorldStart
        underlyingArchive(_worldStart);

        // Retrieve field handles
        ::Inscription::ContainerSize size;
        underlyingArchive(size);
        while (size-- > 0)
        {
            FieldHandle handle(underlyingArchive);
            handle.LoadPosition();
            fieldHandles.emplace(handle.fieldID, handle);
        }
    }

    void InputWorldArchive::FillField(Field& fill, FieldHandleMap::iterator handle)
    {
        handle->second.LoadObject(fill);
    }

    File::Path InputWorldArchive::GetForcedFilePath(const File::Path& filePath, OpenMode openMode)
    {
        File::Path ret(filePath);
        if (openMode == OpenMode::FORCE_EXTENSION)
            ret.SetExtension(worldExtension);
        return ret;
    }
}