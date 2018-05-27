#pragma once

#include "FalconScriptUtility.h"

namespace Atmos
{
    namespace Fal
    {
        namespace BasePrototype
        {
            template<class T>
            struct FalconVariableTraitsClass
            {
                typedef T Type;
                static const Type DefaultValue;
                static bool Is(Falcon::Item &item);
                static Type FromItem(Falcon::Item &item);
                static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
                static void SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set);
                static Falcon::Item CreateItem(Falcon::VMachine &vm, const Type &value);

                static String GetTypeString();
            };

            template<class T>
            typename const FalconVariableTraitsClass<T>::Type FalconVariableTraitsClass<T>::DefaultValue;
            template<class T>
            bool FalconVariableTraitsClass<T>::Is(Falcon::Item &item)
            {
                return true;
            }

            template<class T>
            typename FalconVariableTraitsClass<T>::Type FalconVariableTraitsClass<T>::FromItem(Falcon::Item &item)
            {
                return Type();
            }

            template<class T>
            void FalconVariableTraitsClass<T>::SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set)
            {

            }

            template<class T>
            void FalconVariableTraitsClass<T>::SetItem(Falcon::Module &mod, Falcon::VarDef &item, const Type &set)
            {

            }

            template<class T>
            Falcon::Item FalconVariableTraitsClass<T>::CreateItem(Falcon::VMachine &vm, const Type &value)
            {
                Falcon::Item item;
                return item;
            }

            template<class T>
            String FalconVariableTraitsClass<T>::GetTypeString()
            {
                return Type::GetClsName();
            }

            template<class T>
            struct FalconVariableTraitsEnum
            {
                typedef T Type;
                typedef typename std::underlying_type<Type>::type UnderlyingType;
                static const UnderlyingType DefaultValue;

                static bool Is(Falcon::Item &item);
                static Type FromItem(Falcon::Item &item);
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
            typename FalconVariableTraitsEnum<T>::Type FalconVariableTraitsEnum<T>::FromItem(Falcon::Item &item)
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
        }
    }
}