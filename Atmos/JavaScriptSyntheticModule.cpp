#include "JavaScriptSyntheticModule.h"

#include "JavaScriptString.h"

namespace Atmos::Scripting::JavaScript
{
	void SetSyntheticModuleExport(
        v8::Isolate& isolate, const v8::Local<v8::Module>& module, const String& name, const v8::Local<v8::Value>& value)
	{
        if (module->SetSyntheticModuleExport(&isolate, ToV8(isolate, name), value).IsNothing())
            throw Error(
                "Could not set synthetic module export.",
                {
                    {"Name", name}
                });
	}
}