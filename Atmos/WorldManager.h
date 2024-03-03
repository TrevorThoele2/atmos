
#pragma once

#include "Field.h"
#include "FieldDestination.h"

#include "FilePath.h"
#include "Event.h"
#include "Optional.h"

namespace Atmos
{
    class Camera;
    class WorldStartBase;
    class WorldManager
    {
    public:
        Event<Field&> eventFieldSet;
        NullEvent eventBeforeFieldSet;
        // This will be called right before a new field is made to replace the current one
        Event<Field*> eventFinalizeField;
    public:
        WorldManager() = default;
        WorldManager(const WorldManager &arg) = default;
        WorldManager& operator=(const WorldManager &arg) = default;

        void Work();
        void Draw();
        void LockIn();
        void Clear(bool destroyField = true);

        // Returns if the field exists
        bool Request(FieldID id);
        // Returns if the field exists
        bool Request(const FieldDestination &request);

        // The transition field is the "old" field during a transition period
        Field* GetTransitionField();
        bool IsTransitioning();

        void StartNew();
        void UseWorld(const FilePath &path);
        void UseWorld(const FileName &name);
        void UseStasis(const FileName &name);

        void Autosave();
        void SaveStasis(const FileName &name);

        const FilePath& GetWorldPath();

        Field* GetCurrentField();
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

        void OnFocusLost();
        void OnFocusRegain();

        bool HasField(FieldID id);

        void Autosave(FieldID worldStartFieldID);
        void SaveStasis(FieldID worldStartFieldID, const FileName &name);
    };
}