#include "AngelScriptArcaCreate.h"

namespace Atmos::Scripting::Angel
{
    void Registration<GenericArcaCreate>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(ContainingNamespace(), "Create", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }

    void Registration<GenericArcaCreateChild>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(ContainingNamespace(), "CreateChild", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }

    void Registration<GenericArcaIdentifiedCreate>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(ContainingNamespace(), "IdentifiedCreate", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }

    void Registration<GenericArcaIdentifiedCreateChild>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(ContainingNamespace(), "IdentifiedCreateChild", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }

    void Registration<Arca::Created>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
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

    void Registration<GenericArcaCreatedKnown>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(ContainingNamespace(), "CreatedKnown", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }
}