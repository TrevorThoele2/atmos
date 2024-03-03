#include "AngelScriptFindByBox.h"

#include "AngelScriptScript.h"
#include "AngelScriptCommand.h"
#include "AngelScriptObjectRegistration.h"
#include "Bounds.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::FindImagesByBox>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(&Management::GenerateValue<
                &PullFromParameter<0, Spatial::AxisAlignedBox3D>, &PullFromParameter<1, Spatial::Space>>,
                { "Atmos::Spatial::AxisAlignedBox3D box", "Atmos::Spatial::Space space" })
            .Constructor(&Management::GenerateValue<
                &PullFromParameter<0, Spatial::AxisAlignedBox2D>, &PullFromParameter<1, Spatial::Space>>,
                { "Atmos::Spatial::AxisAlignedBox2D box", "Atmos::Spatial::Space space" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::box>("Atmos::Spatial::AxisAlignedBox3D", "box")
            .Property<&Type::space>("Atmos::Spatial::Space", "space")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine, documentationManager);
    }

    std::vector<Arca::RelicID> Registration<Render::FindImagesByBox>::ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&)
    {
        return fromArca;
    }

    void Registration<Render::FindLinesByBox>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, Spatial::AxisAlignedBox3D>>,
                { "Atmos::Spatial::AxisAlignedBox3D box" })
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, Spatial::AxisAlignedBox2D>>,
                { "Atmos::Spatial::AxisAlignedBox2D box" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::box>("Atmos::Spatial::AxisAlignedBox3D", "box")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine, documentationManager);
    }

    std::vector<Arca::RelicID> Registration<Render::FindLinesByBox>::ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&)
    {
        return fromArca;
    }

    void Registration<Render::FindGridRegionsByBox>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, Spatial::AxisAlignedBox3D>>,
                { "Atmos::Spatial::AxisAlignedBox3D box" })
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, Spatial::AxisAlignedBox2D>>,
                { "Atmos::Spatial::AxisAlignedBox2D box" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::box>("Atmos::Spatial::AxisAlignedBox3D", "box")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&Chroma::Identity<Type>, &ToCommandReturn>(engine, documentationManager);
    }

    std::vector<Arca::RelicID> Registration<Render::FindGridRegionsByBox>::ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&)
    {
        return fromArca;
    }
}