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
            uint32_t imageIndex;
            vk::DescriptorSet descriptorSet;
            KeyedSet(Key key, uint32_t imageIndex, vk::DescriptorSet descriptorSet);
        };
    private:
        using KeyedSets = std::vector<KeyedSet>;
        std::vector<KeyedSet> keyedSets;
    public:
        using iterator = typename KeyedSets::iterator;
        using const_iterator = typename KeyedSets::const_iterator;
    public:
        KeyedDescriptorSetGroup(
            const std::vector<Definition>& definitions, vk::Device device);

        void Initialize(uint32_t swapchainImageCount);

        void AddKey(Key key);

        // Returns true if the sets were recreated
        template<class SetupKey>
        bool UpdateSets(SetupKey setupKey);

        void Clear();

        [[nodiscard]] KeyedSet* KeyedSetFor(Key key, uint32_t imageIndex);

        [[nodiscard]] vk::DescriptorSetLayout Layout() const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;
    private:
        struct Source
        {
            Key key;
            uint32_t swapchainImageIndex;
            Source(Key key, uint32_t swapchainImageIndex);
        };

        std::vector<Source> sources;
        DescriptorSetGroup setGroup;
    private:
        uint32_t swapchainImageCount = 0;
    };

    template<class Key>
    KeyedDescriptorSetGroup<Key>::KeyedSet::KeyedSet(
        Key key, uint32_t imageIndex, vk::DescriptorSet descriptorSet)
        :
        key(key), imageIndex(imageIndex), descriptorSet(descriptorSet)
    {}

    template<class Key>
    KeyedDescriptorSetGroup<Key>::KeyedDescriptorSetGroup(
        const std::vector<Definition>& definitions, vk::Device device) : setGroup(definitions, device)
    {}

    template<class Key>
    void KeyedDescriptorSetGroup<Key>::Initialize(uint32_t swapchainImageCount)
    {
        this->swapchainImageCount = swapchainImageCount;
    }

    template<class Key>
    void KeyedDescriptorSetGroup<Key>::AddKey(Key key)
    {
        for (auto& source : sources)
            if (source.key == key)
                return;

        for (uint32_t swapchainImageIndex = 0; swapchainImageIndex < swapchainImageCount; ++swapchainImageIndex)
            sources.emplace_back(key, swapchainImageIndex);
    }

    template<class Key>
    template<class SetupKey>
    bool KeyedDescriptorSetGroup<Key>::UpdateSets(SetupKey setupKey)
    {
        const auto iterateSources = [this](auto onEach)
        {
            for (uint32_t i = 0; i < sources.size(); ++i)
            {
                const auto& source = sources[i];
                const auto descriptorSet = setGroup.At(i);
                onEach(source, descriptorSet, i);
            }
        };

        const auto requiredSetSize = sources.size();
        if (keyedSets.size() < requiredSetSize)
        {
            keyedSets.clear();
            keyedSets.reserve(requiredSetSize);
            setGroup.Reserve(requiredSetSize);

            iterateSources(
                [this, setupKey](
                    const Source& source, vk::DescriptorSet descriptorSet, uint32_t)
                {
                    setupKey(source.key, descriptorSet);
                    keyedSets.emplace_back(
                        source.key, source.swapchainImageIndex, descriptorSet);
                });

            return true;
        }
        else
        {
            iterateSources(
                [this, setupKey](
                    const Source& source, vk::DescriptorSet descriptorSet, uint32_t index)
                {
                    auto& keyedSet = keyedSets[index];
                    keyedSet.key = source.key;
                    setupKey(source.key, descriptorSet);
                });

            return false;
        }
    }

    template<class Key>
    void KeyedDescriptorSetGroup<Key>::Clear()
    {
        setGroup.Reset();
        keyedSets.clear();
        sources.clear();
    }

    template<class Key>
    auto KeyedDescriptorSetGroup<Key>::KeyedSetFor(Key key, uint32_t imageIndex) -> KeyedSet*
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
    vk::DescriptorSetLayout KeyedDescriptorSetGroup<Key>::Layout() const
    {
        return setGroup.DescriptorSetLayout();
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
        Key key, uint32_t swapchainImageIndex)
        :
        key(key), swapchainImageIndex(swapchainImageIndex)
    {}

    template<>
    class KeyedDescriptorSetGroup<void>
    {
    public:
        using Definition = DescriptorSetGroup::Definition;

        struct KeyedSet
        {
            uint32_t imageIndex;
            vk::DescriptorSet descriptorSet;
            KeyedSet(uint32_t imageIndex, vk::DescriptorSet descriptorSet);
        };
    private:
        using KeyedSets = std::vector<KeyedSet>;
        std::vector<KeyedSet> keyedSets;
    public:
        using iterator = KeyedSets::iterator;
        using const_iterator = KeyedSets::const_iterator;
    public:
        KeyedDescriptorSetGroup(
            const std::vector<Definition>& definitions, vk::Device device);

        void Initialize(uint32_t swapchainImageCount);

        bool UpdateSets();
        void Clear();

        [[nodiscard]] KeyedSet* KeyedSetFor(uint32_t imageIndex);

        [[nodiscard]] vk::DescriptorSetLayout Layout() const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator end() const;
    private:
        DescriptorSetGroup setGroup;

        uint32_t swapchainImageCount = 0;
    };
}