#pragma once

#include "DirectX9Includes.h"
#include "DirectX9ShaderAssetResource.h"

#include "ImageRender.h"
#include "LineRender.h"

#include "ShaderAsset.h"

#include "Angle.h"
#include "Color.h"
#include "ScreenSize.h"

namespace Atmos::Render::DirectX9
{
    class GraphicsManager;

    class Renderer final
    {
    public:
        explicit Renderer(GraphicsManager& owner);

        ~Renderer();

        void StageRender(const ImageRender& imageRender);
        void StageRender(const LineRender& lineRender);

        void DrawFrame(ScreenSize screenSize, const Color& backgroundColor);

        void OnLostDevice();
        void OnResetDevice();
    private:
        GraphicsManager& owner;
    private:
        struct Vertex
        {
            D3DXVECTOR2 position = {0, 0};
            D3DCOLOR color = 0;
            FLOAT u = 0.0f;
            FLOAT v = 0.0f;

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
            LPDIRECT3DTEXTURE9 texture;
            const Asset::Shader* shader = nullptr;

            StagedObject(
                std::vector<Vertex>&& vertices,
                std::vector<Index>&& indices,
                unsigned int primCount,
                LPDIRECT3DTEXTURE9 texture,
                const Asset::Shader* shader);
            StagedObject(StagedObject&& arg) noexcept = default;
            StagedObject& operator=(StagedObject&& arg) noexcept = default;
        };

        StagedObject CreateQuad(
            LPDIRECT3DTEXTURE9 texture,
            const Asset::Shader* shader,
            float x,
            float y,
            const AxisAlignedBox2D& imageBounds,
            const Size2D& size,
            const Angle& rotation,
            const Color& color);

        class StagedLine
        {
        public:
            std::vector<D3DXVECTOR2> points;
            FLOAT width;
            D3DCOLOR color;

            StagedLine(const std::vector<Position2D>& points, float width, const Color& color);
        };
    private:
        using BufferSize = unsigned short;

        static constexpr BufferSize maxVertexSprint = 4000;
        static constexpr BufferSize maxIndexSprint = 6000;

        LPDIRECT3DDEVICE9 device = nullptr;

        LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
        LPDIRECT3DINDEXBUFFER9 indexBuffer = nullptr;
        LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration = nullptr;

        LPD3DXLINE lineInterface = nullptr;

        struct Layer
        {
            using Objects = std::vector<StagedObject>;
            Objects objects{};

            using Lines = std::vector<StagedLine>;
            Lines lines{};

            FLOAT z;

            explicit Layer(FLOAT z);
        };

        using Layers = std::vector<Layer>;
        Layers layers;
        Layers::iterator LayerWithZ(FLOAT z);
    private:
        void InitializeBuffers();
    private:
        void StageRender(
            LPDIRECT3DTEXTURE9 texture,
            const Asset::Shader* shader,
            float x,
            float y,
            float z,
            const AxisAlignedBox2D& imageBounds,
            const Size2D& size,
            const Angle& rotation,
            const Color& color);

        void StageRender(
            const std::vector<Position2D>& points,
            Position2D::Value z,
            float width,
            const Color& color);
    private:
        void PushAllThroughPipeline(const ScreenSize& screenSize, const Color& backgroundColor);

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
            void Flush(Layers& layers, D3DCOLOR backgroundColor);
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
            const Asset::Shader* currentShader = nullptr;
            const Asset::Resource::DirectX9::Shader* currentShaderResource = nullptr;
        private:
            bool Start();
            void SetBackgroundColor(D3DCOLOR color);
            void Sort(Layers& layers) const;
            void Sort(Layer& layer) const;
            void Handle(StagedObject& staged);
            void Handle(StagedLine& staged);
            void DrawPrimitives();
            void DrawEndPrimitives();
            void DrawPrimitivesCommon();
            void End();

            void SetTexture(LPDIRECT3DTEXTURE9 set);
            void SetShader(const Asset::Shader* set);
        };
    };
}
