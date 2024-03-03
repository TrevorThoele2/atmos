#pragma once

#include <Atmos/ScriptDocumentationManager.h>
#include "docgen.h"

class DocumentationManager final : public Atmos::Scripting::DocumentationManager
{
public:
    DocumentationManager(DocumentationGenerator& generator);

    void DocumentGlobalFunction(int funcID, Atmos::String message) override;
    void DocumentInterface(int typeID, Atmos::String message) override;
    void DocumentInterfaceMethod(int funcID, Atmos::String message) override;
    void DocumentObject(int typeID, Atmos::String message) override;
    void DocumentObjectMethod(int funcID, Atmos::String message) override;
private:
    DocumentationGenerator* generator;
};