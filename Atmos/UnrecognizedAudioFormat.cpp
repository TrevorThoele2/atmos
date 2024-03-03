#include "UnrecognizedAudioFormat.h"

namespace Atmos::Audio
{
    UnrecognizedAudioFormat::UnrecognizedAudioFormat(const Name& name) :
        Exception("The audio file format is unrecognized. Name: " + name)
    {}
}