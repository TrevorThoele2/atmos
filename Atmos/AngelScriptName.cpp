#include "AngelScriptName.h"

#include <Chroma/StringUtility.h>

namespace Atmos::Scripting::Angel
{
    String CreateName(const String& name)
    {
        return name;
    }

    String CreateName(const std::vector<String>& namespaces, const String& name)
    {
        auto toJoin = namespaces;
        toJoin.push_back(name);
        for(auto element = toJoin.begin(); element != toJoin.end();)
        {
            if (Chroma::Trim(*element).empty())
                element = toJoin.erase(element);
            else
                ++element;
        }
        return Chroma::Join("::", toJoin.begin(), toJoin.end());
    }
}