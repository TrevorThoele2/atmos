#pragma once

#include <vector>
#include "String.h"

namespace Atmos::Scripting::Angel
{
    [[nodiscard]] String CreateName(const String& name);
    [[nodiscard]] String CreateName(const std::vector<String>& namespaces, const String& name);
}