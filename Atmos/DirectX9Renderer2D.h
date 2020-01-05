#pragma once

#include "DirectX9Includes.h"

#include "ShaderAsset.h"

#include "AxisAlignedBox2D.h"
#include "Scalers2D.h"
#include "Angle.h"
#include "Color.h"

#include "LogSeverity.h"
#include "LogNameValuePairs.h"
#include <Arca/Reliquary.h>

namespace Atmos::Render::DirectX9
{
    class GraphicsManager;

    class Renderer2D
    {
    public:
        using ObjectsSize = size_t;
    public:
        explicit Renderer2D(Arca::Reliquary& reliquary);

        ~Renderer2D();

        void Initialize(GraphicsManager& handler, LPDIRECT3DDEVICE9 device);

        void Start(ObjectsSize size);
        // If the projection exists, then this will flush the sprites
        void Stop(const D3DXMATRIX* projection);
        void Flush(const D3DXMATRIX& projection);

        void Draw
        (
            LPDIRECT3DTEXTURE9 tex,
            Asset::ShaderAsset* shader,
            float X,
            float Y,
            float Z,
            const AxisAlignedBox2D& imageBounds,
            const Size2D& size,
            const Position2D& center,
            const Scalers2D& scalers,
            const Angle& rotation,
            const Color& color
        );

        void OnLostDevice();
        void OnResetDevice();

        Asset::ShaderAsset* DefaultTexturedImageViewShader();
    private:
        struct Vertex
        {
            D3DXVECTOR2 position;
            D3DXVECTOR2 center;
            D3DCOLOR color;
            FLOAT u, v;

            Vertex();
            Vertex(D3DXVECTOR2&& position, D3DCOLOR color, FLOAT u, FLOAT v);
        };

        using Index = unsigned short;

        class Object3D
        {
        public:
            std::vector<Vertex> vertices;
            std::vector<Index> indices;
            unsigned int primCount;
            float z;
            LPDIRECT3DTEXTURE9 tex;
            Asset::ShaderAsset* shader;

            Object3D
            (
                LPDIRECT3DTEXTURE9 tex,
                Asset::ShaderAsset* shader,
                float X,
                float Y,
                float Z,
                const AxisAlignedBox2D& imageBounds,
                const Size2D& size,
                const Position2D& center,
                const Scalers2D& scalers,
                const Angle& rotation,
                const Color& color
            );
            Object3D(Object3D&& arg);
            Object3D& operator=(Object3D&& arg);
        private:
            void SetupQuad
            (
                float X,
                float Y,
                const Position2D& center,
                const Scalers2D& scalers,
                float rotation
            );
            void SetupRegularPolygon
            (
                float radius,
                unsigned int polyCount,
                float X,
                float Y,
                const Scalers2D& scalers,
                float rotation,
                const Color& color
            );

            static void SetupVertexCommon
            (
                Vertex& vertex,
                const D3DXMATRIX& matrix,
                const D3DXVECTOR2& position,
                const D3DXVECTOR2& center
            );
            static void SetupVertexCommon
            (
                Vertex& vertex,
                const D3DXMATRIX& matrix,
                const D3DXVECTOR2& position,
                const D3DXVECTOR2& center,
                const Color& color
            );
        };
    private:
        using BufferSize = unsigned short;

        static constexpr BufferSize maxVertexSprint = 4000;
        static constexpr BufferSize maxIndexSprint = 6000;

        LPDIRECT3DDEVICE9 device;
        LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
        LPDIRECT3DINDEXBUFFER9 indexBuffer;
        LPDIRECT3DVERTEXDECLARATION9 vertexDecl;
        Arca::LocalPtr<Asset::ShaderAsset> defaultTexturedImageViewShader;

        using Objects = std::vector<Object3D>;
        Objects objects;
    private:
        Arca::Reliquary* reliquary = nullptr;
    private:
        void InitializeBuffers();
    private:
        void LogIfError(
            HRESULT hr,
            const String& message,
            Logging::Severity severity,
            const std::optional<Logging::NameValuePairs>& nameValuePairs = {}) const;
    };
}