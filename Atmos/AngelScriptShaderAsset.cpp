#include "AngelScriptShaderAsset.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Asset::Shader>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(containingNamespace, name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Name>, "string", "Name", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Asset::Shader>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Asset::Shader>>::RegisterTo(engine, documentationManager);
        Registration<Asset::FindByName<Asset::Shader>>::RegisterTo(engine, documentationManager);
    }

    String Registration<Asset::Shader>::Name(Type type)
    {
        return RequiredValue(type)->Name();
    }
}