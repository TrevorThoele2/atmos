#pragma once

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptResultVerification.h"
#include "AngelScriptHandle.h"
#include "AngelScriptPrimitive.h"
#include "ArcaIndexEquality.h"
#include <Arca/RelicIndex.h>
#include <Arca/IsGlobal.h>
#include <Arca/Reliquary.h>

namespace Atmos::Scripting::Angel
{
    template<class T>
    void RegisterArcaIndexCommon(ValueTypeRegistration<Arca::Index<T>>& registration)
    {
        using Management = typename Registration<Arca::Index<T>>::Management;

        const auto equals = [](asIScriptGeneric* generic)
        {
            const auto arg = static_cast<Arca::Index<T>*>(generic->GetArgObject(0));
            const auto self = static_cast<Arca::Index<T>*>(generic->GetObject());
            const auto result = IndexEqualsValue(*self, *arg);
            PushToReturn(result, *generic);
        };

        registration
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .template ConstImplicitConversion<Arca::Handle>(&Management::template Cast<Arca::Handle>)
            .template ConstImplicitConversion<bool>(&Management::template Cast<bool>)
            .ConstMethod(&Management::template Method<&Arca::Index<T>::ID>, "Arca::RelicID", "ID", {});
    }

    template<class T, std::enable_if_t<Arca::is_local_v<T>, int> = 0>
    void RegisterArcaIndex(ValueTypeRegistration<Arca::Index<T>>& registration)
    {
        using Management = typename Registration<Arca::Index<T>>::Management;

        const auto constructor = [](asIScriptGeneric* generic)
        {
            auto id = PullFromParameter<0, Arca::RelicID>(*generic);
            auto reliquary = UserData::RequiredReliquaryFrom(*generic);
            auto value = reliquary->Find<T>(id);
            new (generic->GetObject()) Arca::Index<T>{value};
        };

        RegisterArcaIndexCommon(registration);
        registration
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(constructor, { "Arca::RelicID id" });
    }

    template<class T, std::enable_if_t<Arca::is_global_v<T>, int> = 0>
    void RegisterArcaIndex(ValueTypeRegistration<Arca::Index<T>>& registration)
    {
        using Management = typename Registration<Arca::Index<T>>::Management;

        const auto defaultConstructor = [](asIScriptGeneric* generic)
        {
            auto reliquary = UserData::RequiredReliquaryFrom(*generic);
            auto value = reliquary->Find<T>();
            new (generic->GetObject()) Arca::Index<T>{value};
        };

        RegisterArcaIndexCommon(registration);
        registration.DefaultConstructor(defaultConstructor);
    }

    template<class ArcaT>
    Arca::Index<ArcaT> IsRequired(Arca::Index<ArcaT> index)
    {
        if (!index)
            throw Error("Required index was not occupied.");

        return index;
    }

    template<class ArcaT>
    const ArcaT* RequiredValue(Arca::Index<ArcaT> index)
    {
        return IsRequired(index).Get();
    }

    template<class ArcaT>
    const ArcaT* OptionalValue(Arca::Index<ArcaT> index)
    {
        return index.Get();
    }
}