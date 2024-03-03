#pragma once

#include <unordered_set>

#include "Field.h"
#include "FieldDestination.h"

#include "ObjectManagerFactory.h"

#include "Event.h"
#include "FileName.h"
#include "FilePath.h"
#include "Optional.h"

namespace Atmos
{
    class ObjectManager;
    class LoggingSystem;
}

namespace Atmos::File
{
    class FileSystem;
}

namespace Atmos::World
{
    class WorldManager
    {
    public:
        Event<Field&> eventFieldSet;
        Event<> eventBeforeFieldSet;
        // This will be called right before a new field is made to replace the current one
        Event<Field*> eventFinalizeField;
    public:
        WorldManager(ObjectManager& globalObjectManager, ObjectManagerFactory& objectManagerFactory);

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
        void UseWorld(const File::Path& path);
        void UseWorld(const File::Name& name);
        void UseStasis(const File::Name& name);

        void Autosave();
        void SaveStasis(const File::Name& name);

        const File::Path& WorldPath();

        Field* CurrentField();
    private:
        ObjectManager* globalObjectManager;
        File::FileSystem* FindFileSystem() const;
        LoggingSystem* FindLoggingSystem() const;

        ObjectManagerFactory* objectManagerFactory;
    private:
        std::unique_ptr<Field> field;
        std::unique_ptr<Field> oldField;

        bool change;
        FieldID requestedFieldID;
        typedef Optional<FieldDestination> DestinationT;
        DestinationT destination;
        typedef Optional<File::Name> StasisNameT;
        StasisNameT stasisName;
        std::unordered_set<FieldID> fieldIDs;
        bool useWorldStart;

        File::Path worldPath;
    private:
        bool HasField(FieldID id);

        void Autosave(FieldID worldStartFieldID);
        void SaveStasis(FieldID worldStartFieldID, const File::Name& name);
    private:
        File::Path CreateWorldFilePath(const File::Name& fileName) const;
        File::Path CreateStasisFilePath(const File::Name& fileName) const;

        File::Path WorldFolderFilePath() const;
        File::Path StasisFolderFilePath() const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::World::WorldManager, BinaryArchive> :
        public CompositeScribe<::Atmos::World::WorldManager, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}