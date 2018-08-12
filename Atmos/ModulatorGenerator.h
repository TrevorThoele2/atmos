#pragma once

#include <unordered_map>

#include "Name.h"
#include "Modulator.h"
#include "ModulatorDelta.h"
#include "ModulatorObserver.h"

#include <Chroma/Function.h>

namespace Atmos
{
    namespace Modulator
    {
        class TrackBase;
        template<class Object>
        class Track;

        class ModulatorBase;
        template<class T>
        class Modulator;

        class TrackGeneratorBase
        {
        public:
            typedef std::unique_ptr<TrackBase> GeneratePtrT;
        private:
            virtual TrackGeneratorBase* CloneImpl() const = 0;
            virtual GeneratePtrT GenerateImpl() const = 0;
        public:
            virtual ~TrackGeneratorBase() = 0 {}

            TrackGeneratorBase* Clone() const;
            GeneratePtrT Generate() const;
        };

        template<class Object>
        class TrackGenerator : public TrackGeneratorBase
        {
        public:
            typedef Object ObjectT;
            typedef Track<ObjectT> GenerateT;
            typedef typename GenerateT::ModifierT ModifierT;
            typedef typename GenerateT::GetCurrentValueT GetCurrentValueT;
        private:
            TrackGenerator* CloneImpl() const override final;
            GeneratePtrT GenerateImpl() const override final;
        private:
            Value::Type variantType;
            ModifierT modifier;
            GetCurrentValueT getCurrentValue;
        public:
            TrackGenerator();
            TrackGenerator(Value::Type variantType, ModifierT &&modifier, GetCurrentValueT &&getCurrentValue);
            TrackGenerator(const TrackGenerator &arg) = default;
            TrackGenerator(TrackGenerator &&arg);
            TrackGenerator& operator=(const TrackGenerator &arg) = default;
            TrackGenerator& operator=(TrackGenerator &&arg);
            bool operator==(const TrackGenerator &arg) const;
            bool operator!=(const TrackGenerator &arg) const;

            Value::Type GetVariantType() const;
        };

        template<class Object>
        TrackGenerator<Object>* TrackGenerator<Object>::CloneImpl() const
        {
            return new TrackGenerator(*this);
        }

        template<class Object>
        typename TrackGenerator<Object>::GeneratePtrT TrackGenerator<Object>::GenerateImpl() const
        {
            return GeneratePtrT(new GenerateT(variantType, modifier, getCurrentValue));
        }

        template<class Object>
        TrackGenerator<Object>::TrackGenerator() : variantType(Value::Type::NONE)
        {}

        template<class Object>
        TrackGenerator<Object>::TrackGenerator(Value::Type variantType, ModifierT &&modifier, GetCurrentValueT &&getCurrentValue) : variantType(variantType), modifier(std::move(modifier)), getCurrentValue(std::move(getCurrentValue))
        {}

        template<class Object>
        TrackGenerator<Object>::TrackGenerator(TrackGenerator &&arg) : variantType(std::move(arg.variantType)), modifier(std::move(arg.modifier)), getCurrentValue(std::move(arg.getCurrentValue))
        {}

        template<class Object>
        TrackGenerator<Object>& TrackGenerator<Object>::operator=(TrackGenerator &&arg)
        {
            variantType = std::move(arg.variantType);
            modifier = std::move(arg.modifier);
            getCurrentValue = std::move(arg.getCurrentValue);
            return *this;
        }

        template<class Object>
        bool TrackGenerator<Object>::operator==(const TrackGenerator &arg) const
        {
            return variantType == arg.variantType && modifier == arg.modifier && getCurrentValue == arg.getCurrentValue;
        }

        template<class Object>
        bool TrackGenerator<Object>::operator!=(const TrackGenerator &arg) const
        {
            return !(*this == arg);
        }

        template<class Object>
        Value::Type TrackGenerator<Object>::GetVariantType() const
        {
            return variantType;
        }

