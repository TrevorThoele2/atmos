
#include "ScriptEngineManager.h"

#include <angelscript.h>
#include "AngelScriptTile.h"
#include "AngelScriptGridPosition.h"

#include "Logger.h"

namespace Atmos
{
    namespace Scripting
    {
        void MessageCallback(const asSMessageInfo* msg, void* param)
        {
            Logger::Type logType = Logger::Type::ERROR_MODERATE;
            if (msg->type == asMSGTYPE_WARNING)
                logType = Logger::Type::WARNING;
            else if (msg->type == asMSGTYPE_INFORMATION)
                logType = Logger::Type::INFORMATION;

            Logger::Log(msg->message, logType,
                Logger::NameValueVector{
                    NameValuePair("Section", String(msg->section)),
                    NameValuePair("Row", msg->row),
                    NameValuePair("Column", msg->col),
                });
        }

        EngineManager::EngineManager(ObjectManager& manager) : ObjectSystem(manager)
        {
            engine = asCreateScriptEngine();
            if (!engine)
                Logger::Log("Creation of the scripting engine has failed.", Logger::Type::ERROR_SEVERE);
            engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

            Scripting::GridPosition::RegisterToAngelScript(engine);

            Scripting::Tile::RegisterToAngelScript(engine);
        }

        asIScriptEngine* EngineManager::Engine()
        {
            return engine;
        }
    }
}