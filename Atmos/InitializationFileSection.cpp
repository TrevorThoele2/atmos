#include "InitializationFileSection.h"

#include "StringUtility.h"

#include <Chroma/Contract.h>

namespace Atmos::Initialization
{
    FileSection::FileSection(const String& header) : header(header)
    {}

    FileSection::EntryBase* FileSection::FindEntry(const String& name)
    {
        const auto found = entries.find(name);
        if (found == entries.end())
            return nullptr;

        return found->second.get();
    }

    void FileSection::SetTo(const ExtractedSection& extracted)
    {
        DEBUG_PRECONDITION(header == extracted.header)

        for (auto& line : extracted.nameValues)
        {
            auto name = line.first;
            auto value = line.second;

            auto found = entries.find(name);
            if (found == entries.end())
                continue;

            found->second->SetToFileString(value);
        }
    }

    String FileSection::FileString() const
    {
        auto fileString = header;
        for (auto& loop : entries)
            fileString.append(loop.second->FileString());
        return fileString;
    }

    String FileSection::Header() const
    {
        return header;
    }
}