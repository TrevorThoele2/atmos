#pragma once

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptResultVerification.h"
#include "AngelScriptHandle.h"
#include "AngelScriptPrimitive.h"
#include <Arca/RelicIndex.h>
#include <Arca/IsGlobal.h>

namespace Atmos::Scripting::Angel
{
    template<class T>
    void RegisterArcaIndexCommon(ValueTypeRegistration<Arca::Index<T>>& registration)
    {
        using Management = typename Registration<Arca::Index<T>>::Management;

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

        RegisterArcaIndexCommon(registration);
        registration
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::template GenerateValue<&PullFromParameter<0, Arca::RelicID>, &UserData::RequiredReliquaryFrom>,
                { "Arca::RelicID id" });
    }

    template<class T, std::enable_if_t<Arca::is_global_v<T>, int> = 0>
    void RegisterArcaIndex(ValueTypeRegistration<Arca::Index<T>>& registration)
    {
        using Management = typename Registration<Arca::Index<T>>::Management;

        RegisterArcaIndexCommon(registration);
        registration
            .DefaultConstructor(&Management::template GenerateValue<&UserData::RequiredReliquaryFrom>);
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