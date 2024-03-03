#pragma once

#include <memory>
#include <map>

#include "InitializationFileEntry.h"
#include "InitializationFileExtractedSection.h"

#include "String.h"

namespace Atmos
{
    class InitializationFileSection
    {
    public:
        typedef InitializationFileEntryBase EntryBase;
        template<class T>
        using Entry = InitializationFileEntry<T>;

        typedef InitializationFileExtractedSection ExtractedSection;
    public:
        InitializationFileSection(const String& header);

        template<class T>
        Entry<T>* CreateEntry(const String& name, typename Entry<T>::FromOutside&& fromOutside);
        EntryBase* FindEntry(const String& name);

        void SetTo(const ExtractedSection& extracted);
        String FileString() const;

        String Header() const;
    private:
        String header;

        typedef std::unique_ptr<EntryBase> EntryPtr;
        typedef std::map<String, EntryPtr> EntryMap;
        EntryMap entries;
    };

    template<class T>
    InitializationFileSection::Entry<T>* InitializationFileSection::CreateEntry(const String& name, typename Entry<T>::FromOutside&& fromOutside)
    {
        auto made = new Entry<T>(name, std::move(fromOutside));
        auto emplaced = entries.emplace(name, EntryPtr(made));
        return made;
    }
}