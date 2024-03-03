#pragma once

#include "Random.h"
#include "InputDefines.h"
#include "TimeHandler.h"
#include "FrameTimer.h"
#include "Currency.h"
#include "Sprite.h"
#include "FieldID.h"
#include "Speech.h"
#include "Pathfinding.h"
#include "Name.h"
#include "OffsetManagedList.h"
#include "Script.h"
#include "ActionID.h"
#include "ActionParameter.h"
#include "ModulatorController.h"

#include "Entity.h"
#include "SenseComponent.h"
#include "InventoryComponent.h"
#include "BattleComponent.h"

#include "GridPosition.h"

#include "ActionParameter.h"

namespace Atmos
{
    namespace Act
    {
        namespace Test
        {
            void Func(int myInt);
        }

        namespace Speech
        {
            void StartSpeech(const Name &name);
        }

        namespace Script
        {
            void CreateAndStart(const Name &name);
            void CreateAndStartGlobal(const Name &name, const std::vector<NameValuePair> &globals);
            void CreateAndStartExternal(const Name &name, const ::Atmos::Script::SymbolName &overrideExecuteName, const std::vector<NameValuePair> &overrideArguments);
            void CreateAndStartGlobalExternal(const Name &name, const std::vector<NameValuePair> &globals, const ::Atmos::Script::SymbolName &overrideExecuteName, const std::vector<NameValuePair> &overrideArguments);
        }

        namespace Field
        {
            void Change(FieldID id, const GridPosition &pos, unsigned char dir);
        }

        namespace Music
        {
            void Change(const String &name);
        }

        namespace Scheduler
        {
            void Schedule(::Atmos::Act::ID id, const std::vector<::Atmos::Act::ParameterValueSingle> &args, TimeValue::ValueT timeTaken);
        }
    }
}