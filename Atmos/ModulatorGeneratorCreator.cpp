
#include "ModulatorGeneratorCreator.h"
#include "ModulatorGenerator.h"
#include "ModulatorDescribers.h"

#include "GameEnvironment.h"

namespace Atmos
{
    namespace Modulator
    {
        // Use this if we want to use floats
        template<class T, class Object>
        T CommonSetFloat(const TrackModifierArgs<Object> &args)
        {
            return static_cast<T>((Delta(args.ending.AsFloat()) * args.delta) + (Delta(args.starting.AsFloat()) * (Delta(1) - args.delta)));
        }

        // Use this if we want to use ints
        template<class T, class Object>
        T CommonSetInt(const TrackModifierArgs<Object> &args)
        {
            return static_cast<T>((Delta(args.ending.AsInt()) * args.delta) + (Delta(args.starting.AsInt()) * (Delta(1) - args.delta)));
        }

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
                    args.object.SetX(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.SetY(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.SetZ(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Scaling X
                START_TRACK_GENERATOR(Description::Track::ScalingX)
                {
                    args.object.SetXScaler(CommonSetFloat<Size3D::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetXScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Y
                START_TRACK_GENERATOR(Description::Track::ScalingY)
                {
                    args.object.SetYScaler(CommonSetFloat<Size3D::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetYScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Z
                START_TRACK_GENERATOR(Description::Track::ScalingZ)
                {
                    args.object.SetZScaler(CommonSetFloat<Size3D::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetZScaler()); }
                END_TRACK_GENERATOR;

                // Rotation X
                START_TRACK_GENERATOR(Description::Track::RotationX)
                {
                    args.object.SetXRotation(Angle(Angle::RADIANS, CommonSetFloat<Angle::ValueT>(args)));
                }
                END_MODIFIER { return Value(object.GetXRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Y
                START_TRACK_GENERATOR(Description::Track::RotationY)
                {
                    args.object.SetYRotation(Angle(Angle::RADIANS, CommonSetFloat<Angle::ValueT>(args)));
                }
                END_MODIFIER { return Value(object.GetYRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Z
                START_TRACK_GENERATOR(Description::Track::RotationZ)
                {
                    args.object.SetZRotation(Angle(Angle::RADIANS, CommonSetFloat<Angle::ValueT>(args)));
                }
                END_MODIFIER { return Value(object.GetZRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Index
                START_TRACK_GENERATOR(Description::Track::Index)
                {
                    args.object.SetIndex(CommonSetInt<T::Index>(args));
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
                    args.object.InformOffsetXChange(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.InformOffsetYChange(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.InformOffsetZChange(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Scaling X
                START_TRACK_GENERATOR(Description::Track::ScalingX)
                {
                    args.object.Get().SetXScaler(CommonSetFloat<Size3D::ValueT>(args));
                }
                END_MODIFIER { return Value(object.Get().GetXScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Y
                START_TRACK_GENERATOR(Description::Track::ScalingY)
                {
                    args.object.Get().SetYScaler(CommonSetFloat<Size3D::ValueT>(args));
                }
                END_MODIFIER { return Value(object.Get().GetYScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Z
                START_TRACK_GENERATOR(Description::Track::ScalingZ)
                {
                    args.object.Get().SetZScaler(CommonSetFloat<Size3D::ValueT>(args));
                }
                END_MODIFIER { return Value(object.Get().GetZScaler()); }
                END_TRACK_GENERATOR;

                // Rotation X
                START_TRACK_GENERATOR(Description::Track::RotationX)
                {
                    args.object.Get().SetXRotation(Angle(Angle::RADIANS, CommonSetFloat<Angle::ValueT>(args)));
                }
                END_MODIFIER { return Value(object.Get().GetXRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Y
                START_TRACK_GENERATOR(Description::Track::RotationY)
                {
                    args.object.Get().SetYRotation(Angle(Angle::RADIANS, CommonSetFloat<Angle::ValueT>(args)));
                }
                END_MODIFIER { return Value(object.Get().GetYRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Z
                START_TRACK_GENERATOR(Description::Track::RotationZ)
                {
                    args.object.Get().SetZRotation(Angle(Angle::RADIANS, CommonSetFloat<Angle::ValueT>(args)));
                }
                END_MODIFIER { return Value(object.Get().GetZRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Index
                START_TRACK_GENERATOR(Description::Track::Index)
                {
                    args.object.Get().SetIndex(CommonSetInt<T::ObjectT::Index>(args));
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
                    args.object.SetX(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.SetY(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.SetZ(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Volume
                START_TRACK_GENERATOR(Description::Track::Volume)
                {
                    args.object.SetBaseVolume(CommonSetFloat<T::Volume>(args));
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
                    args.object.InformOffsetXChange(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER{ return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.InformOffsetYChange(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER{ return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.InformOffsetZChange(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER{ return Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Volume
                START_TRACK_GENERATOR(Description::Track::Volume)
                {
                    args.object.Get().SetBaseVolume(CommonSetFloat<T::ObjectT::Volume>(args));
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
                    args.object.SetX(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position 3D Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.SetY(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position 3D Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.SetZ(CommonSetFloat<T::PositionT::ValueT>(args));
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
                    args.object.SetX(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position 3D Y
                START_TRACK_GENERATOR(Description::Track::PositionY)
                {
                    args.object.SetY(CommonSetFloat<T::PositionT::ValueT>(args));
                }
                END_MODIFIER { return Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position 3D Z
                START_TRACK_GENERATOR(Description::Track::PositionZ)
                {
                    args.object.SetZ(CommonSetFloat<T::PositionT::ValueT>(args));
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