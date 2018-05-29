
#include "ScriptLoader.h"

#include "ScriptController.h"
#include "FalconScriptUtility.h"
#include "FalconScriptObjects.h"
#include "FalconScriptFunctions.h"

#include "Enum.h"
#include "Logger.h"
#include "StringUtility.h"

// Required for export to the module
#include "ActionFunctions.h"
#include "Speech.h"
#include "AvatarSystem.h"
#include "AvatarComponent.h"
#include "CharacterComponent.h"
#include "WorldManager.h"
#include "PlayerParty.h"
#include "EntityNameSystem.h"
#include "EntityPositionSystem.h"
#include "ModulatorController.h"
#include "ModulatorDescribers.h"
#include "GameEnvironment.h"

#include <falcon/engine.h>

namespace Atmos
{
    using namespace ::Atmos::Fal;

    namespace GeneralScript
    {
        FALCON_FUNC Suspend(Falcon::VMachine *vm)
        {
            ScriptController::Current()->Suspend();
        }

        FALCON_FUNC InstantiateScript(Falcon::VMachine *vm)
        {

        }
    }

    namespace ErrorScript
    {
        FALCON_FUNC LogError(Falcon::VMachine *vm)
        {
            try { Logger::Log(CheckedConversionFromFalcon<String>("message", vm),
                Logger::Type::ERROR_LOW); }
            catch (const ScriptException&) { return; }
        }
    }

    namespace RandomScript
    {
        FALCON_FUNC GetRandomBool(Falcon::VMachine *vm)
        {
            const char *probabilityName("probability");

            auto probability = vm->findLocalSymbolItem(probabilityName);
            if (!probability)
                vm->retval(Act::Random::GenerateBool());
            else
            {
                try { vm->retval(Act::Random::GenerateBool(CheckedConversionFromFalcon<Falcon::numeric>(probabilityName, probability))); }
                catch (const ScriptException&) { return; }
            }
        }

        FALCON_FUNC GetRandomInteger(Falcon::VMachine *vm)
        {
            typedef typename FalconVariableTraits<std::uint64_t>::Type Type;

            const char *floorName("floor");
            const char *ceilingName("ceiling");

            auto isSigned = FromItem<bool>(*vm, "signed");
            if (!isSigned)
                isSigned = false;

            if (*isSigned)
            {
                typedef std::int64_t T;
                auto floor = FromItem<T>(*vm, "floor");
                auto ceiling = FromItem<T>(*vm, "ceiling");
                if (!floor || !ceiling)
                    vm->retval(Act::Random::GenerateIntFullRange<T>());
                else
                    vm->retval(Act::Random::GenerateInt<T>(*floor, *ceiling));
            }
            else
            {
                typedef std::uint64_t T;
                auto floor = FromItem<T>(*vm, "floor");
                auto ceiling = FromItem<T>(*vm, "ceiling");
                if (!floor || !ceiling)
                    vm->retval(CreateItem(*vm, Act::Random::GenerateIntFullRange<T>()));
                else
                    vm->retval(CreateItem(*vm, Act::Random::GenerateInt<T>(*floor, *ceiling)));
            }
        }
    }

    namespace inputScript
    {
        FALCON_FUNC IsActionActive(Falcon::VMachine *vm)
        {
            try { vm->retval(Act::Input::IsActionActive(static_cast<Input::ActionID>(CheckedConversionFromFalcon<Falcon::int64>("id", vm)))); }
            catch (const ScriptException&) { return; }
        }

        FALCON_FUNC IsActionPressed(Falcon::VMachine *vm)
        {
            try { vm->retval(Act::Input::IsActionPressed(static_cast<Input::ActionID>(CheckedConversionFromFalcon<Falcon::int64>("id", vm)))); }
            catch (const ScriptException&) { return; }
        }

        FALCON_FUNC IsActionDepressed(Falcon::VMachine *vm)
        {
            try { vm->retval(Act::Input::IsActionDepressed(static_cast<Input::ActionID>(CheckedConversionFromFalcon<Falcon::int64>("id", vm)))); }
            catch (const ScriptException&) { return; }
        }
    }

    namespace timeScript
    {
        FALCON_FUNC GetFrameTimeRaw(Falcon::VMachine *vm)
        {
            vm->retval(CreateItem<::Atmos::FrameTimer>(*vm, Act::Time::GetFrameTime()));
        }

        FALCON_FUNC FrameTimerConstructor(Falcon::VMachine *vm)
        {
            CheckedItemPropertySet(vm->self(), FalconVariableTraits<::Atmos::FrameTimer>::goalName, RetrieveItemFromVM("goal", vm), &Falcon::Item::isOrdinal, &Falcon::Item::forceNumeric, 0.0);
            RetrieveItemFromVM(FalconVariableTraits<::Atmos::FrameTimer>::startName, vm)->setInteger(0);
        }

