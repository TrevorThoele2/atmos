
#include "nModulatorTrack.h"

#include "ModulatorDelta.h"

#include "ObjectManager.h"

namespace Atmos
{
    namespace Modulator
    {
        Track::Track(const Link& link) : link(link)
        {}

        Track::Track(const ::Inscription::Table<Track>& table) :
            INSCRIPTION_TABLE_GET_BASE(Object), INSCRIPTION_TABLE_GET_MEM(link)
        {}

        Track::iterator Track::AddNode()
        {
            auto node = Manager()->CreateObject<Node>();
            nodes.push_back(node);
            return --nodes.end();
        }

        void Track::RemoveNode(iterator itr)
        {
            nodes.erase(itr);
        }

        void Track::RemoveNode(const_iterator itr)
        {
            nodes.erase(itr);
        }

        void Track::Start()
        {
            if (!startValue.IsNone())
                link.SetValue(startValue);

            timer.SetGoal(timeTaken);
            timer.Start();

            curNode = nodes.begin();
        }

        void Track::Work()
        {
            if (timer.HasReachedGoal())
            {
                Delta delta(1.0, RadixPoint(9));
                //ModifierArgs args(object, startValue, GetCurrentValue(delta), curPos->GetEndValue(), delta);
                //modifier(args);
                link.SetValue(CurrentValue(delta));

                ++curNode;
                if (curNode == nodes.end())
                    return;

                curStartingValue = link.GetValue();
                timer.SetGoal((*curNode)->timeTaken);
                timer.Start();
            }
            else
            {
                auto& endValue = (*curNode)->endValue;
                if (endValue != startValue)
                {
                    auto& delta = CurrentDelta();
                    link.SetValue(CurrentValue(delta));
                    //ModifierArgs args(object, startValue, GetCurrentValue(delta), curPos->GetEndValue(), delta);
                    //modifier(args);
                }
            }
        }

        bool Track::IsDone() const
        {
            return curNode == nodes.end();
        }

        Track::iterator Track::begin()
        {
            return nodes.begin();
        }

        Track::const_iterator Track::begin() const
        {
            return nodes.begin();
        }

        Track::iterator Track::end()
        {
            return nodes.end();
        }

        Track::const_iterator Track::end() const
        {
            return nodes.end();
        }

        ObjectTypeDescription Track::TypeDescription() const
        {
            return ObjectTraits<Track>::TypeDescription();
        }

        Value Track::CurrentValue(Delta delta) const
        {
            auto& endValue = (*curNode)->endValue;
            if (link.Type() == Value::Type::FLOAT)
                return Value(static_cast<float>(Delta(endValue.AsFloat()) * delta + (Delta(startValue.AsFloat()) * (Delta(1) - delta))));
            else
                return Value(static_cast<std::int64_t>(Delta(endValue.AsInt()) * delta + (Delta(startValue.AsInt()) * (Delta(1) - delta))));
        }

        Delta Track::CurrentDelta() const
        {
            return (*curNode)->interpolation->TransformDelta(timer.PercentageElapsed().Get());
        }
    }

    const ObjectTypeName ObjectTraits<Modulator::Track>::typeName = "ModulatorTrack";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Modulator::Track)
    {
        INSCRIPTION_TABLE_ADD(link);
    }
}