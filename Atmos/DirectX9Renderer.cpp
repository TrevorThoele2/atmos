#include "DirectX9Renderer.h"

#include "DirectX9GraphicsManager.h"
#include "DirectX9ImageAssetData.h"
#include "DirectX9ShaderAssetData.h"
#include "DirectX9SurfaceData.h"
#include "DirectX9Utilities.h"

#include "Camera.h"

namespace Atmos::Render::DirectX9
{
    Renderer::Renderer(
        GraphicsManager& owner,
        Arca::Index<Asset::ShaderAsset> defaultTexturedMaterialShader,
        Arca::Reliquary& reliquary)
        :
        owner(owner),
        reliquary(&reliquary),
        device(owner.Device()),
        defaultTexturedMaterialShader(defaultTexturedMaterialShader)
    {
        InitializeBuffers();

        D3DXCreateLine(device, &lineInterface);
    }

    Renderer::~Renderer()
    {
        if (vertexBuffer)
            vertexBuffer->Release();
        if(indexBuffer)
            indexBuffer->Release();
        if(vertexDeclaration)
            vertexDeclaration->Release();

        if(lineInterface)
            lineInterface->Release();
    }

    void Renderer::StageRender(const ImageRender& imageRender)
    {
        if (!imageRender.asset)
            return;

        StageRender(
            imageRender.asset->FileDataAs<ImageAssetDataImplementation>()->Texture(),
            imageRender.material->VertexShader()
                ? &*imageRender.material->VertexShader()
                : DefaultTexturedMaterialShader(),
            imageRender.position.x,
            imageRender.position.y,
            imageRender.position.z,
            imageRender.assetSlice,
            imageRender.size,
            Angle(),
            imageRender.color);
    }

    void Renderer::StageRender(const LineRender& lineRender)
    {
        StageRender(lineRender.points, lineRender.z, lineRender.width, lineRender.color);
    }

    void Renderer::DrawFrame(ScreenSize screenSize, const Color& backgroundColor)
    {
        PushAllThroughPipeline(screenSize, backgroundColor);
    }

    void Renderer::OnLostDevice()
    {
        if (vertexBuffer)
        {
            vertexBuffer->Release();
            vertexBuffer = nullptr;
        }

        if (indexBuffer)
        {
            indexBuffer->Release();
            indexBuffer = nullptr;
        }

        if (defaultTexturedMaterialShader)
            defaultTexturedMaterialShader->FileDataAs<ShaderAssetDataImplementation>()->Effect()->OnLostDevice();

        if (lineInterface)
        {
            LogIfError(
                lineInterface->OnLostDevice(),
                []() { return Logging::Log(
                    "The DirectX line interface was not able to be released.",
                    Logging::Severity::Error); });
        }
    }

    void Renderer::OnResetDevice()
    {
        InitializeBuffers();
        if (defaultTexturedMaterialShader)
            defaultTexturedMaterialShader->FileDataAs<ShaderAssetDataImplementation>()->Effect()->OnResetDevice();

        if (lineInterface)
        {
            LogIfError(
                lineInterface->OnResetDevice(),
                []() { return Logging::Log(
                    "The DirectX line interface was not able to be reset.",
                    Logging::Severity::Error); });
        }
    }

    Arca::Index<Asset::ShaderAsset> Renderer::DefaultTexturedMaterialShader() const
    {
        return defaultTexturedMaterialShader;
    }

    Renderer::Vertex::Vertex(const D3DXVECTOR2& position, D3DCOLOR color, FLOAT u, FLOAT v) :
        position(position), color(color)
    {}

