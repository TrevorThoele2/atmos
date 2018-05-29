#pragma once

#include <vector>
#include "FalconScriptBasePrototype.h"
#include "FalconScriptUtility.h"

#include "Name.h"
#include "GridPosition.h"

namespace Atmos
{
    namespace Fal
    {
        namespace Classes
        {
            template<class Mixin>
            class Prototype
            {
            public:
                typedef std::unique_ptr<Prototype> PrototypePtr;

                class ValueBase
                {
                protected:
                    bool wasSet;
                public:
                    const Name name;
                    const bool strictType;
                    ValueBase(const Name &name, bool strictType = true);
                    virtual ValueBase* Clone(Falcon::Item &item) const = 0;
                    // Use for setting a class property
                    virtual void SetProperty(Falcon::VMachine &vm, Falcon::Item &self) = 0;
                    void SetItem(Falcon::VMachine &vm);
                    virtual void SetItem(Falcon::VMachine &vm, Falcon::Item &obj) = 0;
                    // Returns true if it could find the item
                    virtual bool FromItem(Falcon::VMachine &vm) = 0;
                    // Is true if this was actually set from Falcon
                    bool WasSet() const;
                };

                class PropertyBase : public ValueBase
                {
                public:
                    PropertyBase(const Name &name, bool strictType = true);
                    virtual void PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo) = 0;
                };

                typedef std::unique_ptr<PropertyBase> PropertyPtr;

                class ParameterBase : public ValueBase
                {
                public:
                    ParameterBase(const Name &name, bool strictType = true);
                    virtual void PushToFalcon(Falcon::Symbol *func, Falcon::Module &pushTo) = 0;
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
                    T value;
                    ValueJoin(const Name &name, bool strictType = true);
                    ValueJoin(const Name &name, const T &value, bool strictType = true);
                    ValueJoin(const Name &name, T &&value, bool strictType = true);
                    // Use for setting a class property
                    void SetProperty(Falcon::VMachine &vm, Falcon::Item &self) override final;
                    void SetPropertyTo(Falcon::VMachine &vm, Falcon::Item &self, const T &set);
                    void SetPropertyTo(Falcon::VMachine &vm, Falcon::Item &self, T &&set);
                    void SetItem(Falcon::VMachine &vm, Falcon::Item &obj) override final;
                    // Returns true if it could find the item
                    bool FromItem(Falcon::VMachine &vm) override final;
                };

                template<class T>
                class Property : public ValueJoin<T, PropertyBase>
                {
                private:
                    bool readonly;
                    bool RetrieveItemFromVMImpl(Falcon::VMachine &vm, Falcon::Item &item) override final;
                public:
                    Property(const Name &name, bool readonly = false, bool strictType = true);
                    Property(const Name &name, const T &value, bool readonly = false, bool strictType = true);
                    Property(const Name &name, T &&value, bool readonly = false, bool strictType = true);
                    Property* Clone(Falcon::Item &item) const override final;
                    bool IsReadonly() const;
                    void PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo) override final;
                };

                template<class T>
                class Parameter : public ValueJoin<T, ParameterBase>
                {
                private:
                    bool RetrieveItemFromVMImpl(Falcon::VMachine &vm, Falcon::Item &item) override final;
                public:
                    Parameter(const Name &name, bool strictType = true);
                    Parameter(const Name &name, const T &value, bool strictType = true);
                    Parameter(const Name &name, T &&value, bool strictType = true);
                    Parameter* Clone(Falcon::Item &item) const override final;
                    void PushToFalcon(Falcon::Symbol *func, Falcon::Module &pushTo) override final;
                };

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
                    template<class As>
                    As Get(const Name &name);
                    template<class U>
                    Property<U>* GetProperty(const Name &name);
                    template<class U>
                    Property<U>* GetProperty(Property<U> &parameter);

                    bool IsEmpty() const;
                    iterator begin();
                    iterator end();
                    const_iterator begin() const;
                    const_iterator end() const;
                };

                class MethodBase
                {
                private:
                    virtual bool ShouldSetupSelf() const;
                public:
                    typedef Falcon::ext_func_t FalconFuncT;
                    FalconFuncT falconFunc;
                    List<ParameterPtr> parameters;

                    MethodBase(std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc);
                    // Returns true if it found everything
                    // Generally indicates if a method can continue
                    bool Setup(Falcon::VMachine &vm);
                    // Returns true if it found all of the parameters
                    // Generally indicates if a method can continue
                    bool SetupParameters(Falcon::VMachine &vm);
                    // Returns true if it found all of the properties
                    // Generally indicates if a method can continue
                    bool SetupSelf(Falcon::VMachine &vm);

