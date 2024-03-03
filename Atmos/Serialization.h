
#pragma once

#include <Inscription/Access.h>
#include <Inscription/InscripterBase.h>
#include <Inscription/Convenience.h>

namespace Inscription
{
    class Scribe;
    template<class T>
    class Inscripter;
    template<class T>
    class Table;
}

#define ATMOS_SERIALIZATION_SIMPLE_CONSTRUCT_SPEC_DECLARE(type) \
template<>                                                      \
void Construct(Scribe &scribe, type *&obj);

#define ATMOS_SERIALIZATION_SIMPLE_CONSTRUCT_SPEC_DEFINE(type, ...)     \
template<>                                                              \
void Construct(Scribe &scribe, type *&obj)                              \
{                                                                       \
    obj = new type(__VA_ARGS__);                                        \
    scribe(*obj);                                                       \
}

#define ATMOS_SERIALIZATION_SIMPLE_CONSTRUCT_SPEC(type, ...)    \
ATMOS_SERIALIZATION_SIMPLE_CONSTRUCT_DEFINE(type, __VA_ARGS__);