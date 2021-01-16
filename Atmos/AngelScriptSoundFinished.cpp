#include "AngelScriptSoundFinished.h"

#include "AngelScriptSignal.h"
#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Audio::SoundFinished>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, Arca::RelicID>>,
                { "Arca::RelicID id" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Actualize(engine, documentationManager);

        RegisterSignalHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}