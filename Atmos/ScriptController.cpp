
#include "ScriptController.h"

#include "FalconScriptUtility.h"
#include "Logger.h"

#include <falcon/engine.h>

namespace Atmos
{
    ScriptController::Piece::U::U() : instance(nullptr)
    {}

    ScriptController::Piece::U::U(Script::Instance *set) : instance(set)
    {}

    ScriptController::Piece::U::U(Script::Instance &&set) : owned(std::move(set))
    {}

    ScriptController::Piece::U::~U()
    {}

    ScriptController::Piece::Piece(Script::Instance &instance) : u(&instance), owns(false), justAdded(true), executedThisFrame(false), executeMain(true)
    {}

    ScriptController::Piece::Piece(Script::Instance &instance, const Script::SymbolName &executeSymbol, const Script::Instance::ItemVector &parameters) : u(&instance), owns(false), executeSymbol(executeSymbol), parameters(parameters), justAdded(true), executedThisFrame(false), executeMain(false)
    {}

    ScriptController::Piece::Piece(Script::Instance &&instance) : u(std::move(instance)), owns(true), justAdded(true), executeMain(true)
    {}

    ScriptController::Piece::Piece(Script::Instance &&instance, const Script::SymbolName &executeSymbol, const Script::Instance::ItemVector &parameters) : u(std::move(instance)), owns(true), executeSymbol(executeSymbol), parameters(parameters), justAdded(true), executedThisFrame(false), executeMain(false)
    {}

    ScriptController::Piece::Piece(Piece &&arg) : owns(std::move(arg.owns)), executeSymbol(std::move(arg.executeSymbol)), parameters(std::move(arg.parameters)), justAdded(std::move(arg.justAdded)), executedThisFrame(std::move(arg.executedThisFrame)), executeMain(std::move(arg.executeMain))
    {
        if (arg.owns)
            u.owned = std::move(arg.u.owned);
        else
            u.instance = arg.u.instance;
    }

    ScriptController::Piece& ScriptController::Piece::operator=(Piece &&arg)
    {
        owns = std::move(arg.owns);
        executeSymbol = std::move(arg.executeSymbol);
        parameters = std::move(arg.parameters);
        justAdded = std::move(arg.justAdded);
        executedThisFrame = std::move(arg.executedThisFrame);
        executeMain = std::move(arg.executeMain);
        if (arg.owns)
            u.owned = std::move(arg.u.owned);
        else
            u.instance = arg.u.instance;
        return *this;
    }

    Script::Instance* ScriptController::Piece::GetInstance()
    {
        return (owns) ? &u.owned : u.instance;
    }

    const Script::Instance* ScriptController::Piece::GetInstance() const
    {
        return (owns) ? &u.owned : u.instance;
    }

    ScriptController::ScriptController() : isWorking(false)
    {}

    void ScriptController::LaunchOrRunScript(Stack::iterator current)
    {
        bool error = false;
        try
        {
            if (current->justAdded)
            {
                current->justAdded = false;
                // Execute the scripts
                if (current->executeMain)
                    current->GetInstance()->GetVM()->launch();
                else
                    current->GetInstance()->GetVM()->launch(Fal::Convert(current->executeSymbol), current->parameters.size());
            }
            else
                current->GetInstance()->GetVM()->run();
        }
        catch (Falcon::Error *err)
        {
            // Output an error
            Logger::Log(String("The execution of a script has encountered an error.\n") + Falcon::AutoCString(err->toString()).c_str(),
                Logger::Type::ERROR_MODERATE,
                Logger::NameValueVector{ NameValuePair("Script File Name", current->GetInstance()->GetFileName().GetValue()) });
            error = true;
        }
        catch (const Falcon::VMEventQuit&)
        {

        }

        current->executedThisFrame = true;

        if (!error)
        {
            if (!current->GetInstance()->IsSuspended())
                Remove(*current->GetInstance());
        }
        else
        {
            if (current->GetInstance()->IsSuspended())
                Remove(*current->GetInstance());
        }
    }

    ScriptController::Stack::iterator ScriptController::Find(const Script::Instance &find)
    {
        auto &stack = Instance().stack;
        for (auto loop = stack.begin(); loop != stack.end(); ++loop)
        {
            if (loop->GetInstance() == &find)
                return loop;
        }

        return stack.end();
    }

    ScriptController::Stack::const_iterator ScriptController::FindConst(const Script::Instance &find)
    {
        auto &stack = Instance().stack;
        for (auto loop = stack.begin(); loop != stack.end(); ++loop)
        {
            if (loop->GetInstance() == &find)
                return loop;
        }

        return stack.end();
    }

    ScriptController& ScriptController::Instance()
    {
        static ScriptController instance;
        return instance;
    }

    void ScriptController::Add(Script::Instance &instance)
    {
        if (IsRunning(instance))
            return;

        Instance().stack.push_back(Piece(instance));
    }

    void ScriptController::Add(Script::Instance &instance, const Script::SymbolName &executeSymbol, const Script::Instance::ItemVector &parameters)
    {
        if (IsRunning(instance))
            return;

        Instance().stack.push_back(Piece(instance, executeSymbol, parameters));
    }

    void ScriptController::AddAndLaunch(Script::Instance &instance)
    {
        if (IsRunning(instance))
            return;

        Instance().stack.push_back(Piece(instance));
        Instance().current = --Instance().stack.end();
        LaunchOrRunScript(Instance().current);
    }

    void ScriptController::AddAndLaunch(Script::Instance &instance, const Script::SymbolName &executeSymbol, const Script::Instance::ItemVector &parameters)
    {
        if (IsRunning(instance))
            return;

        Instance().stack.push_back(Piece(instance, executeSymbol, parameters));
        Instance().current = --Instance().stack.end();
        LaunchOrRunScript(Instance().current);
    }

    void ScriptController::Remove(Script::Instance &instance)
    {
        auto &stack = Instance().stack;
        if (stack.empty())
            return;

        auto found = Find(instance);
        if (found == stack.end())
            return;

        if (Instance().isWorking)
        {
            // Push things into the deferred remove vector
            for (auto &loop : Instance().deferredRemove)
            {
                if (loop == found)
                    return;
            }

            Instance().deferredRemove.push_back(found);
        }
        else
            // Remove immediately
            stack.erase(found);
    }

    void ScriptController::Work()
    {
        Instance().isWorking = true;
        auto &stack = Instance().stack;
        auto &current = Instance().current;

        // Resume all of the stack
        current = stack.begin();
        while (current != stack.end())
        {
            auto next = current;
            ++next;

            if (!current->executedThisFrame && !current->GetInstance()->IsSuspended())
            {
                LaunchOrRunScript(current);
                current->executedThisFrame = true;
            }

            current = next;
        }

        current = stack.end();

        // Remove all scripts through deferred remove
        auto &deferredRemove = Instance().deferredRemove;
        for (auto &loop : deferredRemove)
            stack.erase(loop);
        deferredRemove.clear();

        for (auto &loop : stack)
            loop.executedThisFrame = false;

        Instance().isWorking = false;
    }

    size_t ScriptController::GetWorkedSize()
    {
        return Instance().stack.size();
    }

    bool ScriptController::IsRunning(const Script::Instance &check)
    {
        return Find(check) != Instance().stack.end();
    }

    Script::Instance* ScriptController::Current()
    {
        if (Instance().current == Instance().stack.end())
            return nullptr;

        return Instance().current->GetInstance();
    }
}