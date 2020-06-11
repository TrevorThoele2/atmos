#pragma once

#include <unordered_set>
#include <variant>

#include "Field.h"
#include "FieldDestination.h"

#include "FilePath.h"

#include "InputFieldArchiveInterface.h"

namespace Atmos::World
{
    class WorldManager
    {
    public:
        explicit WorldManager(ExternalManagers externalManagers);

        void Work();
        void LockIn();

        void Request(FieldID id);
        void Request(const FieldDestination& request);

        void UseWorld(const File::Path& path);
        void UseField(Field&& field);
        void UseStasis(const File::Path& path);

        void Autosave();

        [[nodiscard]] const File::Path& WorldPath() const;

        [[nodiscard]] Field* CurrentField();
        [[nodiscard]] const Field* CurrentField() const;
    private:
        ExternalManagers externalManagers;
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
        };

        using Utilization = std::variant<WorldUtilization, StasisUtilization, FieldUtilization>;
        std::optional<Utilization> utilization;
    private:
        std::optional<Field> currentField;

        std::optional<File::Path> stasisName;
        std::unordered_set<FieldID> fieldIDs;

        File::Path worldPath;
    private:
        bool ContainsField(FieldID id);

        void Autosave(FieldID worldStartFieldID);
    private:
        void ChangeField(FieldID id);

        using InputArchiveInterfacePtr = std::unique_ptr<Serialization::InputFieldArchiveInterface>;
        [[nodiscard]] InputArchiveInterfacePtr InputArchiveInterface() const;

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
    template<>
    class Scribe<::Atmos::World::WorldManager, BinaryArchive> final :
        public NullScribe<::Atmos::World::WorldManager, BinaryArchive>
    {};
}