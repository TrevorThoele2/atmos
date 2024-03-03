#include "AngelScriptEvent.h"

#include "AngelScriptRegistrationInterface.h"

#include <Arca/Reliquary.h>
#include "AngelScriptCurator.h"

#include "AngelScriptResultVerification.h"
#include <angelscript.h>

namespace Atmos::Script::Angel
{
    Event::Event(Arca::Reliquary& reliquary) : reliquary(&reliquary)
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

    void Event::RegisterTo(asIScriptEngine* engine)
    {
        VerifyResult(engine->RegisterFuncdef
        (
            "void EventFunction()"
        ));

        const char* className = "Event";

        VerifyResult(engine->RegisterObjectType
        (
            className,
            sizeof(Event),
            asOBJ_VALUE
        ));
        VerifyResult(engine->RegisterObjectBehaviour
        (
            className,
            asBEHAVE_CONSTRUCT,
            "void f()",
            asFUNCTION(RegistrationInterface::GenerateGenericValue<Event>),
            asCALL_GENERIC
        ));
        VerifyResult(engine->RegisterObjectBehaviour
        (
            className,
            asBEHAVE_DESTRUCT,
            "void f()",
            asFUNCTION(RegistrationInterface::DestructGenericValue<Event>),
            asCALL_GENERIC
        ));
        VerifyResult(engine->RegisterObjectMethod
        (
            className,
            "void Subscribe(EventFunction@ function)",
            asMETHOD(Event, Subscribe),
            asCALL_THISCALL
        ));
        VerifyResult(engine->RegisterObjectMethod
        (
            className,
            "void Unsubscribe(EventFunction@ function)",
            asMETHOD(Event, Unsubscribe),
            asCALL_THISCALL
        ));
        VerifyResult(engine->RegisterObjectMethod
        (
            className,
            "void Execute()",
            asMETHOD(Event, Execute),
            asCALL_THISCALL
        ));
    }

    asIScriptEngine* Event::Engine()
    {
        return reliquary->Find<ScriptCurator>().Engine();
    }
}