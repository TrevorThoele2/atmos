#include "AngelScriptResultVerification.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    String MessageFromResult(int result)
    {
        switch (result)
        {
        case asERROR:
            return "Generic error.";
        case asCONTEXT_NOT_PREPARED:
            return "Context is not prepared.";
        case asINVALID_ARG:
            return "Argument is invalid.";
        case asNOT_SUPPORTED:
            return "Not supported.";
        case asINVALID_NAME:
            return "Name is invalid.";
        case asNAME_TAKEN:
            return "Name is taken.";
        case asINVALID_DECLARATION:
            return "Declaration is invalid.";
        case asINVALID_TYPE:
            return "Type is invalid.";
        case asALREADY_REGISTERED:
            return "Name is already registered.";
        case asWRONG_CONFIG_GROUP:
            return "Wrong configuration group.";
        case asILLEGAL_BEHAVIOUR_FOR_TYPE:
            return "Illegal behavior for type.";
        case asWRONG_CALLING_CONV:
            return "Wrong calling convention.";
        default:
            return "Generic error.";
        }
    }

    bool IsSuccess(int result)
    {
        return result >= 0;
    }

    bool IsError(int result)
    {
        return result < 0;
    }

    void VerifyResult(int result, const Exception::NameValuePairs& details)
    {
        if (IsError(result))
        {
            const auto message = MessageFromResult(result);
            auto useDetails = details;
            useDetails.emplace_back("Scripting Engine", String("AngelScript"));
            throw Error(message, useDetails);
        }
    }
}