        FALCON_FUNC FrameTimer_Start(Falcon::VMachine *vm)
        {
            SetItem<::Atmos::FrameTimer>(*vm, *RetrieveItemFromVM(FalconVariableTraits<::Atmos::FrameTimer>::startName, vm), Act::Time::GetFrameTime());
        }

        FALCON_FUNC FrameTimer_Elapsed(Falcon::VMachine *vm)
        {
            typedef FalconVariableTraits<TimeValue::ValueT> WrappedTraits;
            auto goal = WrappedTraits::FromItem(*vm->findLocalSymbolItem(Convert(FalconVariableTraits<::Atmos::FrameTimer>::goalName)));
            auto start = WrappedTraits::FromItem(*vm->findLocalSymbolItem(Convert(FalconVariableTraits<::Atmos::FrameTimer>::startName)));
            TimeValue now(Act::Time::GetFrameTime());

            vm->retval(WrappedTraits::CreateItem(*vm, (now.Get() - start) / goal));
        }

        FALCON_FUNC FrameTimer_SetGoal(Falcon::VMachine *vm)
        {
            CheckedItemSet(FalconVariableTraits<::Atmos::FrameTimer>::goalName, "set", vm, &Falcon::Item::isNumeric, &Falcon::Item::asNumeric, &Falcon::Item::setNumeric, 0.0);
        }

        FALCON_FUNC FrameTimer_HasReachedGoal(Falcon::VMachine *vm)
        {
            typedef FalconVariableTraits<TimeValue::ValueT> WrappedTraits;
            auto goal = WrappedTraits::FromItem(*vm->findLocalSymbolItem(Convert(FalconVariableTraits<::Atmos::FrameTimer>::goalName)));
            auto start = WrappedTraits::FromItem(*vm->findLocalSymbolItem(Convert(FalconVariableTraits<::Atmos::FrameTimer>::startName)));
            TimeValue now(Act::Time::GetFrameTime());

            vm->retval(WrappedTraits::CreateItem(*vm, now.Get() - start));
        }
    }

    namespace speechScript
    {
        FALCON_FUNC SetCharacters(Falcon::VMachine *vm)
        {
            try { Act::Speech::SetCharacters(CheckedConversionFromFalcon<String>("output", vm)); }
            catch (const ScriptException&) { return; }
        }

        FALCON_FUNC AppendCharacters(Falcon::VMachine *vm)
        {
            try { Act::Speech::AppendCharacters(CheckedConversionFromFalcon<String>("output", vm)); }
            catch (const ScriptException&) { return; }
        }

        FALCON_FUNC ClearCharacters(Falcon::VMachine *vm)
        {
            Act::Speech::ClearCharacters();
        }

        FALCON_FUNC GetCharacters(Falcon::VMachine *vm)
        {
            vm->retval(Convert(Act::Speech::GetOutputCharacters()));
        }

        FALCON_FUNC LeaveSpeech(Falcon::VMachine *vm)
        {
            Act::Speech::LeaveSpeech();
        }

        FALCON_FUNC EnterShop(Falcon::VMachine *vm)
        {
            try { Act::Speech::EnterShop(CheckedConversionFromFalcon<bool>("buying", vm)); }
            catch (const ScriptException&) { return; }
        }

        FALCON_FUNC IsShopActive(Falcon::VMachine *vm)
        {
            vm->retval(Act::Speech::IsShopActive());
        }

        FALCON_FUNC ActivateInput(Falcon::VMachine *vm)
        {
            try { Act::Speech::ActivateInput(CheckedVectorConversionFromFalconAll<String>("strings", vm)); }
            catch (const ScriptException&) { return; }
        }

        FALCON_FUNC DeactivateInput(Falcon::VMachine *vm)
        {
            Act::Speech::DeactivateInput();
        }

        FALCON_FUNC GetInputPosition(Falcon::VMachine *vm)
        {
            vm->retval(CreateItem(*vm, Act::Speech::GetInputPosition()));
        }
    }

    namespace entityScript
    {
        FALCON_FUNC CanMoveEntity(Falcon::VMachine *vm)
        {
            // This can accept 1 or 2 arguments
            // The first argument MUST be an entity
            // The second argument can be a tile position

            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            Falcon::Item *position = RetrieveItemFromVM("position", vm);
            if (!position || !Is<::Atmos::GridPosition>(*position))
                position = nullptr;

            if (!position)
                vm->retval(::Atmos::Ent::PositionSystem::CanMove(entity));
            else
                vm->retval(::Atmos::Ent::PositionSystem::CanMove(entity, FromItem<::Atmos::GridPosition>(*position)));
        }

