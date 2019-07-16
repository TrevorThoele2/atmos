#include "WorldManager.h"

#include "FileSystem.h"
#include "SoundSystem.h"
#include "DebugStatisticsSystem.h"
#include "LoggingSystem.h"

#include "EntityAvatarSystem.h"
#include "EntityPositionSystem.h"

#include "StringUtility.h"

#include "OutputWorldArchive.h"
#include "InputWorldArchive.h"
#include "OutputStasisArchive.h"
#include "InputStasisArchive.h"

namespace Atmos
{
    const char* autosaveName = "Autosave.stasis";

    WorldManager::WorldManager(ObjectManager& globalObjectManager, ObjectManagerFactory& objectManagerFactory) :
        globalObjectManager(&globalObjectManager), objectManagerFactory(&objectManagerFactory)
    {}

    void WorldManager::Initialize()
    {
        auto debugStatistics = globalObjectManager->FindSystem<DebugStatisticsSystem>();
        debugStatistics->gamePage.fieldID.retrievalFunction = [this]() -> String
        {
            auto currentField = CurrentField();
            if (currentField)
                return ToString(currentField->id);
            else
                return String("None");
        };

        FindFileSystem()->Get()->MakeDirectory(StasisFolderFilePath());
    }

    void WorldManager::Work()
    {
        field->objectManager.Work();
    }

