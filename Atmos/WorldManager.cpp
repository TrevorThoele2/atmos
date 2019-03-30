
#include "WorldManager.h"

#include "WorldScribe.h"
#include "StasisScribe.h"

#include "AvatarSystem.h"
#include "RenderFragmentSystem.h"
#include "nEntityPositionSystem.h"
#include "MainGame.h"
#include "Battle.h"
#include "Environment.h"
#include "StringUtility.h"
#include "SoundSystem.h"

#include "AssetPackage.h"

#include "FileUtility.h"
#include "Logger.h"

namespace Atmos
{
    const char* autosaveName = "Autosave.stasis";

    void WorldManager::Work()
    {
        field->objectManager.Work();
    }

    void WorldManager::Draw()
    {
        if (!field)
            return;

        field->objectManager.FindSystem<RenderFragmentSystem>()->DrawAll();
    }

    void WorldManager::LockIn()
    {
        if (!change)
            return;

        FieldID useFieldID = requestedFieldID;
        if (destination.IsValid())
            useFieldID = destination->id;

        auto prevField = field.get();
        bool hadPreviousField = prevField != nullptr;
        if (!prevField || useFieldID != prevField->GetID() || useWorldStart)
        {
            bool isWorld = !stasisName.IsValid();

            std::unique_ptr<InScribeBase> inScribe;
            String noLoadError;
            String noLoadErrorFileParamName;
            String noLoadErrorFileParam;
            if (isWorld)
            {
                inScribe.reset(new WorldScribeIn(worldPath));
                noLoadError = "The world file was unloadable.";
                noLoadErrorFileParamName = "World File Path";
                noLoadErrorFileParam = worldPath.GetValue();
            }
            else
            {
                inScribe.reset(new StasisScribeIn(stasisName.Get(), worldPath));
                noLoadError = "The stasis file's world is not the same as the provided world.";
                noLoadErrorFileParamName = "Stasis File Name";
                noLoadErrorFileParam = stasisName.Get().GetValue();
            }

            if (!inScribe->WillLoad())
            {
                Logger::Log(std::move(noLoadError),
                    Logger::Type::ERROR_SEVERE,
                    Logger::NameValueVector{ NameValuePair(noLoadErrorFileParamName, noLoadErrorFileParam) });
            }
            else
            {
                // This will load

                // We are going to load simultaneously while saving
                if (prevField)
                    inScribe->Load(InScribeBase::LOAD_FIELD_PLACEHOLDERS);
                else
                    inScribe->Load();

                // Retrieve the world start and check if to use it
                if (useWorldStart)
                    useFieldID = inScribe->GetWorldStart().GetFieldID();

                // Retrieve the new field IDs
                {
                    fieldIDs.clear();
                    std::vector<FieldID> idVector;
                    inScribe->GetIDs(idVector);
                    for (auto& loop : idVector)
                        fieldIDs.emplace(loop);
                }

                // Finalize the old field
                eventFinalizeField(prevField);

                // Create the new field
                auto newField = inScribe->GetAsHeap(useFieldID);
                ATMOS_ASSERT_MESSAGE(newField, "The newly created field must exist.");

                oldField.reset(field.release());
                field = std::move(newField);

                // Execute before-set events
                eventBeforeFieldSet();

                StasisWorldStart worldStart;

                // Save the new stasis
                {
                    auto &tempPath = Environment::GetFileSystem()->GetTempDirectoryPath();
                    tempPath.SetFileName(autosaveName);
                    tempPath.SetExtension("temporary");

                    // Create the out scribe (always outputting into a stasis)
                    {
                        StasisScribeOut outScribe(tempPath, worldPath.GetFileName(), inScribe->GetFieldCount());

                        inScribe->CopyTrackersTo(outScribe);

                        // Set the world start
                        worldStart.Set(useFieldID, worldStart.GetOutsideFieldID());
                        worldStart.SetBattleFieldFromCurrent();
                        worldStart.SetPlayerPartyFromCurrent();
                        outScribe.SetWorldStart(worldStart);

                        // Output the fields
                        for (auto& loop : fieldIDs)
                        {
                            if (prevField && loop == prevField->GetID())
                                outScribe.Save(*prevField);
                            else if (loop == newField->GetID())
                                outScribe.Save(*newField);
                            else
                                outScribe.Save(loop, inScribe->GetAsBuffer(loop));
                        }
                    }
                    // OutScribe has been destroyed
                    inScribe.reset();
                    // InScribe has been destroyed

                    FilePath newPath(StasisScribeOut::MakePathFromName(autosaveName));
                    if (!Environment::GetFileSystem()->RemoveFile(newPath))
                    {
                        Logger::Log("Removing a stasis from the temporary directory has encountered an error.",
                            Logger::Type::ERROR_SEVERE,
                            Logger::NameValueVector{ NameValuePair("Stasis Name", String(autosaveName)), NameValuePair("File Path", newPath.GetValue()) });
                    }

                    if (!Environment::GetFileSystem()->RelocateFile(tempPath, newPath))
                    {
                        Logger::Log("Renaming a stasis has encountered an error.",
                            Logger::Type::ERROR_SEVERE,
                            Logger::NameValueVector{ NameValuePair("Stasis Name", String(autosaveName)), NameValuePair("File Path", tempPath.GetValue()), NameValuePair("Requested File Path", newPath.GetValue()) });
                    }

                    // Set the stasis name to load from
                    stasisName.Set(autosaveName);
                }

                worldStart.Use();

                // Execute after-set events
                eventFieldSet(*newField);

                oldField.reset();
            }
        }

        if (destination.IsValid())
        {
            // Finish handling destination
            auto avatarSystem = field->objectManager.FindSystem<Ent::nEntityAvatarSystem>();
            auto entityPositionSystem = field->objectManager.FindSystem<Ent::nEntityPositionSystem>();
            entityPositionSystem->MoveEntityInstant(avatarSystem->Avatar(), destination->pos);
            entityPositionSystem->SetDirection(avatarSystem->Avatar(), destination->dir);

            destination.Reset();
        }

        change = false;
        useWorldStart = false;
    }

