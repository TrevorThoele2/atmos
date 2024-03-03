
#include "ModulatorGeneratorCreator.h"
#include "ModulatorGenerator.h"
#include "ModulatorDescribers.h"

#include "StringUtility.h"
#include "Logger.h"

#include "GameEnvironment.h"

namespace Atmos
{
    namespace Modulator
    {
        void CreateAllGenerators()
        {

#define CREATE_GENERATOR(describer)                                                                                             \
    auto gen = static_cast<Generator<decltype(describer)::Type>*>(new Generator<decltype(describer)::Type>(describer.name));    \
    GameEnvironment::AddModulatorGenerator(describer.name, GameEnvironment::ModulatorGeneratorPtr(gen));                        \
    typedef decltype(describer)::Type T

#define START_TRACK_GENERATOR(describer)                    \
gen->AddTrackGenerator(describer.name, describer.variantType,                                \
    Track<T>::ModifierT(static_cast<void(*)(const TrackModifierArgs<T>&)>([](const TrackModifierArgs<T> &args)
#define END_MODIFIER \
)), Track<T>::GetCurrentValueT(static_cast<Value(*)(const T&)>([](const T &object)

#define END_TRACK_GENERATOR \
)))
            // Sprite generator
            {
                CREATE_GENERATOR(Description::Sprite);
                // Position X
                START_TRACK_GENERATOR(Description::Track::PositionX)
                {
                    args.object.SetX(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.SetY(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.SetZ(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Scaling X
                START_TRACK_GENERATOR(Description::Track::ScalingX)
                {
                    args.object.SetXScaler(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetXScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Y
                START_TRACK_GENERATOR(Description::Track::ScalingY)
                {
                    args.object.SetYScaler(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetYScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Z
                START_TRACK_GENERATOR(Description::Track::ScalingZ)
                {
                    args.object.SetZScaler(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetZScaler()); }
                END_TRACK_GENERATOR;

                // Rotation X
                START_TRACK_GENERATOR(Description::Track::RotationX)
                {
                    args.object.SetXRotation(Angle(Angle::RADIANS, args.current.AsFloat()));
                }
                END_MODIFIER { return Value(object.GetXRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Y
                START_TRACK_GENERATOR(Description::Track::RotationY)
                {
                    args.object.SetYRotation(Angle(Angle::RADIANS, args.current.AsFloat()));
                }
                END_MODIFIER { return Value(object.GetYRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Z
                START_TRACK_GENERATOR(Description::Track::RotationZ)
                {
                    args.object.SetZRotation(Angle(Angle::RADIANS, args.current.AsFloat()));
                }
                END_MODIFIER { return Value(object.GetZRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Color A
                START_TRACK_GENERATOR(Description::Track::ColorA)
                {
                    args.object.color.SetAlpha(args.current.AsInt());
                }
                END_MODIFIER{ return Value(std::int64_t(object.color.GetAlpha())); }
                END_TRACK_GENERATOR;

                // Color R
                START_TRACK_GENERATOR(Description::Track::ColorR)
                {
                    args.object.color.SetRed(args.current.AsInt());
                }
                END_MODIFIER{ return Value(std::int64_t(object.color.GetRed())); }
                END_TRACK_GENERATOR;

                // Color G
                START_TRACK_GENERATOR(Description::Track::ColorG)
                {
                    args.object.color.SetGreen(args.current.AsInt());
                }
                END_MODIFIER{ return Value(std::int64_t(object.color.GetGreen())); }
                END_TRACK_GENERATOR;

                // Color B
                START_TRACK_GENERATOR(Description::Track::ColorB)
                {
                    args.object.color.SetBlue(args.current.AsInt());
                }
                END_MODIFIER{ return Value(std::int64_t(object.color.GetBlue())); }
                END_TRACK_GENERATOR;

                // Index
                START_TRACK_GENERATOR(Description::Track::Index)
                {
                    args.object.SetIndex(static_cast<::Atmos::Sprite::Index>(args.current.AsInt()));
                }
                END_MODIFIER { return Value(std::int64_t(object.GetIndex())); }
                END_TRACK_GENERATOR;
            }

            // Sprite offset generator
            {
                typedef Ent::SenseComponent::SpriteHandle T;

                CREATE_GENERATOR(Description::SpriteOffset);
                // Position X
                START_TRACK_GENERATOR(Description::Track::PositionX)
                {
                    args.object.InformOffsetXChange(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.InformOffsetYChange(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.InformOffsetZChange(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Scaling X
                START_TRACK_GENERATOR(Description::Track::ScalingX)
                {
                    args.object.Get().SetXScaler(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.Get().GetXScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Y
                START_TRACK_GENERATOR(Description::Track::ScalingY)
                {
                    args.object.Get().SetYScaler(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.Get().GetYScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Z
                START_TRACK_GENERATOR(Description::Track::ScalingZ)
                {
                    args.object.Get().SetZScaler(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.Get().GetZScaler()); }
                END_TRACK_GENERATOR;

                // Rotation X
                START_TRACK_GENERATOR(Description::Track::RotationX)
                {
                    args.object.Get().SetXRotation(Angle(Angle::RADIANS, args.current.AsFloat()));
                }
                END_MODIFIER { return Value(object.Get().GetXRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Y
                START_TRACK_GENERATOR(Description::Track::RotationY)
                {
                    args.object.Get().SetYRotation(Angle(Angle::RADIANS, args.current.AsFloat()));
                }
                END_MODIFIER { return Value(object.Get().GetYRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Z
                START_TRACK_GENERATOR(Description::Track::RotationZ)
                {
                    args.object.Get().SetZRotation(Angle(Angle::RADIANS, args.current.AsFloat()));
                }
                END_MODIFIER { return Value(object.Get().GetZRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Color A
                START_TRACK_GENERATOR(Description::Track::ColorA)
                {
                    args.object.Get().color.SetAlpha(args.current.AsInt());
                }
                END_MODIFIER{ return Value(std::int64_t(object.Get().color.GetAlpha())); }
                END_TRACK_GENERATOR;

                // Color R
                START_TRACK_GENERATOR(Description::Track::ColorR)
                {
                    args.object.Get().color.SetRed(args.current.AsInt());
                }
                END_MODIFIER{ return Value(std::int64_t(object.Get().color.GetRed())); }
                END_TRACK_GENERATOR;

                // Color G
                START_TRACK_GENERATOR(Description::Track::ColorG)
                {
                    args.object.Get().color.SetGreen(args.current.AsInt());
                }
                END_MODIFIER{ return Value(std::int64_t(object.Get().color.GetGreen())); }
                END_TRACK_GENERATOR;

                // Color B
                START_TRACK_GENERATOR(Description::Track::ColorB)
                {
                    args.object.Get().color.SetBlue(args.current.AsInt());
                }
                END_MODIFIER{ return Value(std::int64_t(object.Get().color.GetBlue())); }
                END_TRACK_GENERATOR;

                // Index
                START_TRACK_GENERATOR(Description::Track::Index)
                {
                    args.object.Get().SetIndex(static_cast<Sprite::Index>(args.current.AsInt()));
                }
                END_MODIFIER { return Value(std::int64_t(object.Get().GetIndex())); }
                END_TRACK_GENERATOR;
            }

            // Sound generator
            {
                CREATE_GENERATOR(Description::Sound);
                // Position X
                START_TRACK_GENERATOR(Description::Track::PositionX)
                {
                    args.object.SetX(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.SetY(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.SetZ(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Volume
                START_TRACK_GENERATOR(Description::Track::Volume)
                {
                    args.object.SetBaseVolume(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetBaseVolume()); }
                END_TRACK_GENERATOR;
            }

            // Sound offset generator
            {
                CREATE_GENERATOR(Description::SoundOffset);
                // Position X
                START_TRACK_GENERATOR(Description::Track::PositionX)
                {
                    args.object.InformOffsetXChange(args.current.AsFloat());
                }
                END_MODIFIER{ return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.InformOffsetYChange(args.current.AsFloat());
                }
                END_MODIFIER{ return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.InformOffsetZChange(args.current.AsFloat());
                }
                END_MODIFIER{ return Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Volume
                START_TRACK_GENERATOR(Description::Track::Volume)
                {
                    args.object.Get().SetBaseVolume(args.current.AsFloat());
                }
                END_MODIFIER{ return Value(object.Get().GetBaseVolume()); }
                END_TRACK_GENERATOR;
            }

            // AVEffect generator
            {
                CREATE_GENERATOR(Description::AVEffect);
                // Position 3D X
                START_TRACK_GENERATOR(Description::Track::PositionX)
                {
                    args.object.SetX(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position 3D Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.SetY(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position 3D Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.SetZ(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetZ()); }
                END_TRACK_GENERATOR;
            }

            // Sense component generator
            {
                CREATE_GENERATOR(Description::SenseComponent);
                // Position 3D X
                START_TRACK_GENERATOR(Description::Track::PositionX)
                {
                    args.object.SetX(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position 3D Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.SetY(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position 3D Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.SetZ(args.current.AsFloat());
                }
                END_MODIFIER { return Value(object.GetZ()); }
                END_TRACK_GENERATOR;
            }
        }

#undef CREATE_GENERATOR
#undef START_TRACK_GENERATOR
#undef END_TRACK_GENERATOR
    }
}