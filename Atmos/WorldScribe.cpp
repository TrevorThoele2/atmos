
#include "WorldScribe.h"

#include "Field.h"
#include "WorldManager.h"
#include "Environment.h"
#include "GameEnvironment.h"

// Asset systems
#include "ImageSystem.h"
#include "ShaderSystem.h"
#include "MaterialSystem.h"
#include "AudioSystem.h"
#include "ScriptSystem.h"

// Game data standards
#include "ResourceAttribute.h"
#include "StatAttribute.h"

// Game object registries
#include "Ability.h"
#include "StatusEffect.h"
#include "CharacterClass.h"
#include "Spell.h"
#include "Item.h"
#include "ItemRecipe.h"
#include "Quest.h"

#include "AssetPackage.h"

namespace Atmos
{
    const char *worldExtension = "gaia";

    void WorldScribeOut::FieldSaver::OnBeforeObjectSave()
    {
        positionStart = static_cast<SizeT>(scribe.TellStream());
    }

    void WorldScribeOut::FieldSaver::OnAfterObjectSave()
    {
        size = static_cast<SizeT>(scribe.TellStream() - positionStart);
    }

    void WorldScribeOut::FieldSaver::SavePlaceholderExtra()
    {
        scribe.WriteNumeric(fieldID);
        scribe.WriteNumeric(size);
    }

    void WorldScribeOut::FieldSaver::SaveExtra()
    {
        scribe.WriteNumeric(fieldID);
        scribe.WriteNumeric(size);
    }

    WorldScribeOut::FieldSaver::FieldSaver(ScribeT &scribe) : SkipSaver(scribe)
    {}

    void WorldScribeOut::OutputHeader()
    {
        // Save field placeholders
        if (hasOutputHeader)
            return;

        SkipSaver<ScribeT> registrySizeSaver(basicScribe);
        registrySizeSaver.SavePlaceholder();

        // Save object type names
        objectTypeNameSerializer.AddAll(GameEnvironment::GetLocalObjectTypeGraph().AllDescriptions());
        objectTypeNameSerializer.AddAll(GameEnvironment::GetGlobalObjectTypeGraph().AllDescriptions());
        objectTypeNameSerializer.SaveAll(basicScribe);

        // Save global object manager
        basicScribe.Save(GameEnvironment::GetGlobalObjectManager());

        // Save game data standards
        DataStandard<ResourceAttributeTable>::Serialize(basicScribe);
        DataStandard<StatAttributeTable>::Serialize(basicScribe);

        registrySizeSaver.SaveNothing();

        // Save WorldStart
        basicScribe.Save(worldStart);

        ::Inscription::ContainerSize count(fieldSavers.size());
        basicScribe.Save(count);

        for (auto& loop : fieldSavers)
            loop.SavePlaceholder();
        curSaver = fieldSavers.begin();
        hasOutputHeader = true;
    }

    FilePath WorldScribeOut::GetForcedFilePath(const FilePath &filePath, OpenMode openMode)
    {
        FilePath ret(filePath);
        if (openMode == OpenMode::FORCE_EXTENSION)
            ret.SetExtension(worldExtension);
        return ret;
    }

    ::Inscription::Scribe& WorldScribeOut::GetBasicScribe()
    {
        return basicScribe;
    }

    const ::Inscription::Scribe& WorldScribeOut::GetBasicScribe() const
    {
        return basicScribe;
    }

    WorldScribeOut::WorldScribeOut(const FilePath &filePath, ::Inscription::ContainerSize::ValueT fieldCount, OpenMode openMode) : filePath(GetForcedFilePath(filePath, openMode)), basicScribe(GetForcedFilePath(filePath, openMode).GetValue(), "ATMOS GAIA", GetCurrentVersion()), curSaver(fieldSavers.end()), hasOutputHeader(false)
    {
        fieldSavers.resize(fieldCount, FieldSaver(basicScribe));
    }

    void WorldScribeOut::Save(Field &field)
    {
        OutputHeader();

        // Save the field ID
        curSaver->fieldID = field.GetID();

        // Save the field itself while clearing out the tracking entries made
        basicScribe.StartTrackingSection();
        curSaver->SaveObject(field);
        basicScribe.StopTrackingSection(true);

        ++curSaver;
    }

    void WorldScribeOut::OverwriteFieldCount(::Inscription::ContainerSize::ValueT set)
    {
        fieldSavers.resize(set, FieldSaver(basicScribe));
    }

    void WorldScribeOut::SetWorldStart(const WorldStart &set)
    {
        worldStart = set;
    }

    const FilePath& WorldScribeOut::GetFilePath() const
    {
        return filePath;
    }

