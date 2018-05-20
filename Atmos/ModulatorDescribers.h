#pragma once

#include "Name.h"

#include "Sprite.h"
#include "Sound.h"
#include "AVEffect.h"
#include "SenseComponent.h"

namespace Atmos
{
    namespace Modulator
    {
        namespace Description
        {
            template<class T>
            struct DescriberBase
            {
                typedef T Type;
                typedef ::Atmos::Modulator::Modulator<Type> ModulatorT;
                const Name name;
                DescriberBase(Name &&name) : name(std::move(name)) {}
            };

            extern const DescriberBase<::Atmos::Sprite> Sprite;
            extern const DescriberBase<::Atmos::Ent::SenseComponent::SpriteHandle> SpriteOffset;
            extern const DescriberBase<::Atmos::Sound> Sound;
            extern const DescriberBase<::Atmos::Ent::SenseComponent::SoundHandle> SoundOffset;
            extern const DescriberBase<::Atmos::AVEffect> AVEffect;
            extern const DescriberBase<::Atmos::Ent::SenseComponent::AVEffectHandle> AVEffectOffset;
            extern const DescriberBase<::Atmos::Ent::SenseComponent> SenseComponent;

            namespace Track
            {
                template<class T>
                struct TrackDescriberBase
                {
                    typedef T Type;
                    typedef ::Atmos::Modulator::Track<Type> TrackT;
                    const Name name;
                    const Value::Type variantType;
                    TrackDescriberBase(Name &&name, Value::Type variantType) : name(std::move(name)), variantType(variantType) {}
                };

                extern const TrackDescriberBase<Position3D::ValueT> PositionX;
                extern const TrackDescriberBase<Position3D::ValueT> PositionY;
                extern const TrackDescriberBase<Position3D::ValueT> PositionZ;
                extern const TrackDescriberBase<Size3D::ValueT> ScalingX;
                extern const TrackDescriberBase<Size3D::ValueT> ScalingY;
                extern const TrackDescriberBase<Size3D::ValueT> ScalingZ;
                extern const TrackDescriberBase<Angle::ValueT> RotationX;
                extern const TrackDescriberBase<Angle::ValueT> RotationY;
                extern const TrackDescriberBase<Angle::ValueT> RotationZ;

                extern const TrackDescriberBase<::Atmos::Sound::Volume> Volume;
                extern const TrackDescriberBase<::Atmos::Sprite::Index> Index;
            }
        }
    }
}