
#pragma once

#include "ActionID.h"
#include "ActionParameter.h"
#include "ActionTraits.h"
#include "FilePath.h"

#include <Function/StdFunction.h>
#include <Function/Iterate.h>
#include <Function/Switch.h>

#include "Serialization.h"
#include <Inscription/Array.h>

namespace Atmos
{
    namespace Act
    {
        namespace Detail
        {
            template<class T>
            class StoreParameterMake
            {
            private:
                template<class U>
                struct TypeDef
                {
                    typedef U TD;
                };

                template<>
                struct TypeDef<FileName>
                {
                    typedef String TD;
                };
            public:
                typedef typename TypeDef<typename std::remove_const<typename std::remove_reference<T>::type>::type>::TD Type;
            };
        }

        class Action
        {
        public:
            typedef function::ParameterIndex ParameterIndex;
            typedef ID ID;
            typedef Parameter Parameter;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            class Base
            {
            public:
                virtual ~Base() = 0 {}

                virtual Base* Clone() const = 0;
                virtual void Execute() = 0;
                virtual ID GetID() const = 0;

                virtual void SetParameter(ParameterIndex index) = 0;
                virtual void SetParameter(ParameterIndex index, const Parameter &set) = 0;
                virtual void SetParameter(ParameterIndex index, Parameter &&set) = 0;
                virtual void SetParameters(std::vector<Parameter> &&parameters) = 0;
                virtual Parameter GetParameter(ParameterIndex index) const = 0;
                virtual Variant::Type GetParameterType(ParameterIndex index) const = 0;
                virtual ParameterIndex GetParameterCount() const = 0;

                virtual void Save(inscription::Scribe &scribe) = 0;
            };

            template<ID id>
            class Derived : public Base
            {
            public:
                typedef Traits<id> TraitsT;
                typedef typename TraitsT::FunctionT FunctionT;
                typedef typename TraitsT::ReturnT ReturnT;
                static constexpr ParameterIndex parameterCount = TraitsT::parameterCount;

                typedef std::array<Parameter, parameterCount> Parameters;
            private:
                template<ParameterIndex index>
                struct ArrayBuilder
                {
                    static void Do(Parameters &parameters)
                    {
                        parameters[index] = Parameter(typename Detail::StoreParameterMake<typename function::FunctionTraits<FunctionT>::template Parameter<index>::Type>::Type{});
                    }

                    // Emplacer
                    template<class T>
                    static void DoImprint(T &&arg, Parameters &parameters)
                    {
                        parameters[index] = Parameter(std::move(arg));
                    }
                };

                template<ParameterIndex index>
                struct ParameterSaver
                {
                    static void Do(Parameters &parameters, inscription::Scribe &scribe)
                    {
                        inscription::TrackingChangerStack tracking(scribe, false);

                        scribe.Save(parameters[index].GetAs<typename Detail::StoreParameterMake<typename Traits<id>::ParameterPackT::template Parameter<index>::Type>::Type>());
                    }
                };

                template<ParameterIndex index>
                struct Invoker
                {
                    template<class... HolderArgs>
                    static void Do(FunctionT function, Parameters &parameters, HolderArgs && ... holder)
                    {
                        Invoker<index - 1>::Do(function, parameters, parameters[index - 1].GetAs<typename Detail::StoreParameterMake<typename TraitsT::ParameterPackT::template Parameter<index - 1>::Type>::Type>(), std::forward<HolderArgs>(holder)...);
                    }
                };

                template<>
                struct Invoker<0>
                {
                    template<class... HolderArgs>
                    static void Do(FunctionT function, Parameters &parameters, HolderArgs && ... holder)
                    {
                        (function)(std::forward<HolderArgs>(holder)...);
                    }
                };
            public:
                Parameters parameters;

                Derived();
                template<class... StoreArgs>
                Derived(StoreArgs && ... storeArgs);
                Derived(Parameters &&parameters);

                Derived* Clone() const override;
                void Execute() override;
                ID GetID() const override;

                void SetParameter(ParameterIndex index) override;
                void SetParameter(ParameterIndex index, const Parameter &set) override;
                void SetParameter(ParameterIndex index, Parameter &&set) override;
                void SetParameters(std::vector<Parameter> &&parameters) override;
                Parameter GetParameter(ParameterIndex index) const;
                Variant::Type GetParameterType(ParameterIndex index) const;
                ParameterIndex GetParameterCount() const;

                void Save(inscription::Scribe &scribe) override;
            };

            class FactoryBase
            {
            public:
                virtual ~FactoryBase() = 0 {}
                virtual Base* Create() const = 0;
                virtual Base* Create(inscription::Scribe &scribe) const = 0;
            };

            template<ID id>
            class FactoryDerived : public FactoryBase
            {
            private:
                typedef typename Derived<id>::Parameters Parameters;
                static constexpr ParameterIndex parameterCount = Derived<id>::parameterCount;
            private:
                template<ParameterIndex index>
                struct ParameterLoader
                {
                    static void Do(Parameters &parameters, inscription::Scribe &scribe)
                    {
                        inscription::TrackingChangerStack tracking(scribe, false);

                        inscription::StackConstructor<typename Detail::StoreParameterMake<typename Traits<id>::ParameterPackT::template Parameter<index>::Type>::Type> parameterConstructor(scribe);
                        parameters[index] = std::move(parameterConstructor.GetMove());
                    }
                };
            public:
                static FactoryDerived instance;
                FactoryDerived();
                Derived<id>* Create() const override;
                Derived<id>* Create(inscription::Scribe &scribe) const override;
            };

