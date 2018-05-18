
#include "WorldScribe.h"

#include "Field.h"
#include "WorldManager.h"
#include "Environment.h"

// Asset registries
#include "ImageRegistry.h"
#include "ShaderRegistry.h"
#include "MaterialRegistry.h"
#include "AudioRegistry.h"
#include "ScriptRegistry.h"

// Game data standards
#include "ResourceAttribute.h"
#include "StatAttribute.h"

// Game data contextx
#include "Element.h"

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

        // Save asset registries
        basicScribe.Save(AssetRegistry<ImageAsset>::Instance());
        basicScribe.Save(AssetRegistry<ShaderAsset>::Instance());
        basicScribe.Save(AssetRegistry<Material>::Instance());
        basicScribe.Save(AssetRegistry<AudioAsset>::Instance());
        basicScribe.Save(AssetRegistry<ScriptModuleBase>::Instance());

        // Save game data standards
        DataStandard<ResourceAttributeTable>::Serialize(basicScribe);
        DataStandard<StatAttributeTable>::Serialize(basicScribe);
        GlobalContext<Element>::Serialize(basicScribe);

        // Save registries
        basicScribe.Save(Registry<Ability>::Instance());
        basicScribe.Save(Registry<StatusEffect>::Instance());
        basicScribe.Save(Registry<CharacterClass>::Instance());
        basicScribe.Save(Registry<Spell>::Instance());
        basicScribe.Save(Registry<Item>::Instance());
        basicScribe.Save(Registry<ItemRecipe>::Instance());
        basicScribe.Save(Registry<Quest>::Instance());

        registrySizeSaver.SaveNothing();

        // Save WorldStart
        basicScribe.Save(worldStart);

        inscription::ContainerSize count(fieldSavers.size());
        basicScribe.Save(count);

        for (auto &loop : fieldSavers)
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

    inscription::Scribe& WorldScribeOut::GetBasicScribe()
    {
        return basicScribe;
    }

    const inscription::Scribe& WorldScribeOut::GetBasicScribe() const
    {
        return basicScribe;
    }

    WorldScribeOut::WorldScribeOut(const FilePath &filePath, inscription::ContainerSize::ValueT fieldCount, OpenMode openMode) : filePath(GetForcedFilePath(filePath, openMode)), basicScribe(GetForcedFilePath(filePath, openMode).GetValue(), "ATMOS GAIA", 1), curSaver(fieldSavers.end()), hasOutputHeader(false)
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

    void WorldScribeOut::OverwriteFieldCount(inscription::ContainerSize::ValueT set)
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

        AssetPackage::Load(filePath);

        // Load asset registries
        basicScribe.Load(AssetRegistry<ImageAsset>::Instance());
        basicScribe.Load(AssetRegistry<ShaderAsset>::Instance());
        basicScribe.Load(AssetRegistry<Material>::Instance());
        basicScribe.Load(AssetRegistry<AudioAsset>::Instance());
        basicScribe.Load(AssetRegistry<ScriptModuleBase>::Instance());

        // Save game data standards
        DataStandard<ResourceAttributeTable>::Serialize(basicScribe);
        DataStandard<StatAttributeTable>::Serialize(basicScribe);
        GlobalContext<Element>::Serialize(basicScribe);

        // Save registries
        basicScribe.Load(Registry<Ability>::Instance());
        basicScribe.Load(Registry<StatusEffect>::Instance());
        basicScribe.Load(Registry<CharacterClass>::Instance());
        basicScribe.Load(Registry<Spell>::Instance());
        basicScribe.Load(Registry<Item>::Instance());
        basicScribe.Load(Registry<ItemRecipe>::Instance());
        basicScribe.Load(Registry<Quest>::Instance());
    }

    void WorldScribeIn::LoadAfterGlobal()
    {
        // Load WorldStart
        basicScribe.Load(worldStart);

        // Retrieve field handles
        inscription::ContainerSize size;
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
        // Setup the world manager to temporarily use the new field and find the field handle
        WorldManager::State previousState = WorldManager::TemporaryUse(&fill);

        // Load the field while setting up a section to clear immediately after
        basicScribe.StartTrackingSection();
        handle->second.LoadObject(fill);
        basicScribe.StopTrackingSection(true);

        // Now, release the field from the world manager
        previousState.Restore();
    }

    FilePath WorldScribeIn::GetForcedFilePath(const FilePath &filePath, OpenMode openMode)
    {
        FilePath ret(filePath);
        if (openMode == OpenMode::FORCE_EXTENSION)
            ret.SetExtension(worldExtension);
        return ret;
    }

    inscription::Scribe& WorldScribeIn::GetBasicScribe()
    {
        return basicScribe;
    }

    const inscription::Scribe& WorldScribeIn::GetBasicScribe() const
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

        Field out(id);
        FillField(out, found);
        return Ret(std::move(out));
    }

    Field* WorldScribeIn::GetAsHeap(FieldID id)
    {
        auto found = fieldHandles.find(id);
        if (found == fieldHandles.end())
            return nullptr;

        auto out = new Field(id);
        FillField(*out, found);
        return out;
    }

    inscription::Buffer WorldScribeIn::GetAsBuffer(FieldID id)
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
        for (auto &loop : fieldHandles)
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