
#include "StasisScribe.h"

#include "WorldScribe.h"
#include "WorldManager.h"
#include "AssetPackage.h"
#include "Environment.h"

// Registries
#include "ImageRegistry.h"
#include "ShaderRegistry.h"
#include "AudioRegistry.h"
#include "Script.h"

#include "StatusEffect.h"
#include "CharacterClass.h"
#include "Spell.h"
#include "Item.h"

namespace Atmos
{
    const char *stasisExtension = "stasis";

    void StasisScribeOut::FieldSaver::OnBeforeObjectSave()
    {
        auto tell = scribe.TellStream();
        positionStart = static_cast<SizeT>(scribe.TellStream());
    }

    void StasisScribeOut::FieldSaver::OnAfterObjectSave()
    {
        size = static_cast<SizeT>(scribe.TellStream()) - positionStart;
    }

    void StasisScribeOut::FieldSaver::SavePlaceholderExtra()
    {
        scribe.WriteNumeric(fieldID);
        scribe.WriteNumeric(size);
    }

    void StasisScribeOut::FieldSaver::SaveExtra()
    {
        scribe.WriteNumeric(fieldID);
        scribe.WriteNumeric(size);
    }

    StasisScribeOut::FieldSaver::FieldSaver(ScribeT &scribe) : SkipSaver(scribe), fieldID(0), size(0)
    {}

    void StasisScribeOut::OutputHeader()
    {
        if (!hasOutputHeader)
        {
            // Save base world name
            basicScribe.Save(worldFileName);

            // Save WorldStart
            basicScribe.Save(worldStart);

            // Save field placeholders
            inscription::ContainerSize newCount(fieldSavers.size());
            basicScribe.Save(newCount);

            for (auto &loop : fieldSavers)
                loop.SavePlaceholder();
            curSaver = fieldSavers.begin();

            hasOutputHeader = true;
        }
    }

    inscription::Scribe& StasisScribeOut::GetBasicScribe()
    {
        return basicScribe;
    }

    const inscription::Scribe& StasisScribeOut::GetBasicScribe() const
    {
        return basicScribe;
    }

    StasisScribeOut::StasisScribeOut(const FileName &fileName, const FileName &worldFileName, inscription::ContainerSize::ValueT fieldCount, OpenMode openMode) : fileName(fileName), worldFileName(worldFileName), basicScribe(MakePathFromName(fileName), "ATMOS GAIA STASIS", 1), curSaver(fieldSavers.end()), hasOutputHeader(false)
    {
        if(openMode == OpenMode::FORCE_EXTENSION)
            this->fileName.SetExtension(stasisExtension);

        fieldSavers.resize(fieldCount, FieldSaver(basicScribe));
    }

    StasisScribeOut::StasisScribeOut(const FilePath &filePath, const FileName &worldFileName, inscription::ContainerSize::ValueT fieldCount, OpenMode openMode) : fileName(filePath), worldFileName(worldFileName), basicScribe(filePath, "ATMOS GAIA STASIS", 1), curSaver(fieldSavers.end()), hasOutputHeader(false)
    {
        if (openMode == OpenMode::FORCE_EXTENSION)
            this->fileName.SetExtension(stasisExtension);

        fieldSavers.resize(fieldCount, FieldSaver(basicScribe));
    }

    void StasisScribeOut::Save(Field &save)
    {
        OutputHeader();

        // Save the field ID
        curSaver->fieldID = save.GetID();

        // Save the field itself while clearing out the tracking entries made
        basicScribe.StartTrackingSection();
        curSaver->SaveObject(save);
        basicScribe.StopTrackingSection(true);

        ++curSaver;
    }

    void StasisScribeOut::Save(FieldID id, const inscription::Buffer &buffer)
    {
        OutputHeader();

        // Save the next field
        curSaver->fieldID = id;
        curSaver->SaveBuffer(buffer);
        ++curSaver;
    }

    void StasisScribeOut::OverwriteFieldCount(inscription::ContainerSize::ValueT set)
    {
        fieldSavers.resize(set, FieldSaver(basicScribe));
    }

    void StasisScribeOut::SetWorldStart(const StasisWorldStart &set)
    {
        worldStart = set;
    }

    void StasisScribeOut::SetWorldStartFromCurrent()
    {
        worldStart.SetFromCurrent();
    }

    const FileName& StasisScribeOut::GetFileName() const
    {
        return fileName;
    }

    FilePath StasisScribeOut::MakePathFromName(const FileName &fileName)
    {
        return FilePath(GetPath().GetValue() + fileName.GetValue());
    }

    FilePath StasisScribeOut::GetPath()
    {
        return FilePath(Environment::GetFileSystem()->GetExePath().Append("Saves\\"));
    }

    void StasisScribeIn::FieldHandle::LoadExtra()
    {
        scribe.ReadNumeric(fieldID);
        scribe.ReadNumeric(size);
    }

