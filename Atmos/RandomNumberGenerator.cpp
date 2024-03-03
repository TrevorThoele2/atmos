#include "RandomNumberGenerator.h"

#include <ctime>

namespace Atmos::Random
{
    NumberGenerator::NumberGenerator() : randomEngine(static_cast<unsigned int>(time(nullptr)))
    {}

    bool NumberGenerator::GenerateBool() const
    {
        std::bernoulli_distribution distribution;
        return distribution(randomEngine);
    }
}