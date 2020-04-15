#include "WorldManager.h"

#include "FileManagerProvider.h"
#include "EntityAvatarCurator.h"
#include "DebugStatistics.h"
#include "Log.h"

#include "StringUtility.h"

#include "InputWorldArchiveInterface.h"
#include "InputStasisArchiveInterface.h"

#include <Chroma/Contract.h>

namespace Atmos::World
{
    const char* autosaveName = "Autosave.stasis";

    WorldManager::WorldManager(Arca::Reliquary& globalReliquary) : globalReliquary(&globalReliquary)
    {
        std::filesystem::create_directory(StasisFolderFilePath());
    }

    void WorldManager::Work()
    {
        globalReliquary->Work();
        if (currentField)
            currentField->Reliquary().Work();

        LockIn();
    }

    void WorldManager::LockIn()
    {
        if (!requested.has_value())
            return;

        if (std::holds_alternative<RequestedField>(*requested))
        {
            auto& request = std::get<RequestedField>(*requested);
            ChangeField(request.id);
        }
        else if (std::holds_alternative<RequestedFieldDestination>(*requested))
        {
            auto& request = std::get<RequestedFieldDestination>(*requested);
            ChangeField(request.destination.id);
        }

        requested = {};
    }

    void WorldManager::Request(FieldID id)
    {
        requested = RequestedField{ id };
    }

    void WorldManager::Request(const FieldDestination& request)
    {
        requested = RequestedFieldDestination{ request };
    }

    void WorldManager::UseWorld(const File::Path& path)
    {
        worldPath = path.has_parent_path() ? path : "Worlds" / path;
        utilization = WorldUtilization{ path };
    }

    void WorldManager::UseField(Field&& field)
    {
        utilization = FieldUtilization{ std::move(field) };
    }

    void WorldManager::UseStasis(const File::Path& path)
    {
        stasisName = path.string();
        utilization = WorldUtilization{ File::Path("Stasis" + path.string()) };
    }

    void WorldManager::Autosave()
    {

    }

    const File::Path& WorldManager::WorldPath() const
    {
        return worldPath;
    }

    Field* WorldManager::CurrentField()
    {
        return currentField
            ? &*currentField
            : nullptr;
    }

    const Field* WorldManager::CurrentField() const
    {
        return const_cast<WorldManager&>(*this).CurrentField();
    }

    bool WorldManager::ContainsField(FieldID id)
    {
        return fieldIDs.find(id) == fieldIDs.end();
    }

    void WorldManager::Autosave(FieldID worldStartFieldID)
    {

    }

    void WorldManager::ChangeField(FieldID id)
    {
        DEBUG_ASSERT(utilization.has_value());

        if (std::holds_alternative<FieldUtilization>(*utilization))
        {
            auto& fieldUtilization = std::get<FieldUtilization>(*utilization);
            auto& field = fieldUtilization.field;
            SetFieldIDs({ field.ID() });
            currentField = std::move(field);
        }
        else
        {
            auto inputArchiveInterface = InputArchiveInterface();
            SetFieldIDs(inputArchiveInterface->AllFieldIDs());
            currentField = inputArchiveInterface->ExtractField(id, *globalReliquary);
        }
    }

    std::unique_ptr<Serialization::InputFieldArchiveInterface> WorldManager::InputArchiveInterface() const
    {
        DEBUG_ASSERT(utilization.has_value());

        auto& value = *utilization;

        if (std::holds_alternative<WorldUtilization>(value))
        {
            auto& utilization = std::get<WorldUtilization>(value);
            return std::make_unique<Serialization::InputWorldArchiveInterface>(utilization.filePath);
        }
        else if (std::holds_alternative<StasisUtilization>(value))
        {
            auto& utilization = std::get<StasisUtilization>(value);
            return std::make_unique<Serialization::InputStasisArchiveInterface>(utilization.filePath);
        }

        DEBUG_ASSERT(false);
    }

    void WorldManager::SetFieldIDs(const std::vector<FieldID>& ids)
    {
        fieldIDs.clear();
        for (auto& id : ids)
            fieldIDs.emplace(id);
    }

    File::Path WorldManager::CreateWorldFilePath(const File::Path& fileName)
    {
        return WorldFolderFilePath() / fileName;
    }

    File::Path WorldManager::CreateStasisFilePath(const File::Path& fileName)
    {
        return StasisFolderFilePath() / fileName;
    }

    File::Path WorldManager::WorldFolderFilePath()
    {
        return std::filesystem::current_path() / "Worlds";
    }

    File::Path WorldManager::StasisFolderFilePath()
    {
        return std::filesystem::current_path() / "Saves";
    }
}