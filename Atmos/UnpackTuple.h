
#pragma once

#include <tuple>
#include <type_traits>

namespace Atmos
{
    namespace Detail
    {
        template<unsigned int I, unsigned int goal>
        class TupleUnpackExecute
        {
        public:
            template<class Ret, class... FuncPtrArgs, class... TupleArgs, class... FuncArgs>
            static Ret Unpack(Ret(*function)(FuncPtrArgs...), std::tuple<TupleArgs...> &argTuple, FuncArgs... funcArgs)
            {
                return TupleUnpackExecute<I - 1, goal - 1>::Unpack(function, argTuple, std::get<I - 1>(argTuple), std::forward<FuncArgs>(funcArgs)...);
            }

            template<class Ret, class Obj, class... FuncPtrArgs, class... TupleArgs, class... FuncArgs>
            static Ret Unpack(Ret(Obj::*function)(FuncPtrArgs...), Obj &obj, std::tuple<TupleArgs...> &argTuple, FuncArgs... funcArgs)
            {
                return TupleUnpackExecute<I - 1, goal - 1>::Unpack(function, obj, argTuple, std::get<I - 1>(argTuple), std::forward<FuncArgs>(funcArgs)...);
            }

            template<class Ret, class Obj, class... FuncPtrArgs, class... TupleArgs, class... FuncArgs>
            static Ret Unpack(Ret(Obj::*function)(FuncPtrArgs...) const, const Obj &obj, std::tuple<TupleArgs...> &argTuple, FuncArgs... funcArgs)
            {
                return TupleUnpackExecute<I - 1, goal - 1>::Unpack(function, obj, argTuple, std::get<I - 1>(argTuple), std::forward<FuncArgs>(funcArgs)...);
            }
        };

        template<unsigned int I>
        class TupleUnpackExecute<I, 0>
        {
        public:
            template<class Ret, class... FuncPtrArgs, class... TupleArgs, class... FuncArgs>
            static Ret Unpack(Ret(*function)(FuncPtrArgs...), std::tuple<TupleArgs...> &argTuple, FuncArgs... funcArgs)
            {
                return function(std::forward<FuncArgs>(funcArgs)...);
            }

            template<class Ret, class Obj, class... FuncPtrArgs, class... TupleArgs, class... FuncArgs>
            static Ret Unpack(Ret(Obj::*function)(FuncPtrArgs...), Obj &obj, std::tuple<TupleArgs...> &argTuple, FuncArgs... funcArgs)
            {
                return (obj.*function)(std::forward<FuncArgs>(funcArgs)...);
            }

            template<class Ret, class Obj, class... FuncPtrArgs, class... TupleArgs, class... FuncArgs>
            static Ret Unpack(Ret(Obj::*function)(FuncPtrArgs...) const, const Obj &obj, std::tuple<TupleArgs...> &argTuple, FuncArgs... funcArgs)
            {
                return (obj.*function)(std::forward<FuncArgs>(funcArgs)...);
            }
        };
    }

    template<class Ret, class... Args, class... TupleArgs>
    Ret TupleUnpackExecute(Ret(*function)(Args...), std::tuple<TupleArgs...> &args)
    {
        return detail::TupleUnpackExecute<sizeof...(TupleArgs), sizeof...(TupleArgs)>::Unpack(function, args);
    }

    template<class Ret, class Obj, class... Args, class... TupleArgs>
    Ret TupleUnpackExecute(Ret(Obj::*function)(Args...), Obj &obj, std::tuple<TupleArgs...> &args)
    {
        return detail::TupleUnpackExecute<sizeof...(TupleArgs), sizeof...(TupleArgs)>::Unpack(function, obj, args);
    }

    template<class Ret, class Obj, class... Args, class... TupleArgs>
    Ret TupleUnpackExecute(Ret(Obj::*function)(Args...) const, const Obj &obj, std::tuple<TupleArgs...> &args)
    {
        return detail::TupleUnpackExecute<sizeof...(TupleArgs), sizeof...(TupleArgs)>::Unpack(function, obj, args);
    }

    template<unsigned int start, unsigned int stop, class Ret, class... Args, class... TupleArgs>
    Ret TupleUnpackExecute(Ret(*function)(Args...), std::tuple<TupleArgs...> &args)
    {
        return detail::TupleUnpackExecute<start, stop - start>::Unpack(function, args);
    }

    template<unsigned int start, unsigned int stop, class Ret, class Obj, class... Args, class... TupleArgs>
    Ret TupleUnpackExecute(Ret(Obj::*function)(Args...), Obj &obj, std::tuple<TupleArgs...> &args)
    {
        return detail::TupleUnpackExecute<start, stop - start>::Unpack(function, obj, args);
    }

    template<unsigned int start, unsigned int stop, class Ret, class Obj, class... Args, class... TupleArgs>
    Ret TupleUnpackExecute(Ret(Obj::*function)(Args...) const, const Obj &obj, std::tuple<TupleArgs...> &args)
    {
        return detail::TupleUnpackExecute<start, stop - start>::Unpack(function, obj, args);
    }
}