#include "AngelScriptCurrentEntity.h"

#include "CurrentActualizingEntity.h"

#include <angelscript.h>
#include "AngelScriptGlobalRegistration.h"
#include "AngelScriptUserData.h"

namespace Atmos::Scripting::Angel
{
    void Registration<CurrentEntity>::RegisterTo(asIScriptEngine& engine)
    {
        GlobalRegistration(containingNamespace)
            .Function(&Management::Function<&Retrieve, &UserData::RequiredReliquaryFrom>, "Atmos::Entity::Entity", "CurrentEntity", {})
            .Actualize(engine);
    }

    Arca::Index<Entity::Entity> Registration<CurrentEntity>::Retrieve(Arca::Reliquary* reliquary)
    {
        const auto currentActualizingEntity = Arca::Index<Entity::CurrentActualizing>(reliquary);
        return currentActualizingEntity->entity;
    }
}