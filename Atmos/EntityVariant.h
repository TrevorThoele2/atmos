#pragma once

#include <variant>
#include <cstdint>
#include "String.h"
#include <Arca/HandleSlim.h>

#include <Inscription/VariantScribe.h>

namespace Atmos::Entity
{
    using Variant = std::variant<std::int64_t, String, Arca::HandleSlim>;
}