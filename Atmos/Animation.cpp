
/*
#include "Animation.h"
#include "Sprite.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>
#include <Inscription\Table.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AnimationDescriptor)
    {
        scribe(starting, ending);
    }

    AnimationDescriptor::AnimationDescriptor(ValueT starting, ValueT ending) : starting(starting), ending(ending)
    {}

    AnimationDescriptor& AnimationDescriptor::operator=(const AnimationDescriptor &arg)
    {
        starting = arg.starting;
        ending = arg.ending;
        return *this;
    }

    void AnimationDescriptor::Set(ValueT starting, ValueT ending)
    {
        this->starting = starting;
        this->ending = ending;
    }

    AnimationDescriptor::ValueT AnimationDescriptor::GetStarting() const
    {
        return starting;
    }

    AnimationDescriptor::ValueT AnimationDescriptor::GetEnding() const
    {
        return ending;
    }

    bool AnimationDescriptor::HasDifference() const
    {
        return GetStarting() != GetEnding();
    }

    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(Animation) : INSCRIPTION_TABLE_GET_MEM(desc), INSCRIPTION_TABLE_GET_MEM(timer), INSCRIPTION_TABLE_GET_MEM(endType), INSCRIPTION_TABLE_GET_MEM(continuous)
    {}

    void Animation::Update(double delta)
    {
        if (!desc.HasDifference())
        {
            sprite->SetIndex(desc.GetStarting());
            return;
        }

        unsigned short length = desc.GetEnding() - desc.GetStarting();
        auto set = desc.GetStarting() + (static_cast<unsigned int>(std::floor((length * delta))) % length);
        sprite->SetIndex(set);
    }

    void Animation::OnDone()
    {
        switch (endType)
        {
        case EndType::START:
            sprite->SetIndex(desc.GetStarting());
            return;
        case EndType::END:
            sprite->SetIndex(desc.GetEnding());
            return;
        }
    }

    Animation::Animation(const FrameTimer &timer, Sprite *sprite) : sprite(sprite), timer(timer), endType(EndType::START), continuous(false)
    {}

    Animation::Animation(const AnimationDescriptor &desc, const FrameTimer &timer, Sprite *sprite) : desc(desc), timer(timer), sprite(sprite), endType(EndType::START), continuous(false)
    {}

    void Animation::SetSprite(Sprite *set)
    {
        sprite = set;
    }

    Sprite* Animation::GetSprite()
    {
        return sprite;
    }

    const Sprite* Animation::GetSprite() const
    {
        return sprite;
    }

    void Animation::SetDescriptor(const AnimationDescriptor &set)
    {
        desc = set;
    }

    const AnimationDescriptor& Animation::GetDescriptor() const
    {
        return desc;
    }

    void Animation::SetEndType(EndType set)
    {
        endType = set;
    }

    Animation::EndType Animation::GetEndType() const
    {
        return endType;
    }

    void Animation::SetContinuous(bool set)
    {
        continuous = set;
    }

    bool Animation::IsContinuous() const
    {
        return continuous;
    }

    void Animation::SetTimer(const FrameTimer &set)
    {
        timer = set;
    }

    const FrameTimer& Animation::GetTimer() const
    {
        return timer;
    }

    namespace affecter
    {
        void AnimationEngine::ExecuteImpl(const FunctionT &func, ::affecter::DeltaT delta)
        {
            func.Execute(std::move(delta));
        }

        bool AnimationEngine::IsActiveImpl() const
        {
            return !animation.IsContinuous() && !animation.GetTimer().HasReachedGoal();
        }

        void AnimationEngine::OnDone(const FunctionT &func)
        {
            if (animation.GetEndType() == Animation::EndType::END)
                animation.GetSprite()->SetIndex(animation.GetDescriptor().GetEnding());
            else if (animation.GetEndType() == Animation::EndType::START)
                animation.GetSprite()->SetIndex(animation.GetDescriptor().GetStarting());
        }

        AnimationEngine::AnimationEngine(FunctionT &&function, const Animation &animation) : Engine(std::move(function)), animation(animation)
        {}
    }
}

namespace inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Animation)
        INSCRIPTION_TABLE_ADD(desc)
        INSCRIPTION_TABLE_ADD(timer)
        INSCRIPTION_TABLE_ADD(endType)
        INSCRIPTION_TABLE_ADD(continuous)
    INSCRIPTION_TABLE_END
}
*/