#include "AngelScriptArcaDestroy.h"

namespace Atmos::Scripting::Angel
{
    void Registration<GenericArcaDestroy>::RegisterTo(asIScriptEngine& engine)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "Destroy", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine);
    }

    void Registration<Arca::Destroying>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, Arca::Handle>>,
                { "Arca::Handle handle" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::handle>("Arca::Handle", "handle")
            .Actualize(engine);

        RegisterSignalHandler<&Chroma::Identity<Type>>(engine);
    }

    void Registration<GenericArcaDestroyingKnown>::RegisterTo(asIScriptEngine& engine)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "DestroyingKnown", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine);
    }
}