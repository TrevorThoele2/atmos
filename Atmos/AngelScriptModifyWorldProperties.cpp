#include "AngelScriptModifyWorldProperties.h"

#include "AngelScriptScript.h"
#include "AngelScriptCommand.h"
#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<World::ModifyProperties>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, std::vector<Property>>,
                    &PullFromParameter<1, std::vector<String>>,
                    &PullFromParameter<2, std::vector<Property>>>,
                { "Atmos::Property[]@ add", "string[]@ remove", "Atmos::Property[]@ replace" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::add>("Atmos::Property[]@", "add")
            .Property<&Type::remove>("string[]@", "remove")
            .Property<&Type::replace>("Atmos::Property[]@", "replace")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}