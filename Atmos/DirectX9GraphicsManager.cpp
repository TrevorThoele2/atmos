#include <array>
#include "DirectX9GraphicsManager.h"

#include <Windows.h>
#include <stdio.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
#include <DxErr.h>
#pragma comment(lib, "dxerr.lib")

#include "ImageAsset.h"

#include "CanvasView.h"
#include "Line.h"
#include "Camera.h"
#include "FileManagerProvider.h"

#include "TimeSettings.h"

#include "Flags.h"
#include "SimpleFile.h"

#include "Log.h"

namespace Atmos::Render
{
    D3DCOLOR ColorToD3D(const Color &col)
    {
        return D3DCOLOR_ARGB(col.alpha, col.red, col.green, col.blue);
    }

    void LogError
    (
        HRESULT hr,
        Arca::Reliquary& reliquary,
        const String& message,
        Log::Severity severity,
        const std::optional<Log::NameValuePairs>& nameValuePairs = {}
    ) {
        auto string = message + "\n" + DXGetErrorDescriptionA(hr);

        if (!nameValuePairs)
            reliquary.Raise<Log::Log>(string, severity);
        else
            reliquary.Raise<Log::Log>(string, severity, *nameValuePairs);
    }

    class ImageAssetDataImplementation final : public Asset::ImageAssetData
    {
    public:
        LPDIRECT3DTEXTURE9 tex;

        explicit ImageAssetDataImplementation(LPDIRECT3DTEXTURE9 tex) : tex(tex)
        {}

        ~ImageAssetDataImplementation()
        {
            tex->Release();
        }

        [[nodiscard]] std::unique_ptr<ImageAssetData> Clone() const override
        {
            return std::make_unique<ImageAssetDataImplementation>(*this);
        }
    };

    class ShaderAssetDataImplementation : public Asset::ShaderAssetData
    {
    public:
        LPD3DXEFFECT effect;

        explicit ShaderAssetDataImplementation(LPD3DXEFFECT effect) : effect(effect)
        {}

        ~ShaderAssetDataImplementation()
        {
            effect->Release();
        }

        [[nodiscard]] std::unique_ptr<ShaderAssetData> Clone() const override
        {
            return std::make_unique<ShaderAssetDataImplementation>(*this);
        }

        void Reset() override
        {
            effect->OnResetDevice();
        }

        void Release() override
        {
            effect->OnLostDevice();
        }

        PassCount Begin() const override
        {
            {
                D3DXHANDLE technique;
                effect->FindNextValidTechnique(nullptr, &technique);
                effect->SetTechnique(technique);
            }

            PassCount pass;
            effect->Begin(&pass, 0);
            return pass;
        }

        void End() const override
        {
            effect->End();
        }

        void BeginNextPass(PassCount pass) const override
        {
            effect->BeginPass(pass);
        }

        void EndPass() const override
        {
            effect->EndPass();
        }
    };

    class SurfaceData final : public Surface::Data
    {
    public:
        SurfaceData
        (
            DirectX9GraphicsManager& owner,
            Arca::Reliquary& reliquary,
            LPDIRECT3DSURFACE9 backBuffer
        ) :
            owner(&owner), swapChain(nullptr), backBuffer(backBuffer)
        {}

        SurfaceData
        (
            DirectX9GraphicsManager& owner,
            Arca::Reliquary& reliquary,
            LPDIRECT3DSWAPCHAIN9 swapChain,
            LPDIRECT3DSURFACE9 backBuffer
        ) :
            owner(&owner), swapChain(swapChain), backBuffer(backBuffer)
        {
            swapChain->GetPresentParameters(&presentationParameters);
        }

        ~SurfaceData()
        {
            Release();
        }

        void SetAsRenderTarget() override
        {
            owner->GetDevice()->SetRenderTarget(0, backBuffer);
        }

        void Present() override
        {
            const auto hr = swapChain->Present(nullptr, nullptr, nullptr, nullptr, 0);
            if (hr != S_OK)
                LogError(hr, *reliquary, "A swap chain failed when presenting.", Log::Severity::SevereError);
        }

        void Reset() override
        {
            presentationParameters.BackBufferWidth = 0;
            presentationParameters.BackBufferHeight = 0;

            auto hr = owner->GetDevice()->CreateAdditionalSwapChain
            (
                &presentationParameters,
                &swapChain
            );

            if (hr != S_OK)
                LogError
                (
                    hr,
                    *reliquary,
                    "A DirectX device could not create an additional swap chain.",
                    Log::Severity::SevereError
                );

            hr = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
            if (hr != S_OK)
                LogError
                (
                    hr,
                    *reliquary,
                    "A DirectX device could not retrieve the back buffer from a swap chain.",
                    Log::Severity::SevereError
                );
        }

