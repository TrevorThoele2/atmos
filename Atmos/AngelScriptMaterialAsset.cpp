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

    template<class T, auto doName, auto doPasses>
    void DoRegister(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        using Type = Arca::Index<T>;

        ValueTypeRegistration<Type> registration(Registration<Type>::ContainingNamespace(), Registration<Type>::Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&ObjectManagement<Type>::template Method<doName>, "string", "Name", {})
            .ConstMethod(&ObjectManagement<Type>::template Method<doPasses>, "MaterialPass[]@", "Passes", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<T>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<T>>::RegisterTo(engine, documentationManager);
        Registration<Asset::FindByName<T>>::RegisterTo(engine, documentationManager);
    }

    void Registration<Asset::ImageMaterial>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        DoRegister<Asset::ImageMaterial, &DoName, &DoPasses>(engine, documentationManager);
    }

    String Registration<Asset::ImageMaterial>::DoName(Type type)
    {
        return RequiredValue(type)->Name();
    }

    std::vector<Asset::Material::Pass> Registration<Asset::ImageMaterial>::DoPasses(Type type)
    {
        return RequiredValue(type)->Passes();
    }

    void Registration<Asset::LineMaterial>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        DoRegister<Asset::LineMaterial, &DoName, &DoPasses>(engine, documentationManager);
    }

    String Registration<Asset::LineMaterial>::DoName(Type type)
    {
        return RequiredValue(type)->Name();
    }

    std::vector<Asset::Material::Pass> Registration<Asset::LineMaterial>::DoPasses(Type type)
    {
        return RequiredValue(type)->Passes();
    }

    void Registration<Asset::RegionMaterial>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        DoRegister<Asset::RegionMaterial, &DoName, &DoPasses>(engine, documentationManager);
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