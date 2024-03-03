
#include "Random.h"

namespace Atmos
{
    template<class T>
    T GenerateIntImpl(T floor, T ceiling, std::default_random_engine &randomEngine)
    {
        std::uniform_int_distribution<T> dist(floor, ceiling);
        return dist(randomEngine);
    }

    template<class T>
    T GenerateRealImpl(T floor, T ceiling, std::default_random_engine &randomEngine)
    {
        std::uniform_real_distribution<T> dist(floor, ceiling);
        return dist(randomEngine);
    }

    Random::Random() : randomEngine(static_cast<unsigned int>(time(nullptr)))
    {}

    char Random::GenerateImpl(char floor, char ceiling)
    {
        std::uniform_int_distribution<short> dist(floor, ceiling);
        return static_cast<char>(dist(randomEngine));
    }

    unsigned char Random::GenerateImpl(unsigned char floor, unsigned char ceiling)
    {
        std::uniform_int_distribution<unsigned short> dist(floor, ceiling);
        return static_cast<char>(dist(randomEngine));
    }

    signed char Random::GenerateImpl(signed char floor, signed char ceiling)
    {
        std::uniform_int_distribution<signed short> dist(floor, ceiling);
        return static_cast<char>(dist(randomEngine));
    }

    short Random::GenerateImpl(short floor, short ceiling)
    {
        return GenerateIntImpl(floor, ceiling, randomEngine);
    }

    unsigned short Random::GenerateImpl(unsigned short floor, unsigned short ceiling)
    {
        return GenerateIntImpl(floor, ceiling, randomEngine);
    }

    int Random::GenerateImpl(int floor, int ceiling)
    {
        return GenerateIntImpl(floor, ceiling, randomEngine);
    }

    unsigned int Random::GenerateImpl(unsigned int floor, unsigned int ceiling)
    {
        return GenerateIntImpl(floor, ceiling, randomEngine);
    }

    long long Random::GenerateImpl(long long floor, long long ceiling)
    {
        return GenerateIntImpl(floor, ceiling, randomEngine);
    }

    unsigned long long Random::GenerateImpl(unsigned long long floor, unsigned long long ceiling)
    {
        return GenerateIntImpl(floor, ceiling, randomEngine);
    }

    float Random::GenerateImpl(float floor, float ceiling)
    {
        return GenerateRealImpl(floor, ceiling, randomEngine);
    }

    double Random::GenerateImpl(double floor, double ceiling)
    {
        return GenerateRealImpl(floor, ceiling, randomEngine);
    }

    bool Random::GenerateBool(double probability)
    {
        std::bernoulli_distribution dist(probability);
        return dist(randomEngine);
    }
}