#include "AngelScriptArcaCreate.h"

namespace Atmos::Scripting::Angel
{
    void Registration<GenericArcaCreate>::RegisterTo(asIScriptEngine& engine)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "Create", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine);
    }

    void Registration<GenericArcaCreateChild>::RegisterTo(asIScriptEngine& engine)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "CreateChild", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine);
    }

    void Registration<GenericArcaIdentifiedCreate>::RegisterTo(asIScriptEngine& engine)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "IdentifiedCreate", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine);
    }

    void Registration<GenericArcaIdentifiedCreateChild>::RegisterTo(asIScriptEngine& engine)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "IdentifiedCreateChild", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine);
    }

    void Registration<Arca::Created>::RegisterTo(asIScriptEngine& engine)
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

    void Registration<GenericArcaCreatedKnown>::RegisterTo(asIScriptEngine& engine)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "CreatedKnown", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine);
    }
}