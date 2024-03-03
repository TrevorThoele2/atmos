#include "DocumentationManager.h"

#include <Atmos/AngelScriptResultVerification.h>

DocumentationManager::DocumentationManager(DocumentationGenerator& generator) : generator(&generator)
{}

void DocumentationManager::DocumentGlobalFunction(int funcID, Atmos::String message)
{
    Atmos::Scripting::Angel::VerifyResult(generator->DocumentGlobalFunction(funcID, message.c_str()), {});
}

void DocumentationManager::DocumentInterface(int typeID, Atmos::String message)
{
    Atmos::Scripting::Angel::VerifyResult(generator->DocumentInterface(typeID, message.c_str()), {});
}

void DocumentationManager::DocumentInterfaceMethod(int funcID, Atmos::String message)
{
    Atmos::Scripting::Angel::VerifyResult(generator->DocumentInterfaceMethod(funcID, message.c_str()), {});
}

void DocumentationManager::DocumentObject(int typeID, Atmos::String message)
{
    Atmos::Scripting::Angel::VerifyResult(generator->DocumentObjectType(typeID, message.c_str()), {});
}

void DocumentationManager::DocumentObjectMethod(int funcID, Atmos::String message)
{
    Atmos::Scripting::Angel::VerifyResult(generator->DocumentObjectMethod(funcID, message.c_str()), {});
}