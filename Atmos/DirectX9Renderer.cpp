#include "DirectX9Renderer.h"

#include "DirectX9GraphicsManager.h"
#include "DirectX9Data.h"
#include "DirectX9Utilities.h"

#include "Camera.h"
#include "FileManagerProvider.h"
#include "MathUtility.h"

namespace Atmos::Render::DirectX9
{
    Renderer::Renderer(Arca::Reliquary& reliquary) : reliquary(&reliquary)
    {}

    Renderer::~Renderer()
    {
        vertexBuffer->Release();
        indexBuffer->Release();
        vertexDecl->Release();

        lineInterface->Release();
    }

    void Renderer::Initialize(GraphicsManager& handler, LPDIRECT3DDEVICE9 device)
    {
        this->device = device;
        InitializeBuffers();

        auto texturedSpritePath = File::manager->ExePath() + "Shaders\\TexturedSprite.fx";

        auto shaderData = handler.CreateShaderData(texturedSpritePath);
        defaultTexturedImageViewShader = reliquary->Create<Asset::ShaderAsset>(
            texturedSpritePath,
            std::move(shaderData));

        D3DXCreateLine(device, &lineInterface);
    }

    void Renderer::StartObjects(ObjectsSize size)
    {
        if (size != 0)
            objects.reserve(size);
    }

    void Renderer::StopObjects()
    {
        objects.clear();
    }

    void Renderer::StartLines()
    {
        lineInterface->Begin();
    }

    void Renderer::StopLines()
    {
        lineInterface->End();
    }

    void Renderer::Flush(const D3DXMATRIX& projection)
    {
        if (objects.empty())
            return;

        std::sort(
            objects.begin(),
            objects.end(),
            [](const Object3D& left, const Object3D& right)
            {
                const auto leftZ = left.z;
                const auto rightZ = right.z;
                if (leftZ == rightZ)
                {
                    const auto leftTex = left.tex;
                    const auto rightTex = right.tex;

                    return leftTex == rightTex
                        ? left.shader < right.shader
                        : leftTex < rightTex;
                }
                else
                    return leftZ < rightZ;
            });

        auto camera = Arca::GlobalIndex<Camera>(*reliquary);

        device->SetVertexDeclaration(vertexDecl);
        device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
        device->SetIndices(indexBuffer);

        LPDIRECT3DTEXTURE9 focusedTex = nullptr;
        Asset::ShaderAsset* focusedShader;
        const ShaderAssetDataImplementation* focusedShaderImpl = nullptr;

        const auto setTexture = [&focusedTex, &focusedShaderImpl](LPDIRECT3DTEXTURE9 set)
        {
            focusedTex = set;
            if (focusedTex && focusedShaderImpl)
                focusedShaderImpl->effect->SetTexture("g_Texture", focusedTex);
        };

        const auto setShader =
            [&focusedTex, &focusedShader, &focusedShaderImpl, &camera, &projection](Asset::ShaderAsset* set)
        {
            focusedShader = set;
            focusedShaderImpl = focusedShader->DataAs<ShaderAssetDataImplementation>();
            focusedShaderImpl->effect->SetMatrix("g_Projection", &projection);

            const auto cameraSize = camera->Size();
            FLOAT screenSize[2] =
            {
                static_cast<FLOAT>(cameraSize.width),
                static_cast<FLOAT>(cameraSize.height)
            };
            focusedShaderImpl->effect->SetFloatArray("g_ScreenSize", screenSize, 2);

            if (focusedTex && focusedShaderImpl)
                focusedShaderImpl->effect->SetTexture("g_Texture", focusedTex);
        };

        setShader(objects.begin()->shader);
        setTexture(objects.begin()->tex);

        void* vertexData = nullptr;
        void* indexData = nullptr;
        vertexBuffer->Lock(0, 0, &vertexData, D3DLOCK_DISCARD);
        indexBuffer->Lock(0, 0, &indexData, D3DLOCK_DISCARD);

        BufferSize vertexCount = 0;
        BufferSize indexCount = 0;
        BufferSize primitiveCount = 0;

        const auto drawPrimitives =
            [this, &focusedShaderImpl, &vertexCount, &primitiveCount, &vertexData, &indexData, &indexCount]
        (
            bool endOfDraw)
        {
            vertexBuffer->Unlock();
            indexBuffer->Unlock();

            const auto passCount = focusedShaderImpl->Begin();
            Asset::ShaderAssetData::PassCount passCountCurrent = 0;

            do
            {
                focusedShaderImpl->BeginNextPass(passCountCurrent);
                ++passCountCurrent;

                device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, primitiveCount);

                focusedShaderImpl->EndPass();
            } while (passCountCurrent < passCount);

            focusedShaderImpl->End();

            if (!endOfDraw)
            {
                vertexBuffer->Lock(0, 0, &vertexData, D3DLOCK_DISCARD);
                indexBuffer->Lock(0, 0, &indexData, D3DLOCK_DISCARD);
                vertexCount = 0;
                indexCount = 0;
                primitiveCount = 0;
            }
        };

