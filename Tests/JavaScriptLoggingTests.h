#pragma once

#include "JavaScriptFixture.h"

class JavaScriptLoggingTestsFixture : public JavaScriptFixture
{
public:
    [[nodiscard]] Atmos::String AllLogged() const;
};