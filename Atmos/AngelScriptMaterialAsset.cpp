#include "AngelScriptMaterialAsset.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"

#include <angelscript.h>
#include "AngelScriptEnumRegistration.h"

namespace Atmos::Scripting::Angel
{
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

    template<class T, auto doName, auto doPasses>
    void DoRegister(asIScriptEngine& engine)
    {
        using Type = Arca::Index<T>;

        ValueTypeRegistration<Type> registration(Registration<Type>::containingNamespace, Registration<Type>::name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&ObjectManagement<Type>::template Method<doName>, "string", "Name", {})
            .ConstMethod(&ObjectManagement<Type>::template Method<doPasses>, "MaterialPass[]@", "Passes", {})
            .Actualize(engine);

        Registration<ArcaTraits<T>>::RegisterTo(engine);
        Registration<Arca::Batch<T>>::RegisterTo(engine);
        Registration<Asset::FindByName<T>>::RegisterTo(engine);
    }

    void Registration<Asset::ImageMaterial>::RegisterTo(asIScriptEngine& engine)
    {
        DoRegister<Asset::ImageMaterial, &DoName, &DoPasses>(engine);
    }

    String Registration<Asset::ImageMaterial>::DoName(Type type)
    {
        return RequiredValue(type)->Name();
    }

    std::vector<Asset::Material::Pass> Registration<Asset::ImageMaterial>::DoPasses(Type type)
    {
        return RequiredValue(type)->Passes();
    }

    void Registration<Asset::LineMaterial>::RegisterTo(asIScriptEngine& engine)
    {
        DoRegister<Asset::LineMaterial, &DoName, &DoPasses>(engine);
    }

    String Registration<Asset::LineMaterial>::DoName(Type type)
    {
        return RequiredValue(type)->Name();
    }

    std::vector<Asset::Material::Pass> Registration<Asset::LineMaterial>::DoPasses(Type type)
    {
        return RequiredValue(type)->Passes();
    }

    void Registration<Asset::RegionMaterial>::RegisterTo(asIScriptEngine& engine)
    {
        DoRegister<Asset::RegionMaterial, &DoName, &DoPasses>(engine);
    }

    String Registration<Asset::RegionMaterial>::DoName(Type type)
    {
        return RequiredValue(type)->Name();
    }

    std::vector<Asset::Material::Pass> Registration<Asset::RegionMaterial>::DoPasses(Type type)
    {
        return RequiredValue(type)->Passes();
    }
}