                    template<class U>
                    Parameter<U>* GetParameter(const Name &name);
                    template<class U>
                    Parameter<U>* GetParameter(Property<U> &prop);
                };

                class Method : public MethodBase
                {
                private:
                    bool ShouldSetupSelf() const override;
                public:
                    Name name;
                    bool isStatic;
                    Method(const Name &name, bool isStatic, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc);
                    void PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo);
                };

                class Constructor : public MethodBase
                {
                public:
                    Constructor(std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc);
                    // Returns the class symbol
                    Falcon::Symbol* PushToFalcon(const Name &className, Falcon::Module &pushTo);
                };
            private:
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
                struct TraitsRetriever<U, false, true> { typedef BasePrototype::FalconVariableTraitsEnum<U> TraitsT; };
                template<class U>
                struct TraitsRetriever<U, false, false> { typedef BasePrototype::FalconVariableTraitsClass<U> TraitsT; };
                template<class T>
                using TraitsT = typename TraitsRetriever<T, HasSpecialization<FalconVariableTraits<T>>::value, std::is_enum<T>::value>::TraitsT;

                template<class T>
                struct Void { typedef void type; };

                template <typename T, typename = int>
                struct HasConstructor : std::false_type
                {};

                template <typename T>
                struct HasConstructor<T, decltype((void)T::constructor, 0)> : std::true_type
                {};
            private:
                static Falcon::Symbol* CreateClassSymbol(const Name &className, Falcon::Module &pushTo, std::true_type);
                static Falcon::Symbol* CreateClassSymbol(const Name &className, Falcon::Module &pushTo, std::false_type);
            private:
                static void AddProperty(PropertyBase &add);
                static void AddMethod(Method &add);
            public:
                static Mixin Instance();

                static const Name& GetClsName();
                template<class U>
                static Property<U>* GetProperty(const Name &name);
                template<class U>
                static Property<U>* GetProperty(Property<U> &prop);
                static List<PropertyBase*>& GetProperties();
                static List<Method*>& GetMethods();

                // Checks if the item is the same as this prototype
                static bool Is(Falcon::Item &item);
                // Returns true if the item was an instance of this class, otherwise false
                static bool FromItem(Falcon::Item &item);
                // Returns true if the item was an instance of this class, otherwise false
                static bool FromItem(Falcon::VMachine &vm, const String &str);
                static Falcon::Item CreateItem(Falcon::VMachine &vm);

                static void PushAllToModule(Falcon::Module &pushTo);
            };

            template<class Mixin>
            Prototype<Mixin>::ValueBase::ValueBase(const Name &name, bool strictType) : name(name), strictType(strictType), wasSet(false)
            {}

            template<class Mixin>
            void Prototype<Mixin>::ValueBase::SetItem(Falcon::VMachine &vm)
            {
                auto found = .RetrieveItemFromVM(name, vm);
                if (!found)
                    return;

                SetItem(vm, *found);
            }

            template<class Mixin>
            bool Prototype<Mixin>::ValueBase::WasSet() const
            {
                return wasSet;
            }

            template<class Mixin>
            Prototype<Mixin>::PropertyBase::PropertyBase(const Name &name, bool strictType) : ValueBase(name, strictType)
            {}

            template<class Mixin>
            Prototype<Mixin>::ParameterBase::ParameterBase(const Name &name, bool strictType) : ValueBase(name, strictType)
            {}

            template<class Mixin>
            template<class T, class InheritFrom>
            Prototype<Mixin>::ValueJoin<T, InheritFrom>::ValueJoin(const Name &name, bool strictType) : InheritFrom(name, strictType)
            {}

            template<class Mixin>
            template<class T, class InheritFrom>
            Prototype<Mixin>::ValueJoin<T, InheritFrom>::ValueJoin(const Name &name, const T &value, bool strictType) : InheritFrom(name, strictType), value(value)
            {}

            template<class Mixin>
            template<class T, class InheritFrom>
            Prototype<Mixin>::ValueJoin<T, InheritFrom>::ValueJoin(const Name &name, T &&value, bool strictType) : InheritFrom(name, strictType), value(std::move(value))
            {}

            template<class Mixin>
            template<class T, class InheritFrom>
            void Prototype<Mixin>::ValueJoin<T, InheritFrom>::SetProperty(Falcon::VMachine &vm, Falcon::Item &self)
            {
                self.setProperty(Convert(name), TraitsT<T>::CreateItem(vm, value));
            }

            template<class Mixin>
            template<class T, class InheritFrom>
            void Prototype<Mixin>::ValueJoin<T, InheritFrom>::SetPropertyTo(Falcon::VMachine &vm, Falcon::Item &self, const T &set)
            {
                value = set;
                self.setProperty(Convert(name), TraitsT<T>::CreateItem(vm, value));
            }

            template<class Mixin>
            template<class T, class InheritFrom>
            void Prototype<Mixin>::ValueJoin<T, InheritFrom>::SetPropertyTo(Falcon::VMachine &vm, Falcon::Item &self, T &&set)
            {
                value = std::move(set);
                self.setProperty(Convert(name), TraitsT<T>::CreateItem(vm, value));
            }

            template<class Mixin>
            template<class T, class InheritFrom>
            void Prototype<Mixin>::ValueJoin<T, InheritFrom>::SetItem(Falcon::VMachine &vm, Falcon::Item &obj)
            {
                TraitsT<T>::SetItem(vm, obj, value);
            }

            template<class Mixin>
            template<class T, class InheritFrom>
            bool Prototype<Mixin>::ValueJoin<T, InheritFrom>::FromItem(Falcon::VMachine &vm)
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

                if (strictType && !TraitsT<T>::Is(use))
                {
                    String message("A property for a class was not the type expected.");
                    // Add traceback to the string
                    AddTracebackToString(vm, message);
                    // Create the content string
                    Falcon::String contentString;
                    use.toString(contentString);
                    Logger::Log(message,
                        Logger::Type::ERROR_MODERATE,
                        Logger::NameValueVector{ NameValuePair("Class Name", Mixin::GetClsName()),
                                                 NameValuePair("Property Name", name),
                                                 NameValuePair("Expected Type", TraitsT<T>::GetTypeString()),
                                                 NameValuePair("Content (Falcon)", Convert(contentString)) });
                    return false;
                }

                if(!use.isNil())
                    value = TraitsT<T>::FromItem(use);
                return true;
            }

            template<class Mixin>
            template<class T>
            bool Prototype<Mixin>::Property<T>::RetrieveItemFromVMImpl(Falcon::VMachine &vm, Falcon::Item &item)
            {
                try { vm.self().getProperty(Convert(name), item); }
                catch(...) { return false; }

                return true;
            }

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::Property<T>::Property(const Name &name, bool readonly, bool strictType) : ValueJoin(name, strictType), readonly(readonly)
            {
                Mixin::AddProperty(*this);
            }

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::Property<T>::Property(const Name &name, const T &value, bool readonly, bool strictType) : ValueJoin(name, value, strictType), readonly(readonly)
            {
                Mixin::AddProperty(*this);
            }

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::Property<T>::Property(const Name &name, T &&value, bool readonly, bool strictType) : ValueJoin(name, std::move(value), strictType), readonly(readonly)
            {
                Mixin::AddProperty(*this);
            }

            template<class Mixin>
            template<class T>
            bool Prototype<Mixin>::Property<T>::IsReadonly() const
            {
                return readonly;
            }

            template<class Mixin>
            template<class T>
            typename Prototype<Mixin>::Property<T>* Prototype<Mixin>::Property<T>::Clone(Falcon::Item &item) const
            {
                auto ret = new Property(*this);
                ret->value = TraitsT<T>::FromItem(item);
                return ret;
            }

            template<class Mixin>
            template<class T>
            void Prototype<Mixin>::Property<T>::PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo)
            {
                auto &varDef = pushTo.addClassProperty(cls, *pushTo.addString(Convert(name)));
                TraitsT<T>::SetItem(pushTo, varDef, value);
                varDef.setReadOnly(readonly);
            }

            template<class Mixin>
            template<class T>
            bool Prototype<Mixin>::Parameter<T>::RetrieveItemFromVMImpl(Falcon::VMachine &vm, Falcon::Item &item)
            {
                auto found = RetrieveItemFromVM(name, &vm);
                if (!found)
                    return false;
                item = *found;
                return true;
            }

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::Parameter<T>::Parameter(const Name &name, bool strictType) : ValueJoin(name, strictType)
            {}

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::Parameter<T>::Parameter(const Name &name, const T &value, bool strictType) : ValueJoin(name, value, strictType)
            {}

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::Parameter<T>::Parameter(const Name &name, T &&value, bool strictType) : ValueJoin(name, std::move(value), strictType)
            {}

            template<class Mixin>
            template<class T>
            typename Prototype<Mixin>::Parameter<T>* Prototype<Mixin>::Parameter<T>::Clone(Falcon::Item &item) const
            {
                auto ret = new Parameter(*this);
                ret->value = TraitsT<T>::FromItem(item);
                return ret;
            }

            template<class Mixin>
            template<class T>
            void Prototype<Mixin>::Parameter<T>::PushToFalcon(Falcon::Symbol *func, Falcon::Module &pushTo)
            {
                func->addParam(Convert(name));
            }

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::List<T>::List(UnderlyingT &&underlying) : underlying(std::move(underlying))
            {}

            template<class Mixin>
            template<class T>
            T* Prototype<Mixin>::List<T>::Get(const Name &name)
            {
                for (auto &loop : *this)
                {
                    if (loop->name == name)
                        return &loop;
                }

                return nullptr;
            }

            template<class Mixin>
            template<class T>
            template<class As>
            As Prototype<Mixin>::List<T>::Get(const Name &name)
            {
                for (auto &loop : *this)
                {
                    if (loop->name == name)
                        return static_cast<As>(loop);
                }

                return As();
            }

            template<class Mixin>
            template<class T>
            template<class U>
            typename Prototype<Mixin>::Property<U>* Prototype<Mixin>::List<T>::GetProperty(const Name &name)
            {
                return Get<Property<U>*>(name);
            }

            template<class Mixin>
            template<class T>
            template<class U>
            typename Prototype<Mixin>::Property<U>* Prototype<Mixin>::List<T>::GetProperty(Property<U> &parameter)
            {
                return Get<Property<U>*>(parameter.name);
            }

            template<class Mixin>
            template<class T>
            bool Prototype<Mixin>::List<T>::IsEmpty() const
            {
                return underlying.empty();
            }

            template<class Mixin>
            template<class T>
            typename Prototype<Mixin>::List<T>::iterator Prototype<Mixin>::List<T>::begin()
            {
                return underlying.begin();
            }

            template<class Mixin>
            template<class T>
            typename Prototype<Mixin>::List<T>::iterator Prototype<Mixin>::List<T>::end()
            {
                return underlying.end();
            }

            template<class Mixin>
            template<class T>
            typename Prototype<Mixin>::List<T>::const_iterator Prototype<Mixin>::List<T>::begin() const
            {
                return underlying.begin();
            }

            template<class Mixin>
            template<class T>
            typename Prototype<Mixin>::List<T>::const_iterator Prototype<Mixin>::List<T>::end() const
            {
                return underlying.end();
            }

            template<class Mixin>
            bool Prototype<Mixin>::MethodBase::ShouldSetupSelf() const
            {
                return true;
            }

            template<class Mixin>
            Prototype<Mixin>::MethodBase::MethodBase(std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc) : falconFunc(falconFunc)
            {
                for (auto &loop : parameters)
                    this->parameters.underlying.push_back(ParameterPtr(loop));
            }

            template<class Mixin>
            bool Prototype<Mixin>::MethodBase::Setup(Falcon::VMachine &vm)
            {
                if (!SetupSelf(vm))
                    return false;

                if (!SetupParameters(vm))
                    return false;

                return true;
            }

            template<class Mixin>
            bool Prototype<Mixin>::MethodBase::SetupParameters(Falcon::VMachine &vm)
            {
                for (auto &loop : parameters)
                    if (!loop->FromItem(vm))
                        return false;

                return true;
            }

            template<class Mixin>
            bool Prototype<Mixin>::MethodBase::SetupSelf(Falcon::VMachine &vm)
            {
                if (!ShouldSetupSelf())
                    return true;

                for (auto &loop : Mixin::GetProperties())
                    if (!loop->FromItem(vm))
                        return false;

                return true;
            }

            template<class Mixin>
            template<class U>
            typename Prototype<Mixin>::Parameter<U>* Prototype<Mixin>::MethodBase::GetParameter(const Name &name)
            {
                auto got = parameters.Get(name);
                if (!got)
                    return nullptr;

                return static_cast<Parameter<U>*>(got->get());
            }

            template<class Mixin>
            template<class U>
            typename Prototype<Mixin>::Parameter<U>* Prototype<Mixin>::MethodBase::GetParameter(Property<U> &prop)
            {
                auto got = parameters.Get(prop.name);
                if (!got)
                    return nullptr;

                return static_cast<Parameter<U>*>(got->get());
            }

            template<class Mixin>
            bool Prototype<Mixin>::Method::ShouldSetupSelf() const
            {
                return !isStatic;
            }

            template<class Mixin>
            Prototype<Mixin>::Method::Method(const Name &name, bool isStatic, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc) : MethodBase(std::move(parameters), falconFunc), name(name), isStatic(isStatic)
            {
                Mixin::AddMethod(*this);
            }

            template<class Mixin>
            void Prototype<Mixin>::Method::PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo)
            {
                auto method = pushTo.addClassMethod(cls, *pushTo.addString(Convert(name)), falconFunc).asSymbol();
                for (auto &loop : parameters)
                    loop->PushToFalcon(method, pushTo);
            }

            template<class Mixin>
            Prototype<Mixin>::Constructor::Constructor(std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc) : MethodBase(std::move(parameters), falconFunc)
            {}

            template<class Mixin>
            Falcon::Symbol* Prototype<Mixin>::Constructor::PushToFalcon(const Name &className, Falcon::Module &pushTo)
            {
                auto pushed = pushTo.addClass(*pushTo.addString(Convert(className)), falconFunc);
                for (auto &loop : parameters)
                    loop->PushToFalcon(pushed, pushTo);
                return pushed;
            }

            template<class Mixin>
            Falcon::Symbol* Prototype<Mixin>::CreateClassSymbol(const Name &className, Falcon::Module &pushTo, std::true_type)
            {
                return Mixin::constructor.PushToFalcon(className, pushTo);
            }

            template<class Mixin>
            Falcon::Symbol* Prototype<Mixin>::CreateClassSymbol(const Name &className, Falcon::Module &pushTo, std::false_type)
            {
                return pushTo.addClass(*pushTo.addString(Convert(className)), true);
            }

            template<class Mixin>
            void Prototype<Mixin>::AddProperty(PropertyBase &add)
            {
                GetProperties().underlying.push_back(&add);
            }

            template<class Mixin>
            void Prototype<Mixin>::AddMethod(Method &add)
            {
                GetMethods().underlying.push_back(&add);
            }

            template<class Mixin>
            Mixin Prototype<Mixin>::Instance()
            {
                static Mixin instance;
                return instance;
            }

            template<class Mixin>
            const Name& Prototype<Mixin>::GetClsName()
            {
                return Mixin::className;
            }

            template<class Mixin>
            template<class U>
            typename Prototype<Mixin>::Property<U>* Prototype<Mixin>::GetProperty(const Name &name)
            {
                return GetProperties().GetProperty<U>(name);
            }

            template<class Mixin>
            template<class U>
            typename Prototype<Mixin>::Property<U>* Prototype<Mixin>::GetProperty(Property<U> &prop)
            {
                return GetProperties().GetProperty(prop);
            }

            template<class Mixin>
            typename Prototype<Mixin>::List<typename Prototype<Mixin>::PropertyBase*>& Prototype<Mixin>::GetProperties()
            {
                static List<PropertyBase*> properties;
                return properties;
            }

            template<class Mixin>
            typename Prototype<Mixin>::List<typename Prototype<Mixin>::Method*>& Prototype<Mixin>::GetMethods()
            {
                static List<Method*> methods;
                return methods;
            }

            template<class Mixin>
            bool Prototype<Mixin>::Is(Falcon::Item &item)
            {
                return item.isObject() && item.asObject()->generator()->symbol()->name() == Mixin::className;
            }

            template<class Mixin>
            bool Prototype<Mixin>::FromItem(Falcon::Item &item)
            {
                if (!Is(item))
                    return false;

                self = item;
                for (auto &loop : properties)
                    if (!loop->SetItem(vm));
                        return false;

                return true;
            }

            template<class Mixin>
            bool Prototype<Mixin>::FromItem(Falcon::VMachine &vm, const String &str)
            {
                auto found = RetrieveItemFromVM(str, vm);
                if (!found)
                    return false;

                return FromItem(*found);
            }

            template<class Mixin>
            Falcon::Item Prototype<Mixin>::CreateItem(Falcon::VMachine &vm)
            {
                auto self = Falcon::Item(vm.findGlobalItem(GetClsName().c_str())->asClass()->createInstance());
                for (auto &loop : GetProperties())
                    loop->SetProperty(vm, self);
                return self;
            }

            template<class Mixin>
            void Prototype<Mixin>::PushAllToModule(Falcon::Module &pushTo)
            {
                auto clsName = GetClsName();
                auto cls = CreateClassSymbol(clsName, pushTo, std::integral_constant<bool, HasConstructor<Mixin>::value>{});
                for (auto &loop : GetProperties())
                    loop->PushToFalcon(cls, pushTo);
                for (auto &loop : GetMethods())
                    loop->PushToFalcon(cls, pushTo);
            }
        }
    }
}