        FALCON_FUNC MoveEntity(Falcon::VMachine *vm)
        {
            // Requires 3 arguments
            // The first argument MUST be an entity
            // The second argument MUST be a grid position
            // The third argument MUST be a LargeInteger

            Entity entity = Ent::nullEntity;
            ::Atmos::GridPosition position;
            TimeValue::ValueT timeTaken;
            try
            {
                entity = CheckedConversionFromFalcon<Entity>("entity", vm);
                position = CheckedConversionFromFalcon<::Atmos::GridPosition>("position", vm);
                timeTaken = CheckedConversionFromFalcon<TimeValue::ValueT>("timeTaken", vm);
            }
            catch (const ScriptException&) { return; }

            ::Atmos::Act::Ent::Move(entity, position, timeTaken);
        }

        FALCON_FUNC MoveEntityDirection(Falcon::VMachine *vm)
        {
            // Requires 2 arguments
            // The first argument MUST be an entity
            // The second argument MUST be a direction

            Entity entity = Ent::nullEntity;
            Direction direction;
            try
            {
                entity = CheckedConversionFromFalcon<Entity>("entity", vm);
                direction = static_cast<Direction::ValueT>(CheckedConversionFromFalcon<std::underlying_type<Direction::ValueT>::type>("direction", vm));
            }
            catch (const ScriptException&) { return; }

            ::Atmos::Act::Ent::Move(entity, direction);
        }

        FALCON_FUNC GetEntityTalking(Falcon::VMachine *vm)
        {
            Entity found = Speech::Handler::GetEntityTalking();
            if (found == Ent::nullEntity)
            {
                vm->retnil();
                return;
            }

            vm->retval(CreateItem(*vm, *found));
        }

        FALCON_FUNC GetAvatarGold(Falcon::VMachine *vm)
        {
            vm->retval(CreateItem(*vm, ::Atmos::Ent::AvatarSystem::GetAvatar()->gold));
        }

