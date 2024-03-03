#include "AngelScriptArcaAssign.h"

namespace Atmos::Scripting::Angel
{
    void Registration<GenericArcaAssignCopy>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(ContainingNamespace(), "AssignCopy", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }

    void Registration<GenericArcaAssignMove>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(ContainingNamespace(), "AssignMove", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }
}