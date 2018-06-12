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
            typedef BasePrototype::StrictType StrictType;

            struct Readonly
            {
                bool value;
                Readonly(bool value = false);
                explicit operator bool() const;
            };

            class Prototype
            {
            public:
                typedef BasePrototype::ParameterBase ParameterBase;
                typedef BasePrototype::ParameterPtr ParameterPtr;
                template<class T>
                using Parameter = BasePrototype::Parameter<T>;

                typedef BasePrototype::PropertyBase PropertyBase;
                typedef BasePrototype::PropertyPtr PropertyPtr;

                template<class T>
                class Property : public BasePrototype::ValueJoin<T, PropertyBase>
                {
                private:
                    bool readonly;
                    bool RetrieveItemFromVMImpl(Falcon::VMachine &vm, Falcon::Item &item) override final;
                public:
                    Property(const Name &name, const Readonly &readonly = Readonly(), const StrictType &strictType = StrictType());
                    Property(const Name &name, const T &obj, const Readonly &readonly = Readonly(), const StrictType &strictType = StrictType());
                    Property(const Name &name, T &&obj, const Readonly &readonly = Readonly(), const StrictType &strictType = StrictType());
                    Property* Clone() const override final;
                    Property* Clone(Falcon::VMachine &vm, Falcon::Item &item) const override final;
                    bool IsReadonly() const override final;
                    void PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo) override final;
                };

                template<class T>
                using List = BasePrototype::List<T>;

                class MethodBase
                {
                private:
                    virtual bool ShouldSetupSelf() const;

                    void CopyParametersFrom(const MethodBase &arg);
                public:
                    typedef Falcon::ext_func_t FalconFuncT;
                    FalconFuncT falconFunc;
                    List<ParameterPtr> parameters;

                    Prototype *prototype;

                    MethodBase(Prototype &prototype, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc);
                    MethodBase(const MethodBase &arg);
                    MethodBase& operator=(const MethodBase &arg);
                    virtual MethodBase* Clone(Prototype &prototype) const = 0;
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
                    Method(Prototype &prototype, const Name &name, bool isStatic, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc);
                    Method* Clone(Prototype &prototype) const override final;
                    void PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo);
                };

                typedef std::unique_ptr<Method> MethodPtr;

                class Constructor : public MethodBase
                {
                public:
                    Constructor(Prototype &prototype, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc);
                    Constructor* Clone(Prototype &prototype) const override final;
                    // Returns the class symbol
                    Falcon::Symbol* PushToFalcon(const Name &className, Falcon::Module &pushTo);
                };
            private:
                List<PropertyPtr> properties;
                List<MethodPtr> methods;

                virtual const Name& GetClsName() const = 0;

                void CopyFrom(const Prototype &arg);
            public:
                Prototype() = default;
                Prototype(const Prototype &arg);
                Prototype& operator=(const Prototype &arg);

                // Expects heap memory
                void AddProperty(PropertyBase *add);
                // Expects heap memory
                void AddMethod(Method *add);

                template<class U>
                Property<U>* GetProperty(const Name &name);
                template<class U>
                Property<U>* GetProperty(Property<U> &prop);
                List<PropertyPtr>& GetProperties();
                List<MethodPtr>& GetMethods();

                // Checks if the item is the same as this prototype
                bool Is(Falcon::Item &item);
                // Returns true if the item was an instance of this class, otherwise false
                bool FromItem(Falcon::VMachine &vm, Falcon::Item &item);
                // Returns true if the item was an instance of this class, otherwise false
                bool FromItem(Falcon::VMachine &vm, const String &str);
                Falcon::Item CreateItem(Falcon::VMachine &vm);
            };

            template<class T>
            bool Prototype::Property<T>::RetrieveItemFromVMImpl(Falcon::VMachine &vm, Falcon::Item &item)
            {
                try { vm.self().getProperty(Convert(name), item); }
                catch (...) { return false; }

                return true;
            }

            template<class T>
            Prototype::Property<T>::Property(const Name &name, const Readonly &readonly, const StrictType &strictType) : ValueJoin(name, strictType), readonly(readonly)
            {}

            template<class T>
            Prototype::Property<T>::Property(const Name &name, const T &obj, const Readonly &readonly, const StrictType &strictType) : ValueJoin(name, obj, strictType), readonly(readonly)
            {}

            template<class T>
            Prototype::Property<T>::Property(const Name &name, T &&obj, const Readonly &readonly, const StrictType &strictType) : ValueJoin(name, std::move(obj), strictType), readonly(readonly)
            {}

            template<class T>
            bool Prototype::Property<T>::IsReadonly() const
            {
                return readonly;
            }

            template<class T>
            typename Prototype::Property<T>* Prototype::Property<T>::Clone() const
            {
                return new Property(*this);
            }

            template<class T>
            typename Prototype::Property<T>* Prototype::Property<T>::Clone(Falcon::VMachine &vm, Falcon::Item &item) const
            {
                auto ret = Clone();
                ret->obj = TraitsT<T>::FromItem(vm, item);
                return ret;
            }

            template<class T>
            void Prototype::Property<T>::PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo)
            {
                auto &varDef = pushTo.addClassProperty(cls, *pushTo.addString(Convert(name)));
                TraitsT<T>::SetItem(pushTo, varDef, obj);
                varDef.setReadOnly(readonly);
            }

            template<class U>
            typename Prototype::Parameter<U>* Prototype::MethodBase::GetParameter(const Name &name)
            {
                auto got = parameters.Get(name);
                if (!got)
                    return nullptr;

                return static_cast<Parameter<U>*>(got->get());
            }

            template<class U>
            typename Prototype::Parameter<U>* Prototype::MethodBase::GetParameter(Property<U> &prop)
            {
                auto got = parameters.Get(prop.name);
                if (!got)
                    return nullptr;

                return static_cast<Parameter<U>*>(got->get());
            }

            template<class U>
            typename Prototype::Property<U>* Prototype::GetProperty(const Name &name)
            {
                auto got = GetProperties().Get(name);
                if (!got)
                    return nullptr;

                return static_cast<Property<U>*>(got->get());
            }

            template<class U>
            typename Prototype::Property<U>* Prototype::GetProperty(Property<U> &prop)
            {
                return GetProperty<U>(prop.name);
            }

            template<class Mixin>
            class Definition : public Prototype
            {
            private:
            private:
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
                const Name& GetClsName() const override final;
            public:
                static Mixin& Instance();
                static const Name& ClsName();

                static void PushAllToModule(Falcon::Module &pushTo);
            };

            template<class Mixin>
            Falcon::Symbol* Definition<Mixin>::CreateClassSymbol(const Name &className, Falcon::Module &pushTo, std::true_type)
            {
                return Mixin::constructor.PushToFalcon(className, pushTo);
            }

            template<class Mixin>
            Falcon::Symbol* Definition<Mixin>::CreateClassSymbol(const Name &className, Falcon::Module &pushTo, std::false_type)
            {
                return pushTo.addClass(*pushTo.addString(Convert(className)), true);
            }

            template<class Mixin>
            const Name& Definition<Mixin>::GetClsName() const
            {
                return ClsName();
            }

            template<class Mixin>
            Mixin& Definition<Mixin>::Instance()
            {
                static Mixin instance;
                return instance;
            }

            template<class Mixin>
            const Name& Definition<Mixin>::ClsName()
            {
                return Mixin::className;
            }

            template<class Mixin>
            void Definition<Mixin>::PushAllToModule(Falcon::Module &pushTo)
            {
                auto clsName = Instance().GetClsName();
                auto cls = CreateClassSymbol(clsName, pushTo, std::integral_constant<bool, HasConstructor<Mixin>::value>{});
                for (auto &loop : Instance().GetProperties())
                    loop->PushToFalcon(cls, pushTo);
                for (auto &loop : Instance().GetMethods())
                    loop->PushToFalcon(cls, pushTo);
            }
        }
    }
}