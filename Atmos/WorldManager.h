#pragma once

#include "Field.h"
#include "FieldDestination.h"

#include "ObjectRegistration.h"

#include "Event.h"
#include "FilePath.h"
#include "Optional.h"

namespace Atmos
{
    class FileSystem;
    class LoggingSystem;

    class WorldManager
    {
    public:
        Event<Field&> eventFieldSet;
        NullEvent eventBeforeFieldSet;
        // This will be called right before a new field is made to replace the current one
        Event<Field*> eventFinalizeField;
    public:
        WorldManager(ObjectManager& globalObjectManager, ObjectRegistration& objectRegistration);
        WorldManager(const ::Inscription::Table<WorldManager>& table);

        void Initialize();

        void Work();
        void LockIn();
        void Clear(bool destroyField = true);

        // Returns if the field exists
        bool Request(FieldID id);
        // Returns if the field exists
        bool Request(const FieldDestination& request);

        // The transition field is the "old" field during a transition period
        Field* GetTransitionField();
        bool IsTransitioning();

        void StartNew();
        void UseWorld(const FilePath& path);
        void UseWorld(const FileName& name);
        void UseStasis(const FileName& name);

        void Autosave();
        void SaveStasis(const FileName& name);

        const FilePath& WorldPath();

        Field* CurrentField();
    private:
        ObjectManager* globalObjectManager;
        FileSystem* FindFileSystem() const;
        LoggingSystem* FindLoggingSystem() const;

        ObjectRegistration* objectRegistration;
    private:
        std::unique_ptr<Field> field;
        std::unique_ptr<Field> oldField;

        bool change;
        FieldID requestedFieldID;
        typedef Optional<FieldDestination> DestinationT;
        DestinationT destination;
        typedef Optional<FileName> StasisNameT;
        StasisNameT stasisName;
        std::unordered_set<FieldID> fieldIDs;
        bool useWorldStart;

        FilePath worldPath;
    private:
        bool HasField(FieldID id);

        void Autosave(FieldID worldStartFieldID);
        void SaveStasis(FieldID worldStartFieldID, const FileName& name);
    private:
        FilePath CreateWorldFilePath(const FileName& fileName) const;
        FilePath CreateStasisFilePath(const FileName& fileName) const;

        FilePath WorldFolderFilePath() const;
        FilePath StasisFolderFilePath() const;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::WorldManager)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}