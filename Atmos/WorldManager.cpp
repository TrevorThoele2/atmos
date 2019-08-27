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

    WorldManager::WorldManager(Arca::Reliquary& reliquary) : reliquary(&reliquary)
    {
        debugStatistics = Arca::Ptr<Debug::Statistics>(reliquary);

        File::manager->MakeDirectory(StasisFolderFilePath());
    }

    void WorldManager::Work()
    {
        field->reliquary->Work();

        {
            const auto currentField = CurrentField();
            debugStatistics->game.fieldID = currentField ?
                currentField->id :
                std::optional<FieldID>{};
        }

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
        worldPath = path;
        utilization = WorldUtilization{ path };
    }

    void WorldManager::UseWorld(const File::Name& name)
    {
        worldPath = "Worlds" + name.GetValue();
        utilization = WorldUtilization{ worldPath };
    }

    void WorldManager::UseStasis(const File::Name& name)
    {
        stasisName = name;
        utilization = WorldUtilization{ File::Path("Stasis" + name.GetValue()) };
    }

    void WorldManager::Autosave()
    {
        if (field)
            Autosave(field->id);
    }

    const File::Path& WorldManager::WorldPath()
    {
        return worldPath;
    }

    Field* WorldManager::CurrentField()
    {
        return field.get();
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
        auto inputArchiveInterface = InputArchiveInterface();
        SetFieldIDs(inputArchiveInterface->AllFieldIDs());
        field = inputArchiveInterface->ExtractFieldAsHeap(id, *reliquary);
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

    File::Path WorldManager::CreateWorldFilePath(const File::Name& fileName)
    {
        return WorldFolderFilePath().Append(fileName);
    }

    File::Path WorldManager::CreateStasisFilePath(const File::Name& fileName)
    {
        return StasisFolderFilePath().Append(fileName);
    }

    File::Path WorldManager::WorldFolderFilePath()
    {
        return (File::manager->ExePath() + "Worlds").AppendSeparator();
    }

    File::Path WorldManager::StasisFolderFilePath()
    {
        return (File::manager->ExePath() + "Saves").AppendSeparator();
    }
}