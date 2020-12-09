#include "AngelScriptArcaBatch.h"

namespace Atmos::Scripting::Angel
{
    void Registration<GenericArcaBatch>::RegisterTo(asIScriptEngine& engine)
    {
        TemplateValueTypeRegistration<Type>(containingNamespace, "Batch", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .DefaultConstructor(&Management::UndefinedTemplate)
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine);
    }
}