#include "AngelScriptGridPosition.h"

#include "AngelScriptRegistrationInterface.h"

#include "AngelScriptResultVerification.h"
#include <angelscript.h>

namespace Atmos::Script::Angel
{
    GridPosition::GridPosition() : x(0), y(0), z(0)
    {}

    void GridPosition::RegisterTo(asIScriptEngine* engine)
    {
        const char* className = "GridPosition";

        VerifyResult(engine->RegisterObjectType
        (
            className,
            sizeof(GridPosition),
            asOBJ_VALUE
        ));
        VerifyResult(engine->RegisterObjectBehaviour
        (
            className,
            asBEHAVE_CONSTRUCT,
            "void f()",
            asFUNCTION(RegistrationInterface::GenerateValue<GridPosition>),
            asCALL_GENERIC
        ));
        VerifyResult(engine->RegisterObjectBehaviour
        (
            className,
            asBEHAVE_DESTRUCT,
            "void f()",
            asFUNCTION(RegistrationInterface::DestructValue<GridPosition>),
            asCALL_GENERIC
        ));
        VerifyResult(engine->RegisterObjectProperty
        (
            className,
            "int x",
            asOFFSET(GridPosition, x)
        ));
        VerifyResult(engine->RegisterObjectProperty
        (
            className,
            "int y",
            asOFFSET(GridPosition, y)
        ));
        VerifyResult(engine->RegisterObjectProperty
        (
            className,
            "int z",
            asOFFSET(GridPosition, z)
        ));
    }
}