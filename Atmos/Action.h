
#pragma once

#include "ActionID.h"
#include "ActionParameter.h"
#include "ActionTraits.h"
#include "FilePath.h"

#include <Chroma/StdFunction.h>
#include <Chroma/Iterate.h>
#include <Chroma/Switch.h>

#include "Serialization.h"
#include <Inscription/Array.h>

namespace Atmos
{
    namespace Act
    {
        class Action
        {
        public:
            typedef ::Chroma::ParameterIndex ParameterIndex;
            typedef ID ID;
            typedef Parameter Parameter;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            template<ID funcID>
            using FunctionTBase = typename Traits<funcID>::FunctionT;
            template<ID funcID, ParameterIndex index>
            using TypeOfIndex = typename std::remove_const<typename std::remove_reference<typename ::Chroma::FunctionTraits<FunctionTBase<funcID>>::template Parameter<index>::Type>::type>::type;

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
                virtual ParameterType GetParameterType(ParameterIndex index) const = 0;
                virtual ParameterIndex GetParameterCount() const = 0;

                virtual void Save(::Inscription::Scribe &scribe) = 0;
            };

            template<ID id>
            class Derived : public Base
            {
            public:
                typedef Traits<id> TraitsT;
                typedef FunctionTBase<id> FunctionT;
                typedef typename TraitsT::ReturnT ReturnT;
                static constexpr ParameterIndex parameterCount = TraitsT::parameterCount;

                typedef std::array<Parameter, parameterCount> Parameters;
            private:
                template<ParameterIndex index>
                using FromParameter = TypeOfIndex<id, index>;
            private:
                template<ParameterIndex index>
                struct ArrayBuilder
                {
                    static void Do(Parameters &parameters)
                    {
                        parameters[index] = Parameter(TypeOfIndex<id, index>{});
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
                    static void Do(Parameters &parameters, ::Inscription::Scribe &scribe)
                    {
                        ::Inscription::TrackingChangerStack tracking(scribe, false);

                        scribe.Save(parameters[index].Get<FromParameter<index>>());
                    }
                };

                template<ParameterIndex index>
                struct Invoker
                {
                    template<class... HolderArgs>
                    static void Do(FunctionT function, Parameters &parameters, HolderArgs && ... holder)
                    {
                        auto &fromParameter = parameters[index - 1].Get<TypeOfIndex<id, index - 1>>();
                        Invoker<index - 1>::Do(function, parameters, fromParameter, std::forward<HolderArgs>(holder)...);
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
                Derived(Parameters &&parameters);

                Derived* Clone() const override;
                void Execute() override;
                ID GetID() const override;

                void SetParameter(ParameterIndex index) override;
                void SetParameter(ParameterIndex index, const Parameter &set) override;
                void SetParameter(ParameterIndex index, Parameter &&set) override;
                void SetParameters(std::vector<Parameter> &&parameters) override;
                Parameter GetParameter(ParameterIndex index) const;
                ParameterType GetParameterType(ParameterIndex index) const;
                ParameterIndex GetParameterCount() const;

                void Save(::Inscription::Scribe &scribe) override;
            };

            class FactoryBase
            {
            public:
                virtual ~FactoryBase() = 0 {}
                virtual Base* Create() const = 0;
                virtual Base* Create(::Inscription::Scribe &scribe) const = 0;
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
                    static void Do(Parameters &parameters, ::Inscription::Scribe &scribe)
                    {
                        ::Inscription::TrackingChangerStack tracking(scribe, false);

                        ::Inscription::StackConstructor<TypeOfIndex<id, index>> parameterConstructor(scribe);
                        parameters[index] = std::move(parameterConstructor.GetMove());
                    }
                };
            public:
                static FactoryDerived instance;
                FactoryDerived();
                Derived<id>* Create() const override;
                Derived<id>* Create(::Inscription::Scribe &scribe) const override;
            };

            typedef std::unordered_map<ID, FactoryBase*> FactoryMap;
            static FactoryMap& Factories();
        private:
            std::unique_ptr<Base> base;
        private:
            void RequireParameterTypesEqual(ParameterIndex index, const Parameter &parameter);
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
            ParameterType GetParameterType(ParameterIndex index) const;
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
            ::Chroma::IterateRange<ParameterIndex, ArrayBuilder, parameterCount - 1, 0>(parameters);
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
        ParameterType Action::Derived<id>::GetParameterType(ParameterIndex index) const
        {
            return ::Atmos::Act::GetParameterType(parameters[index]);
        }

        template<ID id>
        Action::ParameterIndex Action::Derived<id>::GetParameterCount() const
        {
            return TraitsT::parameterCount;
        }

        template<ID id>
        void Action::Derived<id>::Save(::Inscription::Scribe &scribe)
        {
            ::Chroma::IterateRange<ParameterIndex, ParameterSaver, parameterCount - 1, 0>(parameters, scribe);
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
        Action::Derived<id>* Action::FactoryDerived<id>::Create(::Inscription::Scribe &scribe) const
        {
            Derived<id>*(*trueFunc)(::Inscription::Scribe&) = [](::Inscription::Scribe &scribe)
            {
                Parameters parameters;
                ::Chroma::IterateRange<ParameterIndex, ParameterLoader, parameterCount - 1, 0>(parameters, scribe);
                return new Derived<id>(std::move(parameters));
            };

            Derived<id>*(*falseFunc)(::Inscription::Scribe&) = [](::Inscription::Scribe &scribe)
            {
                return new Derived<id>();
            };

            return ::Chroma::Switch(std::integral_constant<bool, (parameterCount > 0)>{}, ::Chroma::CreateFunction(trueFunc), ::Chroma::CreateFunction(falseFunc), scribe);
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
        void TraitsCommon<idGiven, T, func>::Instantiate()
        {
            Action::InstantiateFactory<id>();
        }
    }

    using Act::Action;
}