#pragma once

#include "ActionID.h"
#include "ActionFunctions.h"

#include <Function/FunctionTraits.h>

namespace Atmos
{
    namespace Act
    {
        template<ID idGiven, class T, T func>
        class TraitsBase
        {
        protected:
            TraitsBase() = default;
            static void Instantiate();
        public:
            static constexpr ::Atmos::Act::ID id = idGiven;

            // Utility types
            typedef function::ParameterIndex ParameterIndex;

            // Function
            typedef typename function::FunctionTraits<T>::Type FunctionT;
            static constexpr FunctionT wrapped = func;

            // Return/parameters
            typedef typename function::FunctionTraits<T>::ReturnT ReturnT;
            typedef typename function::FunctionTraits<T>::ParameterPackT ParameterPackT;
            static constexpr ParameterIndex parameterCount = ParameterPackT::count;
        };

        template<ID id>
        class Traits;

#define TRAITS_BASE_PASS(passT) decltype(passT), passT
#define TRAITS_INSTANTIATE Traits() { Instantiate(); }
        template<>
        class Traits<1> : public TraitsBase<1, TRAITS_BASE_PASS(&Speech::StartSpeech)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<2> : public TraitsBase<2, TRAITS_BASE_PASS(&Music::Change)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<3> : public TraitsBase<3, TRAITS_BASE_PASS(&Battle::GivePlacement)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<4> : public TraitsBase<4, TRAITS_BASE_PASS(&Scheduler::Schedule)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<5> : public TraitsBase<5, TRAITS_BASE_PASS(&Script::CreateAndStart)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<6> : public TraitsBase<6, TRAITS_BASE_PASS(&Script::CreateAndStartGlobal)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<7> : public TraitsBase<7, TRAITS_BASE_PASS(&Script::CreateAndStartExternal)>
        {
        private:
            TRAITS_INSTANTIATE;
        };

        template<>
        class Traits<8> : public TraitsBase<8, TRAITS_BASE_PASS(&Script::CreateAndStartGlobalExternal)>
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