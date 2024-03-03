#include "AngelScriptTimePoint.h"

#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<GenericTimePoint>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(ContainingNamespace(), "Point", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }
}