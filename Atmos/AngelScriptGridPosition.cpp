
#include "AngelScriptGridPosition.h"

#include "AngelScriptAssert.h"
#include <angelscript.h>

namespace Atmos
{
    namespace Scripting
    {
        void GridPosition::Constructor(void* memory)
        {
            new(memory) GridPosition();
        }

        void GridPosition::Destructor(void* memory)
        {
            static_cast<GridPosition*>(memory)->~GridPosition();
        }

        void GridPosition::RegisterToAngelScript(asIScriptEngine* engine)
        {
            AngelScriptAssert(engine->RegisterObjectType("GridPosition", sizeof(GridPosition), asOBJ_VALUE));
            AngelScriptAssert(engine->RegisterObjectBehaviour(
                "GridPosition", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(GridPosition::Constructor), asCALL_CDECL_OBJLAST));
            AngelScriptAssert(engine->RegisterObjectBehaviour(
                "GridPosition", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(GridPosition::Destructor), asCALL_CDECL_OBJLAST));
            AngelScriptAssert(engine->RegisterObjectProperty("GridPosition", "int x", asOFFSET(GridPosition, x)));
            AngelScriptAssert(engine->RegisterObjectProperty("GridPosition", "int y", asOFFSET(GridPosition, y)));
            AngelScriptAssert(engine->RegisterObjectProperty("GridPosition", "int z", asOFFSET(GridPosition, z)));
        }
    }
}