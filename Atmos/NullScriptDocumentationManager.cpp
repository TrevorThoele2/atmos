#include "NullScriptDocumentationManager.h"

namespace Atmos::Scripting
{
    void NullDocumentationManager::DocumentGlobalFunction(int funcID, String message)
    {}

    void NullDocumentationManager::DocumentInterface(int typeID, String message)
    {}

    void NullDocumentationManager::DocumentInterfaceMethod(int funcID, String message)
    {}

    void NullDocumentationManager::DocumentObject(int typeID, String message)
    {}

    void NullDocumentationManager::DocumentObjectMethod(int funcID, String message)
    {}
}