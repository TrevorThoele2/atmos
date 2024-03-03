#include "RealWorldManager.h"

#include "StringUtility.h"

#include "InputWorldArchiveInterface.h"
#include "InputStasisArchiveInterface.h"

#include <Chroma/Contract.h>

namespace Atmos::World
{
    void RealManager::LockIn(
        std::unique_ptr<Arca::Reliquary>&& reliquary, Inscription::LoadAssetsUserContext& loadAssetsUserContext)
    {
        if (!requested.has_value())
            return;

        if (std::holds_alternative<RequestedField>(*requested))
        {
            auto& request = std::get<RequestedField>(*requested);
            ChangeField(request.id, std::move(reliquary), loadAssetsUserContext);
        }
        else if (std::holds_alternative<RequestedFieldDestination>(*requested))
        {
            auto& request = std::get<RequestedFieldDestination>(*requested);
            ChangeField(request.destination.id, std::move(reliquary), loadAssetsUserContext);
        }

        requested = {};
    }

    void RealManager::Request(FieldID id)
    {
        requested = RequestedField{ id };
    }

    void RealManager::Request(const FieldDestination& request)
    {
        requested = RequestedFieldDestination{ request };
    }

    void RealManager::UseWorld(const File::Path& path)
    {
        worldPath = path.has_parent_path() ? path : "Worlds" / path;
        utilization = WorldUtilization{ path };
    }

    void RealManager::UseField(Field&& field, std::vector<Property>&& properties)
    {
        utilization = FieldUtilization{ std::move(field), std::move(properties) };
    }

    void RealManager::UseStasis(const File::Path& path)
    {
        stasisName = path.string();
        utilization = WorldUtilization{ File::Path("Stasis") / path.string() };
    }

    File::Path RealManager::WorldPath() const
    {
        return worldPath;
    }

    Field* RealManager::CurrentField()
    {
        return currentField
            ? &*currentField
            : nullptr;
    }

    const Field* RealManager::CurrentField() const
    {
        return const_cast<RealManager&>(*this).CurrentField();
    }

    void RealManager::SetWorldProperties(const std::vector<Property>& set)
    {
        currentProperties = set;
    }

    std::vector<Property> RealManager::CurrentWorldProperties()
    {
        return currentProperties;
    }

    bool RealManager::ContainsField(FieldID id)
    {
        return fieldIDs.find(id) == fieldIDs.end();
    }

    void RealManager::ChangeField(
        FieldID id,
        std::unique_ptr<Arca::Reliquary>&& reliquary,
        Inscription::LoadAssetsUserContext& loadAssetsUserContext)
    {
        DEBUG_ASSERT(utilization.has_value());

        if (std::holds_alternative<FieldUtilization>(*utilization))
        {
            auto& fieldUtilization = std::get<FieldUtilization>(*utilization);
            auto& field = fieldUtilization.field;
            auto& worldData = fieldUtilization.properties;
            SetFieldIDs({ field.ID() });
            currentField = std::move(field);
            currentProperties = std::move(worldData);
        }
        else
        {
            auto inputArchiveInterface = InputArchiveInterface(loadAssetsUserContext);
            SetFieldIDs(inputArchiveInterface->AllFieldIDs());
            currentField = inputArchiveInterface->ExtractField(id, std::move(reliquary));
            currentProperties = inputArchiveInterface->WorldProperties();
        }
    }

    std::unique_ptr<Serialization::InputFieldArchiveInterface> RealManager::InputArchiveInterface(
        Inscription::LoadAssetsUserContext& loadAssetsUserContext) const
    {
        DEBUG_ASSERT(utilization.has_value());

        auto& value = *utilization;

        if (std::holds_alternative<WorldUtilization>(value))
        {
            auto& utilization = std::get<WorldUtilization>(value);
            return std::make_unique<Serialization::InputWorldArchiveInterface>(
                utilization.filePath, loadAssetsUserContext);
        }
        else if (std::holds_alternative<StasisUtilization>(value))
        {
            auto& utilization = std::get<StasisUtilization>(value);
            return std::make_unique<Serialization::InputStasisArchiveInterface>(
                utilization.filePath, loadAssetsUserContext);
        }

        DEBUG_ASSERT(false);
        return {};
    }

    void RealManager::SetFieldIDs(const std::vector<FieldID>& ids)
    {
        fieldIDs.clear();
        for (auto& id : ids)
            fieldIDs.emplace(id);
    }

    File::Path RealManager::CreateWorldFilePath(const File::Path& fileName)
    {
        return WorldFolderFilePath() / fileName;
    }

    File::Path RealManager::CreateStasisFilePath(const File::Path& fileName)
    {
        return StasisFolderFilePath() / fileName;
    }

    File::Path RealManager::WorldFolderFilePath()
    {
        return std::filesystem::current_path() / "Worlds";
    }

    File::Path RealManager::StasisFolderFilePath()
    {
        return std::filesystem::current_path() / "Saves";
    }
}