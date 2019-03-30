
#include "GameEnvironment.h"

#include "Environment.h"
#include "WorldManager.h"
#include "StateManager.h"
#include "FPS.h"
#include "ScriptController.h"
#include "Debug.h"
#include "Render.h"
#include "SpeechController.h"
#include "Shop.h"
#include "CurrentField.h"

#include "Sprite.h"

#include "ActionSystem.h"

//////////////////
#include "AudioAsset.h"

#include "Tile.h"
#include "PositionalObject.h"
#include "RenderFragment.h"
#include "Sense.h"
#include "Sprite.h"
#include "nEntity.h"
#include "nEntityComponent.h"
#include "ActionComponent.h"

#include "Ability.h"
#include "CharacterClass.h"
#include "Item.h"
#include "Quest.h"
#include "Spell.h"
#include "StatusEffect.h"

#include "nModulator.h"
#include "nModulatorTrack.h"
#include "nModulatorNode.h"

#include "AudioSystem.h"
#include "ImageSystem.h"
#include "MaterialSystem.h"
#include "ScriptSystem.h"
#include "ShaderSystem.h"
#include "TileSystem.h"
#include "RenderFragmentSystem.h"

#include "AbilitySystem.h"
#include "CharacterClassSystem.h"
#include "ItemSystem.h"
#include "QuestSystem.h"
#include "SpellSystem.h"
#include "StatusEffectSystem.h"

#include "ActionSystem.h"
#include "AvatarSystem.h"
#include "EntityAISystem.h"
#include "nEntityPositionSystem.h"

#include "SoundSystem.h"
#include "MusicSystem.h"

#include "TileFactory.h"
#include "AudioFactory.h"
#include "ImageFactory.h"
#include "MaterialFactory.h"
#include "ScriptFactory.h"
#include "ShaderFactory.h"

#include "AbilityFactory.h"
#include "CharacterClassFactory.h"
#include "ItemFactory.h"
#include "QuestFactory.h"
#include "SpellFactory.h"
#include "StatusEffectFactory.h"

namespace Atmos
{
    NullEvent GameEnvironment::eventFrameStart;
    NullEvent GameEnvironment::eventFrameStop;

    GameEnvironment& GameEnvironment::Instance()
    {
        static GameEnvironment instance;
        return instance;
    }

    void GameEnvironment::Init()
    {
        RegisterAllKnownObjectTypes();
        RegisterAllObjectTypesToSerialization();
        Instance().globalObjectManager = std::move(std::make_unique<ObjectManager>());
        PushAllObjectTypes(Instance().globalTypes, *Instance().globalObjectManager);

        Environment::GetInput()->eventKeys.pressed.Subscribe(&GameEnvironment::OnInputPressed);

        // World
        Instance().worldManager.LockIn();

        // States
        StateManager::Init();

        // Debug
        DebugScreen::Init();
        DebugScreen::Idle().Start();

        return;

        // Modulator maps
        Modulator::LinkGeneratorGroup<nSprite>::Instance();
    }

    void GameEnvironment::Loop()
    {
        while (Environment::StartFrame())
        {
            if (Environment::IsFocused()) // Check for focus on the window 
            {
                // Handle focus regained
                if (Environment::GetFlag(Environment::Flag::FOCUS_LOST))
                    OnFocusRegain();

                // Profiling and FPS handling
                DebugScreen::Idle().Calc();
                eventFrameStart();
                FpsHandler::StartFrame();

                // Input, logic, and rendering
                Environment::GetInput()->Work();
                Work();
                Instance().renderer.Work();

                // Lock in new stuff
                StateManager::LockIn();
                Instance().worldManager.LockIn();

                eventFrameStop();
                // Update debugscreen and start the idle profiler
                DebugScreen::Update();
                DebugScreen::Idle().Start();

                // Handle fps idling
                while (FpsHandler::DoIdle())
                    Environment::Suspend(TimeValue(FixedPoint64(0), TimeValueEpoch::MILLISECONDS));
            }
            else // Window is not in focus
            {
                if (!Environment::GetFlag(Environment::Flag::FOCUS_LOST))
                    OnFocusLost();

                Environment::Suspend(TimeValue(FixedPoint64(1), TimeValueEpoch::MILLISECONDS));
            }
        }
    }

    void GameEnvironment::StopAll()
    {}

    void GameEnvironment::LoadWorld(const FilePath &path)
    {
        Instance().worldManager.UseWorld(path);
    }

    void GameEnvironment::LoadStasis(const FilePath &path)
    {
        Instance().worldManager.UseStasis(path);
    }

    ObjectManager GameEnvironment::MakeObjectManager()
    {
        ObjectManager objectManager;

        PushAllObjectTypes(Instance().localTypes, objectManager);

        objectManager.Initialize();

        return objectManager;
    }

    ObjectManager& GameEnvironment::GetGlobalObjectManager()
    {
        return *Instance().globalObjectManager;
    }

    ObjectTypeGraph GameEnvironment::GetLocalObjectTypeGraph()
    {
        ObjectTypeGraph typeGraph;

        for (auto& loop : Instance().localTypes)
            loop->ToTypeGraph(typeGraph);

        return typeGraph;
    }

    ObjectTypeGraph GameEnvironment::GetGlobalObjectTypeGraph()
    {
        return Instance().globalObjectManager->TypeGraph();
    }

