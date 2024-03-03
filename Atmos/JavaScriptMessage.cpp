#include "JavaScriptMessage.h"

#include "JavaScriptString.h"

namespace Atmos::Scripting::JavaScript
{
    Failure ToFailure(
        v8::Isolate& isolate, const v8::Local<v8::Context>& context, const v8::Local<v8::Message>& message)
    {
        const auto messageString = FromV8Required<String>(isolate, message->Get());
        
        const auto line = FromV8(message->GetLineNumber(context));
        const auto column = ToString(message->GetStartColumn()) + ":" + ToString(message->GetEndColumn());
        return Failure{ messageString, {}, line ? ToString(*line) : "", column, {}};
    }
}