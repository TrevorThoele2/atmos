#pragma once

#include <unordered_map>

#include "ModulatorTrackNode.h"

#include "Variant.h"
#include "FrameTimer.h"
#include "Optional.h"

namespace Atmos
{
    namespace Modulator
    {
        template<class Object>
        struct TrackModifierArgs
        {
            typedef Object ObjectT;

            ObjectT &object;
            Value starting;
            Value current;
            Value ending;
            Delta delta;

            TrackModifierArgs(ObjectT &objectValue, Value starting, Value current, Value ending, Delta delta);
            TrackModifierArgs(const TrackModifierArgs &arg) = default;
            TrackModifierArgs& operator=(const TrackModifierArgs &arg) = default;
            bool operator==(const TrackModifierArgs &arg) const;
            bool operator!=(const TrackModifierArgs &arg) const;
        };

        template<class Object>
        TrackModifierArgs<Object>::TrackModifierArgs(ObjectT &object, Value starting, Value current, Value ending, Delta delta) : object(object), starting(std::move(starting)), current(std::move(current)), ending(std::move(ending)), delta(delta)
        {}

        template<class Object>
        bool TrackModifierArgs<Object>::operator==(const TrackModifierArgs &arg) const
        {
            return &object == &arg.object && starting == arg.starting && current == arg.current && ending == arg.ending && delta == arg.delta;
        }

        template<class Object>
        bool TrackModifierArgs<Object>::operator!=(const TrackModifierArgs &arg) const
        {
            return !(*this == arg);
        }

        class TrackBase
        {
        public:
            typedef size_t NodeID;
        private:
            virtual TrackBase* CloneImpl() const = 0;

            virtual void SetDefaultStartValueImpl(const Value &set) = 0;
            virtual void ResetDefaultStartValueImpl() = 0;

            virtual NodeID AddNodeImpl() = 0;
            virtual void RemoveNodeImpl(NodeID id) = 0;
            virtual TrackNode* FindNodeImpl(NodeID id) = 0;
            virtual const TrackNode* FindNodeImpl(NodeID id) const = 0;

            virtual TimeValue GetSumTimeTakenImpl() const = 0;
        protected:
            TrackBase() = default;
        public:
            virtual ~TrackBase() = 0;

            TrackBase* Clone() const;

            void SetDefaultStartValue(const Value &set);
            void ResetDefaultStartValue();

            NodeID AddNode();
            void RemoveNode(NodeID id);
            TrackNode* FindNode(NodeID id);
            const TrackNode* FindNode(NodeID id) const;
            bool HasNode(NodeID id) const;

            TimeValue GetSumTimeTaken() const;
        };

        template<class Object>
        class Track : public TrackBase
        {
        public:
            typedef Object ObjectT;
            typedef TrackNode NodeT;
            typedef TrackModifierArgs<ObjectT> ModifierArgs;
            typedef ::function::Function<void, const ModifierArgs&> ModifierT;
            typedef ::function::Function<Value, const ObjectT&> GetCurrentValueT;
        private:
            typedef std::list<NodeT> NodeContainer;
            const NodeID startingID = 0;
        private:
            TrackBase* CloneImpl() const override final;

            void SetDefaultStartValueImpl(const Value &set) override final;
            void ResetDefaultStartValueImpl() override final;

            NodeID AddNodeImpl() override final;
            void RemoveNodeImpl(NodeID id) override final;
            NodeT* FindNodeImpl(NodeID id) override final;
            const NodeT* FindNodeImpl(NodeID id) const override final;

            TimeValue GetSumTimeTakenImpl() const override final;
        private:
            Value::Type variantType;
            Value startValue;

            ModifierT modifier;
            GetCurrentValueT getCurrentValue;

            Optional<Value> defaultStartValue;
            NodeContainer nodes;
            typename NodeContainer::iterator curPos;
            NodeID curPosID;

            FrameTimer timer;

            typename NodeContainer::iterator GetNodeGuaranteed(NodeID pos);
            Value GetCurrentValue(Delta delta);
        public:
            Track();
            Track(Value::Type variantType, const ModifierT &modifier, const GetCurrentValueT &getCurrentValue);
            Track(const Track &arg);
            Track(Track &&arg);
            Track& operator=(const Track &arg);
            Track& operator=(Track &&arg);
            bool operator==(const Track &arg) const;
            bool operator!=(const Track &arg) const;

            void Start(Object &object);
            // Returns true if the track is done
            bool Work(Object &object);
        };

        template<class Object>
        TrackBase* Track<Object>::CloneImpl() const
        {
            return new Track(*this);
        }

        template<class Object>
        void Track<Object>::SetDefaultStartValueImpl(const Value &set)
        {
            defaultStartValue.Set(set);
        }

        template<class Object>
        void Track<Object>::ResetDefaultStartValueImpl()
        {
            defaultStartValue.Reset();
        }

        template<class Object>
        typename Track<Object>::NodeID Track<Object>::AddNodeImpl()
        {
            nodes.push_back(NodeT(variantType));
            return nodes.size() - 1;
        }

        template<class Object>
        void Track<Object>::RemoveNodeImpl(NodeID id)
        {
            if(id >= nodes.size())
                return;

            size_t count = 0;
            for (auto loop = nodes.begin(); loop != nodes.end(); ++loop, ++count)
            {
                if (count == id)
                {
                    nodes.erase(loop);
                    return;
                }
            }
        }

