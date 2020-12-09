#include "ScriptManager.h"

namespace Atmos::Scripting
{
    void Manager::Initialize(Arca::Reliquary& reliquary)
    {
        if (isInitialized)
            return;

        isInitialized = true;
        InitializeImpl(reliquary);
    }

    Manager::~Manager() = default;
}