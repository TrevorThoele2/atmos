#pragma once

#include "JavaScriptFixture.h"

class JavaScriptParameterTestsFixture : public JavaScriptFixture
{
public:
	void CompileAndCreateScript(const Scripting::Variant& value);
};