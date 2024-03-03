#include "AngelScriptScriptAsset.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Asset::Script>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type> registration(containingNamespace, name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Name>, "string", "Name", {})
            .Actualize(engine);

        Registration<ArcaTraits<Asset::Script>>::RegisterTo(engine);
        Registration<Arca::Batch<Asset::Script>>::RegisterTo(engine);
        Registration<Asset::FindByName<Asset::Script>>::RegisterTo(engine);
    }

    String Registration<Asset::Script>::Name(Type type)
    {
        return RequiredValue(type)->Name();
    }
}