        void Release() override
        {
            backBuffer->Release();
            backBuffer = nullptr;

            swapChain->Release();
            swapChain = nullptr;
        }

        [[nodiscard]] ScreenDimensions Size() override
        {
            RECT rect;
            GetClientRect(presentationParameters.hDeviceWindow, &rect);
            return ScreenDimensions(rect.right, rect.bottom);
        }

        [[nodiscard]] const D3DPRESENT_PARAMETERS& GetPresentationParameters() const
        {
            return presentationParameters;
        }
    private:
        DirectX9GraphicsManager* owner;
        Arca::Reliquary* reliquary;
    private:
        D3DPRESENT_PARAMETERS presentationParameters;
        LPDIRECT3DSWAPCHAIN9 swapChain;
        LPDIRECT3DSURFACE9 backBuffer;
    };

    class CanvasData final : public Canvas::Data
    {
    public:
        DirectX9GraphicsManager* owner;
        Arca::Reliquary* reliquary;

        LPDIRECT3DTEXTURE9 tex;
        LPDIRECT3DSURFACE9 backBuffer;

        D3DLOCKED_RECT lockedRect;

        CanvasData(DirectX9GraphicsManager& owner, Arca::Reliquary& reliquary, LPDIRECT3DTEXTURE9 tex) :
            owner(&owner), reliquary(&reliquary), tex(tex)
        {
            this->tex->GetSurfaceLevel(0, &backBuffer);
        }

        ~CanvasData()
        {
            tex->Release();
        }

        void StartPainting() override
        {
            const auto hr = backBuffer->LockRect(&lockedRect, nullptr, 0);
            if (hr != S_OK)
                LogError(hr, *reliquary, "A canvas' back buffer could not be locked.", Log::Severity::LowError);
        }

        void StopPainting() override
        {
            const auto hr = backBuffer->UnlockRect();
            if (hr != S_OK)
                LogError(hr, *reliquary, "A canvas' back buffer could not be unlocked.", Log::Severity::LowError);
        }

        void PaintPixel(
            const Canvas::Position& position,
            const Color& color,
            Canvas::DimensionValue height) override
        {
            const auto row =
                reinterpret_cast<DWORD*>(reinterpret_cast<char*>(lockedRect.pBits)
                    + lockedRect.Pitch * position.y);
            row[position.x] = ColorToD3D(color);
        }

        void Clear(const Color &color) override
        {
            std::memset(lockedRect.pBits, ColorToD3D(color), lockedRect.Pitch * Owner()->Height());
        }

        void Release() override
        {
            tex->Release();
        }

        void Reset(Canvas::DimensionValue width, Canvas::DimensionValue height) override
        {
            D3DXCreateTexture
            (
                owner->GetDevice(),
                width,
                height,
                D3DX_DEFAULT,
                0,
                D3DFMT_X8R8G8B8,
                D3DPOOL_MANAGED,
                &tex
            );

            tex->GetSurfaceLevel(0, &backBuffer);
        }
    };

    class DirectX9GraphicsManager::Renderer2D
    {
    public:
        using ObjectsSize = size_t;
    public:
        Renderer2D(Arca::Reliquary& reliquary);

        ~Renderer2D();

        void Initialize(DirectX9GraphicsManager& handler, LPDIRECT3DDEVICE9 device);

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
            const Scalings& scalings,
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
            Vertex() :
                position(0, 0), color(0), u(0.0f), v(0.0f)
            {}

            Vertex(D3DXVECTOR2&& position, D3DCOLOR color, FLOAT u, FLOAT v) :
                position(std::move(position)), center(0.0f, 0.0f), color(color), u(u), v(v)
            {}
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
                const Scalings& scalings,
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
                const Scalings& scaling,
                float rotation
            );
            void SetupRegularPolygon
            (
                float radius,
                unsigned int polyCount,
                float X,
                float Y,
                const Scalings& scalings,
                float rotation,
                const Color& color
            );

            static void SetupVertexCommon
            (
                Vertex&vertex,
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
        Arca::Ptr<Asset::ShaderAsset> defaultTexturedImageViewShader;

        using Objects = std::vector<Object3D>;
        Objects objects;
    private:
        Arca::Reliquary* reliquary;
    private:
        void InitializeBuffers();
    };

