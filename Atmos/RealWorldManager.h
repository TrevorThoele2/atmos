#pragma once

#include "WorldManager.h"

#include <unordered_set>
#include <variant>

#include "Field.h"
#include "FieldDestination.h"

#include "FilePath.h"

#include "InputFieldArchiveInterface.h"

namespace Atmos::World
{
    class RealManager final : public Manager
    {
    public:
        using RetrieveReliquary = std::function<std::unique_ptr<Arca::Reliquary>()>;
        using RetrieveLoadAssetsUserContext = std::function<std::unique_ptr<Inscription::LoadAssetsUserContext>()>;
    public:
        RealManager(
            const RetrieveReliquary& retrieveReliquary,
            const RetrieveLoadAssetsUserContext& retrieveLoadAssetsUserContext);

        void LockIn() override;
        [[nodiscard]] bool WillLockIn() const override;

        void Request(FieldID id) override;
        void Request(const FieldDestination& request) override;

        void UseWorld(const File::Path& path) override;
        void UseField(Field&& field, std::vector<Property>&& properties) override;
        void UseStasis(const File::Path& path) override;

        [[nodiscard]] File::Path WorldPath() const override;

        [[nodiscard]] Field* CurrentField() override;
        [[nodiscard]] const Field* CurrentField() const override;

        void SetWorldProperties(const std::vector<Property>& set) override;
        [[nodiscard]] std::vector<Property> CurrentWorldProperties() override;
    private:
        struct RequestedField
        {
            FieldID id;
        };

        struct RequestedFieldDestination
        {
            FieldDestination destination;
        };

        using Requested = std::variant<RequestedField, RequestedFieldDestination>;
        std::optional<Requested> requested;

        struct WorldUtilization
        {
            File::Path filePath;
        };

        struct StasisUtilization
        {
            File::Path filePath;
        };

        struct FieldUtilization
        {
            Field field;
            std::vector<Property> properties;
        };

        using Utilization = std::variant<WorldUtilization, StasisUtilization, FieldUtilization>;
        std::optional<Utilization> utilization;
    private:
        std::optional<Field> currentField;
        std::vector<Property> currentProperties;

        std::optional<File::Path> stasisName;
        std::unordered_set<FieldID> fieldIDs;

        File::Path worldPath;
    private:
        RetrieveReliquary retrieveReliquary;
        RetrieveLoadAssetsUserContext retrieveLoadAssetsUserContext;
    private:
        bool ContainsField(FieldID id);
    private:
        void ChangeField(
            FieldID id,
            std::unique_ptr<Arca::Reliquary>&& reliquary,
            std::unique_ptr<Inscription::LoadAssetsUserContext>&& loadAssetsUserContext);

        using InputArchiveInterfacePtr = std::unique_ptr<Serialization::InputFieldArchiveInterface>;
        [[nodiscard]] InputArchiveInterfacePtr InputArchiveInterface(
            Inscription::LoadAssetsUserContext& loadAssetsUserContext) const;

        void SetFieldIDs(const std::vector<FieldID>& ids);
    private:
        [[nodiscard]] static File::Path CreateWorldFilePath(const File::Path& fileName);
        [[nodiscard]] static File::Path CreateStasisFilePath(const File::Path& fileName);

        [[nodiscard]] static File::Path WorldFolderFilePath();
        [[nodiscard]] static File::Path StasisFolderFilePath();
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::World::RealManager, Archive> final
    {
        using Category = NullScribeCategory<Atmos::World::RealManager>;
    };
}