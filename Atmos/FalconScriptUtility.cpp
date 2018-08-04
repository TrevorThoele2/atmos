
#include "FalconScriptUtility.h"

#include "Script.h"
#include "ScriptController.h"
#include "Logger.h"

#include <Function/Iterate.h>
#include <falcon/engine.h>

namespace Atmos
{
    namespace Fal
    {
        Falcon::String Convert(const String &arg)
        {
            return Falcon::String(arg.c_str());
        }

        String Convert(const Falcon::String &arg)
        {
            return String(Falcon::AutoCString(arg).c_str());
        }

        Falcon::Item* RetrieveItemFromVM(const std::string &name, Falcon::VMachine *vm)
        {
            using Falcon::Symbol;
            using Falcon::SymbolTable;
            using Falcon::Item;
            using Falcon::VMachine;

            // parse self and sender
            if (name == "self")
                return &vm->self();

            // find the symbol
            const Symbol *sym = vm->currentSymbol();
            if (sym != nullptr)
            {
                // get the relevant symbol table.
                const SymbolTable *symtab;
                switch (sym->type())
                {
                case Symbol::tclass:
                    symtab = &sym->getClassDef()->symtab();
                    break;
                case Symbol::tfunc:
                    symtab = &sym->getFuncDef()->symtab();
                    break;
                case Symbol::textfunc:
                    symtab = sym->getExtFuncDef()->parameters();
                    break;
                default:
                    symtab = nullptr;
                }

                if (symtab != nullptr)
                    sym = symtab->findByName(name.c_str());
                else
                    sym = nullptr; // try again
            }

            // -- not a local symbol? -- try the global module table.
            if (sym == nullptr)
            {
                sym = vm->currentModule()->findGlobalSymbol(name.c_str());
                // still zero? Let's try the global symbol table.
                if (sym == nullptr)
                {
                    Item *item = vm->findGlobalItem(name.c_str());
                    if (item != nullptr)
                        return item->dereference();
                }
            }

            Item *item = nullptr;
            if (sym != nullptr)
            {
                Item *found = nullptr;
                if (sym->isLocal())
                    found = vm->local(sym->getItemId());
                else if (sym->isParam())
                    found = vm->param(sym->getItemId());
                else
                    item = &vm->moduleItem(sym->getItemId());

                if (found)
                    item = found->dereference();
            }

            // if the item is zero, we didn't found it
            return item;
        }

        String AddTracebackToString(Falcon::VMachine &vm, const String &string)
        {
            String ret(string);
            Falcon::SyntaxError error;
            vm.fillErrorContext(&error);
            ret.append(String("\n") + Falcon::AutoCString(error.toString()).c_str());
            return ret;
        }

        void FatalScriptError(String &&message, Logger::Type severity)
        {
            auto &instance = *ScriptController::Current();
            Logger::Log(AddTracebackToString(*instance.GetVM().vm(), message),
                severity);
            instance.ForceQuit();
        };

        void FatalScriptError(String &&message, Logger::Type severity, Logger::NameValueVector &nameValueVector)
        {
            auto &instance = *ScriptController::Current();
            Logger::Log(AddTracebackToString(*instance.GetVM().vm(), message),
                severity,
                nameValueVector);
            instance.ForceQuit();
        };

        bool FalconVariableTraits<bool>::Is(Falcon::Item &item)
        {
            return item.isBoolean();
        }

        FalconVariableTraits<bool>::Type FalconVariableTraits<bool>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return item.asBoolean();
        }

