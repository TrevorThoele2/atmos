#pragma once

#include <cstdint>
#include <vector>

#include "LargeInteger.h"

#include "String.h"
#include "Position2D.h"
#include "Position3D.h"
#include "GridPosition.h"

#include <Function/Enum.h>
#include "Serialization.h"

namespace Atmos
{
    template<class T>
    struct VariantReverseTraits;

    class Variant
    {
    public:
        enum class Type : unsigned char
        {
            NONE,

            BOOL,
            UINT_8,
            UINT_16,
            UINT_32,
            UINT_64,
            UINT_128,
            INT_8,
            INT_16,
            INT_32,
            INT_64,

            FLOAT,
            DOUBLE,

            FIXED_64,

            STRING,
            POSITION_2D,
            POSITION_3D,
            GRID_POSITION,

            VECTOR
        };
    private:
        union
        {
            bool b;
            std::uint8_t uint8;
            std::uint16_t uint16;
            std::uint32_t uint32;
            std::uint64_t uint64;
            LargeInteger uint128;
            std::int8_t int8;
            std::int16_t int16;
            std::int32_t int32;
            std::int64_t int64;

            float f;
            double d;

            FixedPoint64 fixed64;

            String string;
            Position2D position2D;
            Position3D position3D;
            GridPosition gridPosition;

            std::vector<Variant> vector;
        };
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        template<Type t>
        friend struct VariantTraits;
    private:
        Type type;

        template<class T>
        void SetImpl(const T &t);
        template<class T>
        void SetImpl(T &&t);
        template<class T>
        void InitImpl(const T &t);
        template<class T>
        void InitImpl(T &&t);
    public:
        Variant();
        Variant(bool set);
        Variant(std::uint8_t set);
        Variant(std::uint16_t set);
        Variant(std::uint32_t set);
        Variant(std::uint64_t set);
        Variant(LargeInteger set);
        Variant(std::int8_t set);
        Variant(std::int16_t set);
        Variant(std::int32_t set);
        Variant(std::int64_t set);
        Variant(float set);
        Variant(double set);
        Variant(FixedPoint64 set);
        Variant(const String &set);
        Variant(const Position2D &set);
        Variant(const Position3D &set);
        Variant(const GridPosition &set);
        Variant(const std::vector<Variant> &set);
        Variant(Type type);
        Variant(const Variant &arg);
        Variant(Variant &&arg);
        ~Variant();
        Variant& operator=(const Variant &arg);
        Variant& operator=(Variant &&arg);

        void Set();
        void Set(bool set);
        void Set(std::uint8_t set);
        void Set(std::uint16_t set);
        void Set(std::uint32_t set);
        void Set(std::uint64_t set);
        void Set(LargeInteger set);
        void Set(std::int8_t set);
        void Set(std::int16_t set);
        void Set(std::int32_t set);
        void Set(std::int64_t set);
        void Set(float set);
        void Set(double set);
        void Set(FixedPoint64 set);
        void Set(const String &set);
        void Set(const Position2D &set);
        void Set(const Position3D &set);
        void Set(const GridPosition &set);
        void Set(const std::vector<Variant> &set);
        void Set(Type set);

        bool IsNone() const;
        bool IsBool() const;
        bool IsUInt8() const;
        bool IsUInt16() const;
        bool IsUInt32() const;
        bool IsUInt64() const;
        bool IsUInt128() const;
        bool IsInt8() const;
        bool IsInt16() const;
        bool IsInt32() const;
        bool IsInt64() const;
        bool IsFloat() const;
        bool IsDouble() const;
        bool IsFixed64() const;
        bool IsString() const;
        bool IsPosition2D() const;
        bool IsPosition3D() const;
        bool IsGridPosition() const;
        bool IsVector() const;
        template<Type t>
        bool Is() const;
        template<class T>
        bool Is() const;

        std::uint8_t AsNone() const;
        bool AsBool() const;
        std::uint8_t AsUInt8() const;
        std::uint16_t AsUInt16() const;
        std::uint32_t AsUInt32() const;
        std::uint64_t AsUInt64() const;
        LargeInteger AsUInt128() const;
        std::int8_t AsInt8() const;
        std::int16_t AsInt16() const;
        std::int32_t AsInt32() const;
        std::int64_t AsInt64() const;
        float AsFloat() const;
        double AsDouble() const;
        FixedPoint64 AsFixed64() const;
        const String& AsString() const;
        const Position2D& AsPosition2D() const;
        const Position3D& AsPosition3D() const;
        const GridPosition& AsGridPosition() const;
        const std::vector<Variant>& AsVector() const;
        template<Type t>
        typename VariantTraits<t>::T As() const;
        template<class T>
        typename VariantReverseTraits<T>::T As() const;

