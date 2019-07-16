
#include "AngelScriptSystem.h"

#include "ObjectManager.h"
#include "LoggingSystem.h"

#include "AngelScriptUserData.h"
#include <angelscript.h>

#include "AngelScriptTile.h"
#include "AngelScriptGridPosition.h"
#include "AngelScriptEvent.h"

namespace Atmos::Scripting
{
    class System::Interface
    {
    public:
        asIScriptEngine* engine;
    public:
        Interface(System& owner, ObjectManager& objectManager);
    private:
        UserData userData;
        void SetAndPushUserData(System& owner, ObjectManager& objectManager);
    private:
        void MessageCallback(const asSMessageInfo* msg, void* param);
    private:
        ObjectManager* objectManager;
        LoggingSystem* FindLoggingSystem();
    };

    System::Interface::Interface(System& owner, ObjectManager& objectManager) : objectManager(&objectManager)
    {
        engine = asCreateScriptEngine();
        if (!engine)
            FindLoggingSystem()->Log("Creation of the scripting engine has failed.",
                LogType::ERROR_SEVERE);
        engine->SetMessageCallback(asMETHOD(Interface, MessageCallback), 0, asCALL_THISCALL);

        SetAndPushUserData(owner, objectManager);

        GridPosition::RegisterToAngelScript(engine);
        Tile::RegisterToAngelScript(engine);
        Event::RegisterToAngelScript(engine);
    }

    void System::Interface::SetAndPushUserData(System& owner, ObjectManager& objectManager)
    {
        userData.objectManager = &objectManager;
        userData.system = &owner;

        engine->SetUserData(&userData);
    }

    void System::Interface::MessageCallback(const asSMessageInfo* msg, void* param)
    {
        LogType logType = LogType::ERROR_SEVERE;
        if (msg->type == asMSGTYPE_WARNING)
            logType = LogType::WARNING;
        else if (msg->type == asMSGTYPE_INFORMATION)
            logType = LogType::INFORMATION;

        FindLoggingSystem()->Log(msg->message, logType,
            LogNameValueVector{
                NameValuePair("Section", String(msg->section)),
                NameValuePair("Row", msg->row),
                NameValuePair("Column", msg->col),
            });
    }

    LoggingSystem* System::Interface::FindLoggingSystem()
    {
        return objectManager->FindSystem<LoggingSystem>();
    }

    System::System(ObjectManager& manager) : ObjectSystem(manager), interface(std::make_unique<Interface>(*this, manager))
    {}

    asIScriptEngine* System::Engine()
    {
        return interface->engine;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Scripting::System, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}