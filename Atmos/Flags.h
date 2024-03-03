
#pragma once

#include <type_traits>
#include <initializer_list>
#include "Serialization.h"

namespace Atmos
{
    // Flags given should be 1, 2, 3, 4, etc.
    // These represent the bit position
    template<class enumT>
    class Flags
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        typedef typename std::underlying_type<enumT>::type Bits;
        Bits bits;
    public:
        Flags();
        Flags(enumT flag);
        Flags(const std::initializer_list<enumT> &list);

        Flags<enumT>& operator=(const Flags<enumT> &arg);
        Flags<enumT>& operator=(enumT arg);
        Flags<enumT>& operator|(const Flags<enumT> &arg);
        Flags<enumT>& operator|(enumT arg);
        Flags<enumT>& operator|=(const Flags<enumT> &arg);
        Flags<enumT>& operator|=(enumT arg);

        bool operator==(const Flags<enumT> &arg) const;
        bool operator!=(const Flags<enumT> &arg) const;

        void Set(enumT flag, bool set = true);
        void Reset();
        void Flip(enumT flag);
        bool Get(enumT flag) const;
        bool Has(enumT flag) const;
        // Checks if all of the bits are the given value
        bool CheckAll(bool value = true);
        typename const Flags<enumT>::Bits& GetData() const;
    };

    template<class enumT>
    void Flags<enumT>::Serialize(::inscription::Scribe &scribe)
    {
        scribe(bits);
    }

    template<class enumT>
    Flags<enumT>::Flags() : bits(0)
    {}

    template<class enumT>
    Flags<enumT>::Flags(enumT flag) : bits(0)
    {
        Set(flag);
    }

    template<class enumT>
    Flags<enumT>::Flags(const std::initializer_list<enumT> &list) : bits(0)
    {
        for (auto &loop : list)
            Set(loop);
    }

    template<class enumT>
    Flags<enumT>& Flags<enumT>::operator=(const Flags<enumT> &arg)
    {
        bits = arg.bits;

        return *this;
    }

    template<class enumT>
    Flags<enumT>& Flags<enumT>::operator=(enumT arg)
    {
        Reset();
        bits = static_cast<Bits>(arg);
        return *this;
    }

    template<class enumT>
    Flags<enumT>& Flags<enumT>::operator|(const Flags<enumT> &arg)
    {
        bits.set(arg);
    }

    template<class enumT>
    Flags<enumT>& Flags<enumT>::operator|(enumT arg)
    {
        Set(arg);
        return *this;
    }

    template<class enumT>
    Flags<enumT>& Flags<enumT>::operator|=(const Flags<enumT> &arg)
    {
        bits.set(arg.bits);

        return *this;
    }

    template<class enumT>
    Flags<enumT>& Flags<enumT>::operator|=(enumT arg)
    {
        Set(arg);
    }

    template<class enumT>
    bool Flags<enumT>::operator==(const Flags<enumT> &arg) const
    {
        return bits == arg.bits;
    }

    template<class enumT>
    bool Flags<enumT>::operator!=(const Flags<enumT> &arg) const
    {
        return !(*this == arg);
    }

    template<class enumT>
    void Flags<enumT>::Set(enumT flag, bool setTo)
    {
        if (Get(flag) != setTo)
            Flip(flag);
    }

    template<class enumT>
    void Flags<enumT>::Reset()
    {
        bits = 0;
    }

    template<class enumT>
    void Flags<enumT>::Flip(enumT flag)
    {
        bits ^= 1 << (static_cast<Bits>(flag) - 1);
    }

    template<class enumT>
    bool Flags<enumT>::Get(enumT flag) const
    {
        return ((bits >> (static_cast<Bits>(flag) - 1)) & 1) != 0;
    }

    template<class enumT>
    bool Flags<enumT>::Has(enumT flag) const
    {
        return Get(flag);
    }

    template<class enumT>
    bool Flags<enumT>::CheckAll(bool value)
    {
        if (value)
            return bits == std::numeric_limits<Bits>::max();
        else
            return bits == 0;
    }

    template<class enumT>
    typename const Flags<enumT>::Bits& Flags<enumT>::GetData() const
    {
        return bits;
    }
}