        void FalconVariableTraits<bool>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setBoolean(set);
        }

        void FalconVariableTraits<bool>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setBool(set);
        }

        String FalconVariableTraits<bool>::GetTypeString()
        {
            return "Bool";
        }

        Falcon::Item FalconVariableTraits<bool>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<std::uint8_t>::Is(Falcon::Item &item)
        {
            return item.isInteger();
        }

        FalconVariableTraits<std::uint8_t>::Type FalconVariableTraits<std::uint8_t>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return static_cast<Type>(item.asInteger());
        }

        void FalconVariableTraits<std::uint8_t>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setInteger(set);
        }

        void FalconVariableTraits<std::uint8_t>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setInteger(set);
        }

        String FalconVariableTraits<std::uint8_t>::GetTypeString()
        {
            return "UInt8";
        }

        Falcon::Item FalconVariableTraits<std::uint8_t>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<std::uint16_t>::Is(Falcon::Item &item)
        {
            return item.isInteger();
        }

        FalconVariableTraits<std::uint16_t>::Type FalconVariableTraits<std::uint16_t>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return static_cast<Type>(item.asInteger());
        }

        void FalconVariableTraits<std::uint16_t>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setInteger(set);
        }

        void FalconVariableTraits<std::uint16_t>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setInteger(set);
        }

        String FalconVariableTraits<std::uint16_t>::GetTypeString()
        {
            return "UInt16";
        }

        Falcon::Item FalconVariableTraits<std::uint16_t>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<std::uint32_t>::Is(Falcon::Item &item)
        {
            return item.isInteger(); 
        }

        FalconVariableTraits<std::uint32_t>::Type FalconVariableTraits<std::uint32_t>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return static_cast<Type>(item.asInteger());
        }

        void FalconVariableTraits<std::uint32_t>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setInteger(set);
        }

        void FalconVariableTraits<std::uint32_t>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setInteger(set);
        }

        String FalconVariableTraits<std::uint32_t>::GetTypeString()
        {
            return "UInt32";
        }

        Falcon::Item FalconVariableTraits<std::uint32_t>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<std::uint64_t>::Is(Falcon::Item &item)
        {
            return item.isInteger();
        }

        FalconVariableTraits<std::uint64_t>::Type FalconVariableTraits<std::uint64_t>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return item.asInteger();
        }

        void FalconVariableTraits<std::uint64_t>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setInteger(set);
        }

        void FalconVariableTraits<std::uint64_t>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setInteger(set);
        }

        String FalconVariableTraits<std::uint64_t>::GetTypeString()
        {
            return "UInt64";
        }

        Falcon::Item FalconVariableTraits<std::uint64_t>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<std::int8_t>::Is(Falcon::Item &item)
        {
            return item.isInteger();
        }

        FalconVariableTraits<std::int8_t>::Type FalconVariableTraits<std::int8_t>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return static_cast<Type>(item.asInteger());
        }

        void FalconVariableTraits<std::int8_t>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setInteger(set);
        }

        void FalconVariableTraits<std::int8_t>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setInteger(set);
        }

        String FalconVariableTraits<std::int8_t>::GetTypeString()
        {
            return "Int8";
        }

        Falcon::Item FalconVariableTraits<std::int8_t>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<std::int16_t>::Is(Falcon::Item &item)
        {
            return item.isInteger();
        }

        FalconVariableTraits<std::int16_t>::Type FalconVariableTraits<std::int16_t>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return static_cast<Type>(item.asInteger());
        }

        void FalconVariableTraits<std::int16_t>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setInteger(set);
        }

        void FalconVariableTraits<std::int16_t>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setInteger(set);
        }

        String FalconVariableTraits<std::int16_t>::GetTypeString()
        {
            return "Int16";
        }

        Falcon::Item FalconVariableTraits<std::int16_t>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<std::int32_t>::Is(Falcon::Item &item)
        {
            return item.isInteger();
        }

        FalconVariableTraits<std::int32_t>::Type FalconVariableTraits<std::int32_t>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return static_cast<Type>(item.asInteger());
        }

        void FalconVariableTraits<std::int32_t>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setInteger(set);
        }

        void FalconVariableTraits<std::int32_t>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setInteger(set);
        }

        String FalconVariableTraits<std::int32_t>::GetTypeString()
        {
            return "Int32";
        }

        Falcon::Item FalconVariableTraits<std::int32_t>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<std::int64_t>::Is(Falcon::Item &item)
        {
            return item.isInteger();
        }

        FalconVariableTraits<std::int64_t>::Type FalconVariableTraits<std::int64_t>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return item.asInteger();
        }

        void FalconVariableTraits<std::int64_t>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setInteger(set);
        }

        void FalconVariableTraits<std::int64_t>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setInteger(set);
        }

        String FalconVariableTraits<std::int64_t>::GetTypeString()
        {
            return "Int64";
        }

        Falcon::Item FalconVariableTraits<std::int64_t>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        const FalconVariableTraits<float>::Type FalconVariableTraits<float>::DefaultValue = 0.0f;

        bool FalconVariableTraits<float>::Is(Falcon::Item &item)
        {
            return item.isNumeric();
        }

        FalconVariableTraits<float>::Type FalconVariableTraits<float>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return static_cast<float>(item.asNumeric());
        }

        void FalconVariableTraits<float>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setNumeric(set);
        }

        void FalconVariableTraits<float>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setNumeric(set);
        }

        String FalconVariableTraits<float>::GetTypeString()
        {
            return "Float";
        }

        Falcon::Item FalconVariableTraits<float>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        const FalconVariableTraits<double>::Type FalconVariableTraits<double>::DefaultValue = 0.0;

        bool FalconVariableTraits<double>::Is(Falcon::Item &item)
        {
            return item.isNumeric() || item.isInteger();
        }

        FalconVariableTraits<double>::Type FalconVariableTraits<double>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            if (item.isNumeric())
                return item.asNumeric();
            else
                return static_cast<double>(item.asInteger());
        }

        void FalconVariableTraits<double>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setNumeric(set);
        }

        void FalconVariableTraits<double>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setNumeric(set);
        }

        String FalconVariableTraits<double>::GetTypeString()
        {
            return "Double";
        }

        Falcon::Item FalconVariableTraits<double>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        const FalconVariableTraits<String>::Type FalconVariableTraits<String>::DefaultValue;

        bool FalconVariableTraits<String>::Is(Falcon::Item &item)
        {
            return item.isString();
        }

        FalconVariableTraits<String>::Type FalconVariableTraits<String>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return Type(Falcon::AutoCString(*item.asString()).c_str());
        }

        void FalconVariableTraits<String>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setString(&Convert(set));
        }

        void FalconVariableTraits<String>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setString(mod.addString(Convert(set)));
        }

        String FalconVariableTraits<String>::GetTypeString()
        {
            return "String";
        }

        Falcon::Item FalconVariableTraits<String>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        /*
        bool FalconVariableTraits<Variant>::Is(Falcon::Item &item)
        {
            return item.isObject() && item.asObject()->generator()->symbol()->name() == "atmos_Variant";
        }

        template<VariantIterationTraits::UnderlyingType t>
        struct VariantCreateConverter
        {
            // Gets a variant from a Falcon item
            static Optional<Variant> Check(Falcon::Item &item)
            {
                typedef Optional<Variant> RetT;
                typedef VariantTraits<static_cast<Variant::Type>(t)> VariantTraitsT;
                typedef FalconVariableTraits<typename VariantTraitsT::T> FalconTraits;
                if (!FalconTraits::Is(item))
                    return RetT();

                return RetT(FalconTraits::FromItem(item));
            }

            // Gets a falcon item from a variant
            static bool Check(const Variant &var, Falcon::VMachine &vm, Falcon::Item &item)
            {
                typedef VariantTraits<static_cast<Variant::Type>(t)> VariantTraitsT;
                typedef FalconVariableTraits<typename VariantTraitsT::T> FalconTraits;
                if (!FalconTraits::Is(item))
                    return false;

                FalconTraits::SetItem(vm, item, var.As<static_cast<Variant::Type>(t)>());
                return true;
            }
        };

        FalconVariableTraits<Variant>::Type FalconVariableTraits<Variant>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            typedef Optional<Type> OpT;
            auto made = ::function::IterateRangeCheckStop<VariantIterationTraits::UnderlyingType, VariantCreateConverter, OpT, VariantIterationTraits::maxU>(OpT(), item);
            if (!made)
                return Type();
            else
                return *made;
        }

        void FalconVariableTraits<Variant>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            if (item.asObject()->generator()->symbol()->name() != Convert(falconTypeName))
                item.setObject(vm.findGlobalItem(Convert(falconTypeName))->asClass()->createInstance());

            Falcon::Item valueProp;
            item.getProperty(Convert(valueName), valueProp);
            ::function::IterateRangeCheckStop<VariantIterationTraits::UnderlyingType, VariantCreateConverter, bool, VariantIterationTraits::maxU>(false, set, vm, valueProp);
        }

        void FalconVariableTraits<Variant>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {

        }

        String FalconVariableTraits<Variant>::GetTypeString()
        {
            return "Variant";
        }

        const String FalconVariableTraits<Variant>::falconTypeName("atmos_Variant");
        const String FalconVariableTraits<Variant>::valueName("value");

        Optional<FalconVariableTraits<Variant>::Type> FalconVariableTraits<Variant>::Create(Falcon::VMachine &vm)
        {
            typedef Optional<Type> OpT;
            auto value = vm.findLocalSymbolItem(Convert(valueName));
            if (!value)
                return OpT();

            return ::function::IterateRangeCheckStop<VariantIterationTraits::UnderlyingType, VariantCreateConverter, OpT, VariantIterationTraits::maxU>(OpT(), *value);
        }

        Falcon::Item FalconVariableTraits<Variant>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item(vm.findGlobalItem(falconTypeName.c_str())->asClass()->createInstance());
            SetItem(vm, item, set);
            return item;
        }
        */

        bool FalconVariableTraits<LargeInteger>::Is(Falcon::Item &item)
        {
            return item.isObject() && item.asObject()->generator()->symbol()->name() == "atmos_LargeInteger";
        }

        FalconVariableTraits<LargeInteger>::Type FalconVariableTraits<LargeInteger>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            Falcon::Item low;
            Falcon::Item high;
            item.getProperty("low", low);
            item.getProperty("high", high);
            return Type(low.asInteger(), high.asInteger());
        }

        void FalconVariableTraits<LargeInteger>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            if (item.asObject()->generator()->symbol()->name() != Convert(falconTypeName))
                item.setObject(vm.findGlobalItem(Convert(falconTypeName))->asClass()->createInstance());

            item.setProperty(Convert(lowName), set.GetLow());
            item.setProperty(Convert(highName), set.GetHigh());
        }

        void FalconVariableTraits<LargeInteger>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {

        }

        String FalconVariableTraits<LargeInteger>::GetTypeString()
        {
            return "LargeInteger";
        }

        const String FalconVariableTraits<LargeInteger>::falconTypeName("atmos_LargeInteger");
        const String FalconVariableTraits<LargeInteger>::lowName("low");
        const String FalconVariableTraits<LargeInteger>::highName("high");

        Optional<FalconVariableTraits<LargeInteger>::Type> FalconVariableTraits<LargeInteger>::Create(Falcon::VMachine &vm)
        {
            typedef Optional<Type> RetT;

            auto low = vm.findLocalSymbolItem(Convert(lowName));
            auto high = vm.findLocalSymbolItem(Convert(highName));
            if (!low || !high)
                return RetT();

            typedef FalconVariableTraits<Type::WrappedT> WrappedTraits;
            if (!WrappedTraits::Is(*low) || !WrappedTraits::Is(*high))
                return RetT();

            return RetT(Type(WrappedTraits::FromItem(vm, *low), WrappedTraits::FromItem(vm, *high)));
        }

        Falcon::Item FalconVariableTraits<LargeInteger>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item(vm.findGlobalItem(falconTypeName.c_str())->asClass()->createInstance());
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<FixedPoint64>::Is(Falcon::Item &item)
        {
            return item.isObject() && item.asObject()->generator()->symbol()->name() == "atmos_FixedPoint64";
        }

        FalconVariableTraits<FixedPoint64>::Type FalconVariableTraits<FixedPoint64>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            Falcon::Item value;
            item.getProperty("value", value);
            return Type(value.asInteger());
        }

        void FalconVariableTraits<FixedPoint64>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            if (item.asObject()->generator()->symbol()->name() != Convert(falconTypeName))
                item.setObject(vm.findGlobalItem(Convert(falconTypeName))->asClass()->createInstance());

            item.setProperty(Convert(valueName), set.GetRawValue());
        }

        void FalconVariableTraits<FixedPoint64>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {

        }

        String FalconVariableTraits<FixedPoint64>::GetTypeString()
        {
            return "FixedPoint64";
        }

        const String FalconVariableTraits<FixedPoint64>::falconTypeName("atmos_FixedPoint64");
        const String FalconVariableTraits<FixedPoint64>::valueName("value");

        Optional<FalconVariableTraits<FixedPoint64>::Type> FalconVariableTraits<FixedPoint64>::Create(Falcon::VMachine &vm)
        {
            typedef Optional<Type> RetT;

            auto value = vm.findLocalSymbolItem(Convert(valueName));
            if (!value)
                return RetT();

            typedef FalconVariableTraits<Type::ValueT> ValueTraits;
            if (!ValueTraits::Is(*value))
                return RetT();

            return RetT(Type(ValueTraits::FromItem(vm, *value)));
        }

        Falcon::Item FalconVariableTraits<FixedPoint64>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item(vm.findGlobalItem(falconTypeName.c_str())->asClass()->createInstance());
            SetItem(vm, item, set);
            return item;
        }

        bool FalconVariableTraits<FrameTimer>::Is(Falcon::Item &item)
        {
            return item.isObject() && item.asObject()->generator()->symbol()->name() == "Atmos_FrameTimer";
        }

        FalconVariableTraits<FrameTimer>::Type FalconVariableTraits<FrameTimer>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            Falcon::Item start;
            Falcon::Item goal;
            item.getProperty(Convert(startName), start);
            item.getProperty(Convert(goalName), goal);
            Type timer(TimeValue::ValueT(goal.asInteger()));
            timer.SetStart(TimeValue::ValueT(start.asInteger()));
            return timer;
        }

        void FalconVariableTraits<FrameTimer>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            if (item.asObject()->generator()->symbol()->name() != Convert(falconTypeName))
                item.setObject(vm.findGlobalItem(Convert(falconTypeName))->asClass()->createInstance());

            Falcon::Item startProp;
            item.getProperty(Convert(startName), startProp);
            FalconVariableTraits<FixedPoint64>::SetItem(vm, startProp, set.GetStart().Get());

            Falcon::Item goalProp;
            item.getProperty(Convert(startName), goalProp);
            FalconVariableTraits<FixedPoint64>::SetItem(vm, goalProp, set.GetGoal().Get());
        }

        const String FalconVariableTraits<FrameTimer>::typeString("FrameTimer");
        const String FalconVariableTraits<FrameTimer>::falconTypeName("Atmos_FrameTimer");
        const String FalconVariableTraits<FrameTimer>::startName("_start");
        const String FalconVariableTraits<FrameTimer>::goalName("_goal");

        Optional<FalconVariableTraits<FrameTimer>::Type> FalconVariableTraits<FrameTimer>::Create(Falcon::VMachine &vm)
        {
            typedef Optional<Type> RetT;

            auto start = vm.findLocalSymbolItem(Convert(startName));
            auto goal = vm.findLocalSymbolItem(Convert(goalName));

            if (!start || !goal)
                return RetT();

            typedef FalconVariableTraits<TimeValue::ValueT> WrappedTraits;
            if (!WrappedTraits::Is(*start) || !WrappedTraits::Is(*goal))
                return RetT();

            RetT ret;
            ret->SetStart(WrappedTraits::FromItem(vm, *start));
            ret->SetGoal(WrappedTraits::FromItem(vm, *goal));
            return ret;
        }

        Falcon::Item FalconVariableTraits<FrameTimer>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item(vm.findGlobalItem(falconTypeName.c_str())->asClass()->createInstance());
            SetItem(vm, item, set);
            return item;
        }

        const FalconVariableTraits<::Falcon::Item>::Type FalconVariableTraits<::Falcon::Item>::DefaultValue;

        bool FalconVariableTraits<::Falcon::Item>::Is(Falcon::Item &item)
        {
            return true;
        }

        FalconVariableTraits<::Falcon::Item>::Type FalconVariableTraits<::Falcon::Item>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return item;
        }

        void FalconVariableTraits<::Falcon::Item>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item = set;
        }

        void FalconVariableTraits<::Falcon::Item>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {

        }

        String FalconVariableTraits<::Falcon::Item>::GetTypeString()
        {
            return "Any";
        }

        Falcon::Item FalconVariableTraits<::Falcon::Item>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            Falcon::Item item;
            SetItem(vm, item, set);
            return item;
        }

        Falcon::Item* CheckedRetrieveFromFalcon(const String &name, Falcon::VMachine *vm)
        {
            auto found = RetrieveItemFromVM(name, vm);
            if (!found)
            {
                //FatalScriptErrorParameterNotExist(*ScriptController::Current(), name);
                throw ScriptException();
            }

            return found;
        }

        Falcon::Item* CheckedRetrieveFromFalcon(const String &name, Falcon::uint32 id, Falcon::VMachine *vm)
        {
            auto found = vm->param(id);
            if (!found)
            {
                //FatalScriptErrorParameterNotExist(*ScriptController::Current(), name);
                throw ScriptException();
            }

            return found;
        }
    }
}