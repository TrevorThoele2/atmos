#include "AngelScriptTile.h"

#include "AngelScriptRegistrationInterface.h"

#include "AngelScriptResultVerification.h"
#include <angelscript.h>

namespace Atmos::Script::Angel
{
    Tile::Tile()
    {}

    void Tile::RegisterTo(asIScriptEngine* engine)
    {
        const char* className = "Tile";

        VerifyResult(engine->RegisterObjectType
        (
            className,
            0,
            asOBJ_REF
        ));
        VerifyResult(engine->RegisterObjectBehaviour
        (
            className,
            asBEHAVE_FACTORY,
            "MyClass@ f()",
            asFUNCTION(RegistrationInterface::GenerateReference<Tile>),
            asCALL_CDECL
        ));
        VerifyResult(engine->RegisterObjectBehaviour
        (
            className,
            asBEHAVE_ADDREF,
            "void f()",
            asMETHOD(Tile, AddRef),
            asCALL_THISCALL
        ));
        VerifyResult(engine->RegisterObjectProperty
        (
            className,
            "GridPosition position",
            asOFFSET(Tile, position)
        ));
    }
}