        for (auto& object : objects)
        {
            const auto loopVerticesSize = object.vertices.size();
            const auto loopIndicesSize = object.indices.size();

            const auto loopTexture = object.tex;
            const auto loopShader = object.shader;

            // Check if the sprint is done
            if (vertexCount + loopVerticesSize >= maxVertexSprint || indexCount + loopIndicesSize >= maxIndexSprint)
                drawPrimitives(false);

            // Check if the texture AND shader is different
            if (focusedTex != loopTexture && focusedShader != loopShader)
            {
                if (vertexCount > 0)
                    drawPrimitives(false);

                setTexture(loopTexture);
                setShader(loopShader);
            }
            // Check if the texture is different
            else if (focusedTex != loopTexture)
            {
                if (vertexCount > 0)
                    drawPrimitives(false);

                setTexture(loopTexture);
            }
            // Check if the shader is different
            else if (focusedShader != loopShader)
            {
                if (vertexCount > 0)
                    drawPrimitives(false);

                setTexture(focusedTex);
                setShader(loopShader);
            }

            // Vertices
            memcpy(
                &reinterpret_cast<Vertex*>(vertexData)[vertexCount],
                object.vertices.data(),
                loopVerticesSize * sizeof(Vertex));

            // Indices
            for (size_t indexLoop = 0; indexLoop != loopIndicesSize; ++indexLoop)
                object.indices[indexLoop] = vertexCount + object.indices[indexLoop];
            memcpy(
                &reinterpret_cast<Index*>(indexData)[indexCount],
                object.indices.data(),
                loopIndicesSize * sizeof(Index));

            vertexCount += static_cast<BufferSize>(loopVerticesSize);
            indexCount += static_cast<BufferSize>(loopIndicesSize);
            primitiveCount += object.primCount;
        }

        vertexBuffer->Unlock();
        indexBuffer->Unlock();

        objects.clear();
        if (vertexCount == 0)
            return;

