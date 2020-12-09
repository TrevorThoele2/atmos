#include "AngelScriptImageAsset.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Asset::ImageSize>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Type::Dimension>,
                    &PullFromParameter<1, Type::Dimension>>,
                { "int width", "int height" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::width>("int", "width")
            .Property<&Type::height>("int", "height")
            .Actualize(engine);
    }

    void Registration<Asset::ImageGridSize>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Type::Dimension>,
                    &PullFromParameter<1, Type::Dimension>>,
                { "int columns", "int rows" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::columns>("int", "columns")
            .Property<&Type::rows>("int", "rows")
            .Actualize(engine);
    }

    void Registration<Asset::Image>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type> registration(containingNamespace, name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Name>, "string", "Name", {})
            .ConstMethod(&Management::Method<&Width>, "int", "Width", {})
            .ConstMethod(&Management::Method<&Height>, "int", "Height", {})
            .ConstMethod(&Management::Method<&Size>, "ImageSize", "Size", {})
            .ConstMethod(&Management::Method<&Columns>, "int", "Columns", {})
            .ConstMethod(&Management::Method<&Rows>, "int", "Rows", {})
            .ConstMethod(&Management::Method<&GridSize>, "ImageGridSize", "GridSize", {})
            .ConstMethod(&Management::Method<&Slice>, "Atmos::Spatial::AxisAlignedBox2D", "Slice", {"int index"})
            .ConstMethod(&Management::Method<&SliceSize>, "Atmos::Spatial::Size2D", "SliceSize", {})
            .Actualize(engine);

        Registration<ArcaTraits<Asset::Image>>::RegisterTo(engine);
        Registration<Arca::Batch<Asset::Image>>::RegisterTo(engine);
        Registration<Asset::FindByName<Asset::Image>>::RegisterTo(engine);
    }

    String Registration<Asset::Image>::Name(Type type)
    {
        return RequiredValue(type)->Name();
    }

    auto Registration<Asset::Image>::Width(Type type) -> Type::ValueT::Dimension
    {
        return RequiredValue(type)->Width();
    }

    auto Registration<Asset::Image>::Height(Type type) -> Type::ValueT::Dimension
    {
        return RequiredValue(type)->Height();
    }

    Asset::ImageSize Registration<Asset::Image>::Size(Type type)
    {
        return RequiredValue(type)->Size();
    }

    auto Registration<Asset::Image>::Columns(Type type) -> Type::ValueT::GridDimension
    {
        return RequiredValue(type)->Columns();
    }

    auto Registration<Asset::Image>::Rows(Type type) -> Type::ValueT::GridDimension
    {
        return RequiredValue(type)->Rows();
    }

    Asset::ImageGridSize Registration<Asset::Image>::GridSize(Type type)
    {
        return RequiredValue(type)->GridSize();
    }

    Spatial::AxisAlignedBox2D Registration<Asset::Image>::Slice(Type type, int index)
    {
        return RequiredValue(type)->Slice(index);
    }

    Spatial::Size2D Registration<Asset::Image>::SliceSize(Type type)
    {
        return RequiredValue(type)->SliceSize();
    }
}