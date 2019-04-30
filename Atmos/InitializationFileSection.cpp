
#include "InitializationFileSection.h"

#include "StringUtility.h"

namespace Atmos
{
    InitializationFileSection::InitializationFileSection(const String& header) : header(header)
    {}

    InitializationFileSection::EntryBase* InitializationFileSection::FindEntry(const String& name)
    {
        auto found = entries.find(name);
        if (found == entries.end())
            return nullptr;

        return found->second.get();
    }

    void InitializationFileSection::SetTo(const ExtractedSection& extracted)
    {
        ATMOS_ASSERT(header == extracted.header);

        for (auto& curLine : extracted.nameValues)
        {
            auto name = curLine.first;
            auto value = curLine.second;

            auto found = entries.find(name);
            if (found == entries.end())
                continue;

            found->second->SetToFileString(value);
        }
    }

    String InitializationFileSection::FileString() const
    {
        String fileString = header;
        for (auto& loop : entries)
            fileString.append(loop.second->FileString());
        return fileString;
    }

    String InitializationFileSection::Header() const
    {
        return header;
    }
}