    auto Renderer::CreateQuad(
        LPDIRECT3DTEXTURE9 texture,
        const Asset::ShaderAsset* shader,
        float x,
        float y,
        const AxisAlignedBox2D& imageBounds,
        const Size2D& size,
        const Angle& rotation,
        const Color& color)
        -> StagedObject
    {
        D3DXMATRIX matrix;
        {
            D3DXVECTOR2 positionVector(x, y);
            D3DXVECTOR2 centerVector(0.0f, 0.0f);
            D3DXVECTOR2 scalingVector(size.width / imageBounds.Width(), size.height / imageBounds.Height());

            D3DXMatrixIdentity(&matrix);
            D3DXMatrixTransformation2D(
                &matrix,
                &centerVector,
                0.0f,
                &scalingVector,
                &centerVector,
                rotation.As<Radians>(),
                &positionVector);
        }

        std::vector<Vertex> vertices =
        {
            {
                D3DXVECTOR2(0.0f, 0.0f),
                ToDirectXColor(color),
                0.0f,
                0.0f
            },
            {
                D3DXVECTOR2(imageBounds.Width(), 0.0f),
                ToDirectXColor(color),
                1.0f,
                0.0f
            },
            {
                D3DXVECTOR2(0.0f, imageBounds.Height()),
                ToDirectXColor(color),
                0.0f,
                1.0f
            },
            {
                D3DXVECTOR2(imageBounds.Width(), imageBounds.Height()),
                ToDirectXColor(color),
                1.0f,
                1.0f
            }
        };

        for (auto& vertex : vertices)
            D3DXVec2TransformCoord(&vertex.position, &vertex.position, &matrix);

        return StagedObject(std::move(vertices), { 0, 1, 2, 1, 3, 2 }, 2, texture, shader);
    }

    Renderer::StagedObject::StagedObject(
        std::vector<Vertex>&& vertices,
        std::vector<Index>&& indices,
        unsigned int primCount,
        LPDIRECT3DTEXTURE9 texture,
        const Asset::ShaderAsset* shader)
        :
        vertices(vertices), indices(indices), primCount(primCount), texture(texture), shader(shader)
    {}

    Renderer::StagedLine::StagedLine(const std::vector<Position2D>& points, float width, const Color& color) :
        width(width),
        color(ToDirectXColor(color))
    {
        this->points.reserve(points.size());
        for (auto& point : points)
            this->points.emplace_back(point.x, point.y);
    }

    Renderer::Layer::Layer(FLOAT z) : z(z)
    {}

    auto Renderer::LayerWithZ(FLOAT z) -> Layers::iterator
    {
        auto found = std::find_if(
            layers.begin(),
            layers.end(),
            [z](const Layer& layer)
            {
                return layer.z == z;
            });
        if (found != layers.end())
            return found;

        layers.emplace_back(z);
        return --layers.end();
    }

