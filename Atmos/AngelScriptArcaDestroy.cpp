#include "AngelScriptArcaDestroy.h"

namespace Atmos::Scripting::Angel
{
    void Registration<GenericArcaDestroy>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "Destroy", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }

    void Registration<Arca::Destroying>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
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
            .Actualize(engine, documentationManager);

        RegisterSignalHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }

    void Registration<GenericArcaDestroyingKnown>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "DestroyingKnown", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }
}