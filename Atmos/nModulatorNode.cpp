
#include "nModulatorNode.h"

namespace Atmos
{
    namespace Modulator
    {
        Node::Node() :
            interpolation([this]() { return this->_interpolation.get(); }, [this](Interpolation* newValue) { this->_interpolation.reset(newValue); })
        {}

        Node::Node(const ::Inscription::Table<Node>& table) :
            INSCRIPTION_TABLE_GET_BASE(Object),
            interpolation([this]() { return this->_interpolation.get(); }, [this](Interpolation* newValue) { this->_interpolation.reset(newValue); })
        {}

        ObjectTypeDescription Node::TypeDescription() const
        {
            return ObjectTraits<Node>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Modulator::Node>::typeName = "ModulatorNode";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Modulator::Node)
    {

    }
}