#include "DirectX9Renderer.h"

#include "DirectX9GraphicsManager.h"
#include "DirectX9ImageAssetData.h"
#include "DirectX9ShaderAssetData.h"
#include "DirectX9CanvasData.h"
#include "DirectX9SurfaceData.h"
#include "DirectX9Utilities.h"

#include "Camera.h"
#include "FileManagerProvider.h"
#include "MathUtility.h"

#include "SimpleFile.h"

namespace Atmos::Render::DirectX9
{
    Renderer::Renderer(GraphicsManager& owner) : owner(owner)
    {}

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

    void Renderer::Initialize(LPDIRECT3DDEVICE9 device, Arca::Reliquary& reliquary)
    {
        this->device = device;
        InitializeBuffers();

        const auto texturedSpritePath = File::manager->ExePath() + "Shaders\\TexturedSprite.fx";

        SimpleInFile inFile(texturedSpritePath);
        const auto texturedSpriteBuffer = inFile.ReadBuffer();

        auto shaderData = owner.CreateShaderData(texturedSpriteBuffer, "TexturedSprite");
        defaultTexturedImageViewShader = reliquary.Do<Arca::Create<Asset::ShaderAsset>>(
            "TexturedSprite",
            std::move(shaderData));

        D3DXCreateLine(device, &lineInterface);
    }

    void Renderer::StageRender(const MaterialRender& materialRender)
    {
        if (!materialRender.material)
            return;

        const auto material = materialRender.material;
        const auto image = material->Image();
        const auto materialSlice = materialRender.materialSlice;
        const auto size = materialRender.size;
        const auto position = materialRender.position;

        StageRender
        (
            image->FileDataAs<ImageAssetDataImplementation>()->Texture(),
            materialRender.patchShader ? materialRender.patchShader : &*material->Shader(),
            position.x,
            position.y,
            materialRender.position.z,
            materialSlice,
            materialSlice.Size(),
            materialSlice.Center(),
            Scalers2D{ size.width, size.height },
            Angle(),
            materialRender.color
        );
    }

    void Renderer::StageRender(const CanvasRender& canvasRender)
    {
        const auto position = canvasRender.position;
        const auto size = canvasRender.size;

        StageRender
        (
            canvasRender.canvas.Data<CanvasDataImplementation>()->Texture(),
            DefaultTexturedImageViewShader(),
            position.x,
            position.y,
            position.z,
            AxisAlignedBox2D{ 0.0f, 0.0f, 1.0f, 1.0f },
            Size2D{ size.width, size.height },
            Position2D{ size.width / 2, size.height / 2 },
            Scalers2D{ 1.0f, 1.0f },
            Angle(),
            Color(255, 255, 255, 255)
        );
    }

    void Renderer::StageRender(const LineRender& lineRender)
    {
        StageRender(lineRender.from, lineRender.to, lineRender.z, lineRender.width, lineRender.color);
    }

    void Renderer::RenderStaged(const ScreenSize& screenSize, const Color& backgroundColor)
    {
        PushThroughPipeline(screenSize, backgroundColor);
    }

    void Renderer::RenderStaged(const SurfaceData& surface, const Color& backgroundColor)
    {
        LPDIRECT3DSURFACE9 previousRenderSurface;
        device->GetRenderTarget(0, &previousRenderSurface);
        device->SetRenderTarget(0, dynamic_cast<const SurfaceDataImplementation&>(surface).BackBuffer());

        PushThroughPipeline(surface.Size(), backgroundColor);

        device->SetRenderTarget(0, previousRenderSurface);
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

        if (defaultTexturedImageViewShader)
            defaultTexturedImageViewShader->FileDataAs<ShaderAssetDataImplementation>()->Effect()->OnLostDevice();

        if (lineInterface)
        {
            LogIfError(
                lineInterface->OnLostDevice(),
                []() { return Logging::Log(
                    "The DirectX line interface was not able to be released.",
                    Logging::Severity::SevereError); });
        }
    }

    void Renderer::OnResetDevice()
    {
        InitializeBuffers();
        if (defaultTexturedImageViewShader)
            defaultTexturedImageViewShader->FileDataAs<ShaderAssetDataImplementation>()->Effect()->OnResetDevice();

        if (lineInterface)
        {
            LogIfError(
                lineInterface->OnResetDevice(),
                []() { return Logging::Log(
                    "The DirectX line interface was not able to be reset.",
                    Logging::Severity::SevereError); });
        }
    }

