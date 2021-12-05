#include "JavaScriptEntityNamespace.h"

#include "JavaScriptEntity.h"
#include "JavaScriptFindEntityByName.h"
#include "JavaScriptFindEntityByPosition.h"
#include "JavaScriptMoveEntityTo.h"
#include "JavaScriptCanMoveEntityTo.h"
#include "JavaScriptModifyEntityTags.h"
#include "JavaScriptFindEntityPath.h"
#include "JavaScriptTraits.h"

#include "JavaScriptObject.h"
#include "JavaScriptFunction.h"
#include "JavaScriptUserData.h"

#include "CurrentActualizingEntity.h"

#include <Arca/Destroy.h>

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<EntityNamespace>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "currentEntity", CreateFunction(*isolate, data.context, &OnCurrentEntity) }
            });
    }

    v8::Local<v8::Value> Type<EntityNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "FindByName", ToV8(*isolate, ArcaTraits::From<Atmos::Entity::FindByName>()) },
                { "FindByPosition", ToV8(*isolate, ArcaTraits::From<Atmos::Entity::FindByPosition>()) },
                { "MoveTo", ToV8(*isolate, ArcaTraits::From<Atmos::Entity::MoveTo>()) },
                { "CanMoveTo", ToV8(*isolate, ArcaTraits::From<Atmos::Entity::CanMoveTo>()) },
                { "ModifyTags", ToV8(*isolate, ArcaTraits::From<Atmos::Entity::ModifyTags>()) },
                { "FindPath", ToV8(*isolate, ArcaTraits::From<Atmos::Entity::FindPath>()) },

                { "Entity", ToV8(*isolate, ArcaTraits::From<Atmos::Entity::Entity>()) },
                { "CreateEntity", ToV8(*isolate, ArcaTraits::From<Arca::Create<Atmos::Entity::Entity>>()) },
                { "DestroyEntity", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Atmos::Entity::Entity>>()) }
            });
    }
    
    void Type<EntityNamespace>::OnCurrentEntity(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();
        const auto userData = static_cast<UserData*>(isolate->GetData(0));

        const auto currentActualizingEntity = userData->reliquary->Find<Atmos::Entity::CurrentActualizing>();
        info.GetReturnValue().Set(JavaScript::ToV8(*isolate, currentActualizingEntity->entity));
    }
}