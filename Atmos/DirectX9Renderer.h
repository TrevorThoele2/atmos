#pragma once

#include "Renderer.h"
#include "DirectX9Includes.h"
#include "DirectX9ShaderAssetData.h"

#include "ShaderAsset.h"

#include "AxisAlignedBox2D.h"
#include "Scalers2D.h"
#include "Angle.h"
#include "Color.h"
#include "ScreenSize.h"

namespace Atmos::Render::DirectX9
{
    class GraphicsManager;

    class Renderer final : public Render::Renderer
    {
    public:
        explicit Renderer(GraphicsManager& owner);

        ~Renderer();

        void Initialize(LPDIRECT3DDEVICE9 device, Arca::Reliquary& reliquary);

        void StageRender(const MaterialRender& materialRender) override;
        void StageRender(const CanvasRender& canvasRender) override;
        void StageRender(const Line& line) override;
        void RenderStaged(const SurfaceData& surface) override;

        void OnLostDevice();
        void OnResetDevice();

        Arca::RelicIndex<Asset::ShaderAsset> DefaultTexturedImageViewShader();
    private:
        GraphicsManager& owner;
    private:
        struct Vertex
        {
            D3DXVECTOR2 position = {0, 0};
            D3DXVECTOR2 center = {0, 0};
            D3DCOLOR color = 0;
            FLOAT u = 0;
            FLOAT v = 0;

            Vertex() = default;
            Vertex(const D3DXVECTOR2& position, D3DCOLOR color, FLOAT u, FLOAT v);
        };

        using Index = unsigned short;

        class StagedObject
        {
        public:
            std::vector<Vertex> vertices;
            std::vector<Index> indices;
            unsigned int primCount = 0;
            float z;
            LPDIRECT3DTEXTURE9 tex;
            const Asset::ShaderAsset* shader;

            StagedObject
            (
                LPDIRECT3DTEXTURE9 tex,
                const Asset::ShaderAsset* shader,
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
            StagedObject(StagedObject&& arg) noexcept = default;
            StagedObject& operator=(StagedObject&& arg) noexcept;
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

        LPDIRECT3DDEVICE9 device = nullptr;

        LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
        LPDIRECT3DINDEXBUFFER9 indexBuffer = nullptr;
        LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration = nullptr;
        Arca::RelicIndex<Asset::ShaderAsset> defaultTexturedImageViewShader{};

        LPD3DXLINE lineInterface = nullptr;

        LPDIRECT3DSURFACE9 mainSurface = nullptr;

        using Objects = std::vector<StagedObject>;
        Objects objects{};
    private:
        void InitializeBuffers();
    private:
        void StageRender
        (
            LPDIRECT3DTEXTURE9 tex,
            const Asset::ShaderAsset* shader,
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

        void StageRender
        (
            const Position2D& from,
            const Position2D& to,
            float width,
            const Color& color
        );
    private:
        class Pipeline
        {
        public:
            Pipeline(
                GraphicsManager& manager,
                LPDIRECT3DDEVICE9 device,
                LPDIRECT3DVERTEXBUFFER9 vertexBuffer,
                LPDIRECT3DINDEXBUFFER9 indexBuffer,
                LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration,
                LPD3DXLINE lineInterface,
                D3DXMATRIX projection,
                const ScreenSize& screenSize);
            void Flush(Objects& objects);
        private:
            GraphicsManager& manager;

            LPDIRECT3DDEVICE9 device;
            LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
            LPDIRECT3DINDEXBUFFER9 indexBuffer;
            LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration;
            LPD3DXLINE lineInterface;

            BufferSize vertexCount = 0;
            BufferSize indexCount = 0;
            BufferSize primitiveCount = 0;

            void* vertexData = nullptr;
            void* indexData = nullptr;

            D3DXMATRIX projection;
            ScreenSize screenSize;

            LPDIRECT3DTEXTURE9 currentTexture = nullptr;
            const Asset::ShaderAsset* currentShader = nullptr;
            const ShaderAssetDataImplementation* currentShaderData = nullptr;
        private:
            bool Start();
            void Sort(Objects& objects) const;
            void HandleObject(StagedObject& object);
            void DrawPrimitives();
            void DrawEndPrimitives();
            void DrawPrimitivesCommon();
            void End();

            void SetTexture(LPDIRECT3DTEXTURE9 set);
            void SetShader(const Asset::ShaderAsset* set);
        };
    };
}
