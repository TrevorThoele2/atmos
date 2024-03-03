#pragma once

#include "FalconScriptUtility.h"

namespace Atmos
{
    namespace Fal
    {
        namespace BasePrototype
        {
            struct StrictType
            {
                bool value;
                StrictType(bool value = true);
                explicit operator bool() const;
            };

            class ValueBase
            {
            protected:
                bool wasSet;
            public:
                const Name name;
                const bool strictType;
                ValueBase(const Name &name, const StrictType &strictType);
                virtual ValueBase* Clone() const = 0;
                virtual ValueBase* Clone(Falcon::VMachine &vm, Falcon::Item &item) const = 0;
                // Use for setting a class property
                virtual void SetProperty(Falcon::VMachine &vm, Falcon::Item &self) = 0;
                void SetItem(Falcon::VMachine &vm);
                virtual void SetItem(Falcon::VMachine &vm, Falcon::Item &item) = 0;
                // Returns true if it could find the item
                virtual bool FromItem(Falcon::VMachine &vm) = 0;
                // Returns true if it could find the item
                virtual bool FromItem(Falcon::VMachine &vm, Falcon::Item &item) = 0;
                // Is true if this was actually set from Falcon
                bool WasSet() const;
            };

            class PropertyBase : public ValueBase
            {
            public:
                PropertyBase(const Name &name);
                PropertyBase(const Name &name, const StrictType &strictType);
                virtual void PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo) = 0;
                virtual bool IsReadonly() const = 0;

                static const String InvalidTypeString() { return "A property for a class was not the type expected."; }
            };

            typedef std::unique_ptr<PropertyBase> PropertyPtr;

            class ParameterBase : public ValueBase
            {
            public:
                ParameterBase(const Name &name);
                ParameterBase(const Name &name, const StrictType &strictType);
                virtual void PushToFalcon(Falcon::Symbol *func, Falcon::Module &pushTo) = 0;

                static const String InvalidTypeString() { return "A parameter for a function was not the type expected."; }
            };

            typedef std::unique_ptr<ParameterBase> ParameterPtr;

            template<class T, class InheritFrom>
            class ValueJoin : public InheritFrom
            {
            public:
                typedef T Type;
            private:
                virtual bool RetrieveItemFromVMImpl(Falcon::VMachine &vm, Falcon::Item &item) = 0;
            public:
                T obj;
                ValueJoin(const Name &name);
                ValueJoin(const Name &name, const StrictType &strictType);
                ValueJoin(const Name &name, const T &obj);
                ValueJoin(const Name &name, const T &obj, const StrictType &strictType);
                ValueJoin(const Name &name, T &&obj);
                ValueJoin(const Name &name, T &&obj, const StrictType &strictType);
                // Use for setting a class property
                void SetProperty(Falcon::VMachine &vm, Falcon::Item &self) override final;
                void SetPropertyTo(Falcon::VMachine &vm, Falcon::Item &self, const T &set);
                void SetPropertyTo(Falcon::VMachine &vm, Falcon::Item &self, T &&set);
                void SetItem(Falcon::VMachine &vm, Falcon::Item &item) override final;
                // Returns true if it could find the item
                bool FromItem(Falcon::VMachine &vm) override final;
                // Returns true if it could find the item
                bool FromItem(Falcon::VMachine &vm, Falcon::Item &item) override final;

                T* operator->();
                const T* operator->() const;
                T& operator*();
                const T& operator*() const;
            };

            template<class T, class InheritFrom>
            ValueJoin<T, InheritFrom>::ValueJoin(const Name &name) : InheritFrom(name, true)
            {}

            template<class T, class InheritFrom>
            ValueJoin<T, InheritFrom>::ValueJoin(const Name &name, const StrictType &strictType) : InheritFrom(name, strictType)
            {}

            template<class T, class InheritFrom>
            ValueJoin<T, InheritFrom>::ValueJoin(const Name &name, const T &obj) : InheritFrom(name, true), obj(obj)
            {}

            template<class T, class InheritFrom>
            ValueJoin<T, InheritFrom>::ValueJoin(const Name &name, const T &obj, const StrictType &strictType) : InheritFrom(name, strictType), obj(obj)
            {}

            template<class T, class InheritFrom>
            ValueJoin<T, InheritFrom>::ValueJoin(const Name &name, T &&obj) : InheritFrom(name, true), obj(std::move(obj))
            {}

            template<class T, class InheritFrom>
            ValueJoin<T, InheritFrom>::ValueJoin(const Name &name, T &&obj, const StrictType &strictType) : InheritFrom(name, strictType), obj(std::move(obj))
            {}

            template<class T, class InheritFrom>
            void ValueJoin<T, InheritFrom>::SetProperty(Falcon::VMachine &vm, Falcon::Item &self)
            {
                self.setProperty(Convert(name), TraitsT<T>::CreateItem(vm, obj));
            }

            template<class T, class InheritFrom>
            void ValueJoin<T, InheritFrom>::SetPropertyTo(Falcon::VMachine &vm, Falcon::Item &self, const T &set)
            {
                obj = set;
                self.setProperty(Convert(name), TraitsT<T>::CreateItem(vm, obj));
            }

            template<class T, class InheritFrom>
            void ValueJoin<T, InheritFrom>::SetPropertyTo(Falcon::VMachine &vm, Falcon::Item &self, T &&set)
            {
                obj = std::move(set);
                self.setProperty(Convert(name), TraitsT<T>::CreateItem(vm, obj));
            }

            template<class T, class InheritFrom>
            void ValueJoin<T, InheritFrom>::SetItem(Falcon::VMachine &vm, Falcon::Item &item)
            {
                TraitsT<T>::SetItem(vm, item, obj);
            }

