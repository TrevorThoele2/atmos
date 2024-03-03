#pragma once

#include "ScriptDocumentationManager.h"

namespace Atmos::Scripting
{
    class NullDocumentationManager final : public DocumentationManager
    {
    public:
        void DocumentGlobalFunction(int funcID, String message) override;
        void DocumentInterface(int typeID, String message) override;
        void DocumentInterfaceMethod(int funcID, String message) override;
        void DocumentObject(int typeID, String message) override;
        void DocumentObjectMethod(int funcID, String message) override;
    };
}