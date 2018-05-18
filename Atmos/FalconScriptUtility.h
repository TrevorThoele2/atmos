#pragma once

#include <vector>

#include "Script.h"
#include "ScriptController.h"

#include "String.h"
#include "Optional.h"
#include "GridPosition.h"
#include "Variant.h"
#include "FrameTimer.h"
#include "Entity.h"

#include "Error.h"

#include <falcon/item.h>
#include <falcon/carray.h>
#include <falcon/cclass.h>
#include <falcon/coreobject.h>

namespace Falcon
{
    class VMachine;
}

namespace Atmos
{
    namespace falcon
    {
        Falcon::String Convert(const String &arg);
        String Convert(const Falcon::String &arg);

        Falcon::Item* RetrieveItemFromVM(const std::string &name, Falcon::VMachine *vm);
        String& AddTracebackToString(Script::Instance &instance, String &string);

        void FatalScriptError(Script::Instance &instance, String &&string, ErrorHandler::Severity severity, ErrorHandler::NameValueVector &nameValueVector);
        void FatalScriptErrorParameterNotExist(Script::Instance &instance, const String &name);
        void FatalScriptErrorParameterNotExpectedType(Script::Instance &instance, const String &name, const String &expectedType, const String &givenType);
        void FatalScriptErrorParameterNotExpectedType(Script::Instance &instance, const String &name, const String &expectedType, Falcon::Item &item);

        template<class T>
        struct FalconVariableTraits;

