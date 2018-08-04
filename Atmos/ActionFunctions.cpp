
#include "ActionFunctions.h"

#include "Input.h"
#include "Speech.h"
#include "Camera.h"
#include "WorldManager.h"
#include "FieldDestination.h"
#include "Battle.h"
#include "Music.h"
#include "CurrentMusic.h"
#include "Scheduler.h"
#include "ScriptRegistry.h"
#include "ScriptController.h"
#include "Action.h"

#include "EntityManager.h"
#include "AvatarSystem.h"
#include "AvatarComponent.h"
#include "DialogueComponent.h"
#include "EntityNameSystem.h"
#include "EntityPositionSystem.h"
#include "EntityAISystem.h"

#include "PlayerParty.h"
#include "Environment.h"

#include <Affecter/SmoothstepType.h>

namespace Atmos
{
    namespace Act
    {
        namespace Speech
        {
            void StartSpeech(const Name &name)
            {
                auto entity = ::Atmos::Ent::NameSystem::FindEntity(name);
                if (entity == ::Atmos::Ent::nullEntity)
                    return;

                auto dialogue = GetCurrentEntities()->FindComponent<::Atmos::Ent::DialogueComponent>(entity);
                if (!dialogue)
                    return;

                ::Atmos::Speech::Handler::Enter(*dialogue);
            }
        }

        namespace Script
        {
            void CreateAndStart(const Name &name)
            {
                auto found = AssetRegistry<ScriptModuleBase>::Find(name);
                if (!found || !found->IsScript())
                    return;

                ScriptController::Add(GetCurrentOrphanScripts()->Add(static_cast<const ::Atmos::Script*>(found.Get())->MakeInstance()));
            }

            void CreateAndStartGlobal(const Name &name, const std::vector<NameValuePair> &globals)
            {
                auto found = AssetRegistry<ScriptModuleBase>::Find(name);
                if (!found || !found->IsScript())
                    return;

                ::Atmos::Script::Instance instance(static_cast<const ::Atmos::Script*>(found.Get())->MakeInstance());

                for (auto &loop : globals)
                    instance.AddGlobalItem(loop);

                ScriptController::Add(GetCurrentOrphanScripts()->Add(std::move(instance)));
            }

            void CreateAndStartExternal(const Name &name, const ::Atmos::Script::SymbolName &overrideExecuteName, const std::vector<NameValuePair> &overrideArguments)
            {
                auto found = AssetRegistry<ScriptModuleBase>::Find(name);
                if (!found || !found->IsScript())
                    return;

                ScriptController::Add(GetCurrentOrphanScripts()->Add(static_cast<const ::Atmos::Script*>(found.Get())->MakeInstance()), overrideExecuteName, overrideArguments);
            }

            void CreateAndStartGlobalExternal(const Name &name, const std::vector<NameValuePair> &globals, const ::Atmos::Script::SymbolName &overrideExecuteName, const std::vector<NameValuePair> &overrideArguments)
            {
                auto found = AssetRegistry<ScriptModuleBase>::Find(name);
                if (!found || !found->IsScript())
                    return;

                ::Atmos::Script::Instance instance(static_cast<const ::Atmos::Script*>(found.Get())->MakeInstance());

                for (auto &loop : globals)
                    instance.AddGlobalItem(loop);

                ScriptController::Add(GetCurrentOrphanScripts()->Add(std::move(instance)), overrideExecuteName, overrideArguments);
            }
        }

        namespace Field
        {
            void Change(FieldID id, const GridPosition &pos, unsigned char dir)
            {
                WorldManager::Request(FieldDestination(Direction::ValueT(dir), pos, id));
            }
        }

        namespace Music
        {
            void Change(const String &name)
            {
                ::Atmos::CurrentMusic::ChangePlaying(FileName(name));
            }
        }

        namespace Scheduler
        {
            void Schedule(::Atmos::Act::ID id, const std::vector<::Atmos::Act::ParameterValueSingle> &args, TimeValue::ValueT timeTaken)
            {
                //Parameter test;
                //auto test2 = test.Get<7>();
                std::vector<Parameter> parameters;
                for (auto &loop : args)
                    parameters.push_back(Parameter(loop));

                Action action(id);
                action.SetParameters(std::move(parameters));
                GetCurrentScheduler()->Schedule(std::move(action), timeTaken);
            }
        }
    }
}