    void GameEnvironment::RegisterAllObjectTypesToSerialization()
    {
        ::Inscription::Scribe::RegisterType<Object>();
        ::Inscription::Scribe::RegisterType<ObjectBatchSourceBase>();
        ::Inscription::Scribe::RegisterType<ObjectManager>();

        ::Inscription::Scribe::RegisterType<nAsset>();
        ::Inscription::Scribe::RegisterType<nFileAsset>();
        ::Inscription::Scribe::RegisterType<AudioAsset>();
        ::Inscription::Scribe::RegisterType<ImageAsset>();
        ::Inscription::Scribe::RegisterType<Material>();
        ::Inscription::Scribe::RegisterType<ShaderAsset>();

        ::Inscription::Scribe::RegisterType<nTile>();

        ::Inscription::Scribe::RegisterType<Ent::nEntity>();
        ::Inscription::Scribe::RegisterType<Ent::nEntityComponent>();
        ::Inscription::Scribe::RegisterType<Ent::nActionComponent>();

        ::Inscription::Scribe::RegisterType<PositionalObject>();
        ::Inscription::Scribe::RegisterType<nRenderFragment>();
        ::Inscription::Scribe::RegisterType<nSense>();
        ::Inscription::Scribe::RegisterType<nSprite>();

        ::Inscription::Scribe::RegisterType<Modulator::Modulator>();
        ::Inscription::Scribe::RegisterType<Modulator::Track>();
        ::Inscription::Scribe::RegisterType<Modulator::Node>();
    }

    WorldManager& GameEnvironment::GetWorldManager()
    {
        return Instance().worldManager;
    }

    Camera& GameEnvironment::GetCamera()
    {
        return Instance().camera;
    }

    Modulator::LinkMap& GameEnvironment::GetModulatorLinkMap()
    {
        return Instance().modulatorLinkMap;
    }

    AssetPackage& GameEnvironment::GetAssetPackage()
    {
        return Instance().assetPackage;
    }

    void GameEnvironment::OnInputPressed(const Input::Key &args)
    {
        if (args.id == Input::KeyID::F6)
            Instance().renderer.flags.Flip(RenderFlags::DONT_DRAW);
    }

    void GameEnvironment::OnFocusLost()
    {
        GetLocalObjectManager()->FindSystem<MusicSystem>()->PauseCurrent();
    }

    void GameEnvironment::OnFocusRegain()
    {
        GetLocalObjectManager()->FindSystem<MusicSystem>()->ResumeCurrent();
    }

    void GameEnvironment::Work()
    {
        DebugScreen::Logic().Start();

        StateManager::Work();

        DebugScreen::Logic().Calc();
    }

    void GameEnvironment::RegisterAllKnownObjectTypes()
    {
        // Global
        RegisterGlobalObjectType<nAsset>();
        RegisterGlobalObjectType<nFileAsset>();
        RegisterGlobalObjectType<AudioAsset>();
        RegisterGlobalObjectType<ImageAsset>();
        RegisterGlobalObjectType<Material>();
        RegisterGlobalObjectType<ShaderAsset>();

        RegisterGlobalObjectType<nAbility>();
        RegisterGlobalObjectType<nCharacterClass>();
        RegisterGlobalObjectType<nItem>();
        RegisterGlobalObjectType<nQuest>();
        RegisterGlobalObjectType<nSpell>();
        RegisterGlobalObjectType<nStatusEffect>();

        RegisterGlobalObjectSystem<AudioSystem>();
        RegisterGlobalObjectSystem<ImageSystem>();
        RegisterGlobalObjectSystem<MaterialSystem>();
        RegisterGlobalObjectSystem<ScriptSystem>();
        RegisterGlobalObjectSystem<ShaderSystem>();

        RegisterGlobalObjectSystem<AbilitySystem>();
        RegisterGlobalObjectSystem<CharacterClassSystem>();
        RegisterGlobalObjectSystem<ItemSystem>();
        RegisterGlobalObjectSystem<QuestSystem>();
        RegisterGlobalObjectSystem<SpellSystem>();
        RegisterGlobalObjectSystem<StatusEffectSystem>();

        // Local
        RegisterLocalObjectType<nTile>();

        RegisterLocalObjectType<Ent::nEntity>();
        RegisterLocalObjectType<Ent::nEntityComponent>();
        RegisterLocalObjectType<Ent::nActionComponent>();

        RegisterLocalObjectType<PositionalObject>();
        RegisterLocalObjectType<nRenderFragment>();
        RegisterLocalObjectType<nSense>();
        RegisterLocalObjectType<nSprite>();

        RegisterLocalObjectType<Modulator::Modulator>();
        RegisterLocalObjectType<Modulator::Track>();
        RegisterLocalObjectType<Modulator::Node>();

        RegisterLocalObjectSystem<Ent::nEntityActionSystem>();
        RegisterLocalObjectSystem<Ent::nEntityAvatarSystem>();
        RegisterLocalObjectSystem<Ent::nEntityAISystem>();
        RegisterLocalObjectSystem<Ent::nEntityPositionSystem>();

        RegisterLocalObjectSystem<TileSystem>();
        RegisterLocalObjectSystem<RenderFragmentSystem>();
        RegisterLocalObjectSystem<SoundSystem>();
        RegisterLocalObjectSystem<MusicSystem>();
        RegisterLocalObjectSystem<Speech::Controller>();
        RegisterLocalObjectSystem<Speech::Shop>();
    }

    GameEnvironment::ObjectTypeRegistratorBase::~ObjectTypeRegistratorBase()
    {}

    void GameEnvironment::PushAllObjectTypes(ObjectTypeRegistratorList& from, ObjectManager& to)
    {
        for (auto& loop : from)
            loop->Register(to);
    }

    ObjectManager* GetGlobalObjectManager()
    {
        return &GameEnvironment::GetGlobalObjectManager();
    }
}