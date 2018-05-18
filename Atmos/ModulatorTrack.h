#pragma once

#include <unordered_map>

#include "ModulatorTrackNode.h"

#include "Variant.h"
#include "FrameTimer.h"

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
            Value ending;
            Delta delta;

            TrackModifierArgs(ObjectT &objectValue, Value starting, Value ending, Delta delta);
            TrackModifierArgs(const TrackModifierArgs &arg) = default;
            TrackModifierArgs& operator=(const TrackModifierArgs &arg) = default;
            bool operator==(const TrackModifierArgs &arg) const;
            bool operator!=(const TrackModifierArgs &arg) const;
        };

        template<class Object>
        TrackModifierArgs<Object>::TrackModifierArgs(ObjectT &object, Value starting, Value ending, Delta delta) : object(object), starting(starting), ending(ending), delta(delta)
        {}

        template<class Object>
        bool TrackModifierArgs<Object>::operator==(const TrackModifierArgs &arg) const
        {
            return &object == &arg.object && starting == arg.starting && ending == arg.ending && delta == arg.delta;
        }

        template<class Object>
        bool TrackModifierArgs<Object>::operator!=(const TrackModifierArgs &arg) const
        {
            return !(*this == arg);
        }

        class TrackBase
        {
        public:
            typedef size_t ID;
        private:
            virtual TrackBase* CloneImpl() const = 0;

            virtual TrackNode* AddNodeImpl() = 0;
            virtual void RemoveNodeImpl(ID id) = 0;
            virtual TrackNode* FindNodeImpl(ID id) = 0;
            virtual const TrackNode* FindNodeImpl(ID id) const = 0;

            virtual TimeValue GetSumTimeTakenImpl() const = 0;
        protected:
            TrackBase() = default;
        public:
            virtual ~TrackBase() = 0;

            TrackBase* Clone() const;

            TrackNode* AddNode();
            void RemoveNode(ID id);
            TrackNode* FindNode(ID id);
            const TrackNode* FindNode(ID id) const;
            bool HasNode(ID id) const;

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
            const ID startingID = 0;
        private:
            TrackBase* CloneImpl() const override final;

            NodeT* AddNodeImpl() override final;
            void RemoveNodeImpl(ID id) override final;
            NodeT* FindNodeImpl(ID id) override final;
            const NodeT* FindNodeImpl(ID id) const override final;

            TimeValue GetSumTimeTakenImpl() const override final;
        private:
            Value::Type variantType;
            Value startValue;

            ModifierT modifier;
            GetCurrentValueT getCurrentValue;

            NodeContainer nodes;
            typename NodeContainer::iterator curPos;
            ID curPosID;

            FrameTimer timer;

            typename NodeContainer::iterator GetNodeGuaranteed(ID pos);
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
        typename Track<Object>::NodeT* Track<Object>::AddNodeImpl()
        {
            nodes.push_back(NodeT(variantType));
            return &nodes.back();
        }

        template<class Object>
        void Track<Object>::RemoveNodeImpl(ID id)
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
        typename Track<Object>::NodeT* Track<Object>::FindNodeImpl(ID id)
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
        typename const Track<Object>::NodeT* Track<Object>::FindNodeImpl(ID id) const
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
        typename Track<Object>::NodeContainer::iterator Track<Object>::GetNodeGuaranteed(ID pos)
        {
            auto selectedNode = nodes.begin();
            for (ID loop = 0; loop != curPosID; ++loop)
                ++selectedNode;
            return selectedNode;
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
            startValue = getCurrentValue(object);
            timer.SetGoal(curPos->GetTimeTaken());
            timer.Start();
        }

        template<class Object>
        bool Track<Object>::Work(Object &object)
        {
            if (curPos == nodes.end())
                return true;

            ModifierArgs args(object, startValue, curPos->GetEndValue(), curPos->GetDelta());
            modifier(args);

            if (timer.HasReachedGoal())
            {
                ++curPos;
                ++curPosID;
                if (curPos == nodes.end())
                    return true;

                startValue = getCurrentValue(object);
                timer.SetGoal(curPos->GetTimeTaken());
                timer.Start();
            }

            return false;
        }
    }
}