            template<class T, class InheritFrom>
            bool ValueJoin<T, InheritFrom>::FromItem(Falcon::VMachine &vm)
            {
                Falcon::Item use;
                {
                    bool found = RetrieveItemFromVMImpl(vm, use);
                    if (!found)
                    {
                        use.setNil();
                        wasSet = false;
                    }
                    else
                        wasSet = true;
                }

                return FromItem(vm, use);
            }

            template<class T, class InheritFrom>
            bool ValueJoin<T, InheritFrom>::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
            {
                if (strictType && !TraitsT<T>::Is(item))
                {
                    String message(InheritFrom::InvalidTypeString() + "\n");
                    // Add traceback to the string
                    message = AddTracebackToString(vm, message);
                    // Create the content string
                    Falcon::String contentString;
                    item.toString(contentString);
                    Logger::Log(message,
                        Logger::Type::ERROR_MODERATE,
                        Logger::NameValueVector{ NameValuePair("Property Name", name),
                        NameValuePair("Expected Type", TraitsT<T>::GetTypeString()),
                        NameValuePair("Content (Falcon)", Convert(contentString)) });
                    return false;
                }

                if (!item.isNil())
                    obj = TraitsT<T>::FromItem(vm, item);
                return true;
            }

            template<class T, class InheritFrom>
            T* ValueJoin<T, InheritFrom>::operator->()
            {
                return &obj;
            }

            template<class T, class InheritFrom>
            const T* ValueJoin<T, InheritFrom>::operator->() const
            {
                return &obj;
            }

            template<class T, class InheritFrom>
            T& ValueJoin<T, InheritFrom>::operator*()
            {
                return obj;
            }

            template<class T, class InheritFrom>
            const T& ValueJoin<T, InheritFrom>::operator*() const
            {
                return obj;
            }

            template<class T>
            class Parameter : public ValueJoin<T, ParameterBase>
            {
            private:
                bool RetrieveItemFromVMImpl(Falcon::VMachine &vm, Falcon::Item &item) override final;
            public:
                Parameter(const Name &name);
                Parameter(const Name &name, const StrictType &strictType);
                Parameter(const Name &name, const T &obj);
                Parameter(const Name &name, const T &obj, const StrictType &strictType);
                Parameter(const Name &name, T &&obj);
                Parameter(const Name &name, T &&obj, const StrictType &strictType);
                Parameter* Clone() const override final;
                Parameter* Clone(Falcon::VMachine &vm, Falcon::Item &item) const override final;
                void PushToFalcon(Falcon::Symbol *func, Falcon::Module &pushTo) override final;
            };

            template<class T>
            bool Parameter<T>::RetrieveItemFromVMImpl(Falcon::VMachine &vm, Falcon::Item &item)
            {
                auto found = RetrieveItemFromVM(name, &vm);
                if (!found)
                    return false;
                item = *found;
                return true;
            }

            template<class T>
            Parameter<T>::Parameter(const Name &name) : ValueJoin(name)
            {}

            template<class T>
            Parameter<T>::Parameter(const Name &name, const StrictType &strictType) : ValueJoin(name, strictType)
            {}

            template<class T>
            Parameter<T>::Parameter(const Name &name, const T &obj) : ValueJoin(name, obj)
            {}

            template<class T>
            Parameter<T>::Parameter(const Name &name, const T &obj, const StrictType &strictType) : ValueJoin(name, obj, strictType)
            {}

            template<class T>
            Parameter<T>::Parameter(const Name &name, T &&obj) : ValueJoin(name, std::move(obj))
            {}

            template<class T>
            Parameter<T>::Parameter(const Name &name, T &&obj, const StrictType &strictType) : ValueJoin(name, std::move(obj), strictType)
            {}

            template<class T>
            typename Parameter<T>* Parameter<T>::Clone() const
            {
                return new Parameter(*this);
            }

            template<class T>
            typename Parameter<T>* Parameter<T>::Clone(Falcon::VMachine &vm, Falcon::Item &item) const
            {
                auto ret = Clone();
                ret->obj = TraitsT<T>::FromItem(vm, item);
                return ret;
            }

            template<class T>
            void Parameter<T>::PushToFalcon(Falcon::Symbol *func, Falcon::Module &pushTo)
            {
                func->addParam(Convert(name));
            }

            template<class T>
            class List
            {
            public:
                typedef T Type;
            private:
                typedef std::vector<T> UnderlyingT;
            public:
                typedef typename UnderlyingT::iterator iterator;
                typedef typename UnderlyingT::const_iterator const_iterator;
            public:
                UnderlyingT underlying;

                List() = default;
                List(UnderlyingT &&underlying);

                T* Get(const Name &name);

                bool IsEmpty() const;
                iterator begin();
                iterator end();
                const_iterator begin() const;
                const_iterator end() const;
            };

            template<class T>
            List<T>::List(UnderlyingT &&underlying) : underlying(std::move(underlying))
            {}

            template<class T>
            T* List<T>::Get(const Name &name)
            {
                for (auto &loop : *this)
                {
                    if (loop->name == name)
                        return &loop;
                }

                return nullptr;
            }

            template<class T>
            bool List<T>::IsEmpty() const
            {
                return underlying.empty();
            }

            template<class T>
            typename List<T>::iterator List<T>::begin()
            {
                return underlying.begin();
            }

            template<class T>
            typename List<T>::iterator List<T>::end()
            {
                return underlying.end();
            }

            template<class T>
            typename List<T>::const_iterator List<T>::begin() const
            {
                return underlying.begin();
            }

            template<class T>
            typename List<T>::const_iterator List<T>::end() const
            {
                return underlying.end();
            }
        }
    }
}