
#include "ModulatorDescribers.h"

namespace Atmos
{
    namespace Modulator
    {
        namespace Description
        {
            const DescriberBase<::Atmos::Sprite> Sprite("Sprite");
            const DescriberBase<::Atmos::Ent::SenseComponent::SpriteHandle> SpriteOffset("SpriteOffset");
            const DescriberBase<::Atmos::Sound> Sound("Sound");
            const DescriberBase<::Atmos::Ent::SenseComponent::SoundHandle> SoundOffset("SoundOffset");
            const DescriberBase<::Atmos::AVEffect> AVEffect("AVEffect");
            const DescriberBase<::Atmos::Ent::SenseComponent::AVEffectHandle> AVEffectOffset("AVEffectOffset");
            const DescriberBase<::Atmos::Ent::SenseComponent> SenseComponent("SenseComponent");

            namespace Track
            {
                const TrackDescriberBase<Position3D::ValueT> PositionX("PositionX", Value::Type::FLOAT);
                const TrackDescriberBase<Position3D::ValueT> PositionY("PositionY", Value::Type::FLOAT);
                const TrackDescriberBase<Position3D::ValueT> PositionZ("PositionZ", Value::Type::FLOAT);
                const TrackDescriberBase<Size3D::ValueT> ScalingX("ScalingX", Value::Type::FLOAT);
                const TrackDescriberBase<Size3D::ValueT> ScalingY("ScalingY", Value::Type::FLOAT);
                const TrackDescriberBase<Size3D::ValueT> ScalingZ("ScalingZ", Value::Type::FLOAT);
                const TrackDescriberBase<Angle::ValueT> RotationX("RotationX", Value::Type::FLOAT);
                const TrackDescriberBase<Angle::ValueT> RotationY("RotationX", Value::Type::FLOAT);
                const TrackDescriberBase<Angle::ValueT> RotationZ("RotationX", Value::Type::FLOAT);

                const TrackDescriberBase<::Atmos::Sound::Volume> Volume("Volume", Value::Type::FLOAT);
                const TrackDescriberBase<::Atmos::Sprite::Index> Index("Index", Value::Type::INT);
            }
        }
    }
}