    DirectX9GraphicsManager::Renderer2D::Renderer2D(Arca::Reliquary& reliquary) : reliquary(&reliquary)
    {}

    DirectX9GraphicsManager::Renderer2D::~Renderer2D()
    {
        vertexBuffer->Release();
        indexBuffer->Release();
        vertexDecl->Release();
    }

    void DirectX9GraphicsManager::Renderer2D::Initialize(DirectX9GraphicsManager& handler, LPDIRECT3DDEVICE9 device)
    {
        this->device = device;
        InitializeBuffers();

        auto path = File::manager->ExePath() + "Shaders\\TexturedSprite.fx";

        auto shaderData = handler.CreateShaderData(path);
        defaultTexturedImageViewShader = reliquary->Create<Asset::ShaderAsset>(path, std::move(shaderData));
    }

    void DirectX9GraphicsManager::Renderer2D::Start(ObjectsSize size)
    {
        if (size != 0)
            objects.reserve(size);
    }

    void DirectX9GraphicsManager::Renderer2D::Stop(const D3DXMATRIX* projection)
    {
        if (projection)
            Flush(*projection);

        objects.clear();
    }

    void DirectX9GraphicsManager::Renderer2D::Flush(const D3DXMATRIX& projection)
    {
        if (objects.empty())
            return;

        // Sort objects
        {
            const auto sorter = [](const Object3D &left, const Object3D &right)
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
            };

            std::sort(objects.begin(), objects.end(), sorter);
        }

        auto camera = reliquary->Find<Camera>();

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

            auto cameraSize = camera->Size();
            FLOAT screenSize[2] = { static_cast<FLOAT>(cameraSize.width), static_cast<FLOAT>(cameraSize.height) };
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

            const Asset::ShaderAssetData::PassCount passCount = focusedShaderImpl->Begin();
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

        for (auto loop = objects.begin(); loop != objects.end(); ++loop)
        {
            const auto loopVerticesSize = loop->vertices.size();
            const auto loopIndicesSize = loop->indices.size();

            const auto loopTexture = loop->tex;
            const auto loopShader = loop->shader;

            // Check if the sprint is done
            if (vertexCount + loopVerticesSize >= maxVertexSprint || indexCount + loopIndicesSize >= maxIndexSprint)
                drawPrimitives(false);

            // Check if the texture AND shader is different
            if(focusedTex != loopTexture && focusedShader != loopShader)
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
                loop->vertices.data(),
                loopVerticesSize * sizeof(Vertex));
            
            // Indices
            for (size_t indexLoop = 0; indexLoop != loopIndicesSize; ++indexLoop)
                loop->indices[indexLoop] = vertexCount + loop->indices[indexLoop];
            memcpy(
                &reinterpret_cast<Index*>(indexData)[indexCount],
                loop->indices.data(),
                loopIndicesSize * sizeof(Index));