    void Renderer::InitializeBuffers()
    {
        device->CreateVertexBuffer(
            maxVertexSprint * sizeof(Vertex),
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            0,
            D3DPOOL_DEFAULT,
            &vertexBuffer,
            nullptr);
        device->CreateIndexBuffer(
            maxIndexSprint * sizeof(Index),
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16, D3DPOOL_DEFAULT,
            &indexBuffer,
            nullptr);

        D3DVERTEXELEMENT9 vertexElements[] =
        {
            { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 8, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };

        device->CreateVertexDeclaration(vertexElements, &vertexDeclaration);
    }

    void Renderer::StageRender(
        LPDIRECT3DTEXTURE9 texture,
        const Asset::ShaderAsset* shader,
        float x,
        float y,
        float z,
        const AxisAlignedBox2D& imageBounds,
        const Size2D& size,
        const Angle& rotation,
        const Color& color)
    {
        const auto layer = LayerWithZ(z);
        layer->objects.push_back(CreateQuad(texture, shader, x, y, imageBounds, size, rotation, color));
    }

    void Renderer::StageRender(
        const std::vector<Position2D>& points,
        Position2D::Value z,
        float width,
        const Color& color)
    {
        const auto layer = LayerWithZ(z);
        layer->lines.emplace_back(points, width, color);
    }

    void Renderer::PushAllThroughPipeline(const ScreenSize& screenSize, const Color& backgroundColor)
    {
        D3DXMATRIX projection;

        D3DXMatrixOrthoOffCenterLH(
            &projection,
            0,
            static_cast<FLOAT>(screenSize.width),
            static_cast<FLOAT>(screenSize.height),
            0,
            0.0f,
            1.0f);

        Pipeline pipeline(
            owner,
            device,
            vertexBuffer,
            indexBuffer,
            vertexDeclaration,
            lineInterface,
            projection,
            screenSize);
        pipeline.Flush(layers, ToDirectXColor(backgroundColor));
    }

    Renderer::Pipeline::Pipeline(
        GraphicsManager& manager,
        LPDIRECT3DDEVICE9 device,
        LPDIRECT3DVERTEXBUFFER9 vertexBuffer,
        LPDIRECT3DINDEXBUFFER9 indexBuffer,
        LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration,
        LPD3DXLINE lineInterface,
        D3DXMATRIX projection,
        const ScreenSize& screenSize)
        :
        manager(manager),
        device(device),
        vertexBuffer(vertexBuffer),
        indexBuffer(indexBuffer),
        vertexDeclaration(vertexDeclaration),
        lineInterface(lineInterface),
        projection(projection),
        screenSize(screenSize)
    {}
    
    void Renderer::Pipeline::Flush(Layers& layers, D3DCOLOR backgroundColor)
    {
        if (!Start())
        {
            layers.clear();
            return;
        }

        SetBackgroundColor(backgroundColor);

        Sort(layers);

        device->SetVertexDeclaration(vertexDeclaration);
        device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
        device->SetIndices(indexBuffer);

        for (auto& layer : layers)
        {
            auto& objects = layer.objects;
            if (!objects.empty())
            {
                SetShader(objects.begin()->shader);
                SetTexture(objects.begin()->texture);

                vertexBuffer->Lock(0, 0, &vertexData, D3DLOCK_DISCARD);
                indexBuffer->Lock(0, 0, &indexData, D3DLOCK_DISCARD);

                for (auto& object : objects)
                    Handle(object);

                vertexBuffer->Unlock();
                indexBuffer->Unlock();
            }

            auto& lines = layer.lines;
            if (!lines.empty())
            {
                lineInterface->Begin();
                for (auto& line : layer.lines)
                    Handle(line);
                lineInterface->End();
            }
        }

        layers.clear();

        DrawEndPrimitives();

        End();
    }

    bool Renderer::Pipeline::Start()
    {
        const auto cooperativeLevel = device->TestCooperativeLevel();

        if (cooperativeLevel == D3DERR_DEVICELOST || cooperativeLevel == D3DERR_DEVICENOTRESET)
            return false;

        device->BeginScene();

        return true;
    }

    void Renderer::Pipeline::SetBackgroundColor(D3DCOLOR color)
    {
        LogIfError(
            device->Clear(0, nullptr, D3DCLEAR_TARGET, color, 1.0f, 0),
            []() { return Logging::Log(
                "Could not set background color.",
                Logging::Severity::Error); });
    }

    void Renderer::Pipeline::Sort(Layers& layers) const
    {
        std::sort(
            layers.begin(),
            layers.end(),
            [](const Layer& left, const Layer& right)
            {
                const auto leftZ = left.z;
                const auto rightZ = right.z;
                return leftZ < rightZ;
            });
    }

    void Renderer::Pipeline::Sort(Layer& layer) const
    {
        std::sort(
            layer.objects.begin(),
            layer.objects.end(),
            [](const StagedObject& left, const StagedObject& right)
            {
                const auto leftTex = left.texture;
                const auto rightTex = right.texture;

                return leftTex == rightTex
                    ? left.shader < right.shader
                    : leftTex < rightTex;
            });

        std::sort(
            layer.lines.begin(),
            layer.lines.end(),
            [](const StagedLine& left, const StagedLine& right)
            {
                return left.width < right.width;
            });
    }

    void Renderer::Pipeline::Handle(StagedObject& staged)
    {
        const auto loopVerticesSize = staged.vertices.size();
        const auto loopIndicesSize = staged.indices.size();

        const auto loopTexture = staged.texture;
        const auto loopShader = staged.shader;

        // Check if the sprint is done
        if (vertexCount + loopVerticesSize >= maxVertexSprint
            || indexCount + loopIndicesSize >= maxIndexSprint)
            DrawPrimitives();

        // Check if the texture AND shader is different
        if (currentTexture != loopTexture && currentShader != loopShader)
        {
            if (vertexCount > 0)
                DrawPrimitives();

            SetTexture(loopTexture);
            SetShader(loopShader);
        }
        // Check if the texture is different
        else if (currentTexture != loopTexture)
        {
            if (vertexCount > 0)
                DrawPrimitives();

            SetTexture(loopTexture);
        }
        // Check if the shader is different
        else if (currentShader != loopShader)
        {
            if (vertexCount > 0)
                DrawPrimitives();

            SetTexture(currentTexture);
            SetShader(loopShader);
        }

        // Vertices
        memcpy(
            &reinterpret_cast<Vertex*>(vertexData)[vertexCount],
            staged.vertices.data(),
            loopVerticesSize * sizeof(Vertex));

        // Indices
        for (size_t indexLoop = 0; indexLoop != loopIndicesSize; ++indexLoop)
            staged.indices[indexLoop] = vertexCount + staged.indices[indexLoop];
        memcpy(
            &reinterpret_cast<Index*>(indexData)[indexCount],
            staged.indices.data(),
            loopIndicesSize * sizeof(Index));

        vertexCount += static_cast<BufferSize>(loopVerticesSize);
        indexCount += static_cast<BufferSize>(loopIndicesSize);
        primitiveCount += staged.primCount;
    }

    void Renderer::Pipeline::Handle(StagedLine& staged)
    {
        if (staged.width != lineInterface->GetWidth())
        {
            lineInterface->End();
            lineInterface->SetWidth(staged.width);
            lineInterface->Begin();
        }

        lineInterface->Draw(staged.points.data(), staged.points.size(), staged.color);
    }

    void Renderer::Pipeline::DrawPrimitives()
    {
        DrawPrimitivesCommon();
        vertexBuffer->Lock(0, 0, &vertexData, D3DLOCK_DISCARD);
        indexBuffer->Lock(0, 0, &indexData, D3DLOCK_DISCARD);
        vertexCount = 0;
        indexCount = 0;
        primitiveCount = 0;
    }

    void Renderer::Pipeline::DrawEndPrimitives()
    {
        if (vertexCount == 0)
            return;

        DrawPrimitivesCommon();
    }

    void Renderer::Pipeline::DrawPrimitivesCommon()
    {
        vertexBuffer->Unlock();
        indexBuffer->Unlock();

        const auto passCount = currentShaderData->Begin();
        size_t passCountCurrent = 0;

        do
        {
            currentShaderData->BeginNextPass(passCountCurrent);
            ++passCountCurrent;

            device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, primitiveCount);

            currentShaderData->EndPass();
        } while (passCountCurrent < passCount);

        currentShaderData->End();
    }

    void Renderer::Pipeline::End()
    {
        device->EndScene();

        LogIfError(
            device->Present(nullptr, nullptr, nullptr, nullptr),
            []() { return Logging::Log(
                "The frame is not presentable.",
                Logging::Severity::Error); });
    }

    void Renderer::Pipeline::SetTexture(LPDIRECT3DTEXTURE9 set)
    {
        currentTexture = set;
        if (currentTexture && currentShaderData)
            currentShaderData->Effect()->SetTexture("g_Texture", currentTexture);
    }

    void Renderer::Pipeline::SetShader(const Asset::ShaderAsset* set)
    {
        currentShader = set;
        currentShaderData = currentShader->FileDataAs<ShaderAssetDataImplementation>();
        currentShaderData->Effect()->SetMatrix("g_Projection", &projection);

        FLOAT g_ScreenSize[2] =
        {
            static_cast<FLOAT>(screenSize.width),
            static_cast<FLOAT>(screenSize.height)
        };
        currentShaderData->Effect()->SetFloatArray(
            "g_ScreenSize",
            g_ScreenSize,
            2);

        if (currentTexture && currentShaderData)
            currentShaderData->Effect()->SetTexture("g_Texture", currentTexture);
    }
}