        class GeneratorBase
        {
        public:
            typedef std::unique_ptr<TrackBase> GenerateTrackPtrT;
        private:
            Name name;

            virtual Observer GenerateImpl() const = 0;
            virtual GenerateTrackPtrT GenerateTrackImpl(const Name &name) const = 0;
        public:
            GeneratorBase(const Name &name);
            GeneratorBase(GeneratorBase &&arg);
            GeneratorBase& operator=(GeneratorBase &&arg);
            virtual ~GeneratorBase() = 0 {}
            bool operator==(const GeneratorBase &arg) const;

            Observer Generate() const;
            GenerateTrackPtrT GenerateTrack(const Name &name) const;

            const Name& GetName() const;
        };

        template<class Object>
        class Generator : public GeneratorBase
        {
        public:
            typedef Object ObjectT;
            typedef Modulator<ObjectT> GenerateT;
            using TrackGeneratorT = TrackGenerator<ObjectT>;
            using TrackGenerateT = Track<ObjectT>;
        private:
            Observer GenerateImpl() const override final;
            GenerateTrackPtrT GenerateTrackImpl(const Name &name) const override final;
        private:
            typedef std::unique_ptr<TrackGeneratorBase> TrackGeneratorPtr;
            std::unordered_map<Name, TrackGeneratorPtr> trackGenerators;
        public:
            Generator(const Name &name);
            Generator(const Generator &arg) = default;
            Generator(Generator &&arg);
            Generator& operator=(const Generator &arg) = default;
            Generator& operator=(Generator &&arg);
            bool operator==(const Generator &arg) const;
            bool operator!=(const Generator &arg) const;

            template<class Modifier, class GetCurrentValue>
            TrackGeneratorT* AddTrackGenerator(const Name &name, Value::Type variantType, Modifier &&modifier, GetCurrentValue &&getCurrentValue);
            TrackGeneratorBase* AddTrackGenerator(const Name &name, TrackGeneratorBase *copy);
        };

        template<class Object>
        Observer Generator<Object>::GenerateImpl() const
        {
            return Observer(new GenerateT(GetName()));
        }

        template<class Object>
        typename Generator<Object>::GenerateTrackPtrT Generator<Object>::GenerateTrackImpl(const Name &name) const
        {
            auto found = trackGenerators.find(name);
            if (found == trackGenerators.end())
                return nullptr;

            return found->second->Generate();
        }

        template<class Object>
        Generator<Object>::Generator(const Name &name) : GeneratorBase(name)
        {}

        template<class Object>
        Generator<Object>::Generator(Generator &&arg) : GeneratorBase(std::move(arg)), trackGenerators(std::move(arg.trackGenerators))
        {}

        template<class Object>
        Generator<Object>& Generator<Object>::operator=(Generator &&arg)
        {
            GeneratorBase::operator=(std::move(arg));
            trackGenerators = std::move(arg.trackGenerators);
            return *this;
        }

        template<class Object>
        bool Generator<Object>::operator==(const Generator &arg) const
        {
            return GeneratorBase::operator=(arg) && trackGenerators == arg.trackGenerators;
        }

        template<class Object>
        bool Generator<Object>::operator!=(const Generator &arg) const
        {
            return !(*this == arg);
        }

        template<class Object>
        template<class Modifier, class GetCurrentValue>
        typename Generator<Object>::TrackGeneratorT* Generator<Object>::AddTrackGenerator(const Name &name, Value::Type variantType, Modifier &&modifier, GetCurrentValue &&getCurrentValue)
        {
            return static_cast<TrackGeneratorT*>(trackGenerators.emplace(name, TrackGeneratorPtr(new TrackGeneratorT(variantType, std::move(modifier), std::move(getCurrentValue)))).first->second.get());
        }

        template<class Object>
        TrackGeneratorBase* Generator<Object>::AddTrackGenerator(const Name &name, TrackGeneratorBase *copy)
        {
            return trackGenerators.emplace(name, TrackGeneratorPtr(copy->Clone())).first->second.get();
        }
    }
}