        bool operator==(const Variant &arg) const;
        bool operator!=(const Variant &arg) const;

        Type GetType() const;
        String ToString() const;
    };

    template<class T>
    void Variant::SetImpl(const T &t)
    {
        constexpr Type setType = VariantReverseTraits<typename std::remove_const<typename std::remove_reference<T>::type>::type>::enumType;
        if (setType == type)
            (this->*VariantTraits<setType>::mem) = t;
        else
            InitImpl(t);
    }

    template<class T>
    void Variant::SetImpl(T &&t)
    {
        constexpr Type setType = VariantReverseTraits<typename std::remove_const<typename std::remove_reference<T>::type>::type>::enumType;
        if (setType == type)
            (this->*VariantTraits<setType>::mem) = std::move(t);
        else
            InitImpl(std::move(t));
    }

    template<class T>
    void Variant::InitImpl(const T &t)
    {
        constexpr Type setType = VariantReverseTraits<typename std::remove_const<typename std::remove_reference<T>::type>::type>::enumType;
        type = VariantReverseTraits<T>::enumType;
        new (&(this->*VariantTraits<setType>::mem)) T(t);
    }

    template<class T>
    void Variant::InitImpl(T &&t)
    {
        constexpr Type setType = VariantReverseTraits<typename std::remove_const<typename std::remove_reference<T>::type>::type>::enumType;
        type = setType;
        new (&(this->*VariantTraits<setType>::mem)) T(std::move(t));
    }

    template<Variant::Type t>
    bool Variant::Is() const
    {
        return (this->*VariantTraits<t>::is)();
    }

    template<class T>
    bool Variant::Is() const
    {
        typedef typename std::remove_const<typename std::remove_reference<T>::type>::type UseType;
        return Is<VariantReverseTraits<UseType>::enumType>();
    }

    template<Variant::Type t>
    typename VariantTraits<t>::T Variant::As() const
    {
        return (this->*VariantTraits<t>::as)();
    }

    template<class T>
    typename VariantReverseTraits<T>::T Variant::As() const
    {
        typedef typename std::remove_const<typename std::remove_reference<T>::type>::type UseType;
        return As<VariantReverseTraits<UseType>::enumType>();
    }

    typedef function::EnumIterationTraits<Variant::Type, Variant::Type::BOOL, Variant::Type::VECTOR> VariantIterationTraits;

    template<Variant::Type t>
    struct VariantTraits;

    template<>
    struct VariantTraits<Variant::Type::NONE>
    {
        typedef std::uint8_t T;
        typedef std::uint8_t ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::NONE;
        static constexpr T defaultValue = 0;

        static constexpr T(Variant::*mem) = &Variant::uint8;
        static constexpr bool(Variant::*is)() const = &Variant::IsNone;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsNone;
    };

    template<>
    struct VariantTraits<Variant::Type::BOOL>
    {
        typedef bool T;
        typedef bool ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::BOOL;
        static constexpr T defaultValue = false;

        static constexpr T(Variant::*mem) = &Variant::b;
        static constexpr bool(Variant::*is)() const = &Variant::IsBool;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsBool;
    };

    template<>
    struct VariantTraits<Variant::Type::UINT_8>
    {
        typedef std::uint8_t T;
        typedef std::uint8_t ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::UINT_8;
        static constexpr T defaultValue = 0;

        static constexpr T(Variant::*mem) = &Variant::uint8;
        static constexpr bool(Variant::*is)() const = &Variant::IsUInt8;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsUInt8;
    };

    template<>
    struct VariantTraits<Variant::Type::UINT_16>
    {
        typedef std::uint16_t T;
        typedef std::uint16_t ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::UINT_16;
        static constexpr T defaultValue = 0;

        static constexpr T(Variant::*mem) = &Variant::uint16;
        static constexpr bool(Variant::*is)() const = &Variant::IsUInt16;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsUInt16;
    };

