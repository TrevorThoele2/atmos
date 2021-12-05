#include <catch.hpp>

#include "FileLoggingSinkTests.h"

#include <Atmos/FileLoggingSink.h>
#include <Atmos/Logger.h>
#include <Inscription/Plaintext.h>

using namespace Atmos;

SCENARIO_METHOD(FileLoggingSinkTestsFixture, "file logging sinks", "[logging]")
{
    GIVEN("logger with file sink")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        logger.Add<Logging::FileSink>();

        WHEN("logging empty string then occupied string")
        {
            logger.Log("");
            logger.Log("hello");

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