            vertexCount += static_cast<BufferSize>(loopVerticesSize);
            indexCount += static_cast<BufferSize>(loopIndicesSize);
            primitiveCount += loop->primCount;
        }

        vertexBuffer->Unlock();
        indexBuffer->Unlock();

        objects.clear();
        if (vertexCount == 0)
            return;

        drawPrimitives(true);
    }

    void DirectX9GraphicsManager::Renderer2D::Draw
    (
        LPDIRECT3DTEXTURE9 tex,
        Asset::ShaderAsset* shader,
        float X,
        float Y,
        float Z,
        const AxisAlignedBox2D& imageBounds,
        const Size2D& size,
        const Position2D& center,
        const Scalings& scalings,
        const Angle& rotation,
        const Color& color
    ) {
        objects.push_back(Object3D(tex, shader, X, Y, Z, imageBounds, size, center, scalings, rotation, color));
    }

    void DirectX9GraphicsManager::Renderer2D::OnLostDevice()
    {
        vertexBuffer->Release();
        indexBuffer->Release();
        defaultTexturedImageViewShader->DataAs<ShaderAssetDataImplementation>()->effect->OnLostDevice();
    }

    void DirectX9GraphicsManager::Renderer2D::OnResetDevice()
    {
        InitializeBuffers();
        defaultTexturedImageViewShader->DataAs<ShaderAssetDataImplementation>()->effect->OnResetDevice();
    }

    Asset::ShaderAsset* DirectX9GraphicsManager::Renderer2D::DefaultTexturedImageViewShader()
    {
        return defaultTexturedImageViewShader;
    }

    DirectX9GraphicsManager::Renderer2D::Object3D::Object3D
    (
        LPDIRECT3DTEXTURE9 tex,
        Asset::ShaderAsset* shader,
        float X,
        float Y,
        float Z,
        const AxisAlignedBox2D& imageBounds,
        const Size2D& size,
        const Position2D& center,
        const Scalings& scalings,
        const Angle& rotation,
        const Color& color
    ) :
        tex(tex), z(Z), shader(shader),
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
    }
    {
        SetupQuad(X, Y, center, scalings, rotation.As<Radians>());
    }

    DirectX9GraphicsManager::Renderer2D::Object3D::Object3D(Object3D&& arg) :
        vertices(std::move(arg.vertices)), indices(std::move(arg.indices)), primCount(std::move(arg.primCount)),
        z(std::move(arg.z)), tex(std::move(arg.tex)), shader(std::move(arg.shader))
    {}

    DirectX9GraphicsManager::Renderer2D::Object3D& DirectX9GraphicsManager::Renderer2D::Object3D::operator=(
        Object3D&& arg)
    {
        vertices = std::move(arg.vertices);
        indices = std::move(arg.indices);
        primCount = std::move(arg.primCount);
        z = std::move(arg.z);
        tex = std::move(arg.tex);
        shader = std::move(arg.shader);
        return *this;
    }

    void DirectX9GraphicsManager::Renderer2D::Object3D::SetupQuad
    (
        float X,
        float Y,
        const Position2D& center,
        const Scalings& scalings,
        float rotation
    ) {
        // Setup matrix and center
        D3DXVECTOR2 transformedCenter;
        D3DXMATRIX matrix;
        {
            D3DXVECTOR2 centerVector(center.x, center.y);
            D3DXVECTOR2 positionVector(X, Y);
            D3DXVECTOR2 scalingVector(scalings.x, scalings.y);

            D3DXMatrixIdentity(&matrix);
            D3DXMatrixTransformation2D(&matrix, nullptr, 0.0f, &scalingVector, &centerVector, rotation, &positionVector);

            D3DXVec2TransformCoord(&transformedCenter, &centerVector, &matrix);
        }

        Vertex *vertex = vertices.data();
        for (int x = 0; x < 4; ++x)
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

    void DirectX9GraphicsManager::Renderer2D::Object3D::SetupRegularPolygon
    (
        float radius,
        unsigned int polyCount,
        float X,
        float Y,
        const Scalings& scalings,
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
            D3DXVECTOR2 scalingVector(scalings.x, scalings.y);

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

    void DirectX9GraphicsManager::Renderer2D::Object3D::SetupVertexCommon
    (
        Vertex& vertex,
        const D3DXMATRIX& matrix,
        const D3DXVECTOR2& position,
        const D3DXVECTOR2& center
    ) {
        D3DXVec2TransformCoord(&vertex.position, &position, &matrix);

        vertex.center = center;
    }

    void DirectX9GraphicsManager::Renderer2D::Object3D::SetupVertexCommon
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

    void DirectX9GraphicsManager::Renderer2D::InitializeBuffers()
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

    DirectX9GraphicsManager::DirectX9GraphicsManager
    (
        Arca::Reliquary& reliquary,
        HWND hwnd,
        const ScreenDimensions& backbuffer,
        bool fullscreen
    ) :
        GraphicsManager(reliquary), hwnd(hwnd), renderer2D(std::make_unique<Renderer2D>(reliquary))
    {
        ZeroMemory(&presentationParameters, sizeof(presentationParameters));

        SetMainDimensions(backbuffer);
        SetFullscreen(fullscreen);

        // Create the Direct3D interface
        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        presentationParameters.hDeviceWindow = hwnd;
        SetupPresentationParameters();

        d3d->CreateDevice
        (
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hwnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &presentationParameters,
            &device
        );
        renderer2D->Initialize(*this, device);
        D3DXCreateLine(device, &lineInterface);

        SetRenderStates();

        device->GetRenderTarget(0, &mainSurface);
    }

    DirectX9GraphicsManager::~DirectX9GraphicsManager()
    {
        d3d->Release();
        device->Release();
        lineInterface->Release();
    }

    void DirectX9GraphicsManager::SetFullscreen(bool set)
    {
        (set) ? presentationParameters.Windowed = false : presentationParameters.Windowed = true;
    }

    void DirectX9GraphicsManager::ClearTarget(const Flags<Target>& target)
    {
        DWORD pass = 0;
        if (target.Get(Target::Main))
            pass |= D3DCLEAR_TARGET;

        if (target.Get(Target::Stencil))
            pass |= D3DCLEAR_STENCIL;

        device->Clear(0, nullptr, pass, 0, 1.0f, 0);
    }

    void DirectX9GraphicsManager::ClearTarget(const Flags<Target>& target, const Color& color)
    {
        DWORD pass = 0;
        if (target.Get(Target::Main))
            pass |= D3DCLEAR_TARGET;

        if (target.Get(Target::Stencil))
            pass |= D3DCLEAR_STENCIL;

        device->Clear(0, nullptr, pass, ColorToD3D(color), 1.0f, 0);
    }

    void DirectX9GraphicsManager::Flush()
    {
        renderer2D->Flush(projection);
    }

    void DirectX9GraphicsManager::SetRenderState(RenderState state, bool set)
    {
        D3DRENDERSTATETYPE pass;

        if (state == RenderState::Stencil)
            pass = D3DRS_STENCILENABLE;

        DWORD tf;
        (set) ? tf = TRUE : tf = FALSE;

        device->SetRenderState(pass, tf);
    }

    bool DirectX9GraphicsManager::Start()
    {
        HRESULT hr = device->TestCooperativeLevel();

        // Device is lost but can't reset it
        if (hr == D3DERR_DEVICELOST)
            return false;

        // Device is lost, and can reset
        if (hr == D3DERR_DEVICENOTRESET)
            ReconstructAll();

        device->BeginScene();
        return true;
    }

    void DirectX9GraphicsManager::End()
    {
        device->EndScene();
    }

    void DirectX9GraphicsManager::StartSprites(size_t spriteCount)
    {
        renderer2D->Start(spriteCount);
    }

    void DirectX9GraphicsManager::EndSprites()
    {
        renderer2D->Stop(&projection);
    }

    void DirectX9GraphicsManager::StartLines()
    {
        lineInterface->Begin();
    }

    void DirectX9GraphicsManager::EndLines()
    {
        lineInterface->End();
    }

    void DirectX9GraphicsManager::StartStencil()
    {
        device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NEVER);

        device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_REPLACE);
        device->SetRenderState(D3DRS_STENCILREF, 1);
        device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
    }

    void DirectX9GraphicsManager::StopStencil()
    {
        Flush();

        device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

        device->SetRenderState(D3DRS_STENCILREF, 0);
        device->SetRenderState(D3DRS_STENCILWRITEMASK, 0);
    }

    LPDIRECT3DDEVICE9& DirectX9GraphicsManager::GetDevice()
    {
        return device;
    }

    DirectX9GraphicsManager::Scalings::Scalings() : x(0.0f), y(0.0f)
    {}

    DirectX9GraphicsManager::Scalings::Scalings(float x, float y) : x(x), y(y)
    {}

    void DirectX9GraphicsManager::RenderObject
    (
        LPDIRECT3DTEXTURE9 tex,
        Asset::ShaderAsset* shader,
        float X,
        float Y,
        float Z,
        const AxisAlignedBox2D& imageBounds,
        const Size2D& size,
        const Position2D& center,
        const Scalings& scalings,
        const Angle& rotation,
        const Color& color
    ) {
        if (!shader)
            shader = renderer2D->DefaultTexturedImageViewShader();

        renderer2D->Draw(tex, &*shader, X, Y, Z, imageBounds, size, center, scalings, rotation, color);
    }

    void DirectX9GraphicsManager::ReconstructInternals()
    {
        SetupPresentationParameters();

        // Destroy interfaces
        renderer2D->OnLostDevice();

        HRESULT hr = lineInterface->OnLostDevice();
        if (hr != S_OK)
            LogError(hr, *reliquary, "The DirectX line interface was not able to be released.", Log::Severity::SevereError);

        // Reset
        hr = device->Reset(&presentationParameters);
        if (hr == D3DERR_DEVICELOST)
            return;
        else if (hr != S_OK)
            LogError(hr, *reliquary, "The DirectX device failed when resetting.", Log::Severity::SevereError);

        // Recreate interfaces
        renderer2D->OnResetDevice();
        hr = lineInterface->OnResetDevice();
        if (hr != S_OK)
            LogError(hr, *reliquary, "The DirectX line interface was not able to be reset.", Log::Severity::SevereError);

        // Reset render states
        SetRenderStates();
    }

    void DirectX9GraphicsManager::SetMainDimensionsImpl(const ScreenDimensions& dimensions)
    {
        presentationParameters.BackBufferWidth = dimensions.width;
        presentationParameters.BackBufferHeight = dimensions.height;
        SetProjectionMatrix();
    }

    ScreenDimensions DirectX9GraphicsManager::MainDimensionsImpl() const
    {
        return ScreenDimensions(presentationParameters.BackBufferWidth, presentationParameters.BackBufferHeight);
    }

    std::unique_ptr<Asset::ImageAssetData> DirectX9GraphicsManager::CreateImageDataImpl(
        const File::Path& path)
    {
        D3DXIMAGE_INFO info;
        HRESULT hr = D3DXGetImageInfoFromFile(path.c_str(), &info);
        if (hr != S_OK)
        {
            Log::NameValuePairs nameValuePairs{ NameValuePair("File Path", path.GetValue()) };
            LogError(hr, *reliquary, "An image asset could not be created.", Log::Severity::SevereError, nameValuePairs);
        }

        LPDIRECT3DTEXTURE9 tex;
        hr = D3DXCreateTextureFromFileEx
        (
            device,
            path.c_str(),
            info.Width,
            info.Height,
            D3DX_DEFAULT,
            0,
            D3DFMT_A8R8G8B8,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            nullptr,
            nullptr,
            &tex
        );
        if (hr != S_OK)
        {
            Log::NameValuePairs nameValuePairs{ NameValuePair("File Path", path.GetValue()) };
            LogError(hr, *reliquary, "An image asset could not be created.", Log::Severity::SevereError, nameValuePairs);
        }

        return std::make_unique<ImageAssetDataImplementation>(tex);
    }

    std::unique_ptr<Asset::ImageAssetData> DirectX9GraphicsManager::CreateImageDataImpl(
        void* buffer, std::int32_t size, const File::Name& name)
    {
        D3DXIMAGE_INFO info;
        HRESULT hr = D3DXGetImageInfoFromFileInMemory(buffer, size, &info);
        if (hr != S_OK)
        {
            Log::NameValuePairs nameValuePairs{ NameValuePair("File Name", name.GetValue()) };
            LogError(hr, *reliquary, "An image asset could not be created.", Log::Severity::SevereError, nameValuePairs);
        };
        
        LPDIRECT3DTEXTURE9 tex;
        hr = D3DXCreateTextureFromFileInMemoryEx
        (
            device,
            buffer,
            size,
            info.Width,
            info.Height,
            D3DX_DEFAULT,
            0,
            D3DFMT_A8R8G8B8,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            nullptr,
            nullptr,
            &tex
        );
        if (hr != S_OK)
        {
            Log::NameValuePairs nameValuePairs{ NameValuePair("File Name", name.GetValue()) };
            LogError(hr, *reliquary, "An image asset could not be created.", Log::Severity::SevereError, nameValuePairs);
        }

        return std::make_unique<ImageAssetDataImplementation>(tex);
    }

    std::unique_ptr<Asset::ShaderAssetData> DirectX9GraphicsManager::CreateShaderDataImpl(
        const File::Path& path)
    {
        LPD3DXEFFECT effect;
        HRESULT hr = D3DXCreateEffectFromFile
        (
            device,
            path.c_str(),
            nullptr,
            nullptr,
            D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER,
            nullptr,
            &effect,
            nullptr
        );
        if (hr != S_OK)
        {
            Log::NameValuePairs nameValuePairs{ NameValuePair("File Path", path.GetValue()) };
            LogError(hr, *reliquary, "A shader asset could not be created.", Log::Severity::SevereError, nameValuePairs);
        }

        return std::make_unique<ShaderAssetDataImplementation>(effect);
    }

    std::unique_ptr<Asset::ShaderAssetData> DirectX9GraphicsManager::CreateShaderDataImpl(
        void* buffer, std::int32_t size, const File::Name& name)
    {
        LPD3DXEFFECT effect;
        HRESULT hr = D3DXCreateEffect
        (
            device,
            buffer,
            size,
            nullptr,
            nullptr,
            D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER,
            nullptr,
            &effect,
            nullptr
        );
        if (hr != S_OK)
        {
            Log::NameValuePairs nameValuePairs{ NameValuePair("File Path", name.GetValue()) };
            LogError(hr, *reliquary, "A shader asset could not be created.", Log::Severity::SevereError, nameValuePairs);
        }

        return std::make_unique<ShaderAssetDataImplementation>(effect);
    }

    Surface DirectX9GraphicsManager::CreateSurfaceImpl(
        void* window)
    {
        D3DPRESENT_PARAMETERS pp;
        ZeroMemory(&pp, sizeof(pp));

        pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        pp.BackBufferCount = 1;
        pp.BackBufferFormat = D3DFMT_A8R8G8B8;
        pp.BackBufferWidth = 0;
        pp.BackBufferHeight = 0;
        pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        pp.Windowed = true;
        pp.hDeviceWindow = static_cast<HWND>(window);
        pp.MultiSampleType = D3DMULTISAMPLE_NONE;
        pp.MultiSampleQuality = 0;

        LPDIRECT3DSWAPCHAIN9 swapChain;
        HRESULT hr = device->CreateAdditionalSwapChain(&pp, &swapChain);
        if (hr != S_OK)
            LogError(hr, *reliquary, "A render surface could not be created.", Log::Severity::SevereError);

        LPDIRECT3DSURFACE9 backBuffer;
        hr = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
        if (hr != S_OK)
            LogError(hr, *reliquary, "A render surface could not be created.", Log::Severity::SevereError);

        return Surface(std::make_unique<SurfaceData>(*this, *reliquary, swapChain, backBuffer));
    }

    Canvas DirectX9GraphicsManager::CreateCanvasImpl(
        const ScreenDimensions& dimensions)
    {
        LPDIRECT3DTEXTURE9 tex;
        HRESULT hr = D3DXCreateTexture
        (
            device,
            dimensions.width,
            dimensions.height,
            D3DX_DEFAULT,
            0,
            D3DFMT_X8R8G8B8,
            D3DPOOL_MANAGED,
            &tex
        );
        if (hr != S_OK)
            LogError(hr, *reliquary, "A canvas could not be created.", Log::Severity::SevereError);

        auto data = std::make_unique<CanvasData>(*this, *reliquary, tex);
        return Canvas(std::move(data), dimensions.width, dimensions.height);
    }

    bool DirectX9GraphicsManager::CanMakeImageImpl(const File::Path& path) const
    {
        D3DXIMAGE_INFO info;
        return D3DXGetImageInfoFromFile(path.c_str(), &info) == S_OK;
    }

    bool DirectX9GraphicsManager::CanMakeImageImpl(void* buffer, std::int32_t size) const
    {
        D3DXIMAGE_INFO info;
        return D3DXGetImageInfoFromFileInMemory(buffer, size, &info) == S_OK;
    }

    void DirectX9GraphicsManager::ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions)
    {
        LPDIRECT3DTEXTURE9 tex;
        HRESULT hr = D3DXCreateTexture
        (
            device,
            dimensions.width,
            dimensions.height,
            D3DX_DEFAULT,
            D3DUSAGE_RENDERTARGET,
            D3DFMT_X8R8G8B8,
            D3DPOOL_DEFAULT,
            &tex
        );
        if (hr != S_OK)
            LogError(hr, *reliquary, "Resizing of a canvas was attempted and failed.", Log::Severity::SevereError);

        canvas.GetData<CanvasData>()->tex->Release();
        canvas.GetData<CanvasData>()->tex = tex;
    }

    void DirectX9GraphicsManager::SetRenderTargetImpl(Surface& set)
    {
        SetProjectionMatrix();
    }

    void DirectX9GraphicsManager::SetRenderTargetToMainImpl()
    {
        device->SetRenderTarget(0, mainSurface);
        SetProjectionMatrix();
    }

    void DirectX9GraphicsManager::ReleaseMainRenderTarget()
    {
        mainSurface->Release();
    }

    void DirectX9GraphicsManager::ResetMainRenderTarget()
    {
        HRESULT hr = device->GetRenderTarget(0, &mainSurface);
        if (hr != S_OK)
            LogError(hr, *reliquary, "The main render target is unretrievable.", Log::Severity::SevereError);
    }

    void DirectX9GraphicsManager::PresentImpl()
    {
        HRESULT hr = device->Present(nullptr, nullptr, nullptr, nullptr);
        if (hr != S_OK)
            LogError(hr, *reliquary, "The frame was not presentable.", Log::Severity::SevereError);
    }

    void DirectX9GraphicsManager::PresentImpl(void* windowOverride)
    {
        HRESULT hr = device->Present(nullptr, nullptr, static_cast<HWND>(windowOverride), nullptr);
        if (hr != S_OK)
            LogError(hr, *reliquary, "The frame was not presentable.", Log::Severity::SevereError);
    }

    void DirectX9GraphicsManager::RenderMaterialViewImpl(MaterialRender& materialRender, float x, float y)
    {
        if (!materialRender.material)
            return;

        const auto material = materialRender.material;
        auto image = material->Image();
        const auto materialSlice = materialRender.materialSlice;
        const auto size = materialRender.size;

        RenderObject
        (
            image->DataAs<ImageAssetDataImplementation>()->tex,
            materialRender.patchShader ? materialRender.patchShader : material->Shader(),
            x,
            y,
            materialRender.position.z,
            materialSlice,
            materialSlice.Size(),
            materialSlice.Center(),
            Scalings(size.width, size.height),
            Angle(),
            materialRender.color
        );
    }

    void DirectX9GraphicsManager::RenderCanvasViewImpl(CanvasRender& canvasRender, float x, float y)
    {
        const auto position = canvasRender.position;
        const auto size = canvasRender.size;

        RenderObject
        (
            canvasRender.canvas.GetData<CanvasData>()->tex,
            renderer2D->DefaultTexturedImageViewShader(),
            x,
            y,
            position.z,
            AxisAlignedBox2D{ 0.0f, 0.0f, 1.0f, 1.0f },
            Size2D{ size.width, size.height },
            Position2D{ size.width / 2, size.height / 2 },
            Scalings(1.0f, 1.0f),
            Angle(),
            Color(255, 255, 255, 255)
        );
    }

    void DirectX9GraphicsManager::RenderLineImpl(const Line& line)
    {
        if (line.width != lineInterface->GetWidth())
        {
            EndLines();
            lineInterface->SetWidth(line.width);
            StartLines();
        }

        D3DXVECTOR2 points[] =
        {
            D3DXVECTOR2(static_cast<float>(line.from.x), static_cast<float>(line.from.y)),
            D3DXVECTOR2(static_cast<float>(line.to.x), static_cast<float>(line.to.y))
        };
        lineInterface->SetWidth(line.width);
        lineInterface->Draw(points, 2, ColorToD3D(line.color));
    }

    void DirectX9GraphicsManager::SetupPresentationParameters()
    {
        presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        presentationParameters.BackBufferCount = 1;
        presentationParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
        presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        presentationParameters.MultiSampleQuality = 0;

        presentationParameters.PresentationInterval = timeSettings->verticalSync ?
            D3DPRESENT_INTERVAL_DEFAULT :
            D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    void DirectX9GraphicsManager::SetRenderStates()
    {
        auto hr = device->SetRenderState(D3DRS_ZENABLE, false);
        if (hr != S_OK)
            LogError(hr, *reliquary, "DirectX9 Z buffer couldn't be set to false.", Log::Severity::SevereError);
        hr = device->SetRenderState(D3DRS_LIGHTING, false);
        if (hr != S_OK)
            LogError(hr, *reliquary, "DirectX9 lighting couldn't be set to false.", Log::Severity::SevereError);
        hr = device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
        if (hr != S_OK)
            LogError(hr, *reliquary, "DirectX9 alpha blend couldn't be set to true.", Log::Severity::SevereError);
        hr = device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        if (hr != S_OK)
            LogError(hr, *reliquary, "DirectX9 source blend couldn't be set to source alpha.", Log::Severity::SevereError);
        hr = device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        if (hr != S_OK)
            LogError
            (
                hr,
                *reliquary,
                "DirectX9 destination blend couldn't be set to inverse source alpha.",
                Log::Severity::SevereError
            );
    }

    void DirectX9GraphicsManager::SetProjectionMatrix()
    {
        const ScreenDimensions dimensions(CurrentDimensions());
        D3DXMatrixOrthoOffCenterLH
        (
            &projection,
            0,
            static_cast<FLOAT>(dimensions.width),
            static_cast<FLOAT>(dimensions.height),
            0,
            0.0f,
            1.0f
        );
    }

    void DirectX9GraphicsManager::OnResolutionChanged(const Agui::Resolution& arg)
    {
        SetMainDimensions(ScreenDimensions
        (
            static_cast<Dimension>(arg.GetWidth()),
            static_cast<Dimension>(arg.GetHeight())
        ));
        ReconstructAll();
    }
}