    template<>
    struct VariantTraits<Variant::Type::UINT_32>
    {
        typedef std::uint32_t T;
        typedef std::uint32_t ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::UINT_32;
        static constexpr T defaultValue = 0;

        static constexpr T(Variant::*mem) = &Variant::uint32;
        static constexpr bool(Variant::*is)() const = &Variant::IsUInt32;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsUInt32;
    };

    template<>
    struct VariantTraits<Variant::Type::UINT_64>
    {
        typedef std::uint64_t T;
        typedef std::uint64_t ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::UINT_64;
        static constexpr T defaultValue = 0;

        static constexpr T(Variant::*mem) = &Variant::uint64;
        static constexpr bool(Variant::*is)() const = &Variant::IsUInt64;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsUInt64;
    };

    template<>
    struct VariantTraits<Variant::Type::UINT_128>
    {
        typedef LargeInteger T;
        typedef LargeInteger ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::UINT_128;
        static constexpr T defaultValue = T(0U);

        static constexpr T(Variant::*mem) = &Variant::uint128;
        static constexpr bool(Variant::*is)() const = &Variant::IsUInt128;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsUInt128;
    };

    template<>
    struct VariantTraits<Variant::Type::INT_8>
    {
        typedef std::int8_t T;
        typedef std::int8_t ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::INT_8;
        static constexpr T defaultValue = 0;

        static constexpr T(Variant::*mem) = &Variant::int8;
        static constexpr bool(Variant::*is)() const = &Variant::IsInt8;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsInt8;
    };

    template<>
    struct VariantTraits<Variant::Type::INT_16>
    {
        typedef std::int16_t T;
        typedef std::int16_t ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::INT_16;
        static constexpr T defaultValue = 0;

        static constexpr T(Variant::*mem) = &Variant::int16;
        static constexpr bool(Variant::*is)() const = &Variant::IsInt16;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsInt16;
    };

    template<>
    struct VariantTraits<Variant::Type::INT_32>
    {
        typedef std::int32_t T;
        typedef std::int32_t ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::INT_32;
        static constexpr T defaultValue = 0;

        static constexpr T(Variant::*mem) = &Variant::int32;
        static constexpr bool(Variant::*is)() const = &Variant::IsInt32;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsInt32;
    };

    template<>
    struct VariantTraits<Variant::Type::INT_64>
    {
        typedef std::int64_t T;
        typedef std::int64_t ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::INT_64;
        static constexpr T defaultValue = 0;

        static constexpr T(Variant::*mem) = &Variant::int64;
        static constexpr bool(Variant::*is)() const = &Variant::IsInt64;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsInt64;
    };

    template<>
    struct VariantTraits<Variant::Type::FLOAT>
    {
        typedef float T;
        typedef float ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::FLOAT;
        static constexpr T defaultValue = 0.0f;

        static constexpr T(Variant::*mem) = &Variant::f;
        static constexpr bool(Variant::*is)() const = &Variant::IsFloat;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsFloat;
    };

    template<>
    struct VariantTraits<Variant::Type::DOUBLE>
    {
        typedef double T;
        typedef double ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::DOUBLE;
        static constexpr T defaultValue = 0.0f;

        static constexpr T(Variant::*mem) = &Variant::d;
        static constexpr bool(Variant::*is)() const = &Variant::IsDouble;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsDouble;
    };

    template<>
    struct VariantTraits<Variant::Type::FIXED_64>
    {
        typedef FixedPoint64 T;
        typedef FixedPoint64 ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::DOUBLE;
        static constexpr T defaultValue = T();

        static constexpr T(Variant::*mem) = &Variant::fixed64;
        static constexpr bool(Variant::*is)() const = &Variant::IsFixed64;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsFixed64;
    };

    template<>
    struct VariantTraits<Variant::Type::STRING>
    {
        typedef String T;
        typedef const String& ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::STRING;
        static const T defaultValue;

        static constexpr T(Variant::*mem) = &Variant::string;
        static constexpr bool(Variant::*is)() const = &Variant::IsString;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsString;
    };

    template<>
    struct VariantTraits<Variant::Type::POSITION_2D>
    {
        typedef Position2D T;
        typedef const Position2D& ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::POSITION_2D;
        static const T defaultValue;