        template<>
        struct FalconVariableTraits<bool>
        {
            typedef bool Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::uint8_t>
        {
            typedef std::uint8_t Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::uint16_t>
        {
            typedef std::uint16_t Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::uint32_t>
        {
            typedef std::uint32_t Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::uint64_t>
        {
            typedef std::uint64_t Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::int8_t>
        {
            typedef std::int8_t Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::int16_t>
        {
            typedef std::int16_t Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::int32_t>
        {
            typedef std::int32_t Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::int64_t>
        {
            typedef std::int64_t Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<double>
        {
            typedef double Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<String>
        {
            typedef String Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<class T>
        struct FalconVariableTraits<std::vector<T>>
        {
            typedef std::vector<T> Type;
            static constexpr bool Is(Falcon::Item &item) { return item.isArray(); }
            static Type FromItem(Falcon::Item &item)
            {
                Type ret;

                auto arr = item.asArray();
                for (Falcon::uint32 loop = 0; loop != arr->length(); ++loop)
                {
                    auto &test = (*arr)[loop];
                    auto &from = FalconVariableTraits<T>::FromItem(test);
                    ret.push_back(from);
                }

                return ret;
            }
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set) { item.setArray(&to); }

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set)
            {
                Falcon::CoreArray ar;
                for (auto &loop : set)
                    ar.append(FalconVariableTraits<T>::CreateItem(vm, loop));
                Falcon::Item item(&ar);
                SetItem(vm, Falcon::Item(&ar), set);
                return item;
            }
        };

        template<class T>
        const String FalconVariableTraits<std::vector<T>>::typeString("Vector");

        template<>
        struct FalconVariableTraits<Variant>
        {
            typedef Variant Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static const String falconTypeName;
            static const String valueName;
            static Optional<Type> Create(Falcon::VMachine &vm);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<LargeInteger>
        {
            typedef LargeInteger Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static const String typeString;

            static const String falconTypeName;
            static const String lowName;
            static const String highName;
            static Optional<Type> Create(Falcon::VMachine &vm);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<FixedPoint64>
        {
            typedef FixedPoint64 Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static const String typeString;

            static const String falconTypeName;
            static const String valueName;
            static Optional<Type> Create(Falcon::VMachine &vm);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<GridPosition>
        {
            typedef GridPosition Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static const String typeString;

            static const String falconTypeName;
            static const String xName;
            static const String yName;
            static const String zName;
            static Optional<Type> Create(Falcon::VMachine &vm);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<FrameTimer>
        {
            typedef FrameTimer Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static const String typeString;

            static const String falconTypeName;
            static const String startName;
            static const String goalName;
            static Optional<Type> Create(Falcon::VMachine &vm);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<Entity>
        {
            typedef Entity Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);

            static const String typeString;

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<class T>
        constexpr bool Is(Falcon::Item &item)
        {
            return FalconVariableTraits<T>::Is(item);
        }

        template<class T>
        T FromItem(Falcon::Item &item)
        {
            return FalconVariableTraits<T>::FromItem(item);
        }

        template<class T>
        Optional<T> FromItem(Falcon::VMachine &vm, const String &name)
        {
            typedef Optional<T> RetT;
            auto found = vm.findLocalSymbolItem(Convert(name));
            if (!found)
                return RetT();

            return FromItem<T>(*found);
        }

        template<class T>
        void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const T &set)
        {
            FalconVariableTraits<T>::SetItem(vm, item, set);
        }

        template<class T>
        Optional<T> Create(Falcon::VMachine &vm)
        {
            return FalconVariableTraits<T>::Create(vm);
        }

        template<class T>
        Falcon::Item CreateItem(Falcon::VMachine &vm, const T &set)
        {
            return FalconVariableTraits<T>::CreateItem(vm, set);
        }

        Falcon::Item* CheckedRetrieveFromFalcon(const String &name, Falcon::VMachine *vm);
        Falcon::Item* CheckedRetrieveFromFalcon(const String &name, Falcon::uint32 id, Falcon::VMachine *vm);

        // Checks if item actually exists and is the right type
        template<class T>
        T CheckedConversionFromFalcon(const String &name, Falcon::Item *item)
        {
            if (!item)
            {
                FatalScriptErrorParameterNotExist(*ScriptController::Current(), name);
                throw ScriptException();
            }
            else if (!FalconVariableTraits<T>::Is(*item))
            {
                Falcon::String typeString;
                item->typeName(typeString);
                FatalScriptErrorParameterNotExpectedType(*ScriptController::Current(), name, FalconVariableTraits<T>::typeString, Falcon::AutoCString(typeString).c_str());
                throw ScriptException();
            }

            return FromItem<T>(*item);
        }

        // Retrieves the parameter from inside of the vm
        // Checks if item actually exists and is the right type
        template<class T>
        T CheckedConversionFromFalcon(const String &name, Falcon::VMachine *vm)
        {
            return CheckedConversionFromFalcon<T>(name, RetrieveItemFromVM(name, vm));
        }

        // Retrieves the parameter from inside of the vm
        // Checks if item actually exists and is the right type
        template<class T>
        T CheckedConversionFromFalcon(const String &name, Falcon::uint32 id, Falcon::VMachine *vm)
        {
            return CheckedConversionFromFalcon<T>(name, vm->param(id));
        }

        // Checks if item actually exists and is the right type
        // If it doesn't exist, will throw an exception
        // If it does exist but is not the right type, will set it to def
        template<class T, class Def>
        T CheckedConversionFromFalcon(const String &name, Falcon::Item *item, const Def &def)
        {
            if (!item)
            {
                FatalScriptErrorParameterNotExist(*ScriptController::Current(), name);
                throw ScriptException();
            }
            else if (!FalconVariableTraits<T>::Is(*item))
                return def;

            return ConvertFromFalcon(FalconVariableTraits<T>::FromItem(*item));
        }

        // Retrieves the parameter from inside of the vm
        // Checks if item actually exists and is the right type
        template<class T, class Def>
        T CheckedConversionFromFalcon(const String &name, Falcon::VMachine *vm, const Def &def)
        {
            return CheckedConversionFromFalcon<T>(name, RetrieveItemFromVM(name, vm), def);
        }

        // Retrieves the parameter from inside of the vm
        // Checks if item actually exists and is the right type
        template<class T, class Def>
        T CheckedConversionFromFalcon(const String &name, Falcon::uint32 id, Falcon::VMachine *vm, const Def &def)
        {
            return CheckedConversionFromFalcon<T>(name, vm->param(id), def);
        }

        // Checks if item actually exists and is the right type
        template<class T>
        std::vector<T> CheckedVectorConversionFromFalconAll(const String &name, Falcon::Item *item)
        {
            typedef FalconVariableTraits<std::vector<T>> VectorTraits;

            if (!item)
            {
                FatalScriptErrorParameterNotExist(*ScriptController::Current(), name);
                throw ScriptException();
            }
            else if (!VectorTraits::Is(*item))
            {
                FatalScriptErrorParameterNotExpectedType(*ScriptController::Current(), name, VectorTraits::typeString, *item);
                throw ScriptException();
            }

            return VectorTraits::FromItem(*item);
        }

        // Retrieves the parameter from inside of the vm
        // Checks if item actually exists and is the right type
        template<class T>
        std::vector<T> CheckedVectorConversionFromFalconAll(const String &name, Falcon::VMachine *vm)
        {
            return CheckedVectorConversionFromFalconAll<T>(name, RetrieveItemFromVM(name, vm));
        }

        // Retrieves the parameter from inside of the vm
        // Checks if item actually exists and is the right type
        template<class T>
        std::vector<T> CheckedVectorConversionFromFalconAll(const String &name, Falcon::uint32 id, Falcon::VMachine *vm)
        {
            return CheckedVectorConversionFromFalconAll<T>(name, vm->param(id));
        }

        template<class T, class Def>
        void CheckedItemPropertySet(Falcon::Item &set, const String &propertyName, Falcon::Item *setAs, bool(Falcon::Item::*isFunc)() const, T(Falcon::Item::*asFunc)() const, Def def)
        {
            if (!setAs || !(setAs->*isFunc)())
                set.setProperty(Convert(propertyName), def);
            else
                set.setProperty(Convert(propertyName), (setAs->*asFunc)());
        }

        template<class T, class Ret, class Set, class Def>
        void CheckedItemSet(Falcon::Item *set, Falcon::Item *setAs, bool(Falcon::Item::*isFunc)() const, T(Falcon::Item::*asFunc)() const, Ret(Falcon::Item::*setFunc)(Set), const Def &def)
        {
            if (!setAs || !(setAs->*isFunc)())
                (set->*setFunc)(def);
            else
                (set->*setFunc)((setAs->*asFunc)());
        }

        template<class T, class Ret, class Set, class Def>
        void CheckedItemSet(const String &set, const String &setAs, Falcon::VMachine *vm, bool(Falcon::Item::*isFunc)() const, T(Falcon::Item::*asFunc)() const, Ret(Falcon::Item::*setFunc)(Set), const Def &def)
        {
            CheckedItemSet(RetrieveItemFromVM(set, vm), RetrieveItemFromVM(setAs, vm), isFunc, asFunc, setFunc, def);
        }

        // Returns if encountering a ScriptException
        template<class Func, class... Args>
        void DoAction(Func func, Args && ... args)
        {
            try
            {
                (*func)(std::forward<Args>(args)...);
            }
            catch (const ScriptException&)
            {
                return;
            }
        }

        template<class T>
        void BroadcastEvent(const String &eventName, T &t)
        {
            for (auto &loop : runningScripts)
            {
                auto &vm = loop->GetVM();
                auto broadcastItem = vm->findLocalSymbolItem("broadcast");
                if (!broadcastItem)
                    continue;

                auto message = new Falcon::VMMessage(Convert(eventName));
                message->addParam(ConvertToFalcon(t));

                vm->postMessage(message);
            }
        }
    }
}