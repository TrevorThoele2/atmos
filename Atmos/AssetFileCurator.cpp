#include <wx/zipstrm.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "AssetFileCurator.h"

#include <Arca/Reliquary.h>
#include "Log.h"

namespace Atmos::Asset
{
    Buffer AssetFileCurator::Handle(const ExtractAsset& command)
    {
        return Buffer{};
    }
}