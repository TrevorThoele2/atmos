#include "AngelScriptScript.h"

#include "AngelScriptScriptAsset.h"
#include "AngelScriptDatum.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Script>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Data>, "Atmos::Datum[]@", "Data", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Script>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Script>>::RegisterTo(engine, documentationManager);

        RegisterArcaCreateRelic<
            Script,
                Chroma::VariadicTemplate<
                Arca::Index<Asset::Script>,
                String,
                std::vector<Variant>>>
        (
            { "Atmos::Asset::Script asset", "string executeName", "Atmos::Variant[]@ parameters" },
            engine,
            documentationManager);
        
        RegisterArcaDestroyRelic<Script>(engine, documentationManager);
    }

    std::vector<Datum> Registration<Script>::Data(Type type)
    {
        return RequiredValue(type)->data;
    }

    void Registration<ModifyData>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::Index<Script>>,
                    &PullFromParameter<1, std::vector<Datum>>,
                    &PullFromParameter<2, std::vector<String>>,
                    &PullFromParameter<3, std::vector<Datum>>>,
                { "Atmos::Scripting::Script script", "Atmos::Datum[]@ add", "string[]@ remove", "Atmos::Datum[]@ replace" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::script>("Atmos::Scripting::Script", "script")
            .Property<&Type::add>("Atmos::Datum[]@", "add")
            .Property<&Type::remove>("string[]@", "remove")
            .Property<&Type::replace>("Atmos::Datum[]@", "replace")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}