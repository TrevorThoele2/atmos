#include <catch.hpp>

#include "FileLoggingSinkTests.h"

#include <Atmos/FileLoggingSink.h>
#include <Atmos/Logger.h>
#include <Inscription/Plaintext.h>

using namespace Atmos;

SCENARIO_METHOD(FileLoggingSinkTestsFixture, "file logging sink", "[logging]")
{
    GIVEN("logger with file sink")
    {
        auto logger = std::make_unique<Logging::Logger>(Logging::Severity::Verbose);
        logger->AddSink<Logging::FileSink>();

        WHEN("logging empty string and occupied string then flushing logger")
        {
            logger->Log("");
            logger->Log("hello");
            logger->Flush();

            THEN("output correct strings")
            {
                std::string output;

                {
                    auto file = Inscription::File::InputText(std::filesystem::current_path() / "log.txt");
                    auto archive = Inscription::Archive::InputText(file);
                    auto format = Inscription::Format::InputPlaintext(archive);
                    format.ReadSize(output);
                }

                REQUIRE(output.find("<INFORMATION> \n") != std::string::npos);
            }
        }

        WHEN("logging empty string and occupied string then destroying logger")
        {
            logger->Log("");
            logger->Log("hello");
            logger.reset();

            THEN("output correct strings")
            {
                std::string output;

                {
                    auto file = Inscription::File::InputText(std::filesystem::current_path() / "log.txt");
                    auto archive = Inscription::Archive::InputText(file);
                    auto format = Inscription::Format::InputPlaintext(archive);
                    format.ReadSize(output);
                }
                
                REQUIRE(output.find("<INFORMATION> \n") != std::string::npos);
            }
        }
    }
}