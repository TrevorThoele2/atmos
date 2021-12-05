#include "JavaScriptFunction.h"

namespace Atmos::Scripting::JavaScript
{
	v8::Local<v8::Function> CreateFunction(
		v8::Isolate& isolate,
		const v8::Local<v8::Context>& context,
		v8::FunctionCallback functionCallback)
	{
		const auto functionTemplate = v8::FunctionTemplate::New(
			&isolate,
			functionCallback,
			v8::Local<v8::Value>(),
			v8::Local<v8::Signature>(),
			0,
			v8::ConstructorBehavior::kThrow);
		return functionTemplate->GetFunction(context).ToLocalChecked();
	}
}