    StasisScribeIn::FieldHandle::FieldHandle(ScribeT &scribe) : SkipHandle(scribe), size(0)
    {}

    bool StasisScribeIn::LoadBeforeGlobal()
    {
        FileName worldFileName;

        // Load base world name
        basicScribe.Load(worldFileName);

        return worldFileName == worldFilePath.GetFileName();
    }

    void StasisScribeIn::LoadGlobal()
    {
        WorldScribeIn world(worldFilePath);
        world.Load(WorldScribeIn::LOAD_GLOBAL);

        world.MoveTrackersTo(basicScribe);
    }

    void StasisScribeIn::LoadAfterGlobal()
    {
        // Load WorldStart
        basicScribe.Load(worldStart);

        // Load field handles
        inscription::ContainerSize size;
        basicScribe.Load(size);
        while (size-- > 0)
        {
            FieldHandle handle(basicScribe);
            handle.LoadPosition();
            fieldHandles.emplace(handle.fieldID, handle);
        }
    }

    void StasisScribeIn::FillField(Field &fill, FieldHandles::iterator handle)
    {
        // Setup the world manager to temporarily use the new field and find the field handle
        WorldManager::State previousState = WorldManager::TemporaryUse(&fill);

        // Load the field while setting up a section to clear immediately after
        basicScribe.StartTrackingSection();
        handle->second.LoadObject(fill);
        basicScribe.StopTrackingSection(true);

        // Now, release the field from the world manager
        previousState.Restore();
    }

    inscription::Scribe& StasisScribeIn::GetBasicScribe()
    {
        return basicScribe;
    }

    const inscription::Scribe& StasisScribeIn::GetBasicScribe() const
    {
        return basicScribe;
    }

    StasisScribeIn::StasisScribeIn(const FileName &fileName, const FilePath &worldFilePath, OpenMode openMode) : fileName(fileName), worldFilePath(worldFilePath), basicScribe(MakePathFromName(fileName), "ATMOS GAIA STASIS")
    {
        if(openMode == OpenMode::FORCE_EXTENSION)
            this->fileName.SetExtension(stasisExtension);
    }

    StasisScribeIn::StasisScribeIn(const FilePath &filePath, const FilePath &worldPath, OpenMode openMode) : fileName(filePath), worldFilePath(worldFilePath), basicScribe(filePath, "ATMOS GAIA STASIS")
    {
        if (openMode == OpenMode::FORCE_EXTENSION)
            this->fileName.SetExtension(stasisExtension);
    }

    bool StasisScribeIn::Load(LoadType load)
    {
        basicScribe.Reset();
        if (!LoadBeforeGlobal())
            return false;

        switch (load)
        {
        case LoadType::LOAD_ALL:
            LoadGlobal();
            LoadAfterGlobal();
            break;
        case LoadType::LOAD_GLOBAL:
            LoadGlobal();
            break;
        case LoadType::LOAD_FIELD_PLACEHOLDERS:
            LoadAfterGlobal();
            break;
        }

        return true;
    }

    bool StasisScribeIn::WillLoad()
    {
        basicScribe.Reset();
        return LoadBeforeGlobal();
    }

    Optional<Field> StasisScribeIn::Get(FieldID fieldID)
    {
        typedef Optional<Field> Ret;

        auto found = fieldHandles.find(fieldID);
        if (found == fieldHandles.end())
            return Ret();

        Field out(fieldID);
        FillField(out, found);
        return Ret(std::move(out));
    }

    Field* StasisScribeIn::GetAsHeap(FieldID id)
    {
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return nullptr;

        auto out = new Field(id);
        FillField(*out, found);
        return out;
    }

    inscription::Buffer StasisScribeIn::GetAsBuffer(FieldID id)
    {
        auto found = fieldHandles.find(id);
        return found->second.LoadBuffer(found->second.size);
    }

    StasisWorldStart& StasisScribeIn::GetWorldStart()
    {
        return worldStart;
    }

    void StasisScribeIn::GetIDs(std::vector<FieldID> &ids) const
    {
        ids.clear();
        for (auto &loop : fieldHandles)
            ids.push_back(loop.first);
    }

    bool StasisScribeIn::HasField(FieldID fieldID) const
    {
        return fieldHandles.find(fieldID) != fieldHandles.end();
    }

    size_t StasisScribeIn::GetFieldCount() const
    {
        return fieldHandles.size();
    }

    const FileName& StasisScribeIn::GetFileName() const
    {
        return fileName;
    }

    FilePath StasisScribeIn::MakePathFromName(const FileName &fileName)
    {
        return StasisScribeOut::MakePathFromName(fileName);
    }

    FilePath StasisScribeIn::GetPath()
    {
        return StasisScribeOut::GetPath();
    }
}