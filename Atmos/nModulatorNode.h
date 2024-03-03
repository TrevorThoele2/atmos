#pragma once

#include <memory>

#include "Object.h"

#include "ModulatorValue.h"
#include "nModulatorInterpolation.h"

#include "Property.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Modulator
    {
        class Node : public Object
        {
        public:
            Value endValue;
            Property<Interpolation*> interpolation;
            TimeValue timeTaken;
        public:
            Node();
            Node(const Node& arg) = default;
            Node(const ::Inscription::Table<Node>& table);

            template<class T, class... Args>
            void ChangeInterpolation(Args&& ... args);

            ObjectTypeDescription TypeDescription() const override;
        private:
            std::unique_ptr<Interpolation> _interpolation;
        };

        template<class T, class... Args>
        void Node::ChangeInterpolation(Args&& ... args)
        {
            auto madeInterpolation = new T(std::forward<Args>(args)...);
            _interpolation.reset(madeInterpolation);
        }
    }

    template<>
    struct ObjectTraits<Modulator::Node> : ObjectTraitsBase<Modulator::Node>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Modulator::Node)
    {
    public:
        static void AddMembers(TableT& table);
    };
}