    Arca::Index<Asset::ShaderAsset> Renderer::DefaultTexturedImageViewShader() const
    {
        return defaultTexturedImageViewShader;
    }

    Renderer::Vertex::Vertex(const D3DXVECTOR2& position, D3DCOLOR color, FLOAT u, FLOAT v) :
        position(position), color(color), u(u), v(v)
    {}

    Renderer::StagedObject::StagedObject
    (
        LPDIRECT3DTEXTURE9 tex,
        const Asset::ShaderAsset* shader,
        float x,
        float y,
        const AxisAlignedBox2D& imageBounds,
        const Size2D& size,
        const Position2D& center,
        const Scalers2D& scalers,
        const Angle& rotation,
        const Color& color
    ) :
        vertices
        {
            Vertex
            (
                D3DXVECTOR2(0.0f, 0.0f),
                ToDirectXColor(color),
                imageBounds.Left(),
                imageBounds.Top()
            ),
            Vertex
            (
                D3DXVECTOR2(size.width, 0.0f),
                ToDirectXColor(color),
                imageBounds.Right(),
                imageBounds.Top()
            ),
            Vertex
            (
                D3DXVECTOR2(0.0f, size.height),
                ToDirectXColor(color),
                imageBounds.Left(),
                imageBounds.Bottom()
            ),
            Vertex
            (
                D3DXVECTOR2(size.width, size.height),
                ToDirectXColor(color),
                imageBounds.Right(),
                imageBounds.Bottom()
            )
        },
        tex(tex),
        shader(shader)
    {
        SetupQuad(x, y, center, scalers, rotation.As<Radians>());
    }

    Renderer::StagedObject& Renderer::StagedObject::operator=(StagedObject&& arg) noexcept
    {
        vertices = std::move(arg.vertices);
        indices = std::move(arg.indices);
        primCount = arg.primCount;
        tex = arg.tex;
        shader = arg.shader;
        return *this;
    }

    void Renderer::StagedObject::SetupQuad
    (
        float x,
        float y,
        const Position2D& center,
        const Scalers2D& scalers,
        float rotation
    ) {
        // Setup matrix and center
        D3DXVECTOR2 transformedCenter;
        D3DXMATRIX matrix;
        {
            D3DXVECTOR2 centerVector(center.x, center.y);
            D3DXVECTOR2 positionVector(x, y);
            D3DXVECTOR2 scalingVector(scalers.x, scalers.y);

            D3DXMatrixIdentity(&matrix);
            D3DXMatrixTransformation2D(
                &matrix,
                nullptr,
                0.0f,
                &scalingVector,
                &centerVector,
                rotation,
                &positionVector);

            D3DXVec2TransformCoord(&transformedCenter, &centerVector, &matrix);
        }

        auto vertex = vertices.data();
        for (auto i = 0; i < 4; ++i)
        {
            SetupVertex(*vertex, matrix, vertex->position, transformedCenter);
            ++vertex;
        }

        // Indices
        // 0, 1, 2
        // 1, 3, 2

        indices.resize(6);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 3;
        indices[5] = 2;

        primCount = 2;
    }

    void Renderer::StagedObject::SetupRegularPolygon
    (
        float radius,
        unsigned int polyCount,
        float x,
        float y,
        const Scalers2D& scalers,
        float rotation,
        const Color& color
    ) {
        const auto angle = FULL_CIRCLE_RADIANS<float> / polyCount;
        const auto useRadius = radius / std::cos(angle / 2);

        // Setup matrix and center
        D3DXVECTOR2 transformedCenter;
        D3DXMATRIX matrix;
        {
            D3DXVECTOR2 centerVector(useRadius, useRadius);
            D3DXVECTOR2 positionVector(x - useRadius, y - useRadius);
            D3DXVECTOR2 scalingVector(scalers.x, scalers.y);

            D3DXMatrixIdentity(&matrix);
            D3DXMatrixTransformation2D(&matrix, &centerVector, 0.0f, &scalingVector, &centerVector, rotation, &positionVector);

            D3DXVec2TransformCoord(&transformedCenter, &centerVector, &matrix);
        }

        // Vertices and indices
        {
            const auto setIndex = [this](size_t currentPoly)
            {
                const auto startIndex = currentPoly * 3;
                indices[startIndex] = static_cast<Index>(currentPoly + 1);
                indices[startIndex + 1] = static_cast<Index>(currentPoly + 2);
                indices[startIndex + 2] = static_cast<Index>(0);
            };

            float currentAngle = FULL_CIRCLE_RADIANS<float> / 2.0f;
            vertices.resize(polyCount + 1);
            indices.resize(polyCount * 3);
            SetupVertex(vertices[0], matrix, D3DXVECTOR2(useRadius, useRadius), transformedCenter, color);

            for (size_t loopPoly = 0; loopPoly != polyCount; ++loopPoly)
            {
                auto vector = D3DXVECTOR2(
                    useRadius + (std::cos(currentAngle) * useRadius),
                    useRadius + (std::sin(currentAngle) * useRadius));

                SetupVertex(vertices[loopPoly + 1], matrix, vector, transformedCenter, color);
                setIndex(loopPoly);
                currentAngle += angle;
            }

            // Fix the second-to-last index
            indices[indices.size() - 2] = 1;
        }

        primCount = polyCount;
    }

