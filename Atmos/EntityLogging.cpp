
#include "EntityLogging.h"

#include "Logger.h"

namespace Atmos
{
    namespace Logging
    {
        void EntityNeedsRequiredComponents(const String &message, Logger::Type type, const std::vector<String> needComponentNames, const std::vector<String> hasComponentNames, const Logger::NameValueVector &additionals)
        {
            String needsString;
            for (auto loop = needComponentNames.begin(); loop != needComponentNames.end(); ++loop)
            {
                needsString.append(*loop);
                if (loop != --needComponentNames.end())
                    needsString.append(", ");
            }

            String hasString;
            for (auto loop = hasComponentNames.begin(); loop != hasComponentNames.end(); ++loop)
            {
                hasString.append(*loop);
                if (loop != --hasComponentNames.end())
                    hasString.append(", ");
            }

            Logger::NameValueVector nameValueVector{
                    NameValuePair("Needs", needsString),
                    NameValuePair("Has", hasString) };
            
            for (auto& loop : additionals)
                nameValueVector.push_back(loop);

            Logger::Log(message, type, nameValueVector);
        }
    }
}