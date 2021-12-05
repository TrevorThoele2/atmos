#pragma once

#include "Logger.h"

#include <iostream>

namespace Atmos::Logging
{
    class StderrToLog
    {
    public:
        StderrToLog(Logger& logger);
        ~StderrToLog();
    private:
        Logger* logger = nullptr;
        std::streambuf* previousStream = nullptr;
    private:
        class Stream final : public std::streambuf
        {
        public:
            Stream(StderrToLog& owner);
        protected:
            std::streamsize xsputn(const char* _Ptr, std::streamsize _Count) override;
        private:
            StderrToLog* owner;
        };

        Stream stream;
    };
}