    ::Inscription::Version WorldScribeOut::GetCurrentVersion()
    {
        return 1;
    }

    void WorldScribeIn::FieldHandle::LoadExtra()
    {
        scribe.ReadNumeric(fieldID);
        scribe.ReadNumeric(size);
    }

    WorldScribeIn::FieldHandle::FieldHandle(ScribeT &scribe) : SkipHandle(scribe), size(0)
    {}

    void WorldScribeIn::LoadGlobal(RegistrySkipHandle *registrySkipper)
    {
        if (registrySkipper)
        {
            registrySkipper->LoadPosition();
            return;
        }
        else
        {
            RegistrySkipHandle notUsed(basicScribe);
            notUsed.LoadPosition();
        }

        GameEnvironment::GetAssetPackage().Load(filePath);

        // Load object type names
        objectTypeNameSerializer.LoadAll(basicScribe);

        // Load global object manager
        basicScribe.Load(GameEnvironment::GetGlobalObjectManager());

        // Save game data standards
        DataStandard<ResourceAttributeTable>::Serialize(basicScribe);
        DataStandard<StatAttributeTable>::Serialize(basicScribe);
    }

    void WorldScribeIn::LoadAfterGlobal()
    {
        // Load WorldStart
        basicScribe.Load(worldStart);

        // Retrieve field handles
        ::Inscription::ContainerSize size;
        basicScribe.Load(size);
        while (size-- > 0)
        {
            FieldHandle handle(basicScribe);
            handle.LoadPosition();
            fieldHandles.emplace(handle.fieldID, handle);
        }
    }

    void WorldScribeIn::FillField(Field &fill, FieldHandles::iterator handle)
    {
        // Load the field while setting up a section to clear immediately after
        basicScribe.StartTrackingSection();
        handle->second.LoadObject(fill);
        basicScribe.StopTrackingSection(true);
    }

    FilePath WorldScribeIn::GetForcedFilePath(const FilePath &filePath, OpenMode openMode)
    {
        FilePath ret(filePath);
        if (openMode == OpenMode::FORCE_EXTENSION)
            ret.SetExtension(worldExtension);
        return ret;
    }

    ::Inscription::Scribe& WorldScribeIn::GetBasicScribe()
    {
        return basicScribe;
    }

    const ::Inscription::Scribe& WorldScribeIn::GetBasicScribe() const
    {
        return basicScribe;
    }

    WorldScribeIn::WorldScribeIn(const FileName &fileName, OpenMode openMode) : WorldScribeIn(FilePath(Environment::GetFileSystem()->GetExePath().Append(String(worldFolder) + '\\' + fileName.GetValue()).GetValue()), openMode)
    {}

    WorldScribeIn::WorldScribeIn(const FilePath &filePath, OpenMode openMode) : filePath(GetForcedFilePath(filePath, openMode)), basicScribe(GetForcedFilePath(filePath, openMode), "ATMOS GAIA")
    {}

    bool WorldScribeIn::Load(LoadType load)
    {
        basicScribe.Reset();

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
            RegistrySkipHandle skipper(basicScribe);

            LoadGlobal(&skipper);
            basicScribe.SeekStream(skipper.GetSkipPosition());
            LoadAfterGlobal();

            break;
        }

        return true;
    }

    bool WorldScribeIn::WillLoad()
    {
        return true;
    }

    Optional<Field> WorldScribeIn::Get(FieldID id)
    {
        typedef Optional<Field> Ret;
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return Ret();

        Field out(id, GameEnvironment::MakeObjectManager());
        FillField(out, found);
        return Ret(std::move(out));
    }

    std::unique_ptr<Field> WorldScribeIn::GetAsHeap(FieldID id)
    {
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return nullptr;

        auto out = std::make_unique<Field>(id, GameEnvironment::MakeObjectManager());
        FillField(*out, found);
        return out;
    }

    ::Inscription::Buffer WorldScribeIn::GetAsBuffer(FieldID id)
    {
        auto found = fieldHandles.find(id);
        return found->second.LoadBuffer(found->second.size);
    }

    WorldStart& WorldScribeIn::GetWorldStart()
    {
        return worldStart;
    }

    void WorldScribeIn::GetIDs(std::vector<FieldID> &ids) const
    {
        ids.clear();
        for (auto& loop : fieldHandles)
            ids.push_back(loop.first);
    }

    bool WorldScribeIn::HasField(FieldID fieldID) const
    {
        return fieldHandles.find(fieldID) != fieldHandles.end();
    }

    size_t WorldScribeIn::GetFieldCount() const
    {
        return fieldHandles.size();
    }

    const FilePath& WorldScribeIn::GetFilePath() const
    {
        return filePath;
    }
}