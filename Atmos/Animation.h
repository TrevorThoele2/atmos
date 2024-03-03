
/*
#pragma once

#include "FrameTimer.h"

#include "Serialization.h"

#include "AffecterSystem.h"
#include <Affecter\Engine.h>
#include <Affecter\LinearType.h>
#include <Affecter\SmoothstepType.h>

namespace Atmos
{
    class AnimationDescriptor
    {
    public:
        typedef unsigned short ValueT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT starting, ending;
    public:
        AnimationDescriptor(ValueT starting = 1, ValueT ending = 1);
        AnimationDescriptor& operator=(const AnimationDescriptor &arg);
        void Set(ValueT starting, ValueT ending);
        ValueT GetStarting() const;
        ValueT GetEnding() const;
        bool HasDifference() const;
    };

    namespace affecter { class AnimationEngine; }

    class Sprite;
    class Animation
    {
    public:
        enum class EndType
        {
            START,
            END
        };
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Animation, private);
    private:
        Sprite *sprite;
        AnimationDescriptor desc;
        EndType endType;
        FrameTimer timer;
        bool continuous;

        void Update(double delta);
        void OnDone();

        friend affecter::AnimationEngine;
    public:
        Animation(const FrameTimer &timer = FrameTimer(1.0), Sprite *sprite = nullptr);
        Animation(const AnimationDescriptor &desc, const FrameTimer &timer, Sprite *sprite = nullptr);

        template<class AffecterType>
        void Start();
        void SetSprite(Sprite *set);
        Sprite* GetSprite();
        const Sprite* GetSprite() const;

        void SetDescriptor(const AnimationDescriptor &set);
        const AnimationDescriptor& GetDescriptor() const;

        void SetEndType(EndType set);
        EndType GetEndType() const;

        void SetContinuous(bool set = true);
        bool IsContinuous() const;

        void SetTimer(const FrameTimer &set);
        const FrameTimer& GetTimer() const;
    };

    namespace affecter
    {
        class AnimationEngine : public ::affecter::Engine<double>
        {
        private:
            Animation animation;
            void ExecuteImpl(const FunctionT &func, ::affecter::DeltaT delta) override;
            bool IsActiveImpl() const override;
            void OnDone(const FunctionT &func) override;
        protected:
            AnimationEngine(FunctionT &&function, const Animation &animation);
        };
    }

    template<class AffecterType>
    void Animation::Start()
    {
        affecterSystem.Create<::affecter::Affecter<AffecterType, affecter::AnimationEngine>>(timer, &Animation::Update, *this, *this);
    }
}

namespace Inscription
{
    template<>
    class Inscripter<::Atmos::Animation> : public InscripterBase<::Atmos::Animation>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };
}
*/