        FALCON_FUNC AddEntityToParty(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            if (entity == Ent::nullEntity)
            {
                vm->retnil();
                return;
            }

            auto charComp = GetCurrentEntities()->FindComponent<Ent::CharacterComponent>(entity);
            if (!charComp || charComp->type != Ent::CharacterComponent::Type::NPC)
            {
                FatalScriptError(*ScriptController::Current(), "An entity was attempted to be put into the player party but was not an NPC.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair("Entity ID", ToString(entity)) });
                return;
            }

            Ent::PlayerParty::Add(entity);
        }

        FALCON_FUNC GetEntityName(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            if (entity == Ent::nullEntity)
            {
                vm->retnil();
                return;
            }

            auto &name = GetCurrentEntities()->GetName(entity);
            if (!name.IsValid())
            {
                FatalScriptError(*ScriptController::Current(), "An entity's name was attempted to be found but the entity did not exist.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair("Entity ID", ToString(entity)) });
                return;
            }

            vm->retval(Convert(name.Get()));
        }

        FALCON_FUNC GetEntityPosition(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            if (entity == Ent::nullEntity)
            {
                vm->retnil();
                return;
            }

            auto &position = GetCurrentEntities()->GetPosition(entity);
            if (!position.IsValid())
            {
                FatalScriptError(*ScriptController::Current(), "An entity's position was attempted to be found but the entity did not exist.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair("Entity ID", ToString(entity)) });
                return;
            }

            vm->retval(CreateItem(*vm, *position));
        }

        FALCON_FUNC GetEntityWithName(Falcon::VMachine *vm)
        {
            Name name;
            try { name = CheckedConversionFromFalcon<String>("name", vm); }
            catch (const ScriptException&) { return; }

            vm->retval(CreateItem(*vm, *::Atmos::Ent::NameSystem::FindEntity(name)));
        }

        FALCON_FUNC GetEntitiesAtPosition(Falcon::VMachine *vm)
        {
            auto position = FromItem<::Atmos::GridPosition>(*vm, "position");
            if (!position)
                return;

            auto ret = new Falcon::CoreArray();
            for (auto &loop : ::Atmos::Ent::PositionSystem::FindEntities(*position))
                ret->append(*loop);
            vm->retval(ret);
        }

        FALCON_FUNC GetItemCount(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            if (entity == Ent::nullEntity)
            {
                vm->retnil();
                return;
            }

            vm->retval(CreateItem(*vm, ::Atmos::Act::Ent::GetItemCount(entity)));
        }

        FALCON_FUNC IsEntityInParty(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            if (entity == Ent::nullEntity)
            {
                vm->retval(false);
                return;
            }

            vm->retval(Act::Ent::IsEntityInParty(entity));
        }

        FALCON_FUNC GetPersistent(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            String persistentName;
            try
            {
                entity = CheckedConversionFromFalcon<Entity>("entity", vm);
                persistentName = CheckedConversionFromFalcon<String>("persistentName", vm);
            }
            catch (const ScriptException&) { return; }

            if (entity == Ent::nullEntity)
            {
                vm->retnil();
                return;
            }

            if (!GetCurrentEntities()->CheckEntityExists(entity))
            {
                vm->retnil();
                return;
            }

            auto general = WorldManager::GetCurrentField()->entities.FindComponent<Ent::GeneralComponent>(entity);
            if (!general->HasStorage(persistentName))
            {
                vm->retnil();
                return;
            }

            typedef Ent::GeneralComponent::Storage::Type Type;

            auto &storage = *general->FindStorage(persistentName);
            vm->retval(CreateItem(*vm, storage.variant));
        }

        FALCON_FUNC StorePersistent(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            String persistentName;
            Variant value;
            try
            {
                entity = CheckedConversionFromFalcon<Entity>("entity", vm);
                persistentName = CheckedConversionFromFalcon<String>("persistentName", vm);
                value = CheckedConversionFromFalcon<Variant>("value", vm);
            }
            catch (const ScriptException&) { return; }

            if (entity == Ent::nullEntity)
            {
                vm->retnil();
                return;
            }

            if (!GetCurrentEntities()->CheckEntityExists(entity))
            {
                vm->retnil();
                return;
            }

            auto general = GetCurrentEntities()->FindComponent<Ent::GeneralComponent>(entity);
            auto storage = general->FindStorage(persistentName);

            if (!storage)
                general->AddStorage(persistentName, value);
            else
                storage->variant = value;
        }
    }

    namespace positionScript
    {
        FALCON_FUNC TilePositionConstructor(Falcon::VMachine *vm)
        {
            auto setter = [](const char *name, Falcon::VMachine *vm)
            {
                CheckedItemPropertySet(vm->self(), name, RetrieveItemFromVM(name, vm), &Falcon::Item::isOrdinal, &Falcon::Item::forceInteger, 0);
            };

            setter("x", vm);
            setter("y", vm);
            setter("z", vm);
        }

        FALCON_FUNC TilePosition_Distance(Falcon::VMachine *vm)
        {
            auto other = FromItem<::Atmos::GridPosition>(*vm, "other");
            if (!other)
                return;

            auto &self = FromItem<::Atmos::GridPosition>(vm->self());

            vm->retval(CreateItem(*vm, self.FindDistance(*other)));
        }

        FALCON_FUNC FindPath(Falcon::VMachine *vm)
        {
            auto positionFrom = FromItem<::Atmos::GridPosition>(*vm, "positionFrom");
            if (!positionFrom)
                return;

            auto positionTo = FromItem<::Atmos::GridPosition>(*vm, "positionTo");
            if (!positionTo)
                return;

            auto &path = ::Atmos::Act::Position::FindPath(*positionFrom, *positionTo);

            auto sendPath = new Falcon::CoreArray();
            while (!path.empty())
            {
                sendPath->append(CreateItem(*vm, path.top()));
                path.pop();
            }

            vm->retval(sendPath);
        }
    }

    namespace modulatorScript
    {
        FALCON_FUNC Create(Falcon::VMachine *vm)
        {
            String name;
            try
            {
                name = CheckedConversionFromFalcon<String>("name", vm);
            }
            catch (const ScriptException&) { return; }

            // Create the modulator
            ::Atmos::Modulator::Observer madeMod(GameEnvironment::GenerateModulator(name));
            if (!madeMod)
            {
                // Log error if not created
                Logger::Log("A modulator was attempted to be created with an invalid name.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair("Name", name) });
                return;
            }

            // Return the ID for the new modulator
            Classes::Modulator::Modulator::modID.value = GameEnvironment::AttachModulator(madeMod);
            vm->retval(Classes::Modulator::Modulator::CreateItem(*vm));
        }

        FALCON_FUNC CreateTrack(Falcon::VMachine *vm)
        {
            ::Atmos::Modulator::Controller::ID modID = 0;
            String name;
            try
            {
                modID = CheckedConversionFromFalcon<::Atmos::Modulator::Controller::ID>("id", vm);
                name = CheckedConversionFromFalcon<String>("name", vm);
            }
            catch (const ScriptException&) { return; }

            ::Atmos::Modulator::Observer mod(GameEnvironment::FindModulator(modID));
            if (!mod)
            {
                // Log error if not found
                Logger::Log("A modulator was attempted to be found with an invalid ID.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair("ID", modID) });
                return;
            }

            auto &madeTrack = GameEnvironment::GenerateModulatorTrack(mod->GetGeneratorName(), name);
            if (!madeTrack)
            {
                // Log error if not made
                Logger::Log("A modulator track was attempted to be created with an invalid name.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair("Track Name", name) });
                return;
            }

            vm->retval(FalconVariableTraits<::Atmos::Modulator::Controller::ID>::CreateItem(*vm, mod->AddTrack(std::move(madeTrack))));
        }

        FALCON_FUNC CreateTrackNode(Falcon::VMachine *vm)
        {
            ::Atmos::Modulator::Controller::ID modID = 0;
            ::Atmos::Modulator::ModulatorBase::TrackID trackID = 0;
            try
            {
                modID = CheckedConversionFromFalcon<::Atmos::Modulator::Controller::ID>("modID", vm);
                trackID = CheckedConversionFromFalcon<::Atmos::Modulator::Controller::ID>("trackID", vm);
            }
            catch (const ScriptException&) { return; }

            ::Atmos::Modulator::Observer foundMod = GameEnvironment::FindModulator(modID);
            if (!foundMod)
            {
                // Log error if not found
                Logger::Log("A modulator was attempted to be found with an invalid ID.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair("Mod ID", modID) });
                return;
            }

            auto foundTrack = foundMod->FindTrack(trackID);
            if (!foundTrack)
            {
                // Log error if not made
                Logger::Log("A modulator track was attempted to be found with an invalid ID.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair("Track ID", trackID) });
                return;
            }

            foundTrack->AddNode();
        }

        FALCON_FUNC ModulatorConstructor(Falcon::VMachine *vm)
        {
            auto setter = [](const char *name, Falcon::VMachine *vm)
            {
                CheckedItemPropertySet(vm->self(), name, RetrieveItemFromVM(name, vm), &Falcon::Item::isOrdinal, &Falcon::Item::forceInteger, 0);
            };

            setter("modID", vm);
        }

        FALCON_FUNC TrackConstructor(Falcon::VMachine *vm)
        {
            auto setter = [](const char *name, Falcon::VMachine *vm)
            {
                CheckedItemPropertySet(vm->self(), name, RetrieveItemFromVM(name, vm), &Falcon::Item::isOrdinal, &Falcon::Item::forceInteger, 0);
            };

            setter("modID", vm);
            setter("trackID", vm);
        }

        FALCON_FUNC TrackNodeConstructor(Falcon::VMachine *vm)
        {
            auto setter = [](const char *name, Falcon::VMachine *vm)
            {
                CheckedItemPropertySet(vm->self(), name, RetrieveItemFromVM(name, vm), &Falcon::Item::isOrdinal, &Falcon::Item::forceInteger, 0);
            };

            setter("modID", vm);
            setter("trackID", vm);
            setter("nodeID", vm);
        }
    }

    namespace battleScript
    {
        FALCON_FUNC FindClosestPlayer(Falcon::VMachine *vm)
        {
            auto position = FromItem<::Atmos::GridPosition>(*vm, "position");
            if (!position)
                return;

            vm->retval(CreateItem(*vm, *::Atmos::Act::Battle::FindClosestPlayer(*position)));
        }

        FALCON_FUNC FindClosestMonster(Falcon::VMachine *vm)
        {
            auto position = FromItem<::Atmos::GridPosition>(*vm, "position");
            if (!position)
                return;

            vm->retval(CreateItem(*vm, *::Atmos::Act::Battle::FindClosestMonster(*position)));
        }

        FALCON_FUNC GetStartingPosition(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            auto battle = GetCurrentEntities()->FindComponent<::Atmos::Ent::BattleComponent>(entity);
            if (!battle)
            {
                vm->retnil();
                return;
            }

            vm->retval(CreateItem(*vm, battle->startingPos));
        }

        FALCON_FUNC GetMovementRange(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            auto combat = GetCurrentEntities()->FindComponent<::Atmos::Ent::CombatComponent>(entity);
            if (!combat)
            {
                vm->retnil();
                return;
            }

            vm->retval(combat->movementRange);
        }

        FALCON_FUNC IsTurnEnded(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            auto battle = GetCurrentEntities()->FindComponent<::Atmos::Ent::BattleComponent>(entity);
            if (!battle)
            {
                vm->retnil();
                return;
            }

            vm->retval(battle->turnEnded);
        }

        FALCON_FUNC Attack(Falcon::VMachine *vm)
        {
            Entity source = Ent::nullEntity;
            Entity target = Ent::nullEntity;
            try
            {
                source = CheckedConversionFromFalcon<Entity>("source", vm);
                target = CheckedConversionFromFalcon<Entity>("target", vm);
            }
            catch (const ScriptException&) { return; }

            auto battle = GetCurrentEntities()->FindComponent<::Atmos::Ent::BattleComponent>(source);
            if (!battle || target == Ent::nullEntity)
            {
                vm->retnil();
                return;
            }

            battle->Attack(target);
        }

        FALCON_FUNC EndTurn(Falcon::VMachine *vm)
        {
            Entity entity = Ent::nullEntity;
            try { entity = CheckedConversionFromFalcon<Entity>("entity", vm); }
            catch (const ScriptException&) { return; }

            ::Atmos::Act::Battle::EndTurn(entity);
        }
    }

    class ScriptLoader::Impl
    {
    public:
        Falcon::ModuleLoader moduleLoader;
        Falcon::Module *mainModule;

        Impl() : moduleLoader("."), mainModule(new Falcon::Module())
        {
            moduleLoader.addFalconPath();
            mainModule->name("mainModule");
        }
    };

    ScriptLoader::ScriptLoader() : impl(new Impl())
    {
        Falcon::Module *mainModule = impl->mainModule;

        // General
        {
            // Suspension
            Functions::General::Suspend.PushAllToModule(*mainModule);
            // Caller
            Classes::General::CallerType::PushAllToModule(*mainModule);
            Classes::General::Caller::PushAllToModule(*mainModule);
            // Logger
            Classes::General::LogType::PushAllToModule(*mainModule);
            Functions::General::LogMessage.PushAllToModule(*mainModule);
            // Random
            Functions::General::RandomBool.PushAllToModule(*mainModule);
            Functions::General::RandomInteger.PushAllToModule(*mainModule);
        }

        // Modulator
        {
            // Types
            Classes::Modulator::Type::PushAllToModule(*mainModule);
            Classes::Modulator::TrackType::PushAllToModule(*mainModule);
            // Objects
            Classes::Modulator::Modulator::PushAllToModule(*mainModule);
            Classes::Modulator::Track::PushAllToModule(*mainModule);
            Classes::Modulator::TrackNode::PushAllToModule(*mainModule);
            // Creation
            Functions::Modulator::Create.PushAllToModule(*mainModule);
        }

        // Entity
        {
            Classes::Ent::ComponentType::PushAllToModule(*mainModule);

            Classes::Ent::ActionComponent::PushAllToModule(*mainModule);
            Classes::Ent::BattleComponent::PushAllToModule(*mainModule);
            Classes::Ent::AIComponent::PushAllToModule(*mainModule);
            Classes::Ent::AvatarComponent::PushAllToModule(*mainModule);
            Classes::Ent::CharacterComponent::PushAllToModule(*mainModule);
            Classes::Ent::CombatComponent::PushAllToModule(*mainModule);
            Classes::Ent::DialogueComponent::PushAllToModule(*mainModule);
            Classes::Ent::GeneralComponent::PushAllToModule(*mainModule);
            Classes::Ent::InventoryComponent::PushAllToModule(*mainModule);
            Classes::Ent::MovementComponent::PushAllToModule(*mainModule);
            Classes::Ent::SenseComponent::PushAllToModule(*mainModule);

            Classes::Ent::Entity::PushAllToModule(*mainModule);
        }

        /*
        // General purpose
        {
            Functions::Suspend.PushAllToModule(*mainModule);
            //mainModule->addExtFunc("Atmos_Suspend", GeneralScript::Suspend);
            mainModule->addExtFunc("Atmos_InstantiateScript", GeneralScript::InstantiateScript);
        }

        // Error
        {
            mainModule->addExtFunc("Atmos_LogError", ErrorScript::LogError)->addParam("message");
        }

        // Random
        {
            mainModule->addExtFunc("Atmos_GetRandomBool", RandomScript::GetRandomBool)->addParam("probability");
            mainModule->addExtFunc("Atmos_GetRandomInteger", RandomScript::GetRandomInteger)->addParam("floor")->addParam("ceiling")->addParam("signed");
        }

        // Input
        {
            mainModule->addExtFunc("Atmos_IsActionActive", inputScript::IsActionActive)->addParam("id");
            mainModule->addExtFunc("Atmos_IsActionPressed", inputScript::IsActionPressed)->addParam("id");
            mainModule->addExtFunc("Atmos_IsActionDepressed", inputScript::IsActionDepressed)->addParam("id");

            auto actionAdder = [&](Falcon::Symbol *symbol, Input::ActionID id, const String &name)
            {
                mainModule->addClassProperty(symbol, name.c_str()).setInteger(ConvertToUnderlyingType(id));
            };

            Falcon::Symbol *focusedSymbol = mainModule->addClass("Atmos_Action");
            for (auto &loop : Environment::GetInput()->GetActions())
                actionAdder(focusedSymbol, loop->id, loop->displayName);
        }

        // Time
        {
            auto typeAdder = [&](Falcon::Symbol *symbol, TimeValueEpoch t, const String &name)
            {
                mainModule->addClassProperty(symbol, name.c_str()).setInteger(ConvertToUnderlyingType(t));
            };

            mainModule->addExtFunc("Atmos_GetFrameTimeRaw", timeScript::GetFrameTimeRaw);

            // Setup frame timer
            auto cls = mainModule->addClass(Convert(FalconVariableTraits<::Atmos::FrameTimer>::falconTypeName), &timeScript::FrameTimerConstructor)
                ->addParam("goal");
            mainModule->addClassProperty(cls, Convert(FalconVariableTraits<::Atmos::FrameTimer>::startName));
            mainModule->addClassProperty(cls, Convert(FalconVariableTraits<::Atmos::FrameTimer>::goalName));
            mainModule->addClassMethod(cls, "Start", &timeScript::FrameTimer_Start);
            mainModule->addClassMethod(cls, "Elapsed", &timeScript::FrameTimer_Elapsed);
            mainModule->addClassMethod(cls, "SetGoal", &timeScript::FrameTimer_SetGoal).asSymbol()->addParam("set");
            mainModule->addClassMethod(cls, "HasReachedGoal", &timeScript::FrameTimer_HasReachedGoal);
        }

        // Speech
        {
            mainModule->addExtFunc("Atmos_SetSpeechCharacters", speechScript::SetCharacters)->addParam("output");
            mainModule->addExtFunc("Atmos_AppendSpeechCharacters", speechScript::AppendCharacters)->addParam("output");
            mainModule->addExtFunc("Atmos_ClearSpeechCharacters", speechScript::ClearCharacters);
            mainModule->addExtFunc("Atmos_GetSpeechCharacters", speechScript::GetCharacters);
            mainModule->addExtFunc("Atmos_LeaveSpeech", speechScript::LeaveSpeech);
            mainModule->addExtFunc("Atmos_EnterShop", speechScript::EnterShop)->addParam("buying");
            mainModule->addExtFunc("Atmos_IsShopActive", speechScript::IsShopActive);

            mainModule->addExtFunc("Atmos_ActivateSpeechInput", speechScript::ActivateInput)->addParam("strings");
            mainModule->addExtFunc("Atmos_DeactivateSpeechInput", speechScript::DeactivateInput);
            mainModule->addExtFunc("Atmos_GetSpeechInputPosition", speechScript::GetInputPosition);
        }

        // Entities
        {
            mainModule->addExtFunc("Atmos_CanMoveEntity", entityScript::CanMoveEntity)->addParam("entity")->addParam("position");
            mainModule->addExtFunc("Atmos_MoveEntity", entityScript::MoveEntity)->addParam("entity")->addParam("position");
            mainModule->addExtFunc("Atmos_MoveEntityDirection", entityScript::MoveEntityDirection)->addParam("entity")->addParam("direction");

            mainModule->addExtFunc("Atmos_GetEntityTalking", entityScript::GetEntityTalking);

            mainModule->addExtFunc("Atmos_GetAvatarGold", entityScript::GetAvatarGold);
            mainModule->addExtFunc("Atmos_AddEntityToParty", entityScript::AddEntityToParty)->addParam("entity");

            mainModule->addExtFunc("Atmos_GetEntityName", &entityScript::GetEntityName)->addParam("entity");
            mainModule->addExtFunc("Atmos_GetEntityPosition", &entityScript::GetEntityPosition)->addParam("entity");
            mainModule->addExtFunc("Atmos_GetEntityWithName", &entityScript::GetEntityWithName)->addParam("name");
            mainModule->addExtFunc("Atmos_GetEntitiesAtPosition", &entityScript::GetEntitiesAtPosition)->addParam("position");
            mainModule->addExtFunc("Atmos_GetItemCount", entityScript::GetItemCount)->addParam("entity");
            mainModule->addExtFunc("Atmos_IsEntityInParty", entityScript::IsEntityInParty)->addParam("entity");

            mainModule->addExtFunc("Atmos_GetEntityPersistent", entityScript::GetPersistent)->addParam("entity")->addParam("persistentName");
            mainModule->addExtFunc("Atmos_StoreEntityPersistent", entityScript::StorePersistent)->addParam("entity")->addParam("persistentName")->addParam("value");
        }

        // Position
        {
            mainModule->addExtFunc("Atmos_FindPath", positionScript::FindPath)->addParam("positionFrom")->addParam("positionTo");

            // Setup grid position
            Fal::Classes::GridPosition::Instance().PushAllToModule(*mainModule);
            */

            /*
            auto cls = mainModule->addClass(Convert(FalconVariableTraits<GridPosition>::falconTypeName), &positionScript::TilePositionConstructor)
                ->addParam("x")
                ->addParam("y")
                ->addParam("z");
            mainModule->addClassProperty(cls, "x");
            mainModule->addClassProperty(cls, "y");
            mainModule->addClassProperty(cls, "z");
            */

/*
            // Setup direction class
            {
                typedef ::Atmos::Direction::ValueT Value;
                auto cls = mainModule->addClass("Atmos_Direction");

                auto setup = [&](const char *name, Value value)
                {
                    mainModule->addClassProperty(cls, name).setInteger(value).setReadOnly(true);
                };

                setup("Up", Value::UP);
                setup("Down", Value::DOWN);
                setup("Left", Value::LEFT);
                setup("Right", Value::RIGHT);
            }
        }

        // Modulators
        {
            // Setup modulators strings
            {
                // First, do the regular modulators
                auto cls = mainModule->addClass("Atmos_ModulatorType");

                auto setup = [&](const char *name, const String &value)
                {
                    mainModule->addClassProperty(cls, name).setString(mainModule->addString(Convert(value))).setReadOnly(true);
                };

                setup("Sprite", ::Atmos::Modulator::Description::Sprite.name);
                setup("SpriteOffset", ::Atmos::Modulator::Description::SpriteOffset.name);
                setup("Sound", ::Atmos::Modulator::Description::Sound.name);
                setup("SoundOffset", ::Atmos::Modulator::Description::SoundOffset.name);
                setup("AVEffect", ::Atmos::Modulator::Description::AVEffect.name);
                setup("AVEffectOffset", ::Atmos::Modulator::Description::AVEffectOffset.name);
                setup("SenseComponent", ::Atmos::Modulator::Description::SenseComponent.name);

                // Now, do the tracks
                cls = mainModule->addClass("Atmos_ModulatorTrackType");

                setup("PositionX", ::Atmos::Modulator::Description::Track::PositionX.name);
                setup("PositionY", ::Atmos::Modulator::Description::Track::PositionY.name);
                setup("PositionZ", ::Atmos::Modulator::Description::Track::PositionZ.name);
                setup("ScalingX", ::Atmos::Modulator::Description::Track::ScalingX.name);
                setup("ScalingY", ::Atmos::Modulator::Description::Track::ScalingY.name);
                setup("ScalingZ", ::Atmos::Modulator::Description::Track::ScalingZ.name);
                setup("RotationX", ::Atmos::Modulator::Description::Track::RotationX.name);
                setup("RotationY", ::Atmos::Modulator::Description::Track::RotationY.name);
                setup("RotationZ", ::Atmos::Modulator::Description::Track::RotationZ.name);

                setup("Volume", ::Atmos::Modulator::Description::Track::Volume.name);
                setup("Index", ::Atmos::Modulator::Description::Track::Index.name);
            }

            // Setup Modulator class
            {
                Fal::Classes::Modulator::PushAllToModule(*mainModule);
                */

                /*
                auto cls = mainModule->addClass("Atmos_Modulator", &Fal::Modulator::constructor)
                    ->addParam("modID");
                mainModule->addClassProperty(cls, "modID");
                */
            //}

            /*
            // Setup Track class
            {
                Fal::Classes::ModulatorTrack::PushAllToModule(*mainModule);
                */
                /*
                auto cls = mainModule->addClass("Atmos_ModulatorTrack", &modulatorScript::TrackConstructor)
                    ->addParam("modID")
                    ->addParam("trackID");
                mainModule->addClassProperty(cls, "modID");
                mainModule->addClassProperty(cls, "trackID");
                */
            /*
            }

            // Setup Track Node class
            {
                Fal::Classes::ModulatorTrackNode::PushAllToModule(*mainModule);
                */
                /*
                auto cls = mainModule->addClass("Atmos_ModulatorTrackNode", &modulatorScript::TrackNodeConstructor)
                    ->addParam("modID")
                    ->addParam("trackID")
                    ->addParam("nodeID");
                mainModule->addClassProperty(cls, "modID");
                mainModule->addClassProperty(cls, "trackID");
                mainModule->addClassProperty(cls, "nodeID");
                */
            /*
            }

            // Create modulators
            mainModule->addExtFunc("Atmos_CreateModulator", &modulatorScript::Create)->addParam("name");
            // Create modulator tracks
            mainModule->addExtFunc("Atmos_CreateModulatorTrack", &modulatorScript::CreateTrack)->addParam("mod")->addParam("name");
            // Create modulator track nodes
            mainModule->addExtFunc("Atmos_CreateModulatorTrackNode", &modulatorScript::CreateTrackNode)->addParam("track");
        }

        // Battle
        {
            mainModule->addExtFunc("AtmosBattle_FindClosestPlayer", &battleScript::FindClosestPlayer)->addParam("position");
            mainModule->addExtFunc("AtmosBattle_FindClosestMonster", &battleScript::FindClosestMonster)->addParam("position");
            mainModule->addExtFunc("AtmosBattle_GetStartingPosition", &battleScript::GetStartingPosition)->addParam("entity");
            mainModule->addExtFunc("AtmosBattle_GetMovementRange", &battleScript::GetMovementRange)->addParam("entity");
            mainModule->addExtFunc("AtmosBattle_IsTurnEnded", &battleScript::IsTurnEnded)->addParam("entity");

            mainModule->addExtFunc("AtmosBattle_Attack", &battleScript::Attack)->addParam("source")->addParam("target");
            mainModule->addExtFunc("AtmosBattle_EndTurn", &battleScript::EndTurn)->addParam("entity");

            auto found = mainModule->findGlobalSymbol("Atmos_Direction");
            int hold = 1 + 1;
        }
        */
    }

    ScriptLoader& ScriptLoader::Instance()
    {
        static ScriptLoader instance;
        return instance;
    }

    Falcon::ModuleLoader& ScriptLoader::GetModuleLoader()
    {
        return Instance().impl->moduleLoader;
    }

    Falcon::Module* ScriptLoader::GetMainModule()
    {
        return Instance().impl->mainModule;
    }
}