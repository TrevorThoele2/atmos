
#include "ScriptLocator.h"

#include "ScriptController.h"

namespace Atmos
{
    ScriptLocator::ScriptLocator(const Script::Instance &owner) : owner(&owner)
    {}

    void* ScriptLocator::Find()
    {
        return FindImpl();
    }

    bool ScriptLocator::IsDone() const
    {
        return !ScriptController::IsRunning(*owner);
    }

    std::type_index ScriptLocator::GetType()
    {
        return GetTypeImpl();
    }

    ScriptLocatorManager& ScriptLocatorManager::Instance()
    {
        static ScriptLocatorManager instance;
        return instance;
    }

    void ScriptLocatorManager::Work()
    {
        for (auto typeLoop = Instance().typeMapping.begin(); typeLoop != Instance().typeMapping.end();)
        {
            for (auto locatorLoop = typeLoop->second.begin(); locatorLoop != typeLoop->second.end();)
            {
                if (locatorLoop->second->IsDone())
                    locatorLoop = typeLoop->second.Remove(locatorLoop);
                else
                    ++locatorLoop;
            }

            if (typeLoop->second.empty())
                typeLoop = Instance().typeMapping.erase(typeLoop);
            else
                ++typeLoop;
        }
    }

    ScriptLocatorManager::ID ScriptLocatorManager::Add(ScriptLocator &locator)
    {
        return Instance().typeMapping.emplace(locator.GetType(), ContainerT()).first->second.Add(LocatorPtr(&locator));
    }

    void ScriptLocatorManager::Remove(const std::type_index &type, ID id)
    {
        auto found = Instance().typeMapping.find(type);
        if (found == Instance().typeMapping.end())
            return;

        found->second.Remove(id);
    }
}