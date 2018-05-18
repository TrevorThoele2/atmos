
#include "ScriptLoader.h"

#include "ScriptController.h"
#include "FalconScriptUtility.h"

#include "Enum.h"
#include "Error.h"
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

#include <falcon/engine.h>

namespace Atmos
{
    using namespace falcon;

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
            try { ErrorHandler::Log(CheckedConversionFromFalcon<String>("message", vm),
                ErrorHandler::Severity::ERROR_LOW); }
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
            vm->retval(CreateItem<FrameTimer>(*vm, Act::Time::GetFrameTime()));
        }

        FALCON_FUNC FrameTimerConstructor(Falcon::VMachine *vm)
        {
            CheckedItemPropertySet(vm->self(), FalconVariableTraits<FrameTimer>::goalName, RetrieveItemFromVM("goal", vm), &Falcon::Item::isOrdinal, &Falcon::Item::forceNumeric, 0.0);
            RetrieveItemFromVM(FalconVariableTraits<FrameTimer>::startName, vm)->setInteger(0);
        }

        FALCON_FUNC FrameTimer_Start(Falcon::VMachine *vm)
        {
            SetItem<FrameTimer>(*vm, *RetrieveItemFromVM(FalconVariableTraits<FrameTimer>::startName, vm), Act::Time::GetFrameTime());
        }

        FALCON_FUNC FrameTimer_Elapsed(Falcon::VMachine *vm)
        {
            typedef FalconVariableTraits<TimeValue::ValueT> WrappedTraits;
            auto goal = WrappedTraits::FromItem(*vm->findLocalSymbolItem(Convert(FalconVariableTraits<FrameTimer>::goalName)));
            auto start = WrappedTraits::FromItem(*vm->findLocalSymbolItem(Convert(FalconVariableTraits<FrameTimer>::startName)));
            TimeValue now(Act::Time::GetFrameTime());

            vm->retval(WrappedTraits::CreateItem(*vm, (now.Get() - start) / goal));
        }

        FALCON_FUNC FrameTimer_SetGoal(Falcon::VMachine *vm)
        {
            CheckedItemSet(FalconVariableTraits<FrameTimer>::goalName, "set", vm, &Falcon::Item::isNumeric, &Falcon::Item::asNumeric, &Falcon::Item::setNumeric, 0.0);
        }

        FALCON_FUNC FrameTimer_HasReachedGoal(Falcon::VMachine *vm)
        {
            typedef FalconVariableTraits<TimeValue::ValueT> WrappedTraits;
            auto goal = WrappedTraits::FromItem(*vm->findLocalSymbolItem(Convert(FalconVariableTraits<FrameTimer>::goalName)));
            auto start = WrappedTraits::FromItem(*vm->findLocalSymbolItem(Convert(FalconVariableTraits<FrameTimer>::startName)));
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
            if (!position || !Is<GridPosition>(*position))
                position = nullptr;

            if (!position)
                vm->retval(::Atmos::Ent::PositionSystem::CanMove(entity));
            else
                vm->retval(::Atmos::Ent::PositionSystem::CanMove(entity, FromItem<GridPosition>(*position)));
        }

        FALCON_FUNC MoveEntity(Falcon::VMachine *vm)
        {
            // Requires 3 arguments
            // The first argument MUST be an entity
            // The second argument MUST be a grid position
            // The third argument MUST be a LargeInteger

            Entity entity = Ent::nullEntity;
            GridPosition position;
            TimeValue::ValueT timeTaken;
            try
            {
                entity = CheckedConversionFromFalcon<Entity>("entity", vm);
                position = CheckedConversionFromFalcon<GridPosition>("position", vm);
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
                    ErrorHandler::Severity::ERROR_LOW,
                    ErrorHandler::NameValueVector{ NameValuePair("Entity ID", ToString(entity)) });
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
                    ErrorHandler::Severity::ERROR_LOW,
                    ErrorHandler::NameValueVector{ NameValuePair("Entity ID", ToString(entity)) });
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
                    ErrorHandler::Severity::ERROR_LOW,
                    ErrorHandler::NameValueVector{ NameValuePair("Entity ID", ToString(entity)) });
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
            auto position = FromItem<GridPosition>(*vm, "position");
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
            auto other = FromItem<GridPosition>(*vm, "other");
            if (!other)
                return;

            auto &self = FromItem<GridPosition>(vm->self());

            vm->retval(CreateItem(*vm, self.FindDistance(*other)));
        }

        FALCON_FUNC FindPath(Falcon::VMachine *vm)
        {
            auto positionFrom = FromItem<GridPosition>(*vm, "positionFrom");
            if (!positionFrom)
                return;

            auto positionTo = FromItem<GridPosition>(*vm, "positionTo");
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

    namespace battleScript
    {
        FALCON_FUNC FindClosestPlayer(Falcon::VMachine *vm)
        {
            auto position = FromItem<GridPosition>(*vm, "position");
            if (!position)
                return;

            vm->retval(CreateItem(*vm, *::Atmos::Act::Battle::FindClosestPlayer(*position)));
        }

        FALCON_FUNC FindClosestMonster(Falcon::VMachine *vm)
        {
            auto position = FromItem<GridPosition>(*vm, "position");
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

        // General purpose
        {
            mainModule->addExtFunc("atmos_Suspend", GeneralScript::Suspend);
            mainModule->addExtFunc("atmos_InstantiateScript", GeneralScript::InstantiateScript);
        }

        // Error
        {
            mainModule->addExtFunc("atmos_LogError", ErrorScript::LogError)->addParam("message");
        }

        // Random
        {
            mainModule->addExtFunc("atmos_GetRandomBool", RandomScript::GetRandomBool)->addParam("probability");
            mainModule->addExtFunc("atmos_GetRandomInteger", RandomScript::GetRandomInteger)->addParam("floor")->addParam("ceiling")->addParam("signed");
        }

        // Input
        {
            mainModule->addExtFunc("atmos_IsActionActive", inputScript::IsActionActive)->addParam("id");
            mainModule->addExtFunc("atmos_IsActionPressed", inputScript::IsActionPressed)->addParam("id");
            mainModule->addExtFunc("atmos_IsActionDepressed", inputScript::IsActionDepressed)->addParam("id");

            auto actionAdder = [&](Falcon::Symbol *symbol, Input::ActionID id, const String &name)
            {
                mainModule->addClassProperty(symbol, name.c_str()).setInteger(ConvertToUnderlyingType(id));
            };

            Falcon::Symbol *focusedSymbol = mainModule->addClass("atmos_Action");
            for (auto &loop : Environment::GetInput()->GetActions())
                actionAdder(focusedSymbol, loop->id, loop->displayName);
        }

        // Time
        {
            auto typeAdder = [&](Falcon::Symbol *symbol, TimeValueEpoch t, const String &name)
            {
                mainModule->addClassProperty(symbol, name.c_str()).setInteger(ConvertToUnderlyingType(t));
            };

            mainModule->addExtFunc("atmos_GetFrameTimeRaw", timeScript::GetFrameTimeRaw);

            // Setup frame timer
            auto cls = mainModule->addClass(Convert(FalconVariableTraits<FrameTimer>::falconTypeName), &timeScript::FrameTimerConstructor)
                ->addParam("goal");
            mainModule->addClassProperty(cls, Convert(FalconVariableTraits<FrameTimer>::startName));
            mainModule->addClassProperty(cls, Convert(FalconVariableTraits<FrameTimer>::goalName));
            mainModule->addClassMethod(cls, "Start", &timeScript::FrameTimer_Start);
            mainModule->addClassMethod(cls, "Elapsed", &timeScript::FrameTimer_Elapsed);
            mainModule->addClassMethod(cls, "SetGoal", &timeScript::FrameTimer_SetGoal).asSymbol()->addParam("set");
            mainModule->addClassMethod(cls, "HasReachedGoal", &timeScript::FrameTimer_HasReachedGoal);
        }

        // Speech
        {
            mainModule->addExtFunc("atmos_SetSpeechCharacters", speechScript::SetCharacters)->addParam("output");
            mainModule->addExtFunc("atmos_AppendSpeechCharacters", speechScript::AppendCharacters)->addParam("output");
            mainModule->addExtFunc("atmos_ClearSpeechCharacters", speechScript::ClearCharacters);
            mainModule->addExtFunc("atmos_GetSpeechCharacters", speechScript::GetCharacters);
            mainModule->addExtFunc("atmos_LeaveSpeech", speechScript::LeaveSpeech);
            mainModule->addExtFunc("atmos_EnterShop", speechScript::EnterShop)->addParam("buying");
            mainModule->addExtFunc("atmos_IsShopActive", speechScript::IsShopActive);

            mainModule->addExtFunc("atmos_ActivateSpeechInput", speechScript::ActivateInput)->addParam("strings");
            mainModule->addExtFunc("atmos_DeactivateSpeechInput", speechScript::DeactivateInput);
            mainModule->addExtFunc("atmos_GetSpeechInputPosition", speechScript::GetInputPosition);
        }

        // Entities
        {
            mainModule->addExtFunc("atmos_CanMoveEntity", entityScript::CanMoveEntity)->addParam("entity")->addParam("position");
            mainModule->addExtFunc("atmos_MoveEntity", entityScript::MoveEntity)->addParam("entity")->addParam("position");
            mainModule->addExtFunc("atmos_MoveEntityDirection", entityScript::MoveEntityDirection)->addParam("entity")->addParam("direction");

            mainModule->addExtFunc("atmos_GetEntityTalking", entityScript::GetEntityTalking);

            mainModule->addExtFunc("atmos_GetAvatarGold", entityScript::GetAvatarGold);
            mainModule->addExtFunc("atmos_AddEntityToParty", entityScript::AddEntityToParty)->addParam("entity");

            mainModule->addExtFunc("atmos_GetEntityName", &entityScript::GetEntityName)->addParam("entity");
            mainModule->addExtFunc("atmos_GetEntityPosition", &entityScript::GetEntityPosition)->addParam("entity");
            mainModule->addExtFunc("atmos_GetEntityWithName", &entityScript::GetEntityWithName)->addParam("name");
            mainModule->addExtFunc("atmos_GetEntitiesAtPosition", &entityScript::GetEntitiesAtPosition)->addParam("position");
            mainModule->addExtFunc("atmos_GetItemCount", entityScript::GetItemCount)->addParam("entity");
            mainModule->addExtFunc("atmos_IsEntityInParty", entityScript::IsEntityInParty)->addParam("entity");

            mainModule->addExtFunc("atmos_GetEntityPersistent", entityScript::GetPersistent)->addParam("entity")->addParam("persistentName");
            mainModule->addExtFunc("atmos_StoreEntityPersistent", entityScript::StorePersistent)->addParam("entity")->addParam("persistentName")->addParam("value");
        }

        // Position
        {
            mainModule->addExtFunc("atmos_FindPath", positionScript::FindPath)->addParam("positionFrom")->addParam("positionTo");

            // Setup grid position
            auto cls = mainModule->addClass(Convert(FalconVariableTraits<GridPosition>::falconTypeName), &positionScript::TilePositionConstructor)
                ->addParam("x")
                ->addParam("y")
                ->addParam("z");
            mainModule->addClassProperty(cls, Convert(FalconVariableTraits<GridPosition>::xName));
            mainModule->addClassProperty(cls, Convert(FalconVariableTraits<GridPosition>::yName));
            mainModule->addClassProperty(cls, Convert(FalconVariableTraits<GridPosition>::zName));
            mainModule->addClassMethod(cls, "FindDistance", &positionScript::TilePosition_Distance).asSymbol()->addParam("other");

            // Setup direction class
            {
                typedef ::Atmos::Direction::ValueT Value;
                cls = mainModule->addClass("atmos_Direction");

                auto directionSetup = [&](const char *name, Value value)
                {
                    mainModule->addClassProperty(cls, name).setInteger(value).setReadOnly(true);
                };

                directionSetup("Up", Value::UP);
                directionSetup("Down", Value::DOWN);
                directionSetup("Left", Value::LEFT);
                directionSetup("Right", Value::RIGHT);
            }
        }

        // Battle
        {
            mainModule->addExtFunc("atmosBattle_FindClosestPlayer", &battleScript::FindClosestPlayer)->addParam("position");
            mainModule->addExtFunc("atmosBattle_FindClosestMonster", &battleScript::FindClosestMonster)->addParam("position");
            mainModule->addExtFunc("atmosBattle_GetStartingPosition", &battleScript::GetStartingPosition)->addParam("entity");
            mainModule->addExtFunc("atmosBattle_GetMovementRange", &battleScript::GetMovementRange)->addParam("entity");
            mainModule->addExtFunc("atmosBattle_IsTurnEnded", &battleScript::IsTurnEnded)->addParam("entity");

            mainModule->addExtFunc("atmosBattle_Attack", &battleScript::Attack)->addParam("source")->addParam("target");
            mainModule->addExtFunc("atmosBattle_EndTurn", &battleScript::EndTurn)->addParam("entity");
        }
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