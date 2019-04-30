
#include "AngelScriptTile.h"

#include "AngelScriptRegistrationInterface.h"

#include "AngelScriptAssert.h"
#include <angelscript.h>

namespace Atmos
{
    namespace Scripting
    {
        Tile::Tile()
        {}

        void Tile::RegisterToAngelScript(asIScriptEngine* engine)
        {
            const char* className = "Tile";

            AngelScriptAssert(engine->RegisterObjectType(
                className, 0, asOBJ_REF));
            AngelScriptAssert(engine->RegisterObjectBehaviour(
                className, asBEHAVE_FACTORY, "MyClass@ f()", asFUNCTION(RegistrationInterface::GenerateReference<Tile>), asCALL_CDECL));
            AngelScriptAssert(engine->RegisterObjectBehaviour(
                className, asBEHAVE_ADDREF, "void f()", asMETHOD(Tile, AddRef), asCALL_THISCALL));
            AngelScriptAssert(engine->RegisterObjectProperty(
                className, "GridPosition position", asOFFSET(Tile, position)));
        }
    }
}