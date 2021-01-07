#pragma once

#include "AngelScriptFixture.h"

#include <Atmos/AngelScriptCamera.h>

template<class>
class AngelScriptGlobalTestsFixture : public AngelScriptFixture
{
public:
    template<class T>
    using CreatedObject = std::tuple<Arca::Index<T>, Atmos::String>;
public:
    template<class T, std::enable_if_t<std::is_same_v<Atmos::Render::Camera, T>, int> = 0>
    CreatedObject<T> CreateObject(Arca::Reliquary& reliquary)
    {
        const auto index = Arca::Index<Atmos::Render::Camera>(reliquary);
        return TupleOf<T>(index);
    }
private:
    template<class T>
    std::tuple<Arca::Index<T>, Atmos::String> TupleOf(Arca::Index<T> index)
    {
        using Registration = Atmos::Scripting::Angel::Registration<T>;
        return { index, Atmos::Scripting::Angel::CreateName({Registration::ContainingNamespace()}, Registration::Name()) };
    }
};