    void Renderer::StagedObject::SetupVertex
    (
        Vertex& vertex,
        const D3DXMATRIX& matrix,
        const D3DXVECTOR2& position,
        const D3DXVECTOR2& center
    ) {
        D3DXVec2TransformCoord(&vertex.position, &position, &matrix);

        vertex.center = center;
    }

    void Renderer::StagedObject::SetupVertex
    (
        Vertex& vertex,
        const D3DXMATRIX& matrix,
        const D3DXVECTOR2& position,
        const D3DXVECTOR2& center,
        const Color& color
    ) {
        D3DXVec2TransformCoord(&vertex.position, &position, &matrix);
        vertex.center = center;
        vertex.color = ToDirectXColor(color);
    }

    Renderer::StagedLine::StagedLine(const Position2D& from, const Position2D& to, float width, const Color& color) :
        points
        {
            D3DXVECTOR2(from.x, from.y),
            D3DXVECTOR2(to.x, to.y)
        },
        width(width),
        color(ToDirectXColor(color))
    {}

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
        device->CreateVertexBuffer
        (
            maxVertexSprint * sizeof(Vertex),
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            0,
            D3DPOOL_DEFAULT,
            &vertexBuffer,
            nullptr
        );
        device->CreateIndexBuffer
        (
            maxIndexSprint * sizeof(Index),
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16, D3DPOOL_DEFAULT,
            &indexBuffer,
            nullptr
        );

        D3DVERTEXELEMENT9 vertexElements[] =
        {
            { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1 },
            { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };

        device->CreateVertexDeclaration(vertexElements, &vertexDeclaration);
    }

    void Renderer::StageRender
    (
        LPDIRECT3DTEXTURE9 tex,
        const Asset::ShaderAsset* shader,
        float x,
        float y,
        float z,
        const AxisAlignedBox2D& imageBounds,
        const Size2D& size,
        const Position2D& center,
        const Scalers2D& scalers,
        const Angle& rotation,
        const Color& color
    ) {
        const auto layer = LayerWithZ(z);
        layer->objects.emplace_back(tex, shader, x, y, imageBounds, size, center, scalers, rotation, color);
    }

    void Renderer::StageRender
    (
        const Position2D& from,
        const Position2D& to,
        Position2D::Value z,
        float width,
        const Color& color
    ) {
        const auto layer = LayerWithZ(z);
        layer->lines.emplace_back(from, to, width, color);
    }

    void Renderer::PushThroughPipeline(const ScreenSize& screenSize, const Color& backgroundColor)
    {
        D3DXMATRIX projection;

        const auto size(screenSize);
        D3DXMatrixOrthoOffCenterLH
        (
            &projection,
            0,
            static_cast<FLOAT>(size.width),
            static_cast<FLOAT>(size.height),
            0,
            0.0f,
            1.0f
        );

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
                SetTexture(objects.begin()->tex);

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
                Logging::Severity::SevereError); });
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
                const auto leftTex = left.tex;
                const auto rightTex = right.tex;

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

        const auto loopTexture = staged.tex;
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

        lineInterface->Draw(staged.points, 2, staged.color);
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
        Asset::ShaderAssetData::PassCount passCountCurrent = 0;

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
                Logging::Severity::SevereError); });
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