
#include "AngelScriptGridPosition.h"

#include "AngelScriptRegistrationInterface.h"

#include "AngelScriptAssert.h"
#include <angelscript.h>

namespace Atmos::Scripting
{
    GridPosition::GridPosition() : x(0), y(0), z(0)
    {}

    void GridPosition::RegisterToAngelScript(asIScriptEngine* engine)
    {
        const char* className = "GridPosition";

        AngelScriptAssert(engine->RegisterObjectType(
            className, sizeof(GridPosition), asOBJ_VALUE));
        AngelScriptAssert(engine->RegisterObjectBehaviour(
            className, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(RegistrationInterface::GenerateValue<GridPosition>), asCALL_GENERIC));
        AngelScriptAssert(engine->RegisterObjectBehaviour(
            className, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(RegistrationInterface::DestructValue<GridPosition>), asCALL_GENERIC));
        AngelScriptAssert(engine->RegisterObjectProperty(
            className, "int x", asOFFSET(GridPosition, x)));
        AngelScriptAssert(engine->RegisterObjectProperty(
            className, "int y", asOFFSET(GridPosition, y)));
        AngelScriptAssert(engine->RegisterObjectProperty(
            className, "int z", asOFFSET(GridPosition, z)));
    }
}