        drawPrimitives(true);
    }

    void Renderer::Draw
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
    ) {
        objects.emplace_back(tex, shader, X, Y, Z, imageBounds, size, center, scalers, rotation, color);
    }

    void Renderer::Draw
    (
        const Position2D& from,
        const Position2D& to,
        float width,
        const Color& color
    ) {
        if (width != lineInterface->GetWidth())
        {
            StopLines();
            lineInterface->SetWidth(width);
            StartLines();
        }

        D3DXVECTOR2 points[] =
        {
            D3DXVECTOR2(from.x, from.y),
            D3DXVECTOR2(to.x, to.y)
        };
        lineInterface->Draw(points, 2, ColorToD3D(color));
    }

    void Renderer::OnLostDevice()
    {
        vertexBuffer->Release();
        indexBuffer->Release();
        defaultTexturedImageViewShader->DataAs<ShaderAssetDataImplementation>()->effect->OnLostDevice();

        LogIfError(
            lineInterface->OnLostDevice(),
            "The DirectX line interface was not able to be released.",
            Logging::Severity::SevereError);
    }

    void Renderer::OnResetDevice()
    {
        InitializeBuffers();
        defaultTexturedImageViewShader->DataAs<ShaderAssetDataImplementation>()->effect->OnResetDevice();

        LogIfError(
            lineInterface->OnResetDevice(),
            "The DirectX line interface was not able to be reset.",
            Logging::Severity::SevereError);
    }

    Asset::ShaderAsset* Renderer::DefaultTexturedImageViewShader()
    {
        return defaultTexturedImageViewShader;
    }

    Renderer::Vertex::Vertex() :
        position(0, 0), color(0), u(0.0f), v(0.0f)
    {}

    Renderer::Vertex::Vertex(D3DXVECTOR2&& position, D3DCOLOR color, FLOAT u, FLOAT v) :
        position(position), center(0.0f, 0.0f), color(color), u(u), v(v)
    {}

    Renderer::Object3D::Object3D
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
    ) :
        vertices
        {
            Vertex
            (
                D3DXVECTOR2(0.0f, 0.0f),
                ColorToD3D(color),
                imageBounds.Left(),
                imageBounds.Top()
            ),
            Vertex
            (
                D3DXVECTOR2(size.width, 0.0f),
                ColorToD3D(color),
                imageBounds.Right(),
                imageBounds.Top()
            ),
            Vertex
            (
                D3DXVECTOR2(0.0f, size.height),
                ColorToD3D(color),
                imageBounds.Left(),
                imageBounds.Bottom()
            ),
            Vertex
            (
                D3DXVECTOR2(size.width, size.height),
                ColorToD3D(color),
                imageBounds.Right(),
                imageBounds.Bottom()
            )
        },
        z(Z),
        tex(tex),
        shader(shader)
    {
        SetupQuad(X, Y, center, scalers, rotation.As<Radians>());
    }

    Renderer::Object3D::Object3D(Object3D&& arg) :
        vertices(std::move(arg.vertices)), indices(std::move(arg.indices)), primCount(arg.primCount),
        z(arg.z), tex(arg.tex), shader(arg.shader)
    {}

    Renderer::Object3D& Renderer::Object3D::operator=(
        Object3D&& arg)
    {
        vertices = std::move(arg.vertices);
        indices = std::move(arg.indices);
        primCount = arg.primCount;
        z = arg.z;
        tex = arg.tex;
        shader = arg.shader;
        return *this;
    }

    void Renderer::Object3D::SetupQuad
    (
        float X,
        float Y,
        const Position2D& center,
        const Scalers2D& scalers,
        float rotation
    ) {
        // Setup matrix and center
        D3DXVECTOR2 transformedCenter;
        D3DXMATRIX matrix;
        {
            D3DXVECTOR2 centerVector(center.x, center.y);
            D3DXVECTOR2 positionVector(X, Y);
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
        for (auto x = 0; x < 4; ++x)
        {
            SetupVertexCommon(*vertex, matrix, vertex->position, transformedCenter);
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

    void Renderer::Object3D::SetupRegularPolygon
    (
        float radius,
        unsigned int polyCount,
        float X,
        float Y,
        const Scalers2D& scalers,
        float rotation,
        const Color& color
    ) {
        const float angle = FULL_CIRCLE_RADIANS<float> / polyCount;
        const float useRadius = radius / std::cos(angle / 2);

        // Setup matrix and center
        D3DXVECTOR2 transformedCenter;
        D3DXMATRIX matrix;
        {
            D3DXVECTOR2 centerVector(useRadius, useRadius);
            D3DXVECTOR2 positionVector(X - useRadius, Y - useRadius);
            D3DXVECTOR2 scalingVector(scalers.x, scalers.y);

            D3DXMatrixIdentity(&matrix);
            D3DXMatrixTransformation2D(&matrix, &centerVector, 0.0f, &scalingVector, &centerVector, rotation, &positionVector);

            D3DXVec2TransformCoord(&transformedCenter, &centerVector, &matrix);
        }

        // Vertices and indicies
        {
            auto setIndex = [&](size_t currentPoly)
            {
                const size_t startIndex = currentPoly * 3;
                indices[startIndex] = static_cast<Index>(currentPoly + 1);
                indices[startIndex + 1] = static_cast<Index>(currentPoly + 2);
                indices[startIndex + 2] = static_cast<Index>(0);
            };

            float currentAngle = FULL_CIRCLE_RADIANS<float> / 2.0f;
            vertices.resize(polyCount + 1);
            indices.resize(polyCount * 3);
            SetupVertexCommon(vertices[0], matrix, D3DXVECTOR2(useRadius, useRadius), transformedCenter, color);

            for (size_t loopPoly = 0; loopPoly != polyCount; ++loopPoly)
            {
                auto vector = D3DXVECTOR2(
                    useRadius + (std::cos(currentAngle) * useRadius),
                    useRadius + (std::sin(currentAngle) * useRadius));

                SetupVertexCommon(vertices[loopPoly + 1], matrix, vector, transformedCenter, color);
                setIndex(loopPoly);
                currentAngle += angle;
            }

            // Fix the second-to-last index
            indices[indices.size() - 2] = 1;
        }

        primCount = polyCount;
    }

    void Renderer::Object3D::SetupVertexCommon
    (
        Vertex& vertex,
        const D3DXMATRIX& matrix,
        const D3DXVECTOR2& position,
        const D3DXVECTOR2& center
    ) {
        D3DXVec2TransformCoord(&vertex.position, &position, &matrix);

        vertex.center = center;
    }

    void Renderer::Object3D::SetupVertexCommon
    (
        Vertex& vertex,
        const D3DXMATRIX& matrix,
        const D3DXVECTOR2& position,
        const D3DXVECTOR2& center,
        const Color& color
    ) {
        D3DXVec2TransformCoord(&vertex.position, &position, &matrix);
        vertex.center = center;
        vertex.color = ColorToD3D(color);
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

        D3DVERTEXELEMENT9 vElements[] =
        {
            { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1 },
            { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };

        device->CreateVertexDeclaration(vElements, &vertexDecl);
    }

    void Renderer::LogIfError(
        HRESULT hr,
        const String& message,
        Logging::Severity severity,
        const std::optional<Logging::Details>& details) const
    {
        DirectX9::LogIfError(hr, *reliquary, message, severity, details);
    }
}
