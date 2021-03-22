#pragma once

#include <exception>

#include "String.h"
#include "NameValuePair.h"

namespace Atmos
{
    class Exception : public std::exception
    {
    public:
        using NameValuePairs = std::vector<NameValuePair>;
    public:
        explicit Exception(const String& message);
        Exception(const String& message, NameValuePairs details);

        [[nodiscard]] String Message() const;

        [[nodiscard]] NameValuePairs Details() const;
    private:
        String message;
        NameValuePairs details;
    };
}