#include "AngelScriptMaterialAsset.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"

#include <angelscript.h>
#include "AngelScriptEnumRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Asset::MaterialType>::RegisterTo(asIScriptEngine& engine)
    {
        EnumRegistration<Type>(containingNamespace, name)
            .Value("Image", Type::Image)
            .Value("Line", Type::Line)
            .Value("Region", Type::Region)
            .Actualize(engine);
    }

    void Registration<Asset::Material::Pass>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .ConstMethod(&Management::Method<&Type::VertexShader>, "Shader", "VertexShader", {})
            .ConstMethod(&Management::Method<&Type::FragmentShader>, "Shader", "FragmentShader", {})
            .Actualize(engine);
    }

    void Registration<Asset::Material>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type> registration(containingNamespace, name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&DoName>, "string", "Name", {})
            .ConstMethod(&Management::Method<&DoType>, "MaterialType", "Type", {})
            .ConstMethod(&Management::Method<&DoPasses>, "MaterialPass[]@", "Passes", {})
            .Actualize(engine);

        Registration<ArcaTraits<Asset::Material>>::RegisterTo(engine);
        Registration<Arca::Batch<Asset::Material>>::RegisterTo(engine);
        Registration<Asset::FindByName<Asset::Material>>::RegisterTo(engine);
    }

    String Registration<Asset::Material>::DoName(Type type)
    {
        return RequiredValue(type)->Name();
    }

    Asset::MaterialType Registration<Asset::Material>::DoType(Type type)
    {
        return RequiredValue(type)->Type();
    }

    std::vector<Asset::Material::Pass> Registration<Asset::Material>::DoPasses(Type type)
    {
        return RequiredValue(type)->Passes();
    }
}