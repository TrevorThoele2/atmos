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

#include "Logger.h"

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
    namespace Fal
    {
        Falcon::String Convert(const String &arg);
        String Convert(const Falcon::String &arg);

        Falcon::Item* RetrieveItemFromVM(const std::string &name, Falcon::VMachine *vm);
        String AddTracebackToString(Falcon::VMachine &vm, const String &string);

        void FatalScriptError(String &&message, Logger::Type severity);
        void FatalScriptError(String &&message, Logger::Type severity, Logger::NameValueVector &nameValueVector);

        template<class T>
        struct FalconVariableTraitsClassPrototype
        {
            typedef T Type;
            static const Type DefaultValue;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, Type &value);

            static String GetTypeString();
        };

        template<class T>
        typename const FalconVariableTraitsClassPrototype<T>::Type FalconVariableTraitsClassPrototype<T>::DefaultValue;
        template<class T>
        bool FalconVariableTraitsClassPrototype<T>::Is(Falcon::Item &item)
        {
            return T::Scaffolding().Is(item);
        }

        template<class T>
        typename FalconVariableTraitsClassPrototype<T>::Type FalconVariableTraitsClassPrototype<T>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            Type type(Type::Scaffolding());
            type.FromItem(vm, item);
            return type;
        }

        template<class T>
        void FalconVariableTraitsClassPrototype<T>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {

        }

        template<class T>
        void FalconVariableTraitsClassPrototype<T>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {

        }

        template<class T>
        Falcon::Item FalconVariableTraitsClassPrototype<T>::CreateItem(Falcon::VMachine &vm, Type &value)
        {
            return value.CreateItem(vm);
        }

        template<class T>
        String FalconVariableTraitsClassPrototype<T>::GetTypeString()
        {
            return Type::ClsName();
        }

        template<class T>
        struct FalconVariableTraitsEnum
        {
            typedef T Type;
            typedef typename std::underlying_type<Type>::type UnderlyingType;
            static const UnderlyingType DefaultValue;

            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &value);

            static String GetTypeString();
        };

        template<class T>
        typename const FalconVariableTraitsEnum<T>::UnderlyingType FalconVariableTraitsEnum<T>::DefaultValue(0);
        template<class T>
        bool FalconVariableTraitsEnum<T>::Is(Falcon::Item &item)
        {
            return item.isInteger();
        }

        template<class T>
        typename FalconVariableTraitsEnum<T>::Type FalconVariableTraitsEnum<T>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            return static_cast<Type>(item.asInteger());
        }

        template<class T>
        void FalconVariableTraitsEnum<T>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            item.setInteger(static_cast<UnderlyingType>(set));
        }

        template<class T>
        void FalconVariableTraitsEnum<T>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            item.setInteger(static_cast<UnderlyingType>(set));
        }

        template<class T>
        Falcon::Item FalconVariableTraitsEnum<T>::CreateItem(Falcon::VMachine &vm, const Type &value)
        {
            Falcon::Item item;
            item.setInteger(static_cast<UnderlyingType>(value));
            return item;
        }

        template<class T>
        String FalconVariableTraitsEnum<T>::GetTypeString()
        {
            return FalconVariableTraits<UnderlyingType>::GetTypeString();
        }

        template<class T>
        struct FalconVariableTraits;

        namespace detail
        {
            template <class T, std::size_t = sizeof(T)>
            static std::true_type HasSpecializationImpl(T *) {}
            static std::false_type HasSpecializationImpl(...) {}
            template <class T>
            using HasSpecialization = decltype(HasSpecializationImpl(std::declval<T*>()));

            template<class U, bool, bool>
            struct TraitsRetriever;
            template<class U>
            struct TraitsRetriever<U, true, false> { typedef FalconVariableTraits<U> TraitsT; };
            template<class U>
            struct TraitsRetriever<U, false, true> { typedef FalconVariableTraitsEnum<U> TraitsT; };
            template<class U>
            struct TraitsRetriever<U, false, false> { typedef FalconVariableTraitsClassPrototype<U> TraitsT; };
        }

        template<class T>
        using TraitsT = typename detail::TraitsRetriever<T, detail::HasSpecialization<FalconVariableTraits<T>>::value, std::is_enum<T>::value>::TraitsT;

        template<>
        struct FalconVariableTraits<bool>
        {
            typedef bool Type;
            static const Type DefaultValue = false;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::uint8_t>
        {
            typedef std::uint8_t Type;
            static const Type DefaultValue = 0;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::uint16_t>
        {
            typedef std::uint16_t Type;
            static const Type DefaultValue = 0;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::uint32_t>
        {
            typedef std::uint32_t Type;
            static const Type DefaultValue = 0;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::uint64_t>
        {
            typedef std::uint64_t Type;
            static const Type DefaultValue = 0;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::int8_t>
        {
            typedef std::int8_t Type;
            static const Type DefaultValue = 0;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::int16_t>
        {
            typedef std::int16_t Type;
            static const Type DefaultValue = 0;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::int32_t>
        {
            typedef std::int32_t Type;
            static const Type DefaultValue = 0;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<std::int64_t>
        {
            typedef std::int64_t Type;
            static const Type DefaultValue = 0;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<float>
        {
            typedef float Type;
            static const Type DefaultValue;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<double>
        {
            typedef double Type;
            static const Type DefaultValue;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<String>
        {
            typedef String Type;
            static const Type DefaultValue;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<class T>
        struct FalconVariableTraits<std::vector<T>>
        {
            typedef std::vector<T> Type;
            static constexpr bool Is(Falcon::Item &item) { return item.isArray(); }
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item)
            {
                Type ret;

                auto arr = item.asArray();
                for (Falcon::uint32 loop = 0; loop != arr->length(); ++loop)
                {
                    auto &test = (*arr)[loop];
                    auto &from = FalconVariableTraits<T>::FromItem(vm, test);
                    ret.push_back(from);
                }

                return ret;
            }

            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
            {
                Falcon::CoreArray ar;
                for (auto &loop : set)
                    ar.append(FalconVariableTraits<T>::CreateItem(vm, loop));
                item.setArray(&ar);
            }

            static void SetItem(Falcon::VarDef &item, const Type &set)
            {

            }

            static String GetTypeString()
            {
                return "Vector";
            }

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

        /*
        template<>
        struct FalconVariableTraits<Variant>
        {
            typedef Variant Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static const String falconTypeName;
            static const String valueName;
            static Optional<Type> Create(Falcon::VMachine &vm);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };
        */

        template<>
        struct FalconVariableTraits<LargeInteger>
        {
            typedef LargeInteger Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);
            static String GetTypeString();

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
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);
            static String GetTypeString();

            static const String falconTypeName;
            static const String valueName;
            static Optional<Type> Create(Falcon::VMachine &vm);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<FrameTimer>
        {
            typedef FrameTimer Type;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);
            static const String typeString;

            static const String falconTypeName;
            static const String startName;
            static const String goalName;
            static Optional<Type> Create(Falcon::VMachine &vm);
            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<>
        struct FalconVariableTraits<::Falcon::Item>
        {
            typedef ::Falcon::Item Type;
            static const Type DefaultValue;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<class... Args>
        struct FalconVariableTraits<::Chroma::Variant<Args...>>
        {
        public:
            typedef ::Chroma::Variant<Args...> Type;
        private:
            enum class VisitStrategy
            {
                IS,
                FROM_ITEM,
                SET_ITEM,
                CREATE_ITEM
            };

            template<VisitStrategy strategy>
            using StrategySelector = ::Chroma::StrategySelector<VisitStrategy, strategy>;

            template<size_t i>
            class Iterator
            {
            private:
                using U = typename ::Chroma::VariadicTemplate<Args...>::Parameter<i>::Type;
            public:
                static bool Check(Falcon::Item &item, StrategySelector<VisitStrategy::IS>)
                {
                    return TraitsT<U>::Is(item);
                }

                static bool Check(Type &out, Falcon::VMachine &vm, Falcon::Item &item, StrategySelector<VisitStrategy::FROM_ITEM>)
                {
                    if (!TraitsT<U>::Is(item))
                        return false;

                    out.Set(TraitsT<U>::FromItem(vm, item));
                    return true;
                }
            };

            class VisitImpl
            {
            public:
                template<class U>
                static void Do(const U &u, Falcon::VMachine &vm, Falcon::Item &item, StrategySelector<VisitStrategy::SET_ITEM>)
                {
                    TraitsT<U>::SetItem(vm, item, u);
                }

                template<class U>
                static void Do(const U &u, Falcon::Module &mod, Falcon::VarDef &item, StrategySelector<VisitStrategy::SET_ITEM>)
                {
                    TraitsT<U>::SetItem(mod, item, u);
                }

                template<class U>
                static Falcon::Item DoReturn(const U &u, Falcon::VMachine &vm, StrategySelector<VisitStrategy::CREATE_ITEM>)
                {
                    return TraitsT<U>::CreateItem(vm, u);
                }
            };
        public:
            static const Type DefaultValue;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::VMachine &vm, Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
            static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);

            static String GetTypeString();

            static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &set);
        };

        template<class... Args>
        typename const FalconVariableTraits<::Chroma::Variant<Args...>>::Type FalconVariableTraits<::Chroma::Variant<Args...>>::DefaultValue;

        template<class... Args>
        bool FalconVariableTraits<::Chroma::Variant<Args...>>::Is(Falcon::Item &item)
        {
            return ::Chroma::IterateRangeCheckStop<size_t, Iterator, bool, sizeof...(Args)-1, 0>(true, item, StrategySelector<VisitStrategy::IS>{});
        }

        template<class... Args>
        typename FalconVariableTraits<::Chroma::Variant<Args...>>::Type FalconVariableTraits<::Chroma::Variant<Args...>>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
        {
            Type ret;
            ::Chroma::IterateRangeCheckStop<size_t, Iterator, bool, sizeof...(Args)-1, 0>(true, ret, vm, item, StrategySelector<VisitStrategy::FROM_ITEM>{});
            return ret;
        }

        template<class... Args>
        void FalconVariableTraits<::Chroma::Variant<Args...>>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
        {
            ::Chroma::Visit<VisitImpl>(set, vm, item, StrategySelector<VisitStrategy::SET_ITEM>{});
        }

        template<class... Args>
        void FalconVariableTraits<::Chroma::Variant<Args...>>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
        {
            ::Chroma::Visit<VisitImpl>(set, mod, item, StrategySelector<VisitStrategy::SET_ITEM>{});
        }

        template<class... Args>
        String FalconVariableTraits<::Chroma::Variant<Args...>>::GetTypeString()
        {
            return "Variant";
        }

        template<class... Args>
        Falcon::Item FalconVariableTraits<::Chroma::Variant<Args...>>::CreateItem(Falcon::VMachine &vm, const Type &set)
        {
            return ::Chroma::VisitReturn<VisitImpl, Falcon::Item>(set, vm, StrategySelector<VisitStrategy::CREATE_ITEM>{});
        }

        template<class T>
        constexpr bool Is(Falcon::Item &item)
        {
            return FalconVariableTraits<T>::Is(item);
        }

        template<class T>
        T FromItem(Falcon::VMachine &vm, Falcon::Item &item)
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
        /*
        template<class T>
        T CheckedConversionFromFalcon(Falcon::VMachine &vm,  String &name, Falcon::Item *item)
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
                FatalScriptErrorParameterNotExpectedType(*ScriptController::Current(), name, FalconVariableTraits<T>::GetTypeString(), Falcon::AutoCString(typeString).c_str());
                throw ScriptException();
            }

            return FromItem<T>(vm, *item);
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
                FatalScriptErrorParameterNotExpectedType(*ScriptController::Current(), name, VectorTraits::GetTypeString(), *item);
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

        template<class T, class DefaultValue>
        void CheckedItemPropertySet(Falcon::Item &set, const String &propertyName, Falcon::Item *setAs, bool(Falcon::Item::*isFunc)() const, T(Falcon::Item::*asFunc)() const, DefaultValue defaultValue)
        {
            if (!setAs || !(setAs->*isFunc)())
                set.setProperty(Convert(propertyName), defaultValue);
            else
                set.setProperty(Convert(propertyName), (setAs->*asFunc)());
        }

        template<class T, class Ret, class Set, class DefaultValue>
        void CheckedItemSet(Falcon::Item *set, Falcon::Item *setAs, bool(Falcon::Item::*isFunc)() const, T(Falcon::Item::*asFunc)() const, Ret(Falcon::Item::*setFunc)(Set), const DefaultValue &defaultValue)
        {
            if (!setAs || !(setAs->*isFunc)())
                (set->*setFunc)(defaultValue);
            else
                (set->*setFunc)((setAs->*asFunc)());
        }

        template<class T, class Ret, class Set, class DefaultValue>
        void CheckedItemSet(const String &set, const String &setAs, Falcon::VMachine *vm, bool(Falcon::Item::*isFunc)() const, T(Falcon::Item::*asFunc)() const, Ret(Falcon::Item::*setFunc)(Set), const DefaultValue &defaultValue)
        {
            CheckedItemSet(RetrieveItemFromVM(set, vm), RetrieveItemFromVM(setAs, vm), isFunc, asFunc, setFunc, defaultValue);
        }
        */

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