        template<class Object>
        typename Track<Object>::NodeT* Track<Object>::FindNodeImpl(NodeID id)
        {
            if(id >= nodes.size())
                return nullptr;

            size_t count = 0;
            for (auto loop = nodes.begin(); loop != nodes.end(); ++loop, ++count)
            {
                if (count == id)
                    return &*loop;
            }

            return nullptr;
        }

        template<class Object>
        typename const Track<Object>::NodeT* Track<Object>::FindNodeImpl(NodeID id) const
        {
            if (id >= nodes.size())
                return nullptr;

            size_t count = 0;
            for (auto loop = nodes.begin(); loop != nodes.end(); ++loop, ++count)
            {
                if (count == id)
                    return &*loop;
            }

            return nullptr;
        }

        template<class Object>
        TimeValue Track<Object>::GetSumTimeTakenImpl() const
        {
            TimeValue ret;
            for (auto &loop : nodes)
                ret += loop.GetTimeTaken();
            return ret;
        }

        template<class Object>
        typename Track<Object>::NodeContainer::iterator Track<Object>::GetNodeGuaranteed(NodeID pos)
        {
            auto selectedNode = nodes.begin();
            for (NodeID loop = 0; loop != pos; ++loop)
                ++selectedNode;
            return selectedNode;
        }

        template<class Object>
        Value Track<Object>::GetCurrentValue(Delta delta)
        {
            auto &endValue = curPos->GetEndValue();
            if (variantType == Value::Type::FLOAT)
                return Value(static_cast<float>((Delta(endValue.AsFloat()) * delta) + (Delta(startValue.AsFloat()) * (Delta(1) - delta))));
            else
                return Value(static_cast<std::int64_t>((Delta(endValue.AsInt()) * delta) + (Delta(startValue.AsInt()) * (Delta(1) - delta))));
        }

        template<class Object>
        Track<Object>::Track() : startValue(Value(std::int64_t(0))), curPos(nodes.end()), curPosID(0)
        {}

        template<class Object>
        Track<Object>::Track(Value::Type variantType, const ModifierT &modifier, const GetCurrentValueT &getCurrentValue) : variantType(variantType), modifier(modifier), getCurrentValue(getCurrentValue), startValue(Value(std::int64_t(0))), curPos(nodes.end())
        {}

        template<class Object>
        Track<Object>::Track(const Track &arg) : variantType(arg.variantType), startValue(arg.startValue), modifier(arg.modifier), getCurrentValue(arg.getCurrentValue), nodes(arg.nodes), curPos(GetNodeGuaranteed(arg.curPosID)), curPosID(arg.curPosID)
        {}

        template<class Object>
        Track<Object>::Track(Track &&arg) : variantType(std::move(arg.variantType)), startValue(std::move(arg.startValue)), modifier(std::move(arg.modifier)), getCurrentValue(std::move(arg.getCurrentValue)), nodes(std::move(arg.nodes)), curPos(GetNodeGuaranteed(arg.curPosID)), curPosID(std::move(arg.curPosID))
        {}

        template<class Object>
        Track<Object>& Track<Object>::operator=(const Track &arg)
        {
            variantType = arg.variantType;
            startValue = arg.startValue;
            modifier = arg.modifier;
            getCurrentValue = arg.getCurrentValue;
            nodes = arg.nodes;
            curPos = GetNodeGuaranteed(arg.curPosID);
            curPosID = arg.curPosID;
            return *this;
        }

        template<class Object>
        Track<Object>& Track<Object>::operator=(Track &&arg)
        {
            variantType = std::move(arg.variantType);
            startValue = std::move(arg.startValue);
            modifier = std::move(arg.modifier);
            getCurrentValue = std::move(arg.getCurrentValue);
            nodes = std::move(arg.nodes);
            curPos = GetNodeGuaranteed(arg.curPosID);
            curPosID = std::move(arg.curPosID);
            return *this;
        }

        template<class Object>
        bool Track<Object>::operator==(const Track &arg) const
        {
            return variantType == arg.variantType && startValue == arg.startValue && modifier == arg.modifier && getCurrentValue == arg.getCurrentValue && nodes == arg.nodes && curPos == arg.curPos && curPosID == arg.curPosID;
        }

        template<class Object>
        bool Track<Object>::operator!=(const Track &arg) const
        {
            return !(*this == arg);
        }

        template<class Object>
        void Track<Object>::Start(Object &object)
        {
            if (nodes.empty())
                return;

            curPos = nodes.begin();
            curPosID = startingID;
            // Retrieve the start value
            if (defaultStartValue.IsValid())
                startValue = *defaultStartValue;
            else
                startValue = getCurrentValue(object);
            timer.SetGoal(curPos->GetTimeTaken());
            timer.Start();
        }

        template<class Object>
        bool Track<Object>::Work(Object &object)
        {
            if (curPos == nodes.end())
                return true;

            if (timer.HasReachedGoal())
            {
                Delta delta(1.0, RadixPoint(9));
                ModifierArgs args(object, startValue, GetCurrentValue(delta), curPos->GetEndValue(), delta);
                modifier(args);

                ++curPos;
                ++curPosID;
                if (curPos == nodes.end())
                    return true;

                startValue = getCurrentValue(object);
                timer.SetGoal(curPos->GetTimeTaken());
                timer.Start();
            }
            else
            {
                auto &endValue = curPos->GetEndValue();
                if (endValue != startValue)
                {
                    auto &delta = curPos->GetDelta(timer);
                    ModifierArgs args(object, startValue, GetCurrentValue(delta), curPos->GetEndValue(), delta);
                    modifier(args);
                }
            }

            return false;
        }
    }
}