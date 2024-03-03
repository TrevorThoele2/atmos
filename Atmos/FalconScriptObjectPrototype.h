#pragma once

#include "Name.h"
#include "GridPosition.h"

#include "FalconScriptUtility.h"

namespace Atmos
{
    namespace Fal
    {
        template<class T>
        struct FalconVariableTraitsClass
        {
            typedef T Type;
            static const Type DefaultValue;
            static bool Is(Falcon::Item &item);
            static Type FromItem(Falcon::Item &item);
            static void SetItem(Falcon::VMachine &vm, Falcon::Item &item, const Type &set);
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
                virtual void SetItem(Falcon::VMachine &vm, Falcon::Item &obj) = 0;
                virtual void FromItem(Falcon::VMachine &vm) = 0;
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
                Property* Clone(Falcon::Item &item) const override final;
                void SetItem(Falcon::VMachine &vm, Falcon::Item &obj) override final;
                void FromItem(Falcon::VMachine &vm) override final;
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
                typedef void(*FalconFuncT)(Falcon::VMachine*);
                const FalconFuncT falconFunc;
                List<PropertyPtr> parameters;
                PrototypePtr self;

                MethodBase(std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc);
                // Returns true if it found everything
                // Generally indicates if a method can continue
                bool Setup();
                // Returns true if it found all of the parameters
                // Generally indicates if a method can continue
                bool SetupProperties();
                // Returns true if it found all of the properties
                // Generally indicates if a method can continue
                bool SetupSelf();
            };

            class Method : public MethodBase
            {
            public:
                bool isStatic;
                PrototypePtr self;
                Method(const Name &name, bool isStatic, std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc);
            };

            class Constructor : public MethodBase
            {
            public:
                Constructor(std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc);

                static FALCON_FUNC SetAllPropertiesToParameters(Falcon::VMachine &vm);
            };
        private:
            template<class U, bool>
            struct TraitsRetriever;
            template<class U>
            struct TraitsRetriever<U, true> { typedef FalconVariableTraitsClass<U> TraitsT; };
            template<class U>
            struct TraitsRetriever<U, false> { typedef FalconVariableTraits<U> TraitsT; };
            template<class T>
            using TraitsT = typename TraitsRetriever<T, std::is_class<T>::value>::TraitsT;
        private:
            static List<PropertyBase*> properties;
            static List<Method*> methods;
        private:
            static void AddProperty(PropertyBase &add);
            static void AddMethod(Method &add);
        public:
            static Mixin Instance();

            template<class U>
            Property<U>* GetProperty(const Name &name);
            template<class U>
            Property<U>* GetProperty(Property<U> &prop);
            List<PropertyBase*> GetProperties();
            List<Method*> GetMethods();

            // Checks if the item is the same as this prototype
            bool Is(Falcon::Item &item);
            List<PropertyPtr> FromItem(Falcon::Item &item);
            List<PropertyPtr> FromItem(Falcon::VMachine &vm, const String &str);

            FALCON_FUNC CreateItem(Falcon::VMachine &vm);

            void PushAllToModule(Falcon::Module &pushTo);
        };

        template<class Mixin>
        Prototype<Mixin>::PropertyBase::PropertyBase(const Name &name) : name(name)
        {
            Mixin::AddProperty(*this);
        }

        template<class Mixin>
        template<class T>
        Prototype<Mixin>::Property<T>::Property(const Name &name) : PropertyBase(name)
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
        void Prototype<Mixin>::Property<T>::SetItem(Falcon::VMachine &vm, Falcon::Item &obj)
        {

        }

        template<class Mixin>
        template<class T>
        void Prototype<Mixin>::Property<T>::FromItem(Falcon::VMachine &vm)
        {
            auto found = RetrieveItemFromVM(name, &vm);
            ATMOS_ASSERT_MESSAGE(found, "Found should always be set.");
            value = TraitsT<T>::FromItem(*found);
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
        bool Prototype<Mixin>::MethodBase::Setup()
        {
            if (!SetupProperties())
                return false;

            if (!SetupSelf())
                return false;

            return true;
        }

        template<class Mixin>
        bool Prototype<Mixin>::MethodBase::SetupProperties()
        {
            for (auto &loop : parameters)
                ;// loop->FromItem();

            return true;
        }

        template<class Mixin>
        bool Prototype<Mixin>::MethodBase::SetupSelf()
        {
            return true;
        }

        template<class Mixin>
        Prototype<Mixin>::Method::Method(const Name &name, bool isStatic, std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc) : MethodBase(std::move(parameters), falconFunc)
        {}

        template<class Mixin>
        Prototype<Mixin>::Constructor::Constructor(std::vector<PropertyBase*> &&parameters, FalconFuncT falconFunc) : MethodBase(std::move(parameters), falconFunc)
        {}

        template<class Mixin>
        FALCON_FUNC Prototype<Mixin>::Constructor::SetAllPropertiesToParameters(Falcon::VMachine &vm)
        {
            for (auto &loop : properties)
                loop->SetItem(vm, vm.self());
        }

        template<class Mixin>
        typename Prototype<Mixin>::List<typename Prototype<Mixin>::PropertyBase*> Prototype<Mixin>::properties;
        template<class Mixin>
        typename Prototype<Mixin>::List<typename Prototype<Mixin>::Method*> Prototype<Mixin>::methods;

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
        FALCON_FUNC Prototype<Mixin>::CreateItem(Falcon::VMachine &vm)
        {
            SetAllProperties(vm);
        }

        template<class Mixin>
        void Prototype<Mixin>::PushAllToModule(Falcon::Module &pushTo)
        {

        }

        template<class Mixin>
        typename Prototype<Mixin>::List<typename Prototype<Mixin>::PropertyPtr> Prototype<Mixin>::FromItem(Falcon::Item &item)
        {
            List<PropertyPtr> ret;

            for (auto &loop : properties)
            {
                Falcon::Item item;
                item.getProperty(Convert(loop.name), item);
                ret.underlying.push_back(PropertyPtr(loop.Clone(item)));
            }

            return ret;
        }

        template<class Mixin>
        typename Prototype<Mixin>::List<typename Prototype<Mixin>::PropertyPtr> Prototype<Mixin>::FromItem(Falcon::VMachine &vm, const String &str)
        {
            auto found = RetrieveItemFromVM(str, vm);
            if (!found)
                return List<PropertyPtr>();

            return Fromitem(*found);
        }
    }
}