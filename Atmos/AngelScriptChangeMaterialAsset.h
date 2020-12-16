#pragma once

#include "ChangeMaterialAsset.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"
#include "AngelScriptCommand.h"

#include "String.h"

#include <Chroma/Identity.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<class Material>
    struct ChangeMaterialAssetTraits;

    template<>
    struct ChangeMaterialAssetTraits<Asset::ImageMaterial>
    {
        static inline String nameToken = "Image";
    };

    template<>
    struct ChangeMaterialAssetTraits<Asset::LineMaterial>
    {
        static inline String nameToken = "Line";
    };

    template<>
    struct ChangeMaterialAssetTraits<Asset::RegionMaterial>
    {
        static inline String nameToken = "Region";
    };

    template<class Material>
    struct Registration<Render::ChangeMaterialAsset<Material>>
    {
        using Type = Render::ChangeMaterialAsset<Material>;

        using Traits = ChangeMaterialAssetTraits<Material>;

        static inline const String name = "Change" + Traits::nameToken + "MaterialAsset";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine);
    };

    template<class Material>
    void Registration<Render::ChangeMaterialAsset<Material>>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .Constructor(
                &Management::template GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Arca::Index<Material>>>,
                { "Arca::RelicID id", "Atmos::Asset::" + Traits::nameToken + "Material to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .template Property<&Type::id>("Arca::RelicID", "id")
            .template Property<&Type::to>("Atmos::Asset::" + Traits::nameToken + "Material", "to")
            .Actualize(engine);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine);
    }
}