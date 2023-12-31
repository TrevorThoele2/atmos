#include "Exception.h"

#include "StringUtility.h"

namespace Atmos
{
    Exception::Exception(const String& message) : exception(message.c_str()), message(message)
    {}

    Exception::Exception(const String& message, NameValuePairs details) :
        exception(
            [message, details]()
            {
                String output = message;
                for (auto& loop : details)
                    output.append("\n        " + loop.name + ": " + ToString(loop.value));
                return output;
            }().c_str()),
        message(message),
        details(details)
    {}

    String Exception::Message() const
    {
        return message;
    }

    auto Exception::Details() const -> NameValuePairs
    {
        return details;
    }
}