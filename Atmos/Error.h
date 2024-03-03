
#pragma once

#include <vector>
#include "String.h"
#include "NameValuePair.h"
#include "Event.h"

namespace Atmos
{
    class ErrorHandler
    {
    public:
        enum class Severity
        {
            ERROR_SEVERE,
            ERROR_MODERATE,
            ERROR_LOW,
            WARNING,
            INFORMATION
        };

        typedef std::vector<NameValuePair> NameValueVector;
    public:
        Event<const String&> onLogged;

        static ErrorHandler& Instance();
        // Before this is called, the file system in environment needs to be initialized
        static void Init();

        // Outputs the error to a text file
        static void Log(const String &string, Severity severity);
        // Outputs the error to a test file
        // Also outputs all of the name-value pairs
        static void Log(const String &string, Severity severity, NameValueVector &nvps);
        static String GetTimeValue();
        static void ClearFile();
    private:
        struct Entry
        {
            String string;
            Severity severity;
            NameValueVector nvps;
            Entry(const String &string, Severity severity, const NameValueVector &nvps);
        };
    private:
        bool initialized;
        std::vector<Entry> entries;

        ErrorHandler();
        ErrorHandler(const ErrorHandler &arg) = delete;
        ErrorHandler& operator=(const ErrorHandler &arg) = delete;
    };
}