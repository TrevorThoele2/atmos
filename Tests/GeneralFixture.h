#pragma once

#include <TestFramework/DataGeneration.h>
#include <Atmos/FileLoggingSink.h>

class GeneralFixture
{
public:
    TestFramework::DataGeneration dataGeneration{};
public:
    GeneralFixture();
};