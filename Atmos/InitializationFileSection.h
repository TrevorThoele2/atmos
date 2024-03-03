#pragma once

#include <memory>
#include <map>

#include "InitializationFileEntry.h"
#include "InitializationFileExtractedSection.h"

#include "String.h"

namespace Atmos::Initialization
{
    class FileSection
    {
    public:
        using EntryBase = FileEntryBase;
        template<class T>
        using Entry = FileEntry<T>;

        using ExtractedSection = FileExtractedSection;
    public:
        FileSection(const String& header);

        template<class T>
        Entry<T>* CreateEntry(const String& name, T* value);
        EntryBase* FindEntry(const String& name);

        void SetTo(const ExtractedSection& extracted);
        String FileString() const;

        String Header() const;
    private:
        String header;

        using EntryPtr = std::unique_ptr<EntryBase>;
        using EntryMap = std::map<String, EntryPtr>;
        EntryMap entries;
    };

    template<class T>
    FileSection::Entry<T>* FileSection::CreateEntry(const String& name, T* value)
    {
        auto made = std::make_unique<Entry<T>>(name, value);
        auto emplaced = entries.emplace(name, std::move(made));
        return static_cast<Entry<T>*>(emplaced.first->second.get());
    }
}