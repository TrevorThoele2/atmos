#pragma once

#include "FalconScriptBasePrototype.h"
#include "FalconScriptUtility.h"

namespace Atmos
{
    namespace Fal
    {
        namespace Functions
        {
            class Prototype
            {
            public:
                class ParameterBase
                {
                public:
                    const Name name;
                    ParameterBase(const Name &name);
                    virtual ParameterBase* Clone(Falcon::Item &item) const = 0;
                    virtual void SetParameter(Falcon::VMachine &vm, Falcon::Item &obj) = 0;
                    void SetItem(Falcon::VMachine &vm);
                    virtual void SetItem(Falcon::VMachine &vm, Falcon::Item &obj) = 0;
                    // Returns true if it could make the item
                    virtual bool FromItem(Prototype &owner, Falcon::VMachine &vm) = 0;
                    virtual void PushToFalcon(Falcon::Symbol *func, Falcon::Module &pushTo) = 0;
                };

                typedef std::unique_ptr<ParameterBase> ParameterPtr;

                template<class T>
                class Parameter : public ParameterBase
                {
                public:
                    typedef T Type;
                public:
                    T value;
                    Parameter(const Name &name);
                    Parameter(const Name &name, const T &value);
                    Parameter(const Name &name, T &&value);
                    Parameter* Clone(Falcon::Item &item) const override final;
                    void SetParameter(Falcon::VMachine &vm, Falcon::Item &obj) override final;
                    void SetItem(Falcon::VMachine &vm, Falcon::Item &obj) override final;
                    // Returns true if it could make the item
                    bool FromItem(Prototype &owner, Falcon::VMachine &vm) override final;
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
                    std::vector<T> underlying;

                    List() = default;
                    List(std::vector<T> &&underlying);

                    T* Get(const Name &name);
                    template<class As>
                    As Get(const Name &name);
                    template<class U>
                    Parameter<U>* GetParameter(const Name &name);
                    template<class U>
                    Parameter<U>* GetParameter(Parameter<U> &parameter);

                    bool IsEmpty() const;
                    iterator begin();
                    iterator end();
                    const_iterator begin() const;
                    const_iterator end() const;
                };

                typedef Falcon::ext_func_t FalconFuncT;
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
            private:
                FalconFuncT falconFunc;
                List<ParameterBase*> parameters;
            public:
                const Name functionName;

                Prototype(const Name &functionName, FalconFuncT falconFunc);
                Prototype(const Name &functionName, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc);
                // Returns true if it found everything
                // Generally indicates if a method can continue
                bool Setup(Falcon::VMachine &vm);

                const Name& GetFunctionName();
                template<class T>
                Parameter<T>* GetParameter(const Name &name);
                template<class T>
                Parameter<T>* GetParameter(Parameter<T> &prop);
                List<ParameterBase*> GetParameters();

                void PushAllToModule(Falcon::Module &pushTo);
            };

            template<class T>
            Prototype::Parameter<T>::Parameter(const Name &name) : ParameterBase(name)
            {}

            template<class T>
            Prototype::Parameter<T>::Parameter(const Name &name, const T &value) : ParameterBase(name), value(value)
            {}

            template<class T>
            Prototype::Parameter<T>::Parameter(const Name &name, T &&value) : ParameterBase(name), value(std::move(arg.value))
            {}

            template<class T>
            typename Prototype::Parameter<T>* Prototype::Parameter<T>::Clone(Falcon::Item &item) const
            {
                auto ret = new Parameter(*this);
                ret->value = TraitsT<T>::FromItem(item);
                return ret;
            }

            template<class T>
            void Prototype::Parameter<T>::SetParameter(Falcon::VMachine &vm, Falcon::Item &item)
            {
                item = TraitsT<T>::CreateItem(vm, value);
            }

            template<class T>
            void Prototype::Parameter<T>::SetItem(Falcon::VMachine &vm, Falcon::Item &obj)
            {
                TraitsT<T>::SetItem(vm, obj, value);
            }

            template<class T>
            bool Prototype::Parameter<T>::FromItem(Prototype &owner, Falcon::VMachine &vm)
            {
                auto found = RetrieveItemFromVM(name, &vm);
                ATMOS_ASSERT_MESSAGE(found, "This object must exist.");

                if (!TraitsT<T>::Is(*found))
                {
                    Falcon::String contentString;
                    found->toString(contentString);
                    Logger::Log("A parameter for a function was not the type expected.",
                        Logger::Type::ERROR_MODERATE,
                        Logger::NameValueVector{ NameValuePair("Function Name", owner.GetFunctionName()),
                                                       NameValuePair("Parameter Name", name),
                                                       NameValuePair("Expected Type", TraitsT<T>::GetTypeString()),
                                                       NameValuePair("Content (Falcon)", Convert(contentString)) });
                    return false;
                }

                value = TraitsT<T>::FromItem(*found);
                return true;
            }

            template<class T>
            void Prototype::Parameter<T>::PushToFalcon(Falcon::Symbol *func, Falcon::Module &pushTo)
            {
                func->addParam(*pushTo.addString(Convert(name)));
            }

            template<class T>
            Prototype::List<T>::List(std::vector<T> &&underlying) : underlying(std::move(underlying))
            {}

            template<class T>
            T* Prototype::List<T>::Get(const Name &name)
            {
                for (auto &loop : *this)
                {
                    if (loop->name == name)
                        return &loop;
                }

                return nullptr;
            }

            template<class T>
            template<class As>
            As Prototype::List<T>::Get(const Name &name)
            {
                for (auto &loop : *this)
                {
                    if (loop->name == name)
                        return static_cast<As>(loop);
                }

                return As();
            }

            template<class T>
            template<class U>
            typename Prototype::Parameter<U>* Prototype::List<T>::GetParameter(const Name &name)
            {
                return Get<Parameter<U>*>(name);
            }

            template<class T>
            template<class U>
            typename Prototype::Parameter<U>* Prototype::List<T>::GetParameter(Parameter<U> &parameter)
            {
                return Get<Parameter<U>*>(parameter.name);
            }

            template<class T>
            bool Prototype::List<T>::IsEmpty() const
            {
                return underlying.empty();
            }

            template<class T>
            typename Prototype::List<T>::iterator Prototype::List<T>::begin()
            {
                return underlying.begin();
            }

            template<class T>
            typename Prototype::List<T>::iterator Prototype::List<T>::end()
            {
                return underlying.end();
            }

            template<class T>
            typename Prototype::List<T>::const_iterator Prototype::List<T>::begin() const
            {
                return underlying.begin();
            }

            template<class T>
            typename Prototype::List<T>::const_iterator Prototype::List<T>::end() const
            {
                return underlying.end();
            }

            template<class T>
            typename Prototype::Parameter<T>* Prototype::GetParameter(const Name &name)
            {
                return GetParameters().GetParameter<T>(name);
            }

            template<class T>
            typename Prototype::Parameter<T>* Prototype::GetParameter(Parameter<T> &prop)
            {
                return GetParameters().GetParameter(prop);
            }
        }
    }
}