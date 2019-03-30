
#include "AngelScriptEvent.h"

#include "ScriptEngineManager.h"
#include "AngelScriptAssert.h"
#include <angelscript.h>

#include "CurrentField.h"

namespace Atmos
{
    namespace Scripting
    {
        Event::Event()
        {
            auto engine = Engine();
            context = engine->CreateContext();
        }

        Event::~Event()
        {
            for (auto loop = subscribedFunctions.begin(); loop != subscribedFunctions.end();)
            {
                (*loop)->Release();
                loop = subscribedFunctions.erase(loop);
            }
            context->Release();
        }

        void Event::Constructor(void* memory)
        {
            new(memory) Event();
        }

        void Event::Destructor(void* memory)
        {
            static_cast<Event*>(memory)->~Event();
        }

        void Event::Subscribe(asIScriptFunction* function)
        {
            subscribedFunctions.push_back(function);
        }

        void Event::Unsubscribe(asIScriptFunction* function)
        {
            for (auto loop = subscribedFunctions.begin(); loop != subscribedFunctions.end(); ++loop)
            {
                if (*loop == function)
                {
                    (*loop)->Release();
                    subscribedFunctions.erase(loop);
                    break;
                }
            }

            function->Release();
        }

        void Event::Execute()
        {
            for (auto& loop : subscribedFunctions)
            {
                context->Prepare(loop);
                context->Execute();
                context->Unprepare();
            }
        }

        void Event::RegisterToAngelScript(asIScriptEngine* engine)
        {
            AngelScriptAssert(engine->RegisterFuncdef("void EventFunction()"));

            const char* className = "Event";

            AngelScriptAssert(engine->RegisterObjectType(className, sizeof(Event), asOBJ_VALUE));
            AngelScriptAssert(engine->RegisterObjectBehaviour(
                className, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Event::Constructor), asCALL_CDECL_OBJLAST));
            AngelScriptAssert(engine->RegisterObjectBehaviour(
                className, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Event::Destructor), asCALL_CDECL_OBJLAST));
            AngelScriptAssert(engine->RegisterObjectMethod(
                className, "void Subscribe(EventFunction@ function)", asMETHOD(Event, Subscribe), asCALL_THISCALL));
            AngelScriptAssert(engine->RegisterObjectMethod(
                className, "void Unsubscribe(EventFunction@ function)", asMETHOD(Event, Unsubscribe), asCALL_THISCALL));
            AngelScriptAssert(engine->RegisterObjectMethod(
                className, "void Execute()", asMETHOD(Event, Execute), asCALL_THISCALL));
        }

        asIScriptEngine* Event::Engine()
        {
            return GetLocalObjectManager()->FindSystem<Scripting::EngineManager>()->Engine();
        }
    }
}