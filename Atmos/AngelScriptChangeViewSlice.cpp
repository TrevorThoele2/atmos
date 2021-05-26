#include "AngelScriptChangeViewSlice.h"

namespace Atmos::Scripting::Angel
{
    void Registration<ChangeViewSliceBox>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, Arca::RelicID>,
                &PullFromParameter<1, Spatial::AxisAlignedBox2D>>,
                { "Arca::RelicID id", "Atmos::Spatial::AxisAlignedBox2D to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Spatial::AxisAlignedBox2D", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::ChangeViewSlice Registration<ChangeViewSliceBox>::ToArca(Type fromAngelScript)
    {
        return Render::ChangeViewSlice{ fromAngelScript.id, fromAngelScript.to};
    }
}