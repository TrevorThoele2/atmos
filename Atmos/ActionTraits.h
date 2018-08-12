#pragma once

#include "ActionID.h"
#include "ActionFunctions.h"

#include <Chroma/FunctionTraits.h>

namespace Atmos
{
    namespace Act
    {
        template<ID idGiven, class T, T func>
        class TraitsCommon
        {
        private:
            typedef ::Chroma::FunctionTraits<T> TraitsBase;
        protected:
            TraitsCommon() = default;
            static void Instantiate();
        public:
            static constexpr ::Atmos::Act::ID id = idGiven;

            typedef ::Chroma::ParameterIndex ParameterIndex;

            typedef typename TraitsBase::Type FunctionT;
            static constexpr FunctionT wrapped = func;

            typedef typename TraitsBase::ReturnT ReturnT;
            typedef typename TraitsBase::ParameterPackT ParameterPackT;
            static constexpr ParameterIndex parameterCount = ParameterPackT::count;
        };

        template<ID id>
        class Traits;

#define TRAITS_BASE_PASS(passT) decltype(passT), passT
#define TRAITS_INSTANTIATE Traits() { Instantiate(); }

        template<>
        class Traits<1> : public TraitsCommon<1, TRAITS_BASE_PASS(&Script::CreateAndStart)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<2> : public TraitsCommon<2, TRAITS_BASE_PASS(&Script::CreateAndStartGlobal)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<3> : public TraitsCommon<3, TRAITS_BASE_PASS(&Script::CreateAndStartExternal)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<4> : public TraitsCommon<4, TRAITS_BASE_PASS(&Script::CreateAndStartGlobalExternal)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<5> : public TraitsCommon<5, TRAITS_BASE_PASS(&Speech::StartSpeech)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<6> : public TraitsCommon<6, TRAITS_BASE_PASS(&Music::Change)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<7> : public TraitsCommon<7, TRAITS_BASE_PASS(&Scheduler::Schedule)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<8> : public TraitsCommon<8, TRAITS_BASE_PASS(&Field::Change)>
        {
        private:
            TRAITS_INSTANTIATE;
        };
#undef TRAITS_BASE_PASS
#undef TRAITS_INSTANTIATE

        static const ID traitsStartID = 1;
        static const ID traitsEndID = 8;
    }
}