            typedef std::unordered_map<ID, FactoryBase*> FactoryMap;
            static FactoryMap& Factories();
        private:
            std::unique_ptr<Base> base;
        public:
            Action() = default;
            Action(ID id);
            Action(const Action &arg);
            Action& operator=(const Action &arg);
            Action(Action &&arg);
            Action& operator=(Action &&arg);
            bool operator==(const Action &arg) const;
            bool operator!=(const Action &arg) const;

            template<ID id, class... Args>
            void Set(Args && ... args);
            // Returns true if the action was successfully set
            bool Set(ID id);

            void SetParameter(ParameterIndex index);
            void SetParameter(ParameterIndex index, const Parameter &parameter);
            void SetParameter(ParameterIndex index, Parameter &&parameter);
            void SetParameters(std::vector<Parameter> &&parameters);
            Optional<Parameter> GetParameter(ParameterIndex index) const;
            Variant::Type GetParameterType(ParameterIndex index) const;
            ParameterIndex GetParameterCount() const;

            ID GetID() const;

            void Execute();

            template<ID id>
            // Used internally
            static void InstantiateFactory();
        };

        template<ID id>
        Action::Derived<id>::Derived()
        {
            function::IterateRange<ParameterIndex, ArrayBuilder, parameterCount - 1, 0>(parameters);
        }

        template<ID id>
        template<class... StoreArgs>
        Action::Derived<id>::Derived(StoreArgs && ... storeArgs)
        {
            function::IterateRangeImprint<ArrayBuilder>(std::forward_as_tuple(std::forward<StoreArgs>(storeArgs)...), parameters);
        }

        template<ID id>
        Action::Derived<id>::Derived(Parameters &&parameters) : parameters(std::forward<Parameters>(parameters))
        {}

        template<ID id>
        Action::Derived<id>* Action::Derived<id>::Clone() const
        {
            return new Derived(*this);
        }

        template<ID id>
        ID Action::Derived<id>::GetID() const
        {
            return id;
        }

        template<ID id>
        void Action::Derived<id>::SetParameter(ParameterIndex index)
        {
            parameters[index] = Parameter();
        }

        template<ID id>
        void Action::Derived<id>::SetParameter(ParameterIndex index, const Parameter &set)
        {
            parameters[index] = set;
        }

        template<ID id>
        void Action::Derived<id>::SetParameter(ParameterIndex index, Parameter &&set)
        {
            parameters[index] = std::move(set);
        }

        template<ID id>
        void Action::Derived<id>::SetParameters(std::vector<Parameter> &&parameters)
        {
            if (this->parameters.size() != parameters.size())
                return;

            auto &thisParamsLoop = this->parameters.begin();
            auto &paramsLoop = parameters.begin();
            while (thisParamsLoop != this->parameters.end())
            {
                *thisParamsLoop = std::move(*paramsLoop);

                ++thisParamsLoop;
                ++paramsLoop;
            }
        }

        template<ID id>
        Parameter Action::Derived<id>::GetParameter(ParameterIndex index) const
        {
            return parameters[index];
        }

        template<ID id>
        Variant::Type Action::Derived<id>::GetParameterType(ParameterIndex index) const
        {
            return parameters[index].GetType();
        }

        template<ID id>
        Action::ParameterIndex Action::Derived<id>::GetParameterCount() const
        {
            return TraitsT::parameterCount;
        }

        template<ID id>
        void Action::Derived<id>::Save(inscription::Scribe &scribe)
        {
            function::IterateRange<ParameterIndex, ParameterSaver, parameterCount - 1, 0>(parameters, scribe);
        }

        template<ID id>
        void Action::Derived<id>::Execute()
        {
            Invoker<parameterCount>::Do(TraitsT::wrapped, parameters);
        }

        template<ID id>
        Action::FactoryDerived<id> Action::FactoryDerived<id>::instance;

        template<ID id>
        Action::FactoryDerived<id>::FactoryDerived()
        {
            Action::Factories().emplace(id, this);
        }

        template<ID id>
        Action::Derived<id>* Action::FactoryDerived<id>::Create() const
        {
            return new Derived<id>();
        }

        template<ID id>
        Action::Derived<id>* Action::FactoryDerived<id>::Create(inscription::Scribe &scribe) const
        {
            Derived<id>*(*trueFunc)(inscription::Scribe&) = [](inscription::Scribe &scribe)
            {
                Parameters parameters;
                function::IterateRange<ParameterIndex, ParameterLoader, parameterCount - 1, 0>(parameters, scribe);
                return new Derived<id>(std::move(parameters));
            };

            Derived<id>*(*falseFunc)(inscription::Scribe&) = [](inscription::Scribe &scribe)
            {
                return new Derived<id>();
            };

            return ::function::Switch(std::integral_constant<bool, (parameterCount > 0)>{}, ::function::CreateFunction(trueFunc), ::function::CreateFunction(falseFunc), scribe);
        }

        template<ID id, class... Args>
        void Action::Set(Args && ... args)
        {
            InstantiateFactory<id>();
            base.reset(new Derived<id>(std::forward<Args>(args)...));
        }

        template<ID id>
        void Action::InstantiateFactory()
        {
            (void)FactoryDerived<id>::instance;
        }

        // TraitsBase
        template<ID idGiven, class T, T func>
        void TraitsBase<idGiven, T, func>::Instantiate()
        {
            Action::InstantiateFactory<id>();
        }
    }

    using Act::Action;
}