
#include "Variant.h"
#include "StringUtility.h"
#include <Inscription\Scribe.h>
#include <Function\Iterate.h>

namespace Atmos
{
    template<VariantIterationTraits::UnderlyingType t>
    struct VariantSerializer
    {
        static bool Check(inscription::Scribe &scribe, Variant &variant)
        {
            constexpr Variant::Type realT = static_cast<Variant::Type>(t);
            if (variant.GetType() != realT)
                return false;

            scribe((variant.*VariantTraits<realT>::as)());
            return true;
        }
    };

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Variant)
    {
        scribe(type);
        function::IterateRangeCheckStop<VariantIterationTraits::UnderlyingType, VariantSerializer, bool, VariantIterationTraits::endU, VariantIterationTraits::beginU>(false, scribe, *this);
    }

    Variant::Variant() : type(Type::NONE), uint8(0)
    {}

    Variant::Variant(bool set) : type(Type::BOOL), b(set)
    {}

    Variant::Variant(std::uint8_t set) : type(Type::UINT_8), uint8(set)
    {}

    Variant::Variant(std::uint16_t set) : type(Type::UINT_16), uint16(set)
    {}

    Variant::Variant(std::uint32_t set) : type(Type::UINT_32), uint32(set)
    {}

    Variant::Variant(std::uint64_t set) : type(Type::UINT_64), uint64(set)
    {}

    Variant::Variant(LargeInteger set) : type(Type::UINT_128), uint128(set)
    {}

    Variant::Variant(std::int8_t set) : type(Type::INT_8), int8(set)
    {}

    Variant::Variant(std::int16_t set) : type(Type::INT_16), int16(set)
    {}

    Variant::Variant(std::int32_t set) : type(Type::INT_32), int32(set)
    {}

    Variant::Variant(std::int64_t set) : type(Type::INT_64), int64(set)
    {}

    Variant::Variant(float set) : type(Type::FLOAT), f(set)
    {}

    Variant::Variant(double set) : type(Type::DOUBLE), d(set)
    {}

    Variant::Variant(FixedPoint64 set) : type(Type::FIXED_64), fixed64(set)
    {}

    Variant::Variant(const String &set) : type(Type::STRING), string(set)
    {}

    Variant::Variant(const Position2D &set) : type(Type::POSITION_2D), position2D(set)
    {}

    Variant::Variant(const Position3D &set) : type(Type::POSITION_3D), position3D(set)
    {}

    Variant::Variant(const GridPosition &set) : type(Type::GRID_POSITION), gridPosition(set)
    {}

    Variant::Variant(const std::vector<Variant> &set) : type(Type::VECTOR), vector(set)
    {}

    Variant::Variant(Type type)
    {
        Set(type);
    }

    Variant::Variant(const Variant &arg) : type(arg.type)
    {
        switch (type)
        {
        case Type::NONE:
            InitImpl(arg.uint8);
            break;
        case Type::BOOL:
            InitImpl(arg.b);
            break;
        case Type::UINT_8:
            InitImpl(arg.uint8);
            break;
        case Type::UINT_16:
            InitImpl(arg.uint16);
            break;
        case Type::UINT_32:
            InitImpl(arg.uint32);
            break;
        case Type::UINT_64:
            InitImpl(arg.uint64);
            break;
        case Type::UINT_128:
            InitImpl(arg.uint128);
            break;
        case Type::INT_8:
            InitImpl(arg.int8);
            break;
        case Type::INT_16:
            InitImpl(arg.int16);
            break;
        case Type::INT_32:
            InitImpl(arg.int32);
            break;
        case Type::INT_64:
            InitImpl(arg.int64);
            break;
        case Type::FLOAT:
            InitImpl(arg.f);
            break;
        case Type::DOUBLE:
            InitImpl(arg.d);
            break;
        case Type::FIXED_64:
            InitImpl(arg.fixed64);
            break;
        case Type::STRING:
            InitImpl(arg.string);
            break;
        case Type::POSITION_2D:
            InitImpl(arg.position2D);
            break;
        case Type::POSITION_3D:
            InitImpl(arg.position3D);
            break;
        case Type::GRID_POSITION:
            InitImpl(arg.gridPosition);
            break;
        case Type::VECTOR:
            InitImpl(arg.vector);
            break;
        }
    }

    Variant::Variant(Variant &&arg) : type(std::move(arg.type))
    {
        switch (type)
        {
        case Type::NONE:
            InitImpl(std::move(arg.uint8));
            break;
        case Type::BOOL:
            InitImpl(std::move(arg.b));
            break;
        case Type::UINT_8:
            InitImpl(std::move(arg.uint8));
            break;
        case Type::UINT_16:
            InitImpl(std::move(arg.uint16));
            break;
        case Type::UINT_32:
            InitImpl(std::move(arg.uint32));
            break;
        case Type::UINT_64:
            InitImpl(std::move(arg.uint64));
            break;
        case Type::UINT_128:
            InitImpl(std::move(arg.uint128));
            break;
        case Type::INT_8:
            InitImpl(std::move(arg.int8));
            break;
        case Type::INT_16:
            InitImpl(std::move(arg.int16));
            break;
        case Type::INT_32:
            InitImpl(std::move(arg.int32));
            break;
        case Type::INT_64:
            InitImpl(std::move(arg.int64));
            break;
        case Type::FLOAT:
            InitImpl(std::move(arg.f));
            break;
        case Type::DOUBLE:
            InitImpl(std::move(arg.d));
            break;
        case Type::FIXED_64:
            InitImpl(std::move(arg.fixed64));
            break;
        case Type::STRING:
            InitImpl(std::move(arg.string));
            break;
        case Type::POSITION_2D:
            InitImpl(std::move(arg.position2D));
            break;
        case Type::POSITION_3D:
            InitImpl(std::move(arg.position3D));
            break;
        case Type::GRID_POSITION:
            InitImpl(std::move(arg.gridPosition));
            break;
        case Type::VECTOR:
            InitImpl(std::move(arg.vector));
            break;
        }
    }

    Variant::~Variant()
    {
        switch (type)
        {
        case Type::NONE:
        case Type::BOOL:
        case Type::UINT_8:
        case Type::UINT_16:
        case Type::UINT_32:
        case Type::UINT_64:
            break;
        case Type::UINT_128:
            uint128.~LargeInteger();
            break;
        case Type::INT_8:
        case Type::INT_16:
        case Type::INT_32:
        case Type::INT_64:
        case Type::FLOAT:
        case Type::DOUBLE:
            break;
        case Type::FIXED_64:
            fixed64.~FixedPoint();
            break;
        case Type::STRING:
            string.~basic_string();
            break;
        case Type::POSITION_2D:
            position2D.~Position2D();
            break;
        case Type::POSITION_3D:
            position3D.~Position3D();
            break;
        case Type::GRID_POSITION:
            gridPosition.~GridPosition();
            break;
        case Type::VECTOR:
            vector.~vector();
            break;
        }
    }

    Variant& Variant::operator=(const Variant &arg)
    {
        switch (type)
        {
        case Type::NONE:
            SetImpl(arg.uint8);
            break;
        case Type::BOOL:
            SetImpl(arg.b);
            break;
        case Type::UINT_8:
            SetImpl(arg.uint8);
            break;
        case Type::UINT_16:
            SetImpl(arg.uint16);
            break;
        case Type::UINT_32:
            SetImpl(arg.uint32);
            break;
        case Type::UINT_64:
            SetImpl(arg.uint64);
            break;
        case Type::UINT_128:
            SetImpl(arg.uint128);
            break;
        case Type::INT_8:
            SetImpl(arg.int8);
            break;
        case Type::INT_16:
            SetImpl(arg.int16);
            break;
        case Type::INT_32:
            SetImpl(arg.int32);
            break;
        case Type::INT_64:
            SetImpl(arg.int64);
            break;
        case Type::FLOAT:
            SetImpl(arg.f);
            break;
        case Type::DOUBLE:
            SetImpl(arg.d);
            break;
        case Type::FIXED_64:
            SetImpl(arg.fixed64);
            break;
        case Type::STRING:
            SetImpl(arg.string);
            break;
        case Type::POSITION_2D:
            SetImpl(arg.position2D);
            break;
        case Type::POSITION_3D:
            SetImpl(arg.position3D);
            break;
        case Type::GRID_POSITION:
            SetImpl(arg.gridPosition);
            break;
        case Type::VECTOR:
            SetImpl(arg.vector);
            break;
        }

        return *this;
    }

    Variant& Variant::operator=(Variant &&arg)
    {
        switch (type)
        {
        case Type::NONE:
            SetImpl(std::move(arg.uint8));
            break;
        case Type::BOOL:
            SetImpl(std::move(arg.b));
            break;
        case Type::UINT_8:
            SetImpl(std::move(arg.uint8));
            break;
        case Type::UINT_16:
            SetImpl(std::move(arg.uint16));
            break;
        case Type::UINT_32:
            SetImpl(std::move(arg.uint32));
            break;
        case Type::UINT_64:
            SetImpl(std::move(arg.uint64));
            break;
        case Type::UINT_128:
            SetImpl(std::move(arg.uint128));
            break;
        case Type::INT_8:
            SetImpl(std::move(arg.int8));
            break;
        case Type::INT_16:
            SetImpl(std::move(arg.int16));
            break;
        case Type::INT_32:
            SetImpl(std::move(arg.int32));
            break;
        case Type::INT_64:
            SetImpl(std::move(arg.int64));
            break;
        case Type::FLOAT:
            SetImpl(std::move(arg.f));
            break;
        case Type::DOUBLE:
            SetImpl(std::move(arg.d));
            break;
        case Type::FIXED_64:
            SetImpl(std::move(arg.fixed64));
            break;
        case Type::STRING:
            SetImpl(std::move(arg.string));
            break;
        case Type::POSITION_2D:
            SetImpl(std::move(arg.position2D));
            break;
        case Type::POSITION_3D:
            SetImpl(std::move(arg.position3D));
            break;
        case Type::GRID_POSITION:
            SetImpl(std::move(arg.gridPosition));
            break;
        case Type::VECTOR:
            SetImpl(std::move(arg.vector));
            break;
        }

        return *this;
    }

    void Variant::Set()
    {
        type = Type::NONE;
        uint8 = 0;
    }

    void Variant::Set(bool set)
    {
        SetImpl(set);
    }

    void Variant::Set(std::uint8_t set)
    {
        SetImpl(set);
    }

    void Variant::Set(std::uint16_t set)
    {
        SetImpl(set);
    }

    void Variant::Set(std::uint32_t set)
    {
        SetImpl(set);
    }

    void Variant::Set(std::uint64_t set)
    {
        SetImpl(set);
    }

    void Variant::Set(LargeInteger set)
    {
        SetImpl(set);
    }

    void Variant::Set(std::int8_t set)
    {
        SetImpl(set);
    }

    void Variant::Set(std::int16_t set)
    {
        SetImpl(set);
    }

    void Variant::Set(std::int32_t set)
    {
        SetImpl(set);
    }

    void Variant::Set(std::int64_t set)
    {
        SetImpl(set);
    }

    void Variant::Set(float set)
    {
        SetImpl(set);
    }

    void Variant::Set(double set)
    {
        SetImpl(set);
    }

    void Variant::Set(FixedPoint64 set)
    {
        SetImpl(set);
    }

    void Variant::Set(const String &set)
    {
        SetImpl(set);
    }

    void Variant::Set(const Position2D &set)
    {
        SetImpl(set);
    }

    void Variant::Set(const Position3D &set)
    {
        SetImpl(set);
    }

    void Variant::Set(const GridPosition &set)
    {
        SetImpl(set);
    }

    void Variant::Set(const std::vector<Variant> &set)
    {
        SetImpl(set);
    }

    void Variant::Set(Type set)
    {
        this->type = set;
        switch (type)
        {
        case Type::NONE:
            uint8 = 0;
            break;
        case Type::BOOL:
            b = VariantTraits<Type::BOOL>::defaultValue;
            break;
        case Type::UINT_8:
            uint8 = VariantTraits<Type::UINT_8>::defaultValue;
            break;
        case Type::UINT_16:
            uint16 = VariantTraits<Type::UINT_16>::defaultValue;
            break;
        case Type::UINT_32:
            uint32 = VariantTraits<Type::UINT_32>::defaultValue;
            break;
        case Type::UINT_64:
            uint64 = VariantTraits<Type::UINT_64>::defaultValue;
            break;
        case Type::UINT_128:
            uint128 = VariantTraits<Type::UINT_128>::defaultValue;
            break;
        case Type::INT_8:
            int8 = VariantTraits<Type::INT_8>::defaultValue;
            break;
        case Type::INT_16:
            int16 = VariantTraits<Type::INT_16>::defaultValue;
            break;
        case Type::INT_32:
            int32 = VariantTraits<Type::INT_32>::defaultValue;
            break;
        case Type::INT_64:
            int64 = VariantTraits<Type::INT_64>::defaultValue;
            break;
        case Type::FLOAT:
            f = VariantTraits<Type::FLOAT>::defaultValue;
            break;
        case Type::DOUBLE:
            d = VariantTraits<Type::DOUBLE>::defaultValue;
            break;
        case Type::FIXED_64:
            fixed64 = VariantTraits<Type::FIXED_64>::defaultValue;
            break;
        case Type::STRING:
            string = VariantTraits<Type::STRING>::defaultValue;
            break;
        case Type::POSITION_2D:
            position2D = VariantTraits<Type::POSITION_2D>::defaultValue;
            break;
        case Type::POSITION_3D:
            position3D = VariantTraits<Type::POSITION_3D>::defaultValue;
            break;
        case Type::GRID_POSITION:
            gridPosition = VariantTraits<Type::GRID_POSITION>::defaultValue;
            break;
        case Type::VECTOR:
            vector = VariantTraits<Type::VECTOR>::defaultValue;
            break;
        }
    }

    bool Variant::IsNone() const
    {
        return type == Type::NONE;
    }

    bool Variant::IsBool() const
    {
        return type == Type::BOOL;
    }

    bool Variant::IsUInt8() const
    {
        return type == Type::UINT_8;
    }

    bool Variant::IsUInt16() const
    {
        return type == Type::UINT_16;
    }

    bool Variant::IsUInt32() const
    {
        return type == Type::UINT_32;
    }

    bool Variant::IsUInt64() const
    {
        return type == Type::UINT_64;
    }

    bool Variant::IsUInt128() const
    {
        return type == Type::UINT_128;
    }

    bool Variant::IsInt8() const
    {
        return type == Type::INT_8;
    }

    bool Variant::IsInt16() const
    {
        return type == Type::INT_16;
    }

    bool Variant::IsInt32() const
    {
        return type == Type::INT_32;
    }

    bool Variant::IsInt64() const
    {
        return type == Type::INT_64;
    }

    bool Variant::IsFloat() const
    {
        return type == Type::FLOAT;
    }

    bool Variant::IsDouble() const
    {
        return type == Type::DOUBLE;
    }

    bool Variant::IsFixed64() const
    {
        return type == Type::FIXED_64;
    }

    bool Variant::IsString() const
    {
        return type == Type::STRING;
    }

    bool Variant::IsPosition2D() const
    {
        return type == Type::POSITION_2D;
    }

    bool Variant::IsPosition3D() const
    {
        return type == Type::POSITION_3D;
    }

    bool Variant::IsGridPosition() const
    {
        return type == Type::GRID_POSITION;
    }

    bool Variant::IsVector() const
    {
        return type == Type::VECTOR;
    }

    std::uint8_t Variant::AsNone() const
    {
        return 0;
    }

    bool Variant::AsBool() const
    {
        return b;
    }

    std::uint8_t Variant::AsUInt8() const
    {
        return uint8;
    }

    std::uint16_t Variant::AsUInt16() const
    {
        return uint16;
    }

    std::uint32_t Variant::AsUInt32() const
    {
        return uint32;
    }

    std::uint64_t Variant::AsUInt64() const
    {
        return uint64;
    }

    LargeInteger Variant::AsUInt128() const
    {
        return uint128;
    }

    std::int8_t Variant::AsInt8() const
    {
        return int8;
    }

    std::int16_t Variant::AsInt16() const
    {
        return int16;
    }

    std::int32_t Variant::AsInt32() const
    {
        return int32;
    }

    std::int64_t Variant::AsInt64() const
    {
        return int64;
    }

    float Variant::AsFloat() const
    {
        return f;
    }

    double Variant::AsDouble() const
    {
        return d;
    }

    FixedPoint64 Variant::AsFixed64() const
    {
        return fixed64;
    }

    const String& Variant::AsString() const
    {
        return string;
    }

    const Position2D& Variant::AsPosition2D() const
    {
        return position2D;
    }

    const Position3D& Variant::AsPosition3D() const
    {
        return position3D;
    }

    const GridPosition& Variant::AsGridPosition() const
    {
        return gridPosition;
    }

    const std::vector<Variant>& Variant::AsVector() const
    {
        return vector;
    }

    bool Variant::operator==(const Variant &arg) const
    {
        if (type != arg.type)
            return false;

        switch (type)
        {
        case Type::NONE:
            return true;

        case Type::BOOL:
            return b == arg.b;
        case Type::UINT_8:
            return uint8 == arg.uint8;
        case Type::UINT_16:
            return uint16 == arg.uint16;
        case Type::UINT_32:
            return uint32 == arg.uint32;
        case Type::UINT_64:
            return uint64 == arg.uint64;
        case Type::UINT_128:
            return uint128 == arg.uint128;
        case Type::INT_8:
            return int8 == arg.int8;
        case Type::INT_16:
            return int16 == arg.int16;
        case Type::INT_32:
            return int32 == arg.int32;
        case Type::INT_64:
            return int64 == arg.int64;

        case Type::FLOAT:
            return f == arg.f;
        case Type::DOUBLE:
            return d == arg.d;
        case Type::FIXED_64:
            return fixed64 == arg.fixed64;

        case Type::STRING:
            return string == arg.string;
        case Type::POSITION_2D:
            return position2D == arg.position2D;
        case Type::POSITION_3D:
            return position3D == arg.position3D;
        case Type::GRID_POSITION:
            return gridPosition == arg.gridPosition;

        case Type::VECTOR:
            return vector == arg.vector;
        }

        return false;
    }

    bool Variant::operator!=(const Variant &arg) const
    {
        return !(*this == arg);
    }

    Variant::Type Variant::GetType() const
    {
        return type;
    }

    String Variant::ToString() const
    {
        switch (type)
        {
        case Type::NONE:
            return String();

        case Type::BOOL:
            return ::Atmos::ToString(b);
        case Type::UINT_8:
            return ::Atmos::ToString(uint8);
        case Type::UINT_16:
            return ::Atmos::ToString(uint16);
        case Type::UINT_32:
            return ::Atmos::ToString(uint32);
        case Type::UINT_64:
            return ::Atmos::ToString(uint64);
        case Type::UINT_128:
            return ::Atmos::ToString(uint128);
        case Type::INT_8:
            return ::Atmos::ToString(int8);
        case Type::INT_16:
            return ::Atmos::ToString(int16);
        case Type::INT_32:
            return ::Atmos::ToString(int32);
        case Type::INT_64:
            return ::Atmos::ToString(int64);

        case Type::FLOAT:
            return ::Atmos::ToString(f);
        case Type::DOUBLE:
            return ::Atmos::ToString(d);
        case Type::FIXED_64:
            return ::Atmos::ToString(fixed64);

        case Type::STRING:
            return string;
        case Type::POSITION_2D:
            return String(::Atmos::ToString(position2D.GetX()) + ::Atmos::ToString(position2D.GetY()));
        case Type::POSITION_3D:
            return String(::Atmos::ToString(position3D.GetX()) + ::Atmos::ToString(position3D.GetY()) + ::Atmos::ToString(position3D.GetZ()));
        case Type::GRID_POSITION:
            return String(::Atmos::ToString(gridPosition.x) + ::Atmos::ToString(gridPosition.y) + ::Atmos::ToString(gridPosition.z));

        case Type::VECTOR:
        {
            String ret("{");
            for (auto loop = vector.begin(); loop != vector.end(); ++loop)
            {
                ret += loop->ToString();
                if (std::next(loop) != vector.end())
                    ret += ",";
            }

            ret += "}";
            return ret;
        }
        }
        
        return String();
    }

    const VariantTraits<Variant::Type::STRING>::T VariantTraits<Variant::Type::STRING>::defaultValue("");
    const VariantTraits<Variant::Type::POSITION_2D>::T VariantTraits<Variant::Type::POSITION_2D>::defaultValue;
    const VariantTraits<Variant::Type::POSITION_3D>::T VariantTraits<Variant::Type::POSITION_3D>::defaultValue;
    const VariantTraits<Variant::Type::GRID_POSITION>::T VariantTraits<Variant::Type::GRID_POSITION>::defaultValue;
    const VariantTraits<Variant::Type::VECTOR>::T VariantTraits<Variant::Type::VECTOR>::defaultValue;

    const VariantReverseTraits<String>::T VariantReverseTraits<String>::defaultValue = VariantTraits<enumType>::defaultValue;
    const VariantReverseTraits<Position2D>::T VariantReverseTraits<Position2D>::defaultValue = VariantTraits<enumType>::defaultValue;
    const VariantReverseTraits<Position3D>::T VariantReverseTraits<Position3D>::defaultValue = VariantTraits<enumType>::defaultValue;
    const VariantReverseTraits<GridPosition>::T VariantReverseTraits<GridPosition>::defaultValue = VariantTraits<enumType>::defaultValue;
    const VariantReverseTraits<std::vector<Variant>>::T VariantReverseTraits<std::vector<Variant>>::defaultValue = VariantTraits<enumType>::defaultValue;
}