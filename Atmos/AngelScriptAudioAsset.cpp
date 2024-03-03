#include "AngelScriptAudioAsset.h"

#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptAsset.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Asset::Audio>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type> registration(containingNamespace, name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Name>, "string", "Name", {})
            .Actualize(engine);

        Registration<ArcaTraits<Asset::Audio>>::RegisterTo(engine);
        Registration<Arca::Batch<Asset::Audio>>::RegisterTo(engine);
        Registration<Asset::FindByName<Asset::Audio>>::RegisterTo(engine);
    }

    String Registration<Asset::Audio>::Name(Type type)
    {
        return RequiredValue(type)->Name();
    }
}