    void WorldManager::LockIn()
    {
        if (!change)
            return;

        auto fileSystem = FindFileSystem()->Get();
        auto loggingSystem = FindLoggingSystem();

        FieldID useFieldID = requestedFieldID;
        if (destination.IsValid())
            useFieldID = destination->id;

        auto prevField = field.get();
        bool hadPreviousField = prevField != nullptr;
        if (!prevField || useFieldID != prevField->id || useWorldStart)
        {
            bool isWorld = !stasisName.IsValid();

            std::unique_ptr<InputArchiveBase> inputArchive;
            String noLoadError;
            String noLoadErrorFileParamName;
            String noLoadErrorFileParam;
            if (isWorld)
            {
                inputArchive.reset(new InputWorldArchive(worldPath, *globalObjectManager));
                noLoadError = "The world file was unloadable.";
                noLoadErrorFileParamName = "World File Path";
                noLoadErrorFileParam = worldPath.GetValue();
            }
            else
            {
                inputArchive.reset(new InputStasisArchive(CreateStasisFilePath(stasisName.Get()), worldPath, *globalObjectManager));
                noLoadError = "The stasis file's world is not the same as the provided world.";
                noLoadErrorFileParamName = "Stasis File Name";
                noLoadErrorFileParam = stasisName.Get().GetValue();
            }

            if (!inputArchive->WillLoad())
            {
                FindLoggingSystem()->Log(std::move(noLoadError),
                    LogType::ERROR_SEVERE,
                    LogNameValueVector{ NameValuePair(noLoadErrorFileParamName, noLoadErrorFileParam) });
            }
            else
            {
                // This will load

                // We are going to load simultaneously while saving
                if (prevField)
                    inputArchive->Load(InputArchiveBase::LOAD_FIELD_PLACEHOLDERS);
                else
                    inputArchive->Load();

                // Retrieve the world start and check if to use it
                if (useWorldStart)
                    useFieldID = inputArchive->worldStart.Get().fieldID;

                // Retrieve the new field IDs
                {
                    fieldIDs.clear();
                    auto idVector = inputArchive->AllFieldIDs();
                    for (auto& loop : idVector)
                        fieldIDs.emplace(loop);
                }

                // Finalize the old field
                eventFinalizeField(prevField);

                // Create the new field
                auto newField = inputArchive->ExtractFieldAsHeap(useFieldID);
                ATMOS_ASSERT_MESSAGE(newField, "The newly created field must exist.");

                oldField.reset(field.release());
                field = std::move(newField);

                // Execute before-set events
                eventBeforeFieldSet();

                WorldStart worldStart;

                // Save the new stasis
                {
                    auto& tempPath = fileSystem->TemporaryDirectoryPath();
                    tempPath.SetFileName(autosaveName);
                    tempPath.SetExtension("temporary");

                    // Create the out scribe (always outputting into a stasis)
                    {
                        OutputStasisArchive outputScribe(tempPath, worldPath.GetFileName(), inputArchive->FieldCount());

                        inputArchive->CopyTrackersTo(outputScribe);

                        // Set the world start
                        worldStart.fieldID = useFieldID;
                        outputScribe.worldStart = worldStart;

                        // Output the fields
                        for (auto& loop : fieldIDs)
                        {
                            if (prevField && loop == prevField->id)
                                outputScribe.Save(*prevField);
                            else if (loop == newField->id)
                                outputScribe.Save(*newField);
                            else
                                outputScribe.Save(loop, inputArchive->ExtractFieldAsBuffer(loop));
                        }
                    }
                    // OutScribe has been destroyed
                    inputArchive.reset();
                    // InScribe has been destroyed

                    FilePath newPath(CreateStasisFilePath(autosaveName));
                    if (!fileSystem->RemoveFile(newPath))
                    {
                        loggingSystem->Log("Removing a stasis file from the temporary directory has encountered an error.",
                            LogType::ERROR_SEVERE,
                            LogNameValueVector{
                                NameValuePair("Stasis Name", String(autosaveName)),
                                NameValuePair("File Path", newPath.GetValue()) });
                    }

                    if (!fileSystem->RelocateFile(tempPath, newPath))
                    {
                        loggingSystem->Log("Renaming a stasis file has encountered an error.",
                            LogType::ERROR_SEVERE,
                            LogNameValueVector{
                                NameValuePair("Stasis Name", String(autosaveName)),
                                NameValuePair("File Path", tempPath.GetValue()),
                                NameValuePair("Requested File Path", newPath.GetValue()) });
                    }

                    // Set the stasis name to load from
                    stasisName.Set(autosaveName);
                }

                // Execute after-set events
                eventFieldSet(*newField);

                oldField.reset();
            }
        }

        if (destination.IsValid())
        {
            // Finish handling destination
            auto avatarSystem = field->objectManager.FindSystem<Entity::AvatarSystem>();
            auto entityPositionSystem = field->objectManager.FindSystem<Entity::PositionSystem>();
            entityPositionSystem->MoveEntityInstant(avatarSystem->Avatar(), destination->position);
            entityPositionSystem->SetDirection(avatarSystem->Avatar(), destination->direction);

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
        if (field.get() && field->id == id)
            return false;

        if (!HasField(id))
            return false;

        requestedFieldID = id;
        change = true;
        return true;
    }

    bool WorldManager::Request(const FieldDestination& request)
    {
        if (field.get() && field->id == request.id)
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

    void WorldManager::UseWorld(const FilePath& path)
    {
        worldPath = path;
        StartNew();
    }

    void WorldManager::UseWorld(const FileName& name)
    {
        worldPath = "Worlds" + name.GetValue();
        StartNew();
    }

    void WorldManager::UseStasis(const FileName& name)
    {
        stasisName.Set(name);
        StartNew();
    }

    void WorldManager::Autosave()
    {
        if (field)
            Autosave(field->id);
    }

    void WorldManager::SaveStasis(const FileName& name)
    {
        if (field)
            SaveStasis(field->id, name);
    }

    const FilePath& WorldManager::WorldPath()
    {
        return worldPath;
    }

    Field* WorldManager::CurrentField()
    {
        return field.get();
    }

    FileSystem* WorldManager::FindFileSystem() const
    {
        return globalObjectManager->FindSystem<FileSystem>();
    }

    LoggingSystem* WorldManager::FindLoggingSystem() const
    {
        return globalObjectManager->FindSystem<LoggingSystem>();
    }

    bool WorldManager::HasField(FieldID id)
    {
        return fieldIDs.find(id) == fieldIDs.end();
    }

    void WorldManager::Autosave(FieldID worldStartFieldID)
    {
        SaveStasis(worldStartFieldID, autosaveName);
    }

    void WorldManager::SaveStasis(FieldID worldStartFieldID, const FileName &name)
    {
        auto fileSystem = globalObjectManager->FindSystem<FileSystem>()->Get();

        Field* currentField = field.get();
        if (stasisName.IsValid() && stasisName == name)
        {
            // If the stasis name is valid and the same as what we're saving as, that means we're loading from an existing stasis
            // So we must use the old stasis, and just overwrite this field
            // We use two scribes - one to load in from the old stasis, and the other to output at the same time
            // We'll put the new stasis with a different name as to not overwrite as we read, and then rename
            auto temporaryPath = fileSystem->TemporaryDirectoryPath();
            temporaryPath.SetFileName(name);
            temporaryPath.SetExtension("temporary");

            // This scope is needed for the scribes to deconstruct themselves
            {
                OutputStasisArchive outputScribe(temporaryPath, worldPath.GetFileName(), 0, OutputStasisArchive::OpenMode::NONE);

                InputStasisArchive inputScribe(CreateStasisFilePath(name), worldPath, *globalObjectManager);
                inputScribe.Load(InputStasisArchive::LOAD_FIELD_PLACEHOLDERS);
                auto fieldIds = inputScribe.AllFieldIDs();

                outputScribe.OverwriteFieldCount(inputScribe.FieldCount());
                inputScribe.CopyTrackersTo(outputScribe);

                // Set world start
                {
                    WorldStart worldStart(worldStartFieldID);
                    outputScribe.worldStart = worldStart;
                }

                // Now, start transferring over the fields
                for (auto& loop : fieldIds)
                {
                    if (loop == currentField->id)
                        outputScribe.Save(*currentField);
                    else
                        outputScribe.Save(loop, inputScribe.ExtractFieldAsBuffer(loop));
                }
            }

            FilePath renamePath(StasisFolderFilePath());
            renamePath.Append(name);
            std::remove(renamePath.c_str());
            // Finish up renaming it
            rename(temporaryPath, renamePath.c_str());
        }
        else
        {
            // If the stasis name is invalid or not the same as the one we're using, that means we're loading from a world
            // So we must use the world and overwrite this field
            // We use two scribes - one to load in from the world, and the other to output at the same time
            OutputStasisArchive outputScribe(CreateStasisFilePath(name), worldPath.GetFileName(), 0);

            InputWorldArchive inputArchive(worldPath, *globalObjectManager);
            if (currentField)
                inputArchive.Load(InputWorldArchive::LOAD_FIELD_PLACEHOLDERS);
            else
                inputArchive.Load();

            auto ids = inputArchive.AllFieldIDs();

            outputScribe.OverwriteFieldCount(ids.size());
            inputArchive.CopyTrackersTo(outputScribe);
            // Set the world start for this stasis
            {
                FieldID useID = (currentField) ? currentField->id.Get() : inputArchive.worldStart.Get().fieldID.Get();
                outputScribe.worldStart = WorldStart(useID);
            }

            // Now, start transferring over the fields
            for (auto& loop : ids)
            {
                if (currentField && loop == currentField->id)
                    outputScribe.Save(*currentField);
                else
                    outputScribe.Save(loop, inputArchive.ExtractFieldAsBuffer(loop));
            }
        }

        stasisName.Set(name);
    }

    FilePath WorldManager::CreateWorldFilePath(const FileName& fileName) const
    {
        return WorldFolderFilePath().Append(fileName);
    }

    FilePath WorldManager::CreateStasisFilePath(const FileName& fileName) const
    {
        return StasisFolderFilePath().Append(fileName);
    }

    FilePath WorldManager::WorldFolderFilePath() const
    {
        return FindFileSystem()->Get()->ExePath().Append("Worlds").AppendSeparator();
    }

    FilePath WorldManager::StasisFolderFilePath() const
    {
        return FindFileSystem()->Get()->ExePath().Append("Saves").AppendSeparator();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::WorldManager, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {}
}