
#include <direct.h>
#include "FileSystem.h"

#include "StasisScribe.h"

namespace Atmos
{
    namespace FileSystem
    {
        void Handler::Init()
        {
            _mkdir(GetDataPath());
            _mkdir(StasisScribeIn::GetPath());
        }
    }
}