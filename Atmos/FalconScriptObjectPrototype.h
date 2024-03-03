#pragma once

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

                class PropertyBase
                {
                public:
                    const Name name;
                    PropertyBase(const Name &name);
                    virtual PropertyBase* Clone(Falcon::Item &item) const = 0;
                    virtual void SetProperty(Falcon::VMachine &vm, Falcon::Item &obj) = 0;
                    void SetItem(Falcon::VMachine &vm);
                    virtual void SetItem(Falcon::VMachine &vm, Falcon::Item &obj) = 0;
                    // Returns true if it could make the item
                    virtual bool FromItem(Falcon::VMachine &vm) = 0;
                    virtual void PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo) = 0;
                };

                typedef std::unique_ptr<PropertyBase> PropertyPtr;

                template<class T>
                class Property : public PropertyBase
                {
                public:
                    typedef T Type;
                public:
                    T value;
                    Property(const Name &name);
                    Property(const Name &name, const T &value);
                    Property(const Name &name, T &&value);
                    Property* Clone(Falcon::Item &item) const override final;
                    void SetProperty(Falcon::VMachine &vm, Falcon::Item &obj) override final;
                    void SetItem(Falcon::VMachine &vm, Falcon::Item &obj) override final;
                    // Returns true if it could make the item
                    bool FromItem(Falcon::VMachine &vm) override final;
                    void PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo) override final;
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
                    std::vector<T> underlying;

                    List() = default;
                    List(std::vector<T> &&underlying);

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
                public:
                    typedef Falcon::ext_func_t FalconFuncT;
                    FalconFuncT falconFunc;
                    List<PropertyPtr> parameters;
                    PrototypePtr self;

                    MethodBase(std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc);
                    // Returns true if it found everything
                    // Generally indicates if a method can continue
                    bool Setup(Falcon::VMachine &vm);
                    // Returns true if it found all of the parameters
                    // Generally indicates if a method can continue
                    bool SetupProperties(Falcon::VMachine &vm);
                    // Returns true if it found all of the properties
                    // Generally indicates if a method can continue
                    bool SetupSelf(Falcon::VMachine &vm);

                    template<class U>
                    Property<U>* GetParameter(const Name &name);
                    template<class U>
                    Property<U>* GetParameter(Property<U> &prop);
                };

                class Method : public MethodBase
                {
                public:
                    Name name;
                    bool isStatic;
                    PrototypePtr self;
                    Method(const Name &name, bool isStatic, std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc);
                    void PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo);
                };

                class Constructor : public MethodBase
                {
                public:
                    Constructor(std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc);
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
                static Falcon::Item self;
                static List<PropertyBase*> properties;
                static List<Method*> methods;
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
                static List<PropertyBase*> GetProperties();
                static List<Method*> GetMethods();

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
            Prototype<Mixin>::PropertyBase::PropertyBase(const Name &name) : name(name)
            {
                Mixin::AddProperty(*this);
            }

            template<class Mixin>
            void Prototype<Mixin>::PropertyBase::SetItem(Falcon::VMachine &vm)
            {
                auto found = vm.findGlobalItem(Convert(name));
                if (!found)
                    return;

                SetItem(vm, *found);
            }

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::Property<T>::Property(const Name &name) : PropertyBase(name)
            {}

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::Property<T>::Property(const Name &name, const T &value) : PropertyBase(name), value(value)
            {}

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::Property<T>::Property(const Name &name, T &&value) : PropertyBase(name), value(std::move(value))
            {}

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
            void Prototype<Mixin>::Property<T>::SetProperty(Falcon::VMachine &vm, Falcon::Item &item)
            {
                item.setProperty(Convert(name), TraitsT<T>::CreateItem(vm, value));
            }

            template<class Mixin>
            template<class T>
            void Prototype<Mixin>::Property<T>::SetItem(Falcon::VMachine &vm, Falcon::Item &obj)
            {
                TraitsT<T>::SetItem(vm, obj, value);
            }

            template<class Mixin>
            template<class T>
            bool Prototype<Mixin>::Property<T>::FromItem(Falcon::VMachine &vm)
            {
                auto found = RetrieveItemFromVM(name, &vm);
                ATMOS_ASSERT_MESSAGE(found, "This object must exist.");

                if (!TraitsT<T>::Is(*found))
                {
                    Falcon::String contentString;
                    found->toString(contentString);
                    Logger::Log("A property for a class was not the type expected.",
                        Logger::Type::ERROR_MODERATE,
                        Logger::NameValueVector{ NameValuePair("Class Name", Mixin::GetClsName()),
                                                       NameValuePair("Property Name", name),
                                                       NameValuePair("Expected Type", TraitsT<T>::GetTypeString()),
                                                       NameValuePair("Content (Falcon)", Convert(contentString)) });
                    return false;
                }

                value = TraitsT<T>::FromItem(*found);
                return true;
            }

            template<class Mixin>
            template<class T>
            void Prototype<Mixin>::Property<T>::PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo)
            {
                TraitsT<T>::SetItem(pushTo, pushTo.addClassProperty(cls, *pushTo.addString(Convert(name))), value);
            }

            template<class Mixin>
            template<class T>
            Prototype<Mixin>::List<T>::List(std::vector<T> &&underlying) : underlying(std::move(underlying))
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
            Prototype<Mixin>::MethodBase::MethodBase(std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc) : falconFunc(falconFunc)
            {
                for (auto &loop : parameters)
                    this->parameters.underlying.push_back(PropertyPtr(loop));
            }

            template<class Mixin>
            bool Prototype<Mixin>::MethodBase::Setup(Falcon::VMachine &vm)
            {
                if (!SetupProperties(vm))
                    return false;

                if (!SetupSelf(vm))
                    return false;

                return true;
            }

            template<class Mixin>
            bool Prototype<Mixin>::MethodBase::SetupProperties(Falcon::VMachine &vm)
            {
                for (auto &loop : parameters)
                    if (!loop->FromItem(vm))
                        return false;

                return true;
            }

            template<class Mixin>
            bool Prototype<Mixin>::MethodBase::SetupSelf(Falcon::VMachine &vm)
            {
                return true;
            }

            template<class Mixin>
            template<class U>
            typename Prototype<Mixin>::Property<U>* Prototype<Mixin>::MethodBase::GetParameter(const Name &name)
            {
                auto got = parameters.Get(name);
                if (!got)
                    return nullptr;

                return static_cast<Property<U>*>(got->get());
            }

            template<class Mixin>
            template<class U>
            typename Prototype<Mixin>::Property<U>* Prototype<Mixin>::MethodBase::GetParameter(Property<U> &prop)
            {
                auto got = parameters.Get(prop.name);
                if (!got)
                    return nullptr;

                return static_cast<Property<U>*>(got->get());
            }

            template<class Mixin>
            Prototype<Mixin>::Method::Method(const Name &name, bool isStatic, std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc) : MethodBase(std::move(parameters), falconFunc), name(name), isStatic(isStatic)
            {
                Mixin::AddMethod(*this);
            }

            template<class Mixin>
            void Prototype<Mixin>::Method::PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo)
            {
                auto method = pushTo.addClassMethod(cls, *pushTo.addString(Convert(name)), falconFunc).asSymbol();
                for (auto &loop : parameters)
                    method->addParam(Convert(loop->name));
            }

            template<class Mixin>
            Prototype<Mixin>::Constructor::Constructor(std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc) : MethodBase(std::move(parameters), falconFunc)
            {}

            template<class Mixin>
            Falcon::Symbol* Prototype<Mixin>::Constructor::PushToFalcon(const Name &className, Falcon::Module &pushTo)
            {
                auto pushed = pushTo.addClass(*pushTo.addString(Convert(className)), falconFunc, true);
                for (auto &loop : parameters)
                    pushed->addParam(Convert(loop->name));
                return pushed;
            }

            template<class Mixin>
            Falcon::Item Prototype<Mixin>::self;
            template<class Mixin>
            typename Prototype<Mixin>::List<typename Prototype<Mixin>::PropertyBase*> Prototype<Mixin>::properties;
            template<class Mixin>
            typename Prototype<Mixin>::List<typename Prototype<Mixin>::Method*> Prototype<Mixin>::methods;

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
                properties.underlying.push_back(&add);
            }

            template<class Mixin>
            void Prototype<Mixin>::AddMethod(Method &add)
            {
                methods.underlying.push_back(&add);
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
            typename Prototype<Mixin>::List<typename Prototype<Mixin>::PropertyBase*> Prototype<Mixin>::GetProperties()
            {
                return properties;
            }

            template<class Mixin>
            typename Prototype<Mixin>::List<typename Prototype<Mixin>::Method*> Prototype<Mixin>::GetMethods()
            {
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
                self = Falcon::Item(vm.findGlobalItem(GetClsName().c_str())->asClass()->createInstance());
                for (auto &loop : properties)
                    loop->SetItem(vm);
                return self;
            }

            template<class Mixin>
            void Prototype<Mixin>::PushAllToModule(Falcon::Module &pushTo)
            {
                auto clsName = GetClsName();
                auto cls = CreateClassSymbol(clsName, pushTo, std::integral_constant<bool, HasConstructor<Mixin>::value>{});
                for (auto &loop : properties)
                    loop->PushToFalcon(cls, pushTo);
                for (auto &loop : methods)
                    loop->PushToFalcon(cls, pushTo);
            }
        }
    }
}