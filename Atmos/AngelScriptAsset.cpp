#include "AngelScriptAsset.h"

namespace Atmos::Scripting::Angel
{
    void Registration<GenericAssetFindByName>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        TemplateValueTypeRegistration<Type>(ContainingNamespace(), "FindByName", { "T" })
            .TemplateCallback(&Management::RequiredSpecializationTemplateCallback)
            .Constructor(&Management::UndefinedTemplate, { "const string &in" })
            .CopyConstructor(&Management::UndefinedTemplate)
            .Destructor(&Management::UndefinedTemplate)
            .CopyAssignment(&Management::UndefinedTemplate)
            .Actualize(engine, documentationManager);
    }
}