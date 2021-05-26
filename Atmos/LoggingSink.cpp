#include "LoggingSink.h"

namespace Atmos::Logging
{
    Sink::~Sink() = default;

    String Sink::StandardFormattedLog(const Logging::Log& log)
    {
        return
            CurrentTimeStamp() +
            ' ' +
            SeverityToString(log.severity) +
            ' ' +
            log.message +
            (log.message.find_last_of('\n') != log.message.size() - 1 ? "\n" : "") +
            DetailsToString(log.details);
    }

    String Sink::SeverityToString(Severity severity)
    {
        return "<" + Chroma::ToUppercase(ToString(severity)) + ">";
    }

    String Sink::DetailsToString(std::optional<Details> details)
    {
        String output;
        if (details)
            for (auto& loop : *details)
                output.append("        " + loop.name + ": " + ToString(loop.value) + '\n');

        return output;
    }

    String Sink::CurrentTimeStamp()
    {
        String message;

        auto t = time(nullptr);
        struct tm now = {};
        localtime_s(&now, &t);

        const auto appendToMessage = [&message](auto append)
        {
            std::ostringstream stream;
            stream << std::setfill('0') << std::setw(2) << append;
            message.append(stream.str());
        };

        message.append("<");
        appendToMessage(now.tm_mday);
        message.append("/");
        appendToMessage(now.tm_mon + 1);
        message.append("/");
        appendToMessage(now.tm_year + 1900);
        message.append("> (");
        appendToMessage(now.tm_hour);
        message.append(":");
        appendToMessage(now.tm_min);
        message.append(":");
        appendToMessage(now.tm_sec);
        message.append(")");

        return message;
    }
}