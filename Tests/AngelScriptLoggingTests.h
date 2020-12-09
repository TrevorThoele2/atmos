#pragma once

#include "AngelScriptFixture.h"

class AngelScriptLoggingTestsFixture : public AngelScriptFixture
{
public:
    [[nodiscard]] Atmos::String AllLogged() const;
};