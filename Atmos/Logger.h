
#pragma once

#include <vector>
#include "String.h"
#include "NameValuePair.h"
#include "Event.h"

namespace Atmos
{
    class Logger
    {
    public:
        enum class Type
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

        static Logger& Instance();
        // Before this is called, the file system in environment needs to be initialized
        static void Init();

        // Outputs the error to a text file
        static void Log(const String& string, Type type);
        // Outputs the error to a test file
        // Also outputs all of the name-value pairs
        static void Log(const String& string, Type type, NameValueVector& nvps);
        static String GetTimeValue();
        static void ClearFile();
    private:
        struct Entry
        {
            String string;
            Type type;
            NameValueVector nvps;
            Entry(const String& string, Type type, const NameValueVector& nvps);
        };
    private:
        bool initialized;
        std::vector<Entry> entries;

        Logger();
        Logger(const Logger &arg) = delete;
        Logger& operator=(const Logger &arg) = delete;
    };
}