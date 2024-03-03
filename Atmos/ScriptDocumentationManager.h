#pragma once

#include "String.h"

namespace Atmos::Scripting
{
    class DocumentationManager
    {
    public:
        virtual ~DocumentationManager() = 0;

        virtual void DocumentGlobalFunction(int funcID, String message) = 0;
        virtual void DocumentInterface(int typeID, String message) = 0;
        virtual void DocumentInterfaceMethod(int funcID, String message) = 0;
        virtual void DocumentObject(int typeID, String message) = 0;
        virtual void DocumentObjectMethod(int funcID, String message) = 0;
    };
}