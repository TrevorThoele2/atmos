#include "AngelScriptCurator.h"

#include <Arca/Reliquary.h>

#include "AngelScriptUserData.h"
#include <angelscript.h>

#include "AngelScriptTile.h"
#include "AngelScriptGridPosition.h"
#include "AngelScriptEvent.h"

#include "Log.h"

namespace Atmos::Script::Angel
{
    class ScriptCurator::Interface
    {
    public:
        asIScriptEngine* engine;
    public:
        Interface(ScriptCurator& owner, Arca::Reliquary& reliquary);
    private:
        UserData userData;
        void SetAndPushUserData(ScriptCurator& owner, Arca::Reliquary& reliquary);
    private:
        void MessageCallback(const asSMessageInfo* messageInfo, void* param);
    private:
        Arca::Reliquary* reliquary;
    };

    ScriptCurator::Interface::Interface(ScriptCurator& owner, Arca::Reliquary& reliquary) :
        reliquary(&reliquary)
    {
        engine = asCreateScriptEngine();
        if (!engine)
            reliquary.Raise<Log::Log>
            (
                "Creation of the scripting engine has failed.", Log::Severity::SevereError
            );
        engine->SetMessageCallback(asMETHOD(Interface, MessageCallback), nullptr, asCALL_THISCALL);

        SetAndPushUserData(owner, reliquary);

        GridPosition::RegisterTo(engine);
        Tile::RegisterTo(engine);
        Event::RegisterTo(engine);
    }

    void ScriptCurator::Interface::SetAndPushUserData(ScriptCurator& owner, Arca::Reliquary& reliquary)
    {
        userData.reliquary = &reliquary;
        userData.curator = &owner;

        engine->SetUserData(&userData);
    }

    void ScriptCurator::Interface::MessageCallback(const asSMessageInfo* messageInfo, void* param)
    {
        auto logType = Log::Severity::SevereError;
        if (messageInfo->type == asMSGTYPE_WARNING)
            logType = Log::Severity::Warning;
        else if (messageInfo->type == asMSGTYPE_INFORMATION)
            logType = Log::Severity::Information;

        reliquary->Raise<Log::Log>
        (
            messageInfo->message,
            logType,
            Log::NameValuePairs
            {
                NameValuePair("Section", String(messageInfo->section)),
                NameValuePair("Row", messageInfo->row),
                NameValuePair("Column", messageInfo->col),
            }
        );
    }

    asIScriptEngine* ScriptCurator::Engine()
    {
        return interface->engine;
    }

    void ScriptCurator::InitializeImplementation()
    {
        interface = std::make_unique<Interface>(*this, Owner());
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Script::Angel::ScriptCurator>::typeName = "AngelScriptCurator";
}