
#include "AngelScriptTile.h"

#include "AngelScriptAssert.h"
#include <angelscript.h>

namespace Atmos
{
    namespace Scripting
    {
        Tile* Tile::Factory()
        {
            return new Tile();
        }

        void Tile::RegisterToAngelScript(asIScriptEngine* engine)
        {
            AngelScriptAssert(engine->RegisterObjectType("Tile", 0, asOBJ_REF));
            AngelScriptAssert(engine->RegisterObjectBehaviour(
                "Tile", asBEHAVE_FACTORY, "MyClass@ f()", asFUNCTION(Tile::Factory), asCALL_CDECL));
            AngelScriptAssert(engine->RegisterObjectBehaviour(
                "Tile", asBEHAVE_ADDREF, "void f()", asMETHOD(Tile, AddRef), asCALL_THISCALL));
            AngelScriptAssert(engine->RegisterObjectProperty("Tile", "GridPosition position", asOFFSET(Tile, position)));
        }
    }
}