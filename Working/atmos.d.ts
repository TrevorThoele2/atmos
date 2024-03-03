declare module Atmos {
    export type Variant = boolean | number | BigInt | string;

    export interface NameValuePair {
        name: string;
        value: string;
    }

    export interface Result {
        done: boolean;
        value?: Variant | null | undefined;
    }

    export type RelicId = number;

    export const nullRelicId: RelicId;

    export interface Command {}

    export interface Signal {}

    export interface Relic {
        id: RelicId;
    }

    export interface Shard {
        id: RelicId;
    }

    export interface Destroy {
        id: RelicId;
    }

    export module Asset {
        export interface ImageAssetSize {
            columns: number;
            rows: number;
        }

        export interface MaterialPass {
            vertexShader: Shader;
            fragmentShader: Shader;
        }

        export interface Asset extends Relic {
            name: string;
        }
        
        export interface Action extends Asset {
            boundKey: Input.Key;
            boundModifiers: Input.Key[];
        }
        
        export interface Audio extends Asset {}
        
        export interface Font extends Asset {}

        export interface Image extends Asset {
            size: Spatial.Size2D;
            gridSize: ImageAssetSize;
            sliceSize: Spatial.Size2D;
        }

        export interface Material extends Asset {
            passes: MaterialPass[];
        }

        export interface Script extends Asset {}

        export interface Shader extends Asset {}

        export interface FindByName {
            name: string;
        }
    }

    export module Audio {
        export type Volume = number;

        export interface ChangeSoundVolume extends Command {
            id: RelicId;
            to: Volume;
        }

        export interface PauseSound extends Command {
            id: RelicId;
        }

        export interface RestartSound extends Command {
            id: RelicId;
        }

        export interface ResumeSound extends Command {
            id: RelicId;
        }

        export interface SoundFinished extends Signal {
            id: RelicId;
        }

        export interface CreatePositionedSound extends Command {
            asset: Asset.Asset | null;
            volume: Volume;
            position: Spatial.Point3D;
        }

        export interface PositionedSound extends Relic {
            asset: Asset.Asset | null;
            volume: Volume;
        }

        export interface CreateUniversalSound extends Command {
            asset: Asset.Asset | null;
            volume: Volume;
        }

        export interface UniversalSound extends Relic {
            asset: Asset.Asset | null;
            volume: Volume;
        }
    }

    export module Diagnostics {
        export interface Profile {
            time: number;
            average: number;
            highest: number;
        }

        export interface Statistics extends Relic {
            relicCount: number;
            shardCount: number;
            nextRelicId: number;
            script: Profile;
            render: Profile;
            frame: Profile;
            idle: Profile;
            misc: Profile;
            framesPerSecond: number;
        }
    }

    export module Entity {
        export interface FindByName extends Command {
            name: string;
        }

        export interface FindByPosition extends Command {
            position: Spatial.Grid.Point;
        }

        export interface MoveTo extends Command {
            entity: RelicId;
            to: Spatial.Grid.Point;
        }

        export interface CanMoveTo extends Command {
            entity: RelicId;
            to: Spatial.Grid.Point;
        }

        export interface ModifyTags extends Command {
            entity: RelicId;
            add: string[];
            remove: string[];
        }

        export interface FindPath extends Command {
            entity: RelicId;
            to: Spatial.Grid.Point;
        }

        export interface Entity extends Relic {
            name: string;
            position: Spatial.Grid.Point;
            isSolid: boolean;
            tags: string[];
        }

        export interface CreateEntity extends Command {
            name: string;
            position: Spatial.Grid.Point;
            isSolid: boolean;
            tags: string[];
        }

        export function currentEntity(): Entity.Entity | null;
    }

    export module Spatial {
        export type Angle2D = number;

        export interface Angle3D {
            yaw: number;
            pitch: number;
        }

        export interface Point2D {
            x: number;
            y: number;
        }

        export interface Point3D {
            x: number;
            y: number;
            z: number;
        }

        export interface Size2D {
            width: number;
            height: number;
        }

        export interface Size3D {
            width: number;
            height: number;
            depth: number;
        }

        export interface Scalers2D {
            x: number;
            y: number;
        }

        export interface Scalers3D {
            x: number;
            y: number;
            z: number;
        }

        export interface AxisAlignedBox2D {
            center: Point2D;
            size: Size2D;
        }

        export interface AxisAlignedBox3D {
            center: Point3D;
            size: Size3D;
        }

        export enum Space {
            World = 0,
            Screen = 1
        }

        export interface Bounds extends Shard {
            space: Space;
            position: Point3D;
            baseSize: Size2D;
            scalers: Scalers2D;
            rotation: Angle2D;
        }

        export interface CreateBounds extends Command {
            id: RelicId;
            space: Space;
            position: Point3D;
            baseSize: Size2D;
            scalers: Scalers2D;
            rotation: Angle2D;
        }

        export interface MoveBounds extends Command {
            id: RelicId;
            to: Spatial.Point3D;
        }

        export interface RotateBounds extends Command {
            id: RelicId;
            to: Spatial.Angle2D;
        }

        export interface ScaleBounds extends Command {
            id: RelicId;
            to: Spatial.Scalers2D;
        }

        export interface ToWorldPoint2D extends Command {
            from: Point2D;
        }

        export interface ToWorldPoint3D extends Command {
            from: Point2D;
            z: number;
        }

        export module Grid {
            export interface Point {
                x: number;
                y: number;
            }

            export interface Size {
                x: number;
                y: number;
            }

            export interface AxisAlignedBox {
                center: Point;
                size: Size;
            }

            export function distance(starting: Point, destination: Point): number;
            export function contains(box: AxisAlignedBox, point: Point): boolean;
            export function contains(box: AxisAlignedBox, other: AxisAlignedBox): boolean;
            export function intersects(one: AxisAlignedBox, two: AxisAlignedBox): boolean;
            export function toPoint(point: Point2D): Point;
            export function toPoint(point: Point3D): Point;
            export function toPoint2D(point: Point): Point2D;
            export function toPoint3D(point: Point, z: number): Point3D;
        }

        export function left(box: AxisAlignedBox2D): number;
        export function left(box: AxisAlignedBox3D): number;
        export function top(box: AxisAlignedBox2D): number;
        export function top(box: AxisAlignedBox3D): number;
        export function farZ(box: AxisAlignedBox3D): number;
        export function right(box: AxisAlignedBox2D): number;
        export function right(box: AxisAlignedBox3D): number;
        export function bottom(box: AxisAlignedBox2D): number;
        export function bottom(box: AxisAlignedBox3D): number;
        export function nearZ(box: AxisAlignedBox3D): number;

        export function distance(from: Point2D, to: Point2D): number;
        export function distance(from: Point3D, to: Point3D): number;
        export function isBetween(one: Point2D, check: Point2D, two: Point2D): boolean;
        export function angleOf(side1: Point2D, middle: Point2D, side2: Point2D): boolean;
        export function contains(box: AxisAlignedBox2D, point: Point2D): boolean;
        export function contains(box: AxisAlignedBox2D, otherBox: AxisAlignedBox2D): boolean;
        export function contains(box: AxisAlignedBox3D, point: Point3D): boolean;
        export function contains(box: AxisAlignedBox3D, otherBox: AxisAlignedBox3D): boolean;
        export function intersects(box: AxisAlignedBox2D, otherBox: AxisAlignedBox2D): boolean;
        export function intersects(box: AxisAlignedBox3D, otherBox: AxisAlignedBox3D): boolean;
        export function envelope(boxes: AxisAlignedBox2D[]): AxisAlignedBox2D;
        export function envelope(boxes: AxisAlignedBox3D[]): AxisAlignedBox3D;
        export function clamp(box: AxisAlignedBox2D, to: AxisAlignedBox2D): AxisAlignedBox2D;
        export function clamp(box: AxisAlignedBox3D, to: AxisAlignedBox3D): AxisAlignedBox3D;
        export function scaleBy(size: Size2D, scalers: Scalers2D): Size2D;
        export function scaleBy(size: Size3D, scalers: Scalers3D): Size3D;
        export function scaleOf(box: AxisAlignedBox2D, against: AxisAlignedBox2D): AxisAlignedBox2D;
        export function scaleOf(box: AxisAlignedBox3D, against: AxisAlignedBox3D): AxisAlignedBox3D;
        export function cell(column: number, row: number, cellSize: Size2D): AxisAlignedBox2D;
        export function cell(column: number, row: number, depth: number, cellSize: Size3D): AxisAlignedBox3D;
        export function add(left: Point2D, right: Point2D): Point2D;
        export function add(left: Point3D, right: Point3D): Point3D;
        export function add(left: AxisAlignedBox2D, right: Point2D): AxisAlignedBox2D;
        export function add(left: AxisAlignedBox3D, right: Point3D): AxisAlignedBox3D;
        export function subtract(left: Point2D, right: Point2D): Point2D;
        export function subtract(left: Point3D, right: Point3D): Point3D;
        export function subtract(left: AxisAlignedBox2D, right: Point2D): AxisAlignedBox2D;
        export function subtract(left: AxisAlignedBox3D, right: Point3D): AxisAlignedBox3D;
        export function toPoint2D(point: Point3D): Point2D;
        export function toPoint2D(angle: Angle2D, distance: number): Point2D;
        export function toPoint3D(point: Point2D, z: number): Point3D;
        export function toPoint3D(angle: Angle3D, distance: number): Point3D;
        export function toDegrees(angle: Angle2D): Angle2D;
        export function toAxisAlignedBox2D(left: number, top: number, right: number, bottom: number): AxisAlignedBox2D;
        export function toAxisAlignedBox3D(left: number, top: number, farZ: number, right: number, bottom: number, nearZ: number): AxisAlignedBox3D;
    }

    export module Logging {
        export enum Severity {
            Verbose = 0,
            Information = 1,
            Warning = 2,
            Error = 3
        }
        
        export interface Log extends Command {
            message: string;
            severity?: Severity;
            details?: NameValuePair[] | null;
        }
    }

    export module Time {
        export interface Point {
            nanoseconds: BigInt;
        }

        export function currentTime(): Point;
        export function currentFrameTime(): Point;
    }

    export module Render {
        export interface Color {
            alpha: number;
            red: number;
            green: number;
            blue: number;
        }

        export interface ChangeColor extends Command {
            id: RelicId;
            to: Color;
        }

        export interface ChangeImageCore extends Command {
            id: RelicId;
            asset: Asset.Image | null;
            assetIndex: number | null;
        }

        export interface MoveGridRegion extends Command {
            id: RelicId;
            points: Spatial.Grid.Point[] | null;
            z: number | null;
        }

        export interface ChangeMaterialAsset extends Command {
            id: RelicId;
            to: Asset.Material | null;
        }

        export interface ChangeTextCore extends Command {
            id: RelicId;
            string: string | null;
            font: Asset.Font | null;
            wrapWidth: number | null;
            bold: boolean | null;
            italics: boolean | null;
        }

        export interface ChangeViewSlice extends Command {
            id: RelicId;
            to: Spatial.AxisAlignedBox2D;
        }

        export interface FindImagesByBox extends Command {
            box: Spatial.AxisAlignedBox3D;
            space: Spatial.Space;
        }

        export interface FindLinesByBox extends Command {
            box: Spatial.AxisAlignedBox3D;
        }

        export interface FindGridRegionsByBox extends Command {
            box: Spatial.AxisAlignedBox3D;
        }

        export interface MoveLine extends Command {
            id: RelicId;
            points: Spatial.Point2D[] | null;
            z: number | null;
        }

        export interface TextBaseSize extends Command {
            string: string;
            font: RelicId;
            wrapWidth: number;
            bold: boolean;
            italics: boolean;
        }

        export interface Camera extends Relic {
            position: Spatial.Point3D;
            size: Spatial.Size2D;
            sides: Spatial.AxisAlignedBox2D;
        }

        export interface DynamicImage extends Relic {
            asset: Asset.Image | null;
            assetIndex: number;
            material: Asset.Material | null;
            color: Color;
            position: Spatial.Point3D;
            scalers: Spatial.Scalers2D;
            rotation: Spatial.Angle2D;
        }

        export interface CreateDynamicImage extends Command {
            asset: Asset.Image | null;
            assetIndex: number;
            material: Asset.Material | null;
            color: Color;
            position: Spatial.Point3D;
            scalers: Spatial.Scalers2D;
            rotation: Spatial.Angle2D;
        }

        export interface DynamicText extends Relic {
            string: string;
            font: Asset.Font | null;
            material: Asset.Material | null;
            wrapWidth: number;
            bold: boolean;
            italics: boolean;
            color: Color;
            position: Spatial.Point3D;
            scalers: Spatial.Scalers2D;
            rotation: Spatial.Angle2D;
        }

        export interface CreateDynamicText extends Command {
            string: string;
            font: Asset.Font | null;
            material: Asset.Material | null;
            wrapWidth: number;
            bold: boolean;
            italics: boolean;
            color: Color;
            position: Spatial.Point3D;
            scalers: Spatial.Scalers2D;
            rotation: Spatial.Angle2D;
        }

        export interface GridRegion extends Relic {
            points: Spatial.Grid.Point[];
            z: number;
            material: Asset.Material | null;
        }

        export interface CreateGridRegion extends Command {
            points: Spatial.Grid.Point[];
            z: number;
            material: Asset.Material | null;
        }

        export interface ImageCore extends Shard {
            asset: Asset.Image | null;
            assetIndex: number;
        }

        export interface CreateImageCore extends Command {
            id: RelicId;
            asset: Asset.Image | null;
            assetIndex: number;
        }

        export interface Line extends Relic {
            points: Spatial.Point2D[];
            z: number;
            material: Asset.Material | null;
            width: number;
            color: Color;
        }

        export interface CreateLine extends Command {
            points: Spatial.Point2D[];
            z: number;
            material: Asset.Material | null;
            width: number;
            color: Color;
        }

        export interface RenderCore extends Shard {
            material: Asset.Material | null;
            color: Color;
        }

        export interface CreateRenderCore extends Command {
            id: RelicId;
            material: Asset.Material | null;
            color: Color;
        }

        export interface ViewSlice extends Shard {
            box: Spatial.AxisAlignedBox2D;
        }

        export interface CreateViewSlice extends Command {
            id: RelicId;
            box: Spatial.AxisAlignedBox2D;
        }
    }

    export module UI {
        export interface Image extends Relic {
            asset: Asset.Image | null;
            assetIndex: number;
            material: Asset.Material | null;
            color: Render.Color;
            position: Spatial.Point3D;
            scalers: Spatial.Scalers2D;
            rotation: Spatial.Angle2D;
        }

        export interface CreateImage extends Command {
            asset: Asset.Image | null;
            assetIndex: number;
            material: Asset.Material | null;
            color: Render.Color;
            position: Spatial.Point3D;
            scalers: Spatial.Scalers2D;
            rotation: Spatial.Angle2D;
        }

        export interface Text extends Relic {
            string: string;
            font: Asset.Font | null;
            material: Asset.Material | null;
            wrapWidth: number;
            bold: boolean;
            italics: boolean;
            color: Render.Color;
            position: Spatial.Point3D;
            scalers: Spatial.Scalers2D;
            rotation: Spatial.Angle2D;
        }

        export interface CreateText extends Command {
            string: string;
            font: Asset.Font | null;
            material: Asset.Material | null;
            wrapWidth: number;
            bold: boolean;
            italics: boolean;
            color: Render.Color;
            position: Spatial.Point3D;
            scalers: Spatial.Scalers2D;
            rotation: Spatial.Angle2D;
        }
    }

    export module World {
        export type FieldId = number;

        export interface RequestField extends Command {
            id: FieldId;
        }
    }

    export module Math {
        export module Float {
            export const maximum: number;
        }
    }

    export module Reliquary {
        export function send<TypeName extends CommandTypeTraits["typeName"]>(
            typeName: TypeName, command: TypeTraitMap[TypeName]["T"]): TypeTraitMap[TypeName]["ReturnT"];
        export function find<TypeName extends LocalRelicTypeTraits["typeName"] | ShardTypeTraits["typeName"]>(
            typeName: TypeName, id: RelicId): TypeTraitMap[TypeName]["T"] | null;
        export function find<TypeName extends GlobalRelicTypeTraits["typeName"]>(
            typeName: TypeName): TypeTraitMap[TypeName]["T"] | null;

        export function listen<TypeName extends SignalTypeTraits["typeName"]>(
            typeName: TypeName, receiver: (signal: TypeTraitMap[TypeName]["T"]) => void): void;
    }

    export module TypeTraits {
        export module Asset {
            export type Action = {
                typeName: "Atmos::Asset::Action";
                T: Atmos.Asset.Action;
            }

            export type Audio = {
                typeName: "Atmos::Asset::Audio";
                T: Atmos.Asset.Audio;
            }

            export type Font = {
                typeName: "Atmos::Asset::Font";
                T: Atmos.Asset.Font;
            }

            export type Image = {
                typeName: "Atmos::Asset::Image";
                T: Atmos.Asset.Image;
            }

            export type Material = {
                typeName: "Atmos::Asset::Material";
                T: Atmos.Asset.Material;
            }

            export type Script = {
                typeName: "Atmos::Asset::Script";
                T: Atmos.Asset.Script;
            }

            export type Shader = {
                typeName: "Atmos::Asset::Shader";
                T: Atmos.Asset.Shader;
            }

            export module FindByName {
                export type Action = {
                    typeName: "Atmos::Asset::FindByName<Atmos::Asset::Action>";
                    T: Atmos.Asset.FindByName;
                    ReturnT: Atmos.Asset.Action | null;
                }

                export type Audio = {
                    typeName: "Atmos::Asset::FindByName<Atmos::Asset::Audio>";
                    T: Atmos.Asset.FindByName;
                    ReturnT: Atmos.Asset.Audio | null;
                }
                
                export type Font = {
                    typeName: "Atmos::Asset::FindByName<Atmos::Asset::Font>";
                    T: Atmos.Asset.FindByName;
                    ReturnT: Atmos.Asset.Font | null;
                }
                
                export type Image = {
                    typeName: "Atmos::Asset::FindByName<Atmos::Asset::Image>";
                    T: Atmos.Asset.FindByName;
                    ReturnT: Atmos.Asset.Image | null;
                }
                
                export type Material = {
                    typeName: "Atmos::Asset::FindByName<Atmos::Asset::Material>";
                    T: Atmos.Asset.FindByName;
                    ReturnT: Atmos.Asset.Material | null;
                }
                
                export type Script = {
                    typeName: "Atmos::Asset::FindByName<Atmos::Asset::Script>";
                    T: Atmos.Asset.FindByName;
                    ReturnT: Atmos.Asset.Script | null;
                }
                
                export type Shader = {
                    typeName: "Atmos::Asset::FindByName<Atmos::Asset::Shader>";
                    T: Atmos.Asset.FindByName;
                    ReturnT: Atmos.Asset.Script | null;
                }
            }
        }

        export module Audio {
            export type ChangeSoundVolume = {
                typeName: "Atmos::Audio::ChangeSoundVolume";
                T: Atmos.Audio.ChangeSoundVolume;
                ReturnT: void;
            }

            export type PauseSound = {
                typeName: "Atmos::Audio::PauseSound";
                T: Atmos.Audio.PauseSound;
                ReturnT: void;
            }

            export type RestartSound = {
                typeName: "Atmos::Audio::RestartSound";
                T: Atmos.Audio.RestartSound;
                ReturnT: void;
            }

            export type ResumeSound = {
                typeName: "Atmos::Audio::ResumeSound";
                T: Atmos.Audio.ResumeSound;
                ReturnT: void;
            }

            export type SoundFinished = {
                typeName: "Atmos::Audio::SoundFinished";
                T: Atmos.Audio.SoundFinished;
            }

            export type PositionedSound = {
                typeName: "Atmos::Audio::PositionedSound";
                T: Atmos.Audio.PositionedSound;
            }

            export type CreatePositionedSound = {
                typeName: "Arca::Create<Atmos::Audio::PositionedSound>";
                T: Atmos.Audio.CreatePositionedSound;
                ReturnT: Atmos.Audio.PositionedSound;
            }

            export type DestroyPositionedSound = {
                typeName: "Arca::Destroy<Atmos::Audio::PositionedSound>";
                T: Atmos.Destroy;
                ReturnT: void;
            }

            export type UniversalSound = {
                typeName: "Atmos::Audio::UniversalSound";
                T: Atmos.Audio.UniversalSound;
            }

            export type CreateUniversalSound = {
                typeName: "Arca::Create<Atmos::Audio::UniversalSound>";
                T: Atmos.Audio.CreateUniversalSound;
                ReturnT: Atmos.Audio.UniversalSound;
            }

            export type DestroyUniversalSound = {
                typeName: "Arca::Destroy<Atmos::Audio::UniversalSound>";
                T: Atmos.Destroy;
                ReturnT: void;
            }
        }

        export module Diagnostics {
            export type Statistics = {
                typeName: "Atmos::Diagnostics::Statistics";
                T: Atmos.Diagnostics.Statistics;
            }
        }

        export module Entity {
            export type FindByName = {
                typeName: "Atmos::Entity::FindByName";
                T: Atmos.Entity.FindByName;
                ReturnT: Atmos.Entity.Entity | null;
            }

            export type FindByPosition = {
                typeName: "Atmos::Entity::FindByPosition";
                T: Atmos.Entity.FindByPosition;
                ReturnT: Atmos.Entity.Entity | null;
            }

            export type MoveTo = {
                typeName: "Atmos::Entity::MoveTo";
                T: Atmos.Entity.MoveTo;
                ReturnT: void;
            }

            export type CanMoveTo = {
                typeName: "Atmos::Entity::CanMoveTo";
                T: Atmos.Entity.CanMoveTo;
                ReturnT: boolean;
            }

            export type ModifyTags = {
                typeName: "Atmos::Entity::ModifyTags";
                T: Atmos.Entity.ModifyTags;
                ReturnT: void;
            }

            export type FindPath = {
                typeName: "Atmos::Entity::FindPath";
                T: Atmos.Entity.FindPath;
                ReturnT: Atmos.Spatial.Grid.Point[];
            }

            export type Entity = {
                typeName: "Atmos::Entity::Entity";
                T: Atmos.Entity.Entity;
            }

            export type CreateEntity = {
                typeName: "Arca::Create<Atmos::Entity::Entity>";
                T: Atmos.Entity.CreateEntity;
                ReturnT: Atmos.Entity.Entity;
            }

            export type DestroyEntity = {
                typeName: "Arca::Destroy<Atmos::Entity::Entity>";
                T: Atmos.Destroy;
                ReturnT: void;
            }
        }

        export module Input {
            export type Information = {
                typeName: "Atmos::Input::Information";
                T: Atmos.Input.Information;
            }

            export type MouseMoved = {
                typeName: "Atmos::Input::MouseMoved";
                T: Atmos.Input.MouseMoved;
            }

            export type ActionPressed = {
                typeName: "Atmos::Input::ActionPressed";
                T: Atmos.Input.ActionPressed;
            }


            export type ActionDepressed = {
                typeName: "Atmos::Input::ActionDepressed";
                T: Atmos.Input.ActionDepressed;
            }
        }
        
        export module Logging {
            export type Log = {
                typeName: "Atmos::Logging::Log";
                T: Atmos.Logging.Log;
                ReturnT: void;
            }
        }
        
        export module Render {
            export type ChangeColor = {
                typeName: "Atmos::Render::ChangeColor";
                T: Atmos.Render.ChangeColor;
                ReturnT: void;
            }

            export type ChangeImageCore = {
                typeName: "Atmos::Render::ChangeImageCore";
                T: Atmos.Render.ChangeImageCore;
                ReturnT: void;
            }

            export type MoveGridRegion = {
                typeName: "Atmos::Render::MoveGridRegion";
                T: Atmos.Render.MoveGridRegion;
                ReturnT: void;
            }

            export type ChangeMaterialAsset = {
                typeName: "Atmos::Render::ChangeMaterialAsset";
                T: Atmos.Render.ChangeMaterialAsset;
                ReturnT: void;
            }

            export type ChangeTextCore = {
                typeName: "Atmos::Render::ChangeTextCore";
                T: Atmos.Render.ChangeTextCore;
                ReturnT: void;
            }

            export type ChangeViewSlice = {
                typeName: "Atmos::Render::ChangeViewSlice";
                T: Atmos.Render.ChangeViewSlice;
                ReturnT: void;
            }

            export type FindImagesByBox = {
                typeName: "Atmos::Render::FindImagesByBox";
                T: Atmos.Render.FindImagesByBox;
                ReturnT: Atmos.Render.DynamicImage[];
            }

            export type FindLinesByBox = {
                typeName: "Atmos::Render::FindLinesByBox";
                T: Atmos.Render.FindLinesByBox;
                ReturnT: Atmos.Render.Line[];
            }

            export type FindGridRegionsByBox = {
                typeName: "Atmos::Render::FindGridRegionsByBox";
                T: Atmos.Render.FindGridRegionsByBox;
                ReturnT: Atmos.Render.GridRegion[];
            }

            export type MoveLine = {
                typeName: "Atmos::Render::MoveLine";
                T: Atmos.Render.MoveLine;
                ReturnT: void;
            }

            export type TextBaseSize = {
                typeName: "Atmos::Render::TextBaseSize";
                T: Atmos.Render.TextBaseSize;
                ReturnT: Atmos.Spatial.Size2D;
            }

            export type Camera = {
                typeName: "Atmos::Render::Camera";
                T: Atmos.Render.Camera;
            }

            export type DynamicImage = {
                typeName: "Atmos::Render::DynamicImage";
                T: Atmos.Render.DynamicImage;
            }

            export type CreateDynamicImage = {
                typeName: "Arca::Create<Atmos::Render::DynamicImage>";
                T: Atmos.Render.CreateDynamicImage;
                ReturnT: Atmos.Render.DynamicImage;
            }

            export type DestroyDynamicImage = {
                typeName: "Arca::Destroy<Atmos::Render::DynamicImage>";
                T: Atmos.Destroy;
                ReturnT: void;
            }

            export type DynamicText = {
                typeName: "Atmos::Render::DynamicText";
                T: Atmos.Render.DynamicText;
            }

            export type CreateDynamicText = {
                typeName: "Arca::Create<Atmos::Render::DynamicText>";
                T: Atmos.Render.CreateDynamicText;
                ReturnT: Atmos.Render.DynamicText;
            }

            export type DestroyDynamicText = {
                typeName: "Arca::Destroy<Atmos::Render::DynamicText>";
                T: Atmos.Destroy;
                ReturnT: void;
            }

            export type GridRegion = {
                typeName: "Atmos::Render::GridRegion";
                T: Atmos.Render.GridRegion;
            }

            export type CreateGridRegion = {
                typeName: "Arca::Create<Atmos::Render::GridRegion>";
                T: Atmos.Render.CreateGridRegion;
                ReturnT: Atmos.Render.GridRegion;
            }

            export type DestroyGridRegion = {
                typeName: "Arca::Destroy<Atmos::Render::GridRegion>";
                T: Atmos.Destroy;
                ReturnT: void;
            }

            export type ImageCore = {
                typeName: "Atmos::Render::ImageCore";
                T: Atmos.Render.ImageCore;
            }

            export type CreateImageCore = {
                typeName: "Arca::Create<Atmos::Render::ImageCore>";
                T: Atmos.Render.CreateImageCore;
                ReturnT: Atmos.Render.ImageCore;
            }

            export type DestroyImageCore = {
                typeName: "Arca::Destroy<Atmos::Render::ImageCore>";
                T: Atmos.Destroy;
                ReturnT: void;
            }

            export type Line = {
                typeName: "Atmos::Render::Line";
                T: Atmos.Render.Line;
            }

            export type CreateLine = {
                typeName: "Arca::Create<Atmos::Render::Line>";
                T: Atmos.Render.CreateLine;
                ReturnT: Atmos.Render.Line;
            }

            export type DestroyLine = {
                typeName: "Arca::Destroy<Atmos::Render::Line>";
                T: Atmos.Destroy;
                ReturnT: void;
            }

            export type RenderCore = {
                typeName: "Atmos::Render::RenderCore";
                T: Atmos.Render.RenderCore;
            }

            export type CreateRenderCore = {
                typeName: "Arca::Create<Atmos::Render::RenderCore>";
                T: Atmos.Render.CreateRenderCore;
                ReturnT: Atmos.Render.RenderCore;
            }

            export type DestroyRenderCore = {
                typeName: "Arca::Destroy<Atmos::Render::RenderCore>";
                T: Atmos.Destroy;
                ReturnT: void;
            }

            export type ViewSlice = {
                typeName: "Atmos::Render::ViewSlice";
                T: Atmos.Render.ViewSlice;
            }

            export type CreateViewSlice = {
                typeName: "Arca::Create<Atmos::Render::ViewSlice>";
                T: Atmos.Render.CreateViewSlice;
                ReturnT: Atmos.Render.ViewSlice;
            }

            export type DestroyViewSlice = {
                typeName: "Arca::Destroy<Atmos::Render::ViewSlice>";
                T: Atmos.Destroy;
                ReturnT: void;
            }
        }

        export module Spatial {
            export type MoveBounds = {
                typeName: "Atmos::Spatial::MoveBounds";
                T: Atmos.Spatial.MoveBounds;
                ReturnT: void;
            }

            export type RotateBounds = {
                typeName: "Atmos::Spatial::RotateBounds";
                T: Atmos.Spatial.RotateBounds;
                ReturnT: void;
            }

            export type ScaleBounds = {
                typeName: "Atmos::Spatial::ScaleBounds";
                T: Atmos.Spatial.ScaleBounds;
                ReturnT: void;
            }

            export type ToWorldPoint2D = {
                typeName: "Atmos::Spatial::ToWorldPoint2D";
                T: Atmos.Spatial.ToWorldPoint2D;
                ReturnT: Atmos.Spatial.Point2D;
            }

            export type ToWorldPoint3D = {
                typeName: "Atmos::Spatial::ToWorldPoint3D";
                T: Atmos.Spatial.ToWorldPoint3D;
                ReturnT: Atmos.Spatial.Point3D;
            }

            export type Bounds = {
                typeName: "Atmos::Spatial::Bounds";
                T: Atmos.Spatial.Bounds;
            }

            export type CreateBounds = {
                typeName: "Arca::Create<Atmos::Spatial::Bounds>";
                T: Atmos.Spatial.CreateBounds;
                ReturnT: Atmos.Spatial.Bounds;
            }

            export type DestroyBounds = {
                typeName: "Arca::Destroy<Atmos::Spatial::Bounds>";
                T: Atmos.Destroy;
                ReturnT: void;
            }
        }

        export module UI {
            export type Image = {
                typeName: "Atmos::UI::Image";
                T: Atmos.UI.Image;
            }

            export type CreateImage = {
                typeName: "Arca::Create<Atmos::UI::Image>";
                T: Atmos.UI.CreateImage;
                ReturnT: Atmos.UI.Image;
            }

            export type DestroyImage = {
                typeName: "Arca::Destroy<Atmos::UI::Image>";
                T: Atmos.Destroy;
                ReturnT: void;
            }

            export type Text = {
                typeName: "Atmos::UI::Text";
                T: Atmos.UI.Text;
            }

            export type CreateText = {
                typeName: "Arca::Create<Atmos::UI::Text>";
                T: Atmos.UI.CreateText;
                ReturnT: Atmos.UI.Text;
            }

            export type DestroyText = {
                typeName: "Arca::Destroy<Atmos::UI::Text>";
                T: Atmos.Destroy;
                ReturnT: void;
            }
        }

        export module World {
            export type RequestField = {
                typeName: "Atmos::World::RequestField";
                T: Atmos.World.RequestField;
                ReturnT: void;
            }
        }
    }

    export module Traits {
        export module Asset {
            export module Action {
                export const typeName: "Atmos::Asset::Action";
            }

            export module Audio {
                export const typeName: "Atmos::Asset::Audio";
            }

            export module Font {
                export const typeName: "Atmos::Asset::Font";
            }

            export module Image {
                export const typeName: "Atmos::Asset::Image";
            }

            export module Material {
                export const typeName: "Atmos::Asset::Material";
            }

            export module Script {
                export const typeName: "Atmos::Asset::Script";
            }

            export module Shader {
                export const typeName: "Atmos::Asset::Shader";
            }

            export module FindByName {
                export module Action {
                    export const typeName: "Atmos::Asset::FindByName<Atmos::Asset::Action>";
                }

                export module Audio {
                    export const typeName: "Atmos::Asset::FindByName<Atmos::Asset::Audio>";
                }
                
                export module Font {
                    export const typeName: "Atmos::Asset::FindByName<Atmos::Asset::Font>";
                }
                
                export module Image {
                    export const typeName: "Atmos::Asset::FindByName<Atmos::Asset::Image>";
                }
                
                export module Material {
                    export const typeName: "Atmos::Asset::FindByName<Atmos::Asset::Material>";
                }
                
                export module Script {
                    export const typeName: "Atmos::Asset::FindByName<Atmos::Asset::Script>";
                }
                
                export module Shader {
                    export const typeName: "Atmos::Asset::FindByName<Atmos::Asset::Shader>";
                }
            }
        }

        export module Audio {
            export module ChangeSoundVolume {
                export const typeName: "Atmos::Audio::ChangeSoundVolume";
            }

            export module PauseSound {
                export const typeName: "Atmos::Audio::PauseSound";
            }

            export module RestartSound {
                export const typeName: "Atmos::Audio::RestartSound";
            }

            export module ResumeSound {
                export const typeName: "Atmos::Audio::ResumeSound";
            }

            export module SoundFinished {
                export const typeName: "Atmos::Audio::SoundFinished";
            }

            export module PositionedSound {
                export const typeName: "Atmos::Audio::PositionedSound";
            }

            export module CreatePositionedSound {
                export const typeName: "Arca::Create<Atmos::Audio::PositionedSound>";
            }

            export module DestroyPositionedSound {
                export const typeName: "Arca::Destroy<Atmos::Audio::PositionedSound>";
            }

            export module UniversalSound {
                export const typeName: "Atmos::Audio::UniversalSound";
            }

            export module CreateUniversalSound {
                export const typeName: "Arca::Create<Atmos::Audio::UniversalSound>";
            }

            export module DestroyUniversalSound {
                export const typeName: "Arca::Destroy<Atmos::Audio::UniversalSound>";
            }
        }

        export module Diagnostics {
            export module Statistics {
                export const typeName: "Atmos::Diagnostics::Statistics";
            }
        }

        export module Entity {
            export module FindByName {
                export const typeName: "Atmos::Entity::FindByName";
            }

            export module FindByPosition {
                export const typeName: "Atmos::Entity::FindByPosition";
            }

            export module MoveTo {
                export const typeName: "Atmos::Entity::MoveTo";
            }

            export module CanMoveTo {
                export const typeName: "Atmos::Entity::CanMoveTo";
            }

            export module ModifyTags {
                export const typeName: "Atmos::Entity::ModifyTags";
            }

            export module FindPath {
                export const typeName: "Atmos::Entity::FindPath";
            }

            export module Entity {
                export const typeName: "Atmos::Entity::Entity";
            }

            export module CreateEntity {
                export const typeName: "Arca::Create<Atmos::Entity::Entity>";
            }

            export module DestroyEntity {
                export const typeName: "Arca::Destroy<Atmos::Entity::Entity>";
            }
        }

        export module Input {
            export module Information {
                export const typeName: "Atmos::Input::Information";
            }

            export module MouseMoved {
                export const typeName: "Atmos::Input::MouseMoved";
            }

            export module ActionPressed {
                export const typeName: "Atmos::Input::ActionPressed";
            }

            export module ActionDepressed {
                export const typeName: "Atmos::Input::ActionDepressed";
            }
        }
        
        export module Logging {
            export module Log {
                export const typeName: "Atmos::Logging::Log";
            }
        }
        
        export module Render {
            export module ChangeColor {
                export const typeName: "Atmos::Render::ChangeColor";
            }

            export module ChangeImageCore {
                export const typeName: "Atmos::Render::ChangeImageCore";
            }

            export module MoveGridRegion {
                export const typeName: "Atmos::Render::MoveGridRegion";
            }

            export module ChangeMaterialAsset {
                export const typeName: "Atmos::Render::ChangeMaterialAsset";
            }

            export module ChangeTextCore {
                export const typeName: "Atmos::Render::ChangeTextCore";
            }

            export module ChangeViewSlice {
                export const typeName: "Atmos::Render::ChangeViewSlice";
            }

            export module FindImagesByBox {
                export const typeName: "Atmos::Render::FindImagesByBox";
            }

            export module FindLinesByBox {
                export const typeName: "Atmos::Render::FindLinesByBox";
            }

            export module FindGridRegionsByBox {
                export const typeName: "Atmos::Render::FindGridRegionsByBox";
            }

            export module MoveLine {
                export const typeName: "Atmos::Render::MoveLine";
            }

            export module TextBaseSize {
                export const typeName: "Atmos::Render::TextBaseSize";
            }

            export module Camera {
                export const typeName: "Atmos::Render::Camera";
            }

            export module DynamicImage {
                export const typeName: "Atmos::Render::DynamicImage";
            }

            export module CreateDynamicImage {
                export const typeName: "Arca::Create<Atmos::Render::DynamicImage>";
            }

            export module DestroyDynamicImage {
                export const typeName: "Arca::Destroy<Atmos::Render::DynamicImage>";
            }

            export module DynamicText {
                export const typeName: "Atmos::Render::DynamicText";
            }

            export module CreateDynamicText {
                export const typeName: "Arca::Create<Atmos::Render::DynamicText>";
            }

            export module DestroyDynamicText {
                export const typeName: "Arca::Destroy<Atmos::Render::DynamicText>";
            }

            export module GridRegion {
                export const typeName: "Atmos::Render::GridRegion";
            }

            export module CreateGridRegion {
                export const typeName: "Arca::Create<Atmos::Render::GridRegion>";
            }

            export module DestroyGridRegion {
                export const typeName: "Arca::Destroy<Atmos::Render::GridRegion>";
            }

            export module ImageCore {
                export const typeName: "Atmos::Render::ImageCore";
            }

            export module CreateImageCore {
                export const typeName: "Arca::Create<Atmos::Render::ImageCore>";
            }

            export module DestroyImageCore {
                export const typeName: "Arca::Destroy<Atmos::Render::ImageCore>";
            }

            export module Line {
                export const typeName: "Atmos::Render::Line";
            }

            export module CreateLine {
                export const typeName: "Arca::Create<Atmos::Render::Line>";
            }

            export module DestroyLine {
                export const typeName: "Arca::Destroy<Atmos::Render::Line>";
            }

            export module RenderCore {
                export const typeName: "Atmos::Render::RenderCore";
            }

            export module CreateRenderCore {
                export const typeName: "Arca::Create<Atmos::Render::RenderCore>";
            }

            export module DestroyRenderCore {
                export const typeName: "Arca::Destroy<Atmos::Render::RenderCore>";
            }

            export module ViewSlice {
                export const typeName: "Atmos::Render::ViewSlice";
            }

            export module CreateViewSlice {
                export const typeName: "Arca::Create<Atmos::Render::ViewSlice>";
            }

            export module DestroyViewSlice {
                export const typeName: "Arca::Destroy<Atmos::Render::ViewSlice>";
            }
        }

        export module Spatial {
            export module MoveBounds {
                export const typeName: "Atmos::Spatial::MoveBounds";
            }

            export module RotateBounds {
                export const typeName: "Atmos::Spatial::RotateBounds";
            }
            
            export module ScaleBounds {
                export const typeName: "Atmos::Spatial::ScaleBounds";
            }

            export module ToWorldPoint2D {
                export const typeName: "Atmos::Spatial::ToWorldPoint2D";
            }

            export module ToWorldPoint3D {
                export const typeName: "Atmos::Spatial::ToWorldPoint3D"
            }

            export module Bounds {
                export const typeName: "Atmos::Spatial::Bounds";
            }

            export module CreateBounds {
                export const typeName: "Arca::Create<Atmos::Spatial::Bounds>";
            }

            export module DestroyBounds {
                export const typeName: "Arca::Destroy<Atmos::Spatial::Bounds>";
            }
        }

        export module UI {
            export module Image {
                export const typeName: "Atmos::UI::Image";
            }

            export module CreateImage {
                export const typeName: "Arca::Create<Atmos::UI::Image>";
            }

            export module DestroyImage {
                export const typeName: "Arca::Destroy<Atmos::UI::Image>";
            }

            export module Text {
                export const typeName: "Atmos::UI::Text";
            }

            export module CreateText {
                export const typeName: "Arca::Create<Atmos::UI::Text>";
            }

            export module DestroyText {
                export const typeName: "Arca::Destroy<Atmos::UI::Text>";
            }
        }

        export module World {
            export module RequestField {
                export const typeName: "Atmos::World::RequestField";
            }
        }
    }

    export type CommandTypeTraits =
        Atmos.TypeTraits.Asset.FindByName.Action |
        Atmos.TypeTraits.Asset.FindByName.Audio |
        Atmos.TypeTraits.Asset.FindByName.Font |
        Atmos.TypeTraits.Asset.FindByName.Image |
        Atmos.TypeTraits.Asset.FindByName.Material |
        Atmos.TypeTraits.Asset.FindByName.Script |
        Atmos.TypeTraits.Asset.FindByName.Shader |
        Atmos.TypeTraits.Audio.ChangeSoundVolume |
        Atmos.TypeTraits.Audio.PauseSound |
        Atmos.TypeTraits.Audio.RestartSound |
        Atmos.TypeTraits.Audio.ResumeSound |
        Atmos.TypeTraits.Audio.CreatePositionedSound |
        Atmos.TypeTraits.Audio.DestroyPositionedSound |
        Atmos.TypeTraits.Audio.CreateUniversalSound |
        Atmos.TypeTraits.Audio.DestroyUniversalSound |
        Atmos.TypeTraits.Entity.FindByName |
        Atmos.TypeTraits.Entity.FindByPosition |
        Atmos.TypeTraits.Entity.MoveTo |
        Atmos.TypeTraits.Entity.CanMoveTo |
        Atmos.TypeTraits.Entity.ModifyTags |
        Atmos.TypeTraits.Entity.FindPath |
        Atmos.TypeTraits.Entity.CreateEntity |
        Atmos.TypeTraits.Entity.DestroyEntity |
        Atmos.TypeTraits.Logging.Log |
        Atmos.TypeTraits.Render.ChangeColor |
        Atmos.TypeTraits.Render.ChangeImageCore |
        Atmos.TypeTraits.Render.MoveGridRegion |
        Atmos.TypeTraits.Render.ChangeMaterialAsset |
        Atmos.TypeTraits.Render.ChangeTextCore |
        Atmos.TypeTraits.Render.ChangeViewSlice |
        Atmos.TypeTraits.Render.FindImagesByBox |
        Atmos.TypeTraits.Render.FindLinesByBox |
        Atmos.TypeTraits.Render.FindGridRegionsByBox |
        Atmos.TypeTraits.Render.MoveLine |
        Atmos.TypeTraits.Render.TextBaseSize |
        Atmos.TypeTraits.Render.CreateDynamicImage |
        Atmos.TypeTraits.Render.DestroyDynamicImage |
        Atmos.TypeTraits.Render.CreateDynamicText |
        Atmos.TypeTraits.Render.DestroyDynamicText |
        Atmos.TypeTraits.Render.CreateGridRegion |
        Atmos.TypeTraits.Render.DestroyGridRegion |
        Atmos.TypeTraits.Render.CreateImageCore |
        Atmos.TypeTraits.Render.DestroyImageCore |
        Atmos.TypeTraits.Render.CreateLine |
        Atmos.TypeTraits.Render.DestroyLine |
        Atmos.TypeTraits.Render.CreateRenderCore |
        Atmos.TypeTraits.Render.DestroyRenderCore |
        Atmos.TypeTraits.Render.CreateViewSlice |
        Atmos.TypeTraits.Render.DestroyViewSlice |
        Atmos.TypeTraits.Spatial.MoveBounds |
        Atmos.TypeTraits.Spatial.RotateBounds |
        Atmos.TypeTraits.Spatial.ScaleBounds |
        Atmos.TypeTraits.Spatial.ToWorldPoint2D |
        Atmos.TypeTraits.Spatial.ToWorldPoint3D |
        Atmos.TypeTraits.Spatial.CreateBounds |
        Atmos.TypeTraits.Spatial.DestroyBounds |
        Atmos.TypeTraits.UI.CreateImage |
        Atmos.TypeTraits.UI.DestroyImage |
        Atmos.TypeTraits.UI.CreateText |
        Atmos.TypeTraits.UI.DestroyText |
        Atmos.TypeTraits.World.RequestField;

    export type SignalTypeTraits =
        Atmos.TypeTraits.Audio.SoundFinished |
        Atmos.TypeTraits.Input.MouseMoved |
        Atmos.TypeTraits.Input.ActionPressed |
        Atmos.TypeTraits.Input.ActionDepressed;

    export type LocalRelicTypeTraits =
        Atmos.TypeTraits.Asset.Action |
        Atmos.TypeTraits.Asset.Audio |
        Atmos.TypeTraits.Asset.Font |
        Atmos.TypeTraits.Asset.Image |
        Atmos.TypeTraits.Asset.Material |
        Atmos.TypeTraits.Asset.Script |
        Atmos.TypeTraits.Asset.Shader |
        Atmos.TypeTraits.Audio.PositionedSound |
        Atmos.TypeTraits.Audio.UniversalSound |
        Atmos.TypeTraits.Entity.Entity |
        Atmos.TypeTraits.UI.Image |
        Atmos.TypeTraits.UI.Text |
        Atmos.TypeTraits.Render.DynamicImage |
        Atmos.TypeTraits.Render.DynamicText |
        Atmos.TypeTraits.Render.GridRegion |
        Atmos.TypeTraits.Render.Line;

    export type GlobalRelicTypeTraits =
        Atmos.TypeTraits.Diagnostics.Statistics |
        Atmos.TypeTraits.Render.Camera |
        Atmos.TypeTraits.Input.Information;

    export type ShardTypeTraits =
        Atmos.TypeTraits.Spatial.Bounds |
        Atmos.TypeTraits.Render.ImageCore |
        Atmos.TypeTraits.Render.RenderCore |
        Atmos.TypeTraits.Render.ViewSlice;

    export interface TypeTraitMap {
        [Atmos.Traits.Asset.FindByName.Action.typeName]: Atmos.TypeTraits.Asset.FindByName.Action;
        [Atmos.Traits.Asset.FindByName.Audio.typeName]: Atmos.TypeTraits.Asset.FindByName.Audio;
        [Atmos.Traits.Asset.FindByName.Font.typeName]: Atmos.TypeTraits.Asset.FindByName.Font;
        [Atmos.Traits.Asset.FindByName.Image.typeName]: Atmos.TypeTraits.Asset.FindByName.Image;
        [Atmos.Traits.Asset.FindByName.Material.typeName]: Atmos.TypeTraits.Asset.FindByName.Material;
        [Atmos.Traits.Asset.FindByName.Script.typeName]: Atmos.TypeTraits.Asset.FindByName.Script;
        [Atmos.Traits.Asset.FindByName.Shader.typeName]: Atmos.TypeTraits.Asset.FindByName.Shader;
        [Atmos.Traits.Audio.ChangeSoundVolume.typeName]: Atmos.TypeTraits.Audio.ChangeSoundVolume;
        [Atmos.Traits.Audio.PauseSound.typeName]: Atmos.TypeTraits.Audio.PauseSound;
        [Atmos.Traits.Audio.RestartSound.typeName]: Atmos.TypeTraits.Audio.RestartSound;
        [Atmos.Traits.Audio.ResumeSound.typeName]: Atmos.TypeTraits.Audio.ResumeSound;
        [Atmos.Traits.Audio.CreatePositionedSound.typeName]: Atmos.TypeTraits.Audio.CreatePositionedSound;
        [Atmos.Traits.Audio.DestroyPositionedSound.typeName]: Atmos.TypeTraits.Audio.DestroyPositionedSound;
        [Atmos.Traits.Audio.CreateUniversalSound.typeName]: Atmos.TypeTraits.Audio.CreateUniversalSound;
        [Atmos.Traits.Audio.DestroyUniversalSound.typeName]: Atmos.TypeTraits.Audio.DestroyUniversalSound;
        [Atmos.Traits.Entity.FindByName.typeName]: Atmos.TypeTraits.Entity.FindByName;
        [Atmos.Traits.Entity.FindByPosition.typeName]: Atmos.TypeTraits.Entity.FindByPosition;
        [Atmos.Traits.Entity.MoveTo.typeName]: Atmos.TypeTraits.Entity.MoveTo;
        [Atmos.Traits.Entity.CanMoveTo.typeName]: Atmos.TypeTraits.Entity.CanMoveTo;
        [Atmos.Traits.Entity.ModifyTags.typeName]: Atmos.TypeTraits.Entity.ModifyTags;
        [Atmos.Traits.Entity.FindPath.typeName]: Atmos.TypeTraits.Entity.FindPath;
        [Atmos.Traits.Entity.CreateEntity.typeName]: Atmos.TypeTraits.Entity.CreateEntity;
        [Atmos.Traits.Entity.DestroyEntity.typeName]: Atmos.TypeTraits.Entity.DestroyEntity;
        [Atmos.Traits.Logging.Log.typeName]: Atmos.TypeTraits.Logging.Log;
        [Atmos.Traits.Render.ChangeColor.typeName]: Atmos.TypeTraits.Render.ChangeColor;
        [Atmos.Traits.Render.ChangeImageCore.typeName]: Atmos.TypeTraits.Render.ChangeImageCore;
        [Atmos.Traits.Render.MoveGridRegion.typeName]: Atmos.TypeTraits.Render.MoveGridRegion;
        [Atmos.Traits.Render.ChangeMaterialAsset.typeName]: Atmos.TypeTraits.Render.ChangeMaterialAsset;
        [Atmos.Traits.Render.ChangeTextCore.typeName]: Atmos.TypeTraits.Render.ChangeTextCore;
        [Atmos.Traits.Render.ChangeViewSlice.typeName]: Atmos.TypeTraits.Render.ChangeViewSlice;
        [Atmos.Traits.Render.FindImagesByBox.typeName]: Atmos.TypeTraits.Render.FindImagesByBox;
        [Atmos.Traits.Render.FindLinesByBox.typeName]: Atmos.TypeTraits.Render.FindLinesByBox;
        [Atmos.Traits.Render.FindGridRegionsByBox.typeName]: Atmos.TypeTraits.Render.FindGridRegionsByBox;
        [Atmos.Traits.Render.MoveLine.typeName]: Atmos.TypeTraits.Render.MoveLine;
        [Atmos.Traits.Render.TextBaseSize.typeName]: Atmos.TypeTraits.Render.TextBaseSize;
        [Atmos.Traits.Render.CreateDynamicImage.typeName]: Atmos.TypeTraits.Render.CreateDynamicImage;
        [Atmos.Traits.Render.DestroyDynamicImage.typeName]: Atmos.TypeTraits.Render.DestroyDynamicImage;
        [Atmos.Traits.Render.CreateDynamicText.typeName]: Atmos.TypeTraits.Render.CreateDynamicText;
        [Atmos.Traits.Render.DestroyDynamicText.typeName]: Atmos.TypeTraits.Render.DestroyDynamicText;
        [Atmos.Traits.Render.CreateGridRegion.typeName]: Atmos.TypeTraits.Render.CreateGridRegion;
        [Atmos.Traits.Render.DestroyGridRegion.typeName]: Atmos.TypeTraits.Render.DestroyGridRegion;
        [Atmos.Traits.Render.CreateImageCore.typeName]: Atmos.TypeTraits.Render.CreateImageCore;
        [Atmos.Traits.Render.DestroyImageCore.typeName]: Atmos.TypeTraits.Render.DestroyImageCore;
        [Atmos.Traits.Render.CreateLine.typeName]: Atmos.TypeTraits.Render.CreateLine;
        [Atmos.Traits.Render.DestroyLine.typeName]: Atmos.TypeTraits.Render.DestroyLine;
        [Atmos.Traits.Render.CreateRenderCore.typeName]: Atmos.TypeTraits.Render.CreateRenderCore;
        [Atmos.Traits.Render.DestroyRenderCore.typeName]: Atmos.TypeTraits.Render.DestroyRenderCore;
        [Atmos.Traits.Render.CreateViewSlice.typeName]: Atmos.TypeTraits.Render.CreateViewSlice;
        [Atmos.Traits.Render.DestroyViewSlice.typeName]: Atmos.TypeTraits.Render.DestroyViewSlice;
        [Atmos.Traits.Spatial.MoveBounds.typeName]: Atmos.TypeTraits.Spatial.MoveBounds;
        [Atmos.Traits.Spatial.RotateBounds.typeName]: Atmos.TypeTraits.Spatial.RotateBounds;
        [Atmos.Traits.Spatial.ScaleBounds.typeName]: Atmos.TypeTraits.Spatial.ScaleBounds;
        [Atmos.Traits.Spatial.ToWorldPoint2D.typeName]: Atmos.TypeTraits.Spatial.ToWorldPoint2D;
        [Atmos.Traits.Spatial.ToWorldPoint3D.typeName]: Atmos.TypeTraits.Spatial.ToWorldPoint3D;
        [Atmos.Traits.Spatial.CreateBounds.typeName]: Atmos.TypeTraits.Spatial.CreateBounds;
        [Atmos.Traits.Spatial.DestroyBounds.typeName]: Atmos.TypeTraits.Spatial.DestroyBounds;
        [Atmos.Traits.UI.CreateImage.typeName]: Atmos.TypeTraits.UI.CreateImage;
        [Atmos.Traits.UI.DestroyImage.typeName]: Atmos.TypeTraits.UI.DestroyImage;
        [Atmos.Traits.UI.CreateText.typeName]: Atmos.TypeTraits.UI.CreateText;
        [Atmos.Traits.UI.DestroyText.typeName]: Atmos.TypeTraits.UI.DestroyText;
        [Atmos.Traits.World.RequestField.typeName]: Atmos.TypeTraits.World.RequestField;

        [Atmos.Traits.Audio.SoundFinished.typeName]: Atmos.TypeTraits.Audio.SoundFinished;
        [Atmos.Traits.Input.MouseMoved.typeName]: Atmos.TypeTraits.Input.MouseMoved;
        [Atmos.Traits.Input.ActionPressed.typeName]: Atmos.TypeTraits.Input.ActionPressed;
        [Atmos.Traits.Input.ActionDepressed.typeName]: Atmos.TypeTraits.Input.ActionDepressed;

        [Atmos.Traits.Asset.Action.typeName]: Atmos.TypeTraits.Asset.Action;
        [Atmos.Traits.Asset.Audio.typeName]: Atmos.TypeTraits.Asset.Audio;
        [Atmos.Traits.Asset.Font.typeName]: Atmos.TypeTraits.Asset.Font;
        [Atmos.Traits.Asset.Image.typeName]: Atmos.TypeTraits.Asset.Image;
        [Atmos.Traits.Asset.Material.typeName]: Atmos.TypeTraits.Asset.Material;
        [Atmos.Traits.Asset.Script.typeName]: Atmos.TypeTraits.Asset.Script;
        [Atmos.Traits.Asset.Shader.typeName]: Atmos.TypeTraits.Asset.Shader;
        [Atmos.Traits.Audio.PositionedSound.typeName]: Atmos.TypeTraits.Audio.PositionedSound;
        [Atmos.Traits.Audio.UniversalSound.typeName]: Atmos.TypeTraits.Audio.UniversalSound;
        [Atmos.Traits.Entity.Entity.typeName]: Atmos.TypeTraits.Entity.Entity;
        [Atmos.Traits.UI.Image.typeName]: Atmos.TypeTraits.UI.Image;
        [Atmos.Traits.UI.Text.typeName]: Atmos.TypeTraits.UI.Text;
        [Atmos.Traits.Render.DynamicImage.typeName]: Atmos.TypeTraits.Render.DynamicImage;
        [Atmos.Traits.Render.DynamicText.typeName]: Atmos.TypeTraits.Render.DynamicText;
        [Atmos.Traits.Render.GridRegion.typeName]: Atmos.TypeTraits.Render.GridRegion;
        [Atmos.Traits.Render.Line.typeName]: Atmos.TypeTraits.Render.Line;

        [Atmos.Traits.Diagnostics.Statistics.typeName]: Atmos.TypeTraits.Diagnostics.Statistics;
        [Atmos.Traits.Render.Camera.typeName]: Atmos.TypeTraits.Render.Camera;
        [Atmos.Traits.Input.Information.typeName]: Atmos.TypeTraits.Input.Information;

        [Atmos.Traits.Spatial.Bounds.typeName]: Atmos.TypeTraits.Spatial.Bounds;
        [Atmos.Traits.Render.ImageCore.typeName]: Atmos.TypeTraits.Render.ImageCore;
        [Atmos.Traits.Render.RenderCore.typeName]: Atmos.TypeTraits.Render.RenderCore;
        [Atmos.Traits.Render.ViewSlice.typeName]: Atmos.TypeTraits.Render.ViewSlice;
    }

    export module Input {
        export interface Information {
            previousMousePosition: Spatial.Point2D;
            currentMousePosition: Spatial.Point2D;
        }

        export interface MouseMoved {
            previous: Spatial.Point2D;
            current: Spatial.Point2D;
        }
        
        export interface ActionPressed {
            action: Atmos.Asset.Action;
        }

        export interface ActionDepressed {
            action: Atmos.Asset.Action;
        }

        export enum Key {
            LeftMouseButton = 0,
            MiddleMouseButton = 1,
            RightMouseButton = 2,
            X1MouseButton = 3,
            X2MouseButton = 4,

            A = 5,
            B = 6,
            C = 7,
            D = 8,
            E = 9,
            F = 10,
            G = 11,
            H = 12,
            I = 13,
            J = 14,
            K = 15,
            L = 16,
            M = 17,
            N = 18,
            O = 19,
            P = 20,
            Q = 21,
            R = 22,
            S = 23,
            T = 24,
            U = 25,
            V = 26,
            W = 27,
            X = 28,
            Y = 29,
            Z = 30,

            Zero = 31,
            One = 32,
            Two = 33,
            Three = 34,
            Four = 35,
            Five = 36,
            Six = 37,
            Seven = 38,
            Eight = 39,
            Nine = 40,

            Left = 41,
            Up = 42,
            Right = 43,
            Down = 44,

            F1 = 45,
            F2 = 46,
            F3 = 47,
            F4 = 48,
            F5 = 49,
            F6 = 50,
            F7 = 51,
            F8 = 52,
            F9 = 53,
            F10 = 54,
            F11 = 55,
            F12 = 56,
            F13 = 57,
            F14 = 58,
            F15 = 59,
            F16 = 60,
            F17 = 61,
            F18 = 62,
            F19 = 63,
            F20 = 64,
            F21 = 65,
            F22 = 66,
            F23 = 67,
            F24 = 68,

            LeftAlt = 69,
            LeftControl = 70,
            LeftShift = 71,
            LeftBracket = 72,
            LeftGui = 73,

            RightAlt = 74,
            RightControl = 75,
            RightShift = 76,
            RightBracket = 77,
            RightGui = 78,

            PageDown = 79,
            PageUp = 80,

            Keypad0 = 81,
            Keypad00 = 82,
            Keypad000 = 83,
            Keypad1 = 84,
            Keypad2 = 85,
            Keypad3 = 86,
            Keypad4 = 87,
            Keypad5 = 88,
            Keypad6 = 89,
            Keypad7 = 90,
            Keypad8 = 91,
            Keypad9 = 92,
            KeypadA = 93,
            KeypadB = 94,
            KeypadC = 95,
            KeypadD = 96,
            KeypadE = 97,
            KeypadF = 98,
            KeypadAmpersand = 99,
            KeypadAt = 100,
            KeypadBackspace = 101,
            KeypadBinary = 102,
            KeypadClear = 103,
            KeypadClearEntry = 104,
            KeypadColon = 105,
            KeypadComma = 106,
            KeypadAmpersandAmpersand = 107,
            KeypadBarBar = 108,
            KeypadDecimal = 109,
            KeypadForwardSlash = 110,
            KeypadEnter = 111,
            KeypadEquals = 112,
            KeypadEqualsAS400 = 113,
            KeypadExclamation = 114,
            KeypadGreater = 115,
            KeypadLess = 116,
            KeypadPound = 117,
            KeypadHexadecimal = 118,
            KeypadLeftBrace = 119,
            KeypadLeftParenthesis = 120,
            KeypadMemAdd = 121,
            KeypadMemClear = 122,
            KeypadMemDivide = 123,
            KeypadMemMultiply = 124,
            KeypadMemRecall = 125,
            KeypadMemStore = 126,
            KeypadMemSubtract = 127,
            KeypadHyphen = 128,
            KeypadAsterisk = 129,
            KeypadOctal = 130,
            KeypadPercent = 131,
            KeypadPeriod = 132,
            KeypadPlus = 133,
            KeypadPlusMinus = 134,
            KeypadPower = 135,
            KeypadRightBrace = 136,
            KeypadRightParenthesis = 137,
            KeypadSpace = 138,
            KeypadTab = 139,
            KeypadBar = 140,
            KeypadXor = 141,

            BrightnessUp = 142,
            BrightnessDown = 143,

            VolumeUp = 144,
            VolumeDown = 145,

            AudioMute = 146,
            AudioNext = 147,
            AudioPlay = 148,
            AudioPrevious = 149,
            AudioStop = 150,

            AcBack = 151,
            AcBookmarks = 152,
            AcForward = 153,
            AcHome = 154,
            AcRefresh = 155,
            AcSearch = 156,
            AcStop = 157,

            IlluminationDown = 158,
            IlluminationUp = 159,
            IlluminationToggle = 160,

            Backslash = 161,
            Backspace = 162,
            Comma = 163,
            Quote = 164,
            Again = 165,
            AltErase = 166,
            Application = 167,
            Calculator = 168,
            Cancel = 169,
            CapsLock = 170,
            Clear = 171,
            ClearAgain = 172,
            Computer = 173,
            Copy = 174,
            CrSel = 175,
            CurrencySubUnit = 176,
            CurrencyUnit = 177,
            Cut = 178,
            DecimalSeparator = 179,
            Del = 180,
            DisplaySwitch = 181,
            Eject = 182,
            End = 183,
            Equals = 184,
            Escape = 185,
            Execute = 186,
            ExSel = 187,
            Find = 188,
            Grave = 189,
            Help = 190,
            Home = 191,
            Insert = 192,
            Mail = 193,
            MediaSelect = 194,
            Menu = 195,
            Hyphen = 196,
            ModeSwitch = 197,
            Mute = 198,
            Numlock = 199,
            Oper = 200,
            Out = 201,
            Paste = 202,
            Pause = 203,
            Period = 204,
            Power = 205,
            PrintScreen = 206,
            Prior = 207,
            Enter = 208,
            ScrollLock = 209,
            Select = 210,
            Semicolon = 211,
            Separator = 212,
            ForwardSlash = 213,
            Sleep = 214,
            Space = 215,
            Stop = 216,
            SysReq = 217,
            Tab = 218,
            ThousandsSeparator = 219,
            Undo = 220,
            WorldWideWeb = 221,
        }
    }
}

export { Atmos };