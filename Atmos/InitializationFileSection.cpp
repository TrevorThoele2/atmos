#include "InitializationFileSection.h"

#include "StringUtility.h"

#include <Chroma/Contract.h>

namespace Atmos::Initialization
{
    FileSection::FileSection(const String& header) : header(header)
    {}

    FileSection::EntryBase* FileSection::FindEntry(const String& name)
    {
        auto found = entries.find(name);
        if (found == entries.end())
            return nullptr;

        return found->second.get();
    }

    void FileSection::SetTo(const ExtractedSection& extracted)
    {
        DEBUG_PRECONDITION(header == extracted.header);

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

    String FileSection::FileString() const
    {
        String fileString = header;
        for (auto& loop : entries)
            fileString.append(loop.second->FileString());
        return fileString;
    }

    String FileSection::Header() const
    {
        return header;
    }
}