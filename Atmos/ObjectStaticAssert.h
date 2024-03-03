#pragma once

#include "ObjectTypeValidation.h"

#define STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT(type)                                    \
static_assert(::Atmos::IsAtmosObject<type>::value, "This must be derived from Object")
#define STATIC_ASSERT_TYPE_DERIVED_FROM_OBJECT_SYSTEM(type)                                         \
static_assert(::Atmos::IsAtmosObjectSystem<type>::value, "This must be derived from ObjectSystem")