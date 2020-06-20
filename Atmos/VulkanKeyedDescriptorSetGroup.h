#pragma once

#include "VulkanIncludes.h"
#include "VulkanDescriptorSetGroup.h"

namespace Atmos::Render::Vulkan
{
    template<class Key>
    class KeyedDescriptorSetGroup
    {
    public:
        using Definition = DescriptorSetGroup::Definition;

        struct KeyedSet
        {
            Key key;
            std::uint32_t imageIndex;
            vk::DescriptorSet descriptorSet;
            KeyedSet(Key key, std::uint32_t imageIndex, vk::DescriptorSet descriptorSet);
        };
    private:
        using KeyedSets = std::vector<KeyedSet>;
        std::vector<KeyedSet> keyedSets;
    public:
        using iterator = typename KeyedSets::iterator;
        using const_iterator = typename KeyedSets::const_iterator;
    public:
        KeyedDescriptorSetGroup(
            const std::vector<Definition>& definitions, uint32_t swapchainImageCount, vk::Device device);

        void Add(Key key);

        template<class SetupKey>
        void Start(SetupKey setupKey);

        KeyedSet* KeyedSetFor(Key key, std::uint32_t imageIndex);

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
    private:
        struct Source
        {
            Key key;
            std::uint32_t swapchainImageIndex;
            Source(Key key, std::uint32_t swapchainImageIndex);
        };

        std::vector<Source> sources;
        DescriptorSetGroup setGroup;
    private:
        std::uint32_t swapchainImageCount = 0;
    };

    template<class Key>
    KeyedDescriptorSetGroup<Key>::KeyedSet::KeyedSet(
        Key key, std::uint32_t imageIndex, vk::DescriptorSet descriptorSet)
        :
        key(key), imageIndex(imageIndex), descriptorSet(descriptorSet)
    {}

    template<class Key>
    KeyedDescriptorSetGroup<Key>::KeyedDescriptorSetGroup(
        const std::vector<Definition>& definitions, uint32_t swapchainImageCount, vk::Device device)
        :
        setGroup(definitions, device), swapchainImageCount(swapchainImageCount)
    {}

    template<class Key>
    void KeyedDescriptorSetGroup<Key>::Add(Key key)
    {
        for (auto& source : sources)
            if (source.key == key)
                return;

        for (uint32_t swapchainImageIndex = 0; swapchainImageIndex < swapchainImageCount; ++swapchainImageIndex)
            sources.emplace_back(key, swapchainImageIndex);
    }

    template<class Key>
    template<class SetupKey>
    void KeyedDescriptorSetGroup<Key>::Start(SetupKey setupKey)
    {
        const auto iterateSources = [this](auto onEach)
        {
            for (std::uint32_t i = 0; i < sources.size(); ++i)
            {
                const auto& source = sources[i];
                const auto descriptorSet = keyedSets->At(i);
                onEach(source, descriptorSet, i);
            }
        };

        const auto requiredSetSize = sources.size();
        if (keyedSets->Size() < requiredSetSize)
        {
            keyedSets.clear();
            keyedSets.reserve(requiredSetSize);
            setGroup.Reserve(requiredSetSize);

            iterateDiscriminatorSources(
                [this, setupKey](
                    const Source& source, vk::DescriptorSet descriptorSet, std::uint32_t)
                {
                    setupKey(source.key, descriptorSet);
                    keyedSets.emplace_back(
                        source.key, source.swapchainImageIndex, descriptorSet);
                });
        }
        else
        {
            iterateDiscriminatorSources(
                [this, setupKey](
                    const Source& source, vk::DescriptorSet descriptorSet, std::uint32_t index)
                {
                    auto& keyedSet = keyedSets[index];
                    keyedSet.key = source.key;
                    setupKey(source.key, descriptorSet);
                });
        }
    }

    template<class Key>
    auto KeyedDescriptorSetGroup<Key>::KeyedSetFor(Key key, std::uint32_t imageIndex) -> KeyedSet*
    {
        auto found = std::find_if(
            keyedSets.begin(),
            keyedSets.end(),
            [key, imageIndex](const KeyedSet& set)
            {
                return key == set.key && imageIndex == set.imageIndex;
            });
        return found == keyedSets.end()
            ? nullptr
            : &*found;
    }

    template<class Key>
    auto KeyedDescriptorSetGroup<Key>::begin() -> iterator
    {
        return keyedSets.begin();
    }

    template<class Key>
    auto KeyedDescriptorSetGroup<Key>::begin() const -> const_iterator
    {
        return keyedSets.begin();
    }

    template<class Key>
    auto KeyedDescriptorSetGroup<Key>::end() -> iterator
    {
        return keyedSets.end();
    }

    template<class Key>
    auto KeyedDescriptorSetGroup<Key>::end() const -> const_iterator
    {
        return keyedSets.end();
    }

    template<class Key>
    KeyedDescriptorSetGroup<Key>::Source::Source(
        Key key, std::uint32_t swapchainImageIndex)
        :
        key(key), swapchainImageIndex(swapchainImageIndex)
    {}

    template<>
    class KeyedDescriptorSetGroup<void>
    {
    public:
        using Definition = DescriptorSetGroup::Definition;

        struct Set
        {
            std::uint32_t imageIndex;
            vk::DescriptorSet descriptorSet;
            Set(std::uint32_t imageIndex, vk::DescriptorSet descriptorSet);
        };
    public:
        KeyedDescriptorSetGroup(
            const std::vector<Definition>& definitions, uint32_t swapchainImageCount, vk::Device device);

        void Start();

        Set* SetFor(std::uint32_t imageIndex);
    private:
        std::vector<Set> sets;
        DescriptorSetGroup setGroup;

        uint32_t swapchainImageCount = 0;
    };
}