#pragma once

#include "AngelScriptUserData.h"
#include <angelscript.h>

namespace Atmos::Script::Angel
{
    class RegistrationInterface
    {
    public:
        template<class T>
        static void GenerateValue(void* memory);
        template<class T>
        static void GenerateGenericValue(asIScriptGeneric* gen);
        template<class T>
        static void DestructValue(void* memory);
        template<class T>
        static void DestructGenericValue(asIScriptGeneric* gen);
        template<class T>
        static T* GenerateReference();
        template<class T>
        static void GenerateGenericReference(asIScriptGeneric* gen);
    private:
        static UserData* UserDataFrom(asIScriptGeneric* gen);
    };

    template<class T>
    void RegistrationInterface::GenerateValue(void* memory)
    {
        new(memory) T();
    }

    template<class T>
    void RegistrationInterface::GenerateGenericValue(asIScriptGeneric* gen)
    {
        auto userData = UserDataFrom(gen);
        new(gen->GetAddressOfReturnLocation()) T(*userData->reliquary);
    }

    template<class T>
    void RegistrationInterface::DestructValue(void* memory)
    {
        static_cast<T*>(memory)->~T();
    }

    template<class T>
    void RegistrationInterface::DestructGenericValue(asIScriptGeneric* gen)
    {
        static_cast<T*>(gen->GetAddressOfReturnLocation())->~T();
    }

    template<class T>
    T* RegistrationInterface::GenerateReference()
    {
        return new T();
    }

    template<class T>
    void RegistrationInterface::GenerateGenericReference(asIScriptGeneric* gen)
    {
        auto userData = UserDataFrom(gen);
        *(T**)gen->GetAddressOfReturnLocation() = new T(*userData->objectManager);
    }
}