        static constexpr T(Variant::*mem) = &Variant::position2D;
        static constexpr bool(Variant::*is)() const = &Variant::IsPosition2D;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsPosition2D;
    };

    template<>
    struct VariantTraits<Variant::Type::POSITION_3D>
    {
        typedef Position3D T;
        typedef const Position3D& ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::POSITION_3D;
        static const T defaultValue;

        static constexpr T(Variant::*mem) = &Variant::position3D;
        static constexpr bool(Variant::*is)() const = &Variant::IsPosition3D;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsPosition3D;
    };

    template<>
    struct VariantTraits<Variant::Type::GRID_POSITION>
    {
        typedef GridPosition T;
        typedef const GridPosition& ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::GRID_POSITION;
        static const T defaultValue;

        static constexpr T(Variant::*mem) = &Variant::gridPosition;
        static constexpr bool(Variant::*is)() const = &Variant::IsGridPosition;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsGridPosition;
    };

    template<>
    struct VariantTraits<Variant::Type::VECTOR>
    {
        typedef std::vector<Variant> T;
        typedef const std::vector<Variant>& ReferenceT;

        static constexpr Variant::Type enumType = Variant::Type::VECTOR;
        static const T defaultValue;

        static constexpr T(Variant::*mem) = &Variant::vector;
        static constexpr bool(Variant::*is)() const = &Variant::IsVector;
        static constexpr ReferenceT(Variant::*as)() const = &Variant::AsVector;
    };

    template<>
    struct VariantReverseTraits<bool>
    {
        typedef bool T;
        static constexpr Variant::Type enumType = Variant::Type::BOOL;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<std::uint8_t>
    {
        typedef std::uint8_t T;
        static constexpr Variant::Type enumType = Variant::Type::UINT_8;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<std::uint16_t>
    {
        typedef std::uint16_t T;
        static constexpr Variant::Type enumType = Variant::Type::UINT_16;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<std::uint32_t>
    {
        typedef std::uint32_t T;
        static constexpr Variant::Type enumType = Variant::Type::UINT_32;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<std::uint64_t>
    {
        typedef std::uint64_t T;
        static constexpr Variant::Type enumType = Variant::Type::UINT_64;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<LargeInteger>
    {
        typedef LargeInteger T;
        static constexpr Variant::Type enumType = Variant::Type::UINT_128;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<std::int8_t>
    {
        typedef std::int8_t T;
        static constexpr Variant::Type enumType = Variant::Type::INT_8;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<std::int16_t>
    {
        typedef std::int16_t T;
        static constexpr Variant::Type enumType = Variant::Type::INT_16;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<std::int32_t>
    {
        typedef std::int32_t T;
        static constexpr Variant::Type enumType = Variant::Type::INT_32;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<std::int64_t>
    {
        typedef std::int64_t T;
        static constexpr Variant::Type enumType = Variant::Type::INT_64;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<float>
    {
        typedef float T;
        static constexpr Variant::Type enumType = Variant::Type::FLOAT;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<double>
    {
        typedef double T;
        static constexpr Variant::Type enumType = Variant::Type::DOUBLE;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<FixedPoint64>
    {
        typedef FixedPoint64 T;
        static constexpr Variant::Type enumType = Variant::Type::FIXED_64;
        static constexpr T defaultValue = VariantTraits<enumType>::defaultValue;
    };

    template<>
    struct VariantReverseTraits<Position2D>
    {
        typedef Position2D T;
        static constexpr Variant::Type enumType = Variant::Type::POSITION_2D;
        static const T defaultValue;
    };

    template<>
    struct VariantReverseTraits<Position3D>
    {
        typedef Position3D T;
        static constexpr Variant::Type enumType = Variant::Type::POSITION_3D;
        static const T defaultValue;
    };

    template<>
    struct VariantReverseTraits<String>
    {
        typedef String T;
        static constexpr Variant::Type enumType = Variant::Type::STRING;
        static const T defaultValue;
    };

    template<>
    struct VariantReverseTraits<GridPosition>
    {
        typedef GridPosition T;
        static constexpr Variant::Type enumType = Variant::Type::GRID_POSITION;
        static const T defaultValue;
    };

    template<>
    struct VariantReverseTraits<std::vector<Variant>>
    {
        typedef std::vector<Variant> T;
        static constexpr Variant::Type enumType = Variant::Type::VECTOR;
        static const T defaultValue;
    };
}