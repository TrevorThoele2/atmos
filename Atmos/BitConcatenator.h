#pragma once

#include <limits.h>

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class BitConcatenator
    {
    public:
        typedef T Data;
        typedef bool Bit;
        typedef size_t BitPosition;
        constexpr static BitPosition bitCount = sizeof(Data) * CHAR_BIT;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        Data data;
    public:
        BitConcatenator();
        BitConcatenator(const BitConcatenator &arg) = default;
        BitConcatenator& operator=(const BitConcatenator &arg) = default;
        bool operator==(const BitConcatenator &arg) const;
        bool operator!=(const BitConcatenator &arg) const;

        Bit operator[](BitPosition bitPos) const;
        explicit operator Data() const;

        void Set(Data set);
        void Reset();
        Data Get() const;

        void SetBit(BitPosition bitPos, Bit setTo = true);
        void FlipBit(BitPosition bitPos);
        void ResetBit(BitPosition bitPos);
        bool GetBit(BitPosition bitPos) const;
        // Bit pushed is at [0]
        void PushBitBack(Bit push);
        // Bit pushed is at [bitCount - 1]
        void PushBitFront(Bit push);
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE_BINARY(BitConcatenator<T>)
    {
        scribe(data);
    }

    template<class T>
    BitConcatenator<T>::BitConcatenator() : data(0)
    {}

    template<class T>
    bool BitConcatenator<T>::operator==(const BitConcatenator &arg) const
    {
        return data == arg.data;
    }

    template<class T>
    bool BitConcatenator<T>::operator!=(const BitConcatenator &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    typename BitConcatenator<T>::Bit BitConcatenator<T>::operator[](BitPosition bitPos) const
    {
        return GetBit(bitPos);
    }

    template<class T>
    BitConcatenator<T>::operator Data() const
    {
        return data;
    }

    template<class T>
    void BitConcatenator<T>::Set(Data set)
    {
        data = set;
    }

    template<class T>
    void BitConcatenator<T>::Reset()
    {
        data = 0;
    }

    template<class T>
    typename BitConcatenator<T>::Data BitConcatenator<T>::Get() const
    {
        return data;
    }

    template<class T>
    void BitConcatenator<T>::SetBit(BitPosition bitPos, Bit setTo)
    {
        (setTo) ? data |= 1 << bitPos : ResetBit(bitPos);
    }

    template<class T>
    void BitConcatenator<T>::FlipBit(BitPosition bitPos)
    {
        data ^= 1 << (bitPos - 1);
    }

    template<class T>
    void BitConcatenator<T>::ResetBit(BitPosition bitPos)
    {
        data &= ~(1 << bitPos);
    }

    template<class T>
    typename BitConcatenator<T>::Bit BitConcatenator<T>::GetBit(BitPosition bitPos) const
    {
        return (Data(1 << bitPos) & data) != 0;
    }

    template<class T>
    void BitConcatenator<T>::PushBitBack(Bit push)
    {
        data <<= 1;
        SetBit(0, push);
    }

    template<class T>
    void BitConcatenator<T>::PushBitFront(Bit push)
    {
        data >>= 1;
        SetBit(bitCount - 1, push);
    }
}