    void WorldManager::Clear(bool destroyField)
    {
        change = false;
        requestedFieldID = 0;

        if (destroyField)
            field.reset();

        destination.Set();
        stasisName.Set();
        useWorldStart = false;
    }

    bool WorldManager::Request(FieldID id)
    {
        if (field.get() && field->GetID() == id)
            return false;

        if (!HasField(id))
            return false;

        requestedFieldID = id;
        change = true;
        return true;
    }

    bool WorldManager::Request(const FieldDestination &request)
    {
        if (field.get() && field->GetID() == request.id)
            return false;

        if (!HasField(request.id))
            return false;

        destination.Set(request);
        change = true;
        return true;
    }

    Field* WorldManager::GetTransitionField()
    {
        return oldField.get();
    }

    bool WorldManager::IsTransitioning()
    {
        return oldField.get() != nullptr;
    }

    void WorldManager::StartNew()
    {
        change = true;
        useWorldStart = true;
    }

    void WorldManager::UseWorld(const FilePath &path)
    {
        worldPath = path;
        StartNew();
    }

    void WorldManager::UseWorld(const FileName &name)
    {
        worldPath = worldFolder + name.GetValue();
        StartNew();
    }

    void WorldManager::UseStasis(const FileName &name)
    {
        stasisName.Set(name);
        StartNew();
    }

    void WorldManager::Autosave()
    {
        if (field)
            Autosave(field->GetID());
    }

    void WorldManager::SaveStasis(const FileName &name)
    {
        if (field)
            SaveStasis(field->GetID(), name);
    }

    const FilePath& WorldManager::GetWorldPath()
    {
        return worldPath;
    }

    Field* WorldManager::GetCurrentField()
    {
        return field.get();
    }

    void WorldManager::OnFocusLost()
    {
        auto soundSystem = field->objectManager.FindSystem<SoundSystem>();
        soundSystem->PauseAll();
    }

    void WorldManager::OnFocusRegain()
    {
        auto soundSystem = field->objectManager.FindSystem<SoundSystem>();
        soundSystem->ResumeAll();
    }

    bool WorldManager::HasField(FieldID id)
    {
        if (fieldIDs.find(id) == fieldIDs.end())
        {
            Logger::Log("A requested field does not exist.",
                Logger::Type::ERROR_MODERATE,
                Logger::NameValueVector{ NameValuePair("Field ID", ToString(id)) });
            return false;
        }

        return true;
    }

    void WorldManager::Autosave(FieldID worldStartFieldID)
    {
        SaveStasis(worldStartFieldID, autosaveName);
    }

    void WorldManager::SaveStasis(FieldID worldStartFieldID, const FileName &name)
    {
        Field *currentField = field.get();
        if (stasisName.IsValid() && stasisName == name)
        {
            // If the stasis name is valid and the same as what we're saving as, that means we're loading from an existing stasis
            // So we must use the old stasis, and just overwrite this field
            // We use two scribes - one to load in from the old stasis, and the other to output at the same time
            // We'll put the new stasis with a different name as to not overwrite as we read, and then rename
            auto &tempPath = Environment::GetFileSystem()->GetTempDirectoryPath();
            tempPath.SetFileName(name);
            tempPath.SetExtension("temporary");

            // This is needed for the scribes to deconstruct themselves
            {
                StasisScribeOut outScribe(tempPath, worldPath.GetFileName(), 0, StasisScribeOut::OpenMode::NONE);

                StasisScribeIn inScribe(name, worldPath);
                inScribe.Load(StasisScribeIn::LOAD_FIELD_PLACEHOLDERS);
                std::vector<FieldID> ids;
                inScribe.GetIDs(ids);

                outScribe.OverwriteFieldCount(inScribe.GetFieldCount());
                inScribe.CopyTrackersTo(outScribe);

                // Set world start
                {
                    StasisWorldStart worldStart;
                    worldStart.SetFromCurrent();
                    worldStart.Set(worldStartFieldID, worldStart.GetOutsideFieldID());
                    outScribe.SetWorldStart(worldStart);
                }

                // Now, start transferring over the fields
                for (auto& loop : ids)
                {
                    if (loop == currentField->GetID())
                        outScribe.Save(*currentField);
                    else
                        outScribe.Save(loop, inScribe.GetAsBuffer(loop));
                }
            }

            FilePath renamePath(StasisScribeOut::MakePathFromName(name));
            std::remove(renamePath.c_str());
            // Finish up renaming it
            rename(tempPath, renamePath.c_str());
        }
        else
        {
            // If the stasis name is invalid or not the same as the one we're using, that means we're loading from a world
            // So we must use the world and overwrite this field
            // We use two scribes - one to load in from the world, and the other to output at the same time
            StasisScribeOut outScribe(name, worldPath.GetFileName(), 0);

            WorldScribeIn inScribe(worldPath);
            if (currentField)
                inScribe.Load(WorldScribeIn::LOAD_FIELD_PLACEHOLDERS);
            else
                inScribe.Load();

            std::vector<FieldID> ids;
            inScribe.GetIDs(ids);

            outScribe.OverwriteFieldCount(ids.size());
            inScribe.CopyTrackersTo(outScribe);
            // Set the world start for this stasis
            {
                FieldID useID = (currentField) ? currentField->GetID() : inScribe.GetWorldStart().GetFieldID();
                outScribe.SetWorldStart(StasisWorldStart(useID));
            }

            // Now, start transferring over the fields
            for (auto& loop : ids)
            {
                if (currentField && loop == currentField->GetID())
                    outScribe.Save(*currentField);
                else
                    outScribe.Save(loop, inScribe.GetAsBuffer(loop));
            }
        }

        stasisName.Set(name);
    }
}