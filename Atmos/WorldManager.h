
#pragma once

#include "Field.h"
#include "FieldDestination.h"

#include "Event.h"
#include "Optional.h"

namespace Atmos
{
    class Camera;
    class WorldStartBase;
    class WorldManager
    {
    public:
        class State
        {
        private:
            bool temporary;
            Field *field;
            State(bool temporary, Field *field);
            friend WorldManager;
        public:
            State(const State &arg) = default;
            State& operator=(const State &arg) = default;

            void Restore();
            bool IsTemporary() const;
            Field* GetField() const;
        };
    private:
        static bool temporaryUse;
        std::unique_ptr<Field> field;
        std::unique_ptr<Field> oldField;

        static bool change;
        static FieldID requestedFieldID;
        typedef Optional<FieldDestination> DestinationT;
        static DestinationT destination;
        typedef Optional<FileName> StasisNameT;
        static StasisNameT stasisName;
        static std::unordered_set<FieldID> fieldIDs;
        static bool useWorldStart;

        static bool renderRoofs;

        static FilePath worldPath;

        WorldManager() = default;
        ~WorldManager();
        WorldManager(const WorldManager &arg) = delete;
        WorldManager& operator=(const WorldManager &arg) = delete;

        static Field* CheckReleaseField(bool force = false);

        static void OnFocusLost();
        static void OnFocusRegain();

        static bool HasField(FieldID id);

        static void Autosave(FieldID worldStartFieldID);
        static void SaveStasis(FieldID worldStartFieldID, const FileName &name);
    public:
        Event<Field&> eventFieldSet;
        NullEvent eventBeforeFieldSet;
        // This will be called right before a new field is made to replace the current one
        NullEvent eventFinalizeField;

        static WorldManager& Instance();

        static void Init();
        static void Work();
        static void Draw();
        static void DoLighting();
        static void LockIn();
        static void Clear(bool destroyField = true);

        // Returns if the field exists
        static bool Request(FieldID id);
        // Returns if the field exists
        static bool Request(const FieldDestination &request);
        // Will not delete the field
        // If this is not manually released, whenever a field is set next, this manager will still NOT delete it
        // Returns the previous field
        static State TemporaryUse(Field *force = nullptr);
        static void RestoreState(const State &state);
        static bool IsFieldTemporary();

        // The transition field is the "old" field during a transition period
        static Field* GetTransitionField();
        static bool IsTransitioning();

        static void StartNew();
        static void UseWorld(const FilePath &path);
        static void UseWorld(const FileName &name);
        static void UseStasis(const FileName &name);

        static void Autosave();
        static void SaveStasis(const FileName &name);

        static const FilePath& GetWorldPath();

        static void ToggleRoofs();
        static void SetRoofs(bool setTo);
        static bool GetRenderRoofs();

        static Field* GetCurrentField();
        static State GetCurrentState();
    };
}