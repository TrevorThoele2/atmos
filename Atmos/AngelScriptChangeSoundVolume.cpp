#include "AngelScriptChangeSoundVolume.h"

#include "AngelScriptScript.h"
#include "AngelScriptCommand.h"
#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Audio::ChangeSoundVolume>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, Arca::RelicID>, &PullFromParameter<1, Audio::Volume>>,
                { "Arca::RelicID id", "float to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("float", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}