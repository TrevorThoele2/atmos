#pragma once

#include <typeindex>

#include "Script.h"
#include "IDManager.h"

#include <Function\Function.h>

namespace Atmos
{
    class ScriptLocator
    {
    private:
        virtual void* FindImpl() = 0;

        virtual std::type_index GetTypeImpl() = 0;
    private:
        const Script::Instance *owner;
    protected:
        ScriptLocator(const Script::Instance &owner);
    public:
        void* Find();

        bool IsDone() const;
        std::type_index GetType();
    };

    class ScriptLocatorManager
    {
    private:
        typedef std::unique_ptr<ScriptLocator> LocatorPtr;
        typedef IDManager<std::unordered_map<IDManagerBase::ID, LocatorPtr>> ContainerT;
        typedef std::unordered_map<std::type_index, ContainerT> TypeMapping;
    public:
        typedef ContainerT::ID ID;
    private:
        TypeMapping typeMapping;

        ScriptLocatorManager() = default;
        ScriptLocatorManager(const ScriptLocatorManager &arg) = delete;
        ScriptLocatorManager& operator=(const ScriptLocatorManager &arg) = delete;
    public:
        static ScriptLocatorManager& Instance();

        static void Work();

        // Expects heap memory
        static ID Add(ScriptLocator &locator);
        static void Remove(const std::type_index &type, ID id);

        template<class T>
        static T* Find(ID id);
    };

    template<class T>
    T* ScriptLocatorManager::Find(ID id)
    {
        auto found = Instance().typeMapping.find(typeid(T));
        if (found == Instance().typeMapping.end())
            return nullptr;

        auto innerFound = found->second.Find(id);
        if (innerFound == found->second.end())
            return nullptr;

        return static_cast<T*>(innerFound->second->Find());
    }
}