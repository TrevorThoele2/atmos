#include "AngelScriptMaterialAsset.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"

#include <angelscript.h>
#include "AngelScriptEnumRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Asset::Material::Pass>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .ConstMethod(&Management::Method<&Type::VertexShader>, "Shader", "VertexShader", {})
            .ConstMethod(&Management::Method<&Type::FragmentShader>, "Shader", "FragmentShader", {})
            .Actualize(engine, documentationManager);
    }
    
    void Registration<Asset::Material>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&ObjectManagement<Type>::Method<&DoName>, "string", "Name", {})
            .ConstMethod(&ObjectManagement<Type>::Method<&DoPasses>, "MaterialPass[]@", "Passes", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Asset::Material>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Asset::Material>>::RegisterTo(engine, documentationManager);
        Registration<Asset::FindByName<Asset::Material>>::RegisterTo(engine, documentationManager);
    }

    String Registration<Asset::Material>::DoName(Type type)
    {
        return RequiredValue(type)->Name();
    }

    std::vector<Asset::Material::Pass> Registration<Asset::Material>::DoPasses(Type type)
    {
        return RequiredValue(type)->Passes();
    }
}