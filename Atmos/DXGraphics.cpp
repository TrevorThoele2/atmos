
#include <array>
#include "DXGraphics.h"

#include <Windows.h>
#include <stdio.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
#include <DxErr.h>
#pragma comment(lib, "dxerr.lib")

#include "ImageAsset.h"

#include "CanvasView.h"
#include "LineRender.h"

#include "Flags.h"
#include "SimpleFile.h"

#include "CameraSystem.h"
#include "FpsSystem.h"
#include "FileSystem.h"
#include "LoggingSystem.h"

namespace Atmos
{
    D3DCOLOR ColorToD3D(const Color &col)
    {
        return D3DCOLOR_ARGB(col.alpha, col.red, col.green, col.blue);
    }

    class ImageAssetDataImplementation : public ImageAssetData
    {
    public:
        LPDIRECT3DTEXTURE9 tex;

        ImageAssetDataImplementation(LPDIRECT3DTEXTURE9 tex) : tex(tex)
        {}

        ~ImageAssetDataImplementation()
        {
            tex->Release();
        }

        std::unique_ptr<ImageAssetData> Clone() const override
        {
            return std::unique_ptr<ImageAssetData>(new ImageAssetDataImplementation(*this));
        }
    };

    class ShaderAssetDataImplementation : public ShaderAssetData
    {
    public:
        LPD3DXEFFECT effect;

        ShaderAssetDataImplementation(LPD3DXEFFECT effect) : effect(effect)
        {}

        ~ShaderAssetDataImplementation()
        {
            effect->Release();
        }

        std::unique_ptr<ShaderAssetData> Clone() const override
        {
            return std::unique_ptr<ShaderAssetData>(new ShaderAssetDataImplementation(*this));
        }

        void Reset() override
        {
            effect->OnResetDevice();
        }

        void Release() override
        {
            effect->OnLostDevice();
        }

        PassT Begin() const override
        {
            {
                D3DXHANDLE technique;
                effect->FindNextValidTechnique(nullptr, &technique);
                effect->SetTechnique(technique);
            }

            PassT pass;
            effect->Begin(&pass, 0);
            return pass;
        }

        void End() const override
        {
            effect->End();
        }

        void BeginNextPass(PassT pass) const override
        {
            effect->BeginPass(pass);
        }

        void EndPass() const override
        {
            effect->EndPass();
        }
    };

    class RenderSurfaceData : public RenderSurface::Data
    {
    public:
        RenderSurfaceData(DX9GraphicsManager& owner, LoggingSystem& loggingSystem, LPDIRECT3DSURFACE9 backBuffer) :
            owner(&owner), loggingSystem(&loggingSystem), swapChain(nullptr), backBuffer(backBuffer)
        {}

        RenderSurfaceData(DX9GraphicsManager& owner, LoggingSystem& loggingSystem, LPDIRECT3DSWAPCHAIN9 swapChain, LPDIRECT3DSURFACE9 backBuffer) :
            owner(&owner), loggingSystem(&loggingSystem), swapChain(swapChain), backBuffer(backBuffer)
        {
            swapChain->GetPresentParameters(&presentationParameters);
        }

        ~RenderSurfaceData()
        {
            Release();
        }

        void SetAsRenderTarget() override
        {
            owner->GetDevice()->SetRenderTarget(0, backBuffer);
        }

        void Present() override
        {
            auto hr = swapChain->Present(nullptr, nullptr, nullptr, nullptr, 0);
            if (hr != S_OK)
                loggingSystem->Log(String("A swap chain failed when presenting.\n") + DXGetErrorDescriptionA(hr),
                    LogType::ERROR_SEVERE);
        }

        void Reset() override
        {
            presentationParameters.BackBufferWidth = 0;
            presentationParameters.BackBufferHeight = 0;

            HRESULT hr = owner->GetDevice()->CreateAdditionalSwapChain(
                &presentationParameters,
                &swapChain);
            if (hr != S_OK)
                loggingSystem->Log(String("A DirectX device could not create an additional swap chain.\n") + DXGetErrorDescriptionA(hr),
                    LogType::ERROR_SEVERE);

            hr = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
            if (hr != S_OK)
                loggingSystem->Log(String("A DirectX device could not retrieve the back bfufer from a swap chain.\n") + DXGetErrorDescriptionA(hr),
                    LogType::ERROR_SEVERE);
        }

        void Release() override
        {
            backBuffer->Release();
            backBuffer = nullptr;

            swapChain->Release();
            swapChain = nullptr;
        }

        Size GetSize() override
        {
            RECT rect;
            GetClientRect(presentationParameters.hDeviceWindow, &rect);
            return Size(rect.right, rect.bottom);
        }

        const D3DPRESENT_PARAMETERS& GetPresentationParameters() const
        {
            return presentationParameters;
        }
    private:
        DX9GraphicsManager* owner;
        LoggingSystem* loggingSystem;
    private:
        D3DPRESENT_PARAMETERS presentationParameters;
        LPDIRECT3DSWAPCHAIN9 swapChain;
        LPDIRECT3DSURFACE9 backBuffer;
    };

    class CanvasData : public Canvas::Data
    {
    public:
        DX9GraphicsManager* owner;
        LoggingSystem* loggingSystem;

        LPDIRECT3DTEXTURE9 tex;
        LPDIRECT3DSURFACE9 backBuffer;

        D3DLOCKED_RECT lockedRect;

        CanvasData(DX9GraphicsManager& owner, LoggingSystem& loggingSystem, LPDIRECT3DTEXTURE9 tex) :
            owner(&owner), loggingSystem(&loggingSystem), tex(tex)
        {
            this->tex->GetSurfaceLevel(0, &backBuffer);
        }

        ~CanvasData()
        {
            tex->Release();
        }

        void StartPainting() override
        {
            if (backBuffer->LockRect(&lockedRect, nullptr, 0) != S_OK)
                loggingSystem->Log("A canvas' back buffer could not be locked.",
                    LogType::ERROR_LOW);
        }

        void StopPainting() override
        {
            if (backBuffer->UnlockRect() != S_OK)
                loggingSystem->Log("A canvas' back buffer could not be unlocked.",
                    LogType::ERROR_LOW);
        }

        void PaintPixel(const Canvas::Position& position, const Color& color, Canvas::DimensionValue height) override
        {
            DWORD *row = reinterpret_cast<DWORD*>(reinterpret_cast<char*>(lockedRect.pBits) + lockedRect.Pitch * position.y);
            row[position.x] = ColorToD3D(color);
        }

        void Clear(const Color &color) override
        {
            std::memset(lockedRect.pBits, ColorToD3D(color), lockedRect.Pitch * GetOwner()->GetHeight());
        }

        void Release() override
        {
            tex->Release();
        }

        void Reset(Canvas::DimensionValue width, Canvas::DimensionValue height) override
        {
            D3DXCreateTexture(
                owner->GetDevice(),
                width,
                height,
                D3DX_DEFAULT,
                0,
                D3DFMT_X8R8G8B8,
                D3DPOOL_MANAGED,
                &tex);
            tex->GetSurfaceLevel(0, &backBuffer);
        }
    };

    class DX9GraphicsManager::Renderer2D
    {
    public:
        typedef size_t ObjectsSize;
    public:
        Renderer2D(ObjectManager& objectManager);

        ~Renderer2D();
        void Initialize(DX9GraphicsManager& handler, LPDIRECT3DDEVICE9 device);

        void Start(ObjectsSize size);
        // If the projection exists, then this will flush the sprites
        void Stop(const D3DXMATRIX* projection);
        void Flush(const D3DXMATRIX& projection);

        void Draw(
            LPDIRECT3DTEXTURE9 tex,
            TypedObjectReference<ShaderAsset> shader,
            float X,
            float Y,
            float Z,
            const AxisBoundingBox2D& imageBounds,
            const Size2D& size,
            const Position2D& center,
            const Scalings& scalings,
            const Angle& rotation,
            const Color& color);

        void OnLostDevice();
        void OnResetDevice();

        TypedObjectReference<ShaderAsset> GetDefaultTexturedSpriteShader();
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

        typedef unsigned short Index;

        class Object3D
        {
        public:
            std::vector<Vertex> vertices;
            std::vector<Index> indicies;
            unsigned int primCount;
            float z;
            LPDIRECT3DTEXTURE9 tex;
            TypedObjectReference<ShaderAsset> shader;

            Object3D(
                LPDIRECT3DTEXTURE9 tex,
                TypedObjectReference<ShaderAsset> shader,
                float X,
                float Y,
                float Z,
                const AxisBoundingBox2D& imageBounds,
                const Size2D& size,
                const Position2D& center,
                const Scalings& scalings,
                const Angle& rotation,
                const Color& color);
            Object3D(Object3D&& arg);
            Object3D& operator=(Object3D&& arg);
        private:
            void SetupQuad(
                float X,
                float Y,
                const Position2D& center,
                const Scalings& scaling,
                float rotation);
            void SetupRegularPolygon(
                float radius,
                unsigned int polyCount,
                float X,
                float Y,
                const Scalings& scalings,
                float rotation,
                const Color& color);

            static void SetupVertexCommon(
                Vertex&vertex,
                const D3DXMATRIX& matrix,
                const D3DXVECTOR2& position,
                const D3DXVECTOR2& center);
            static void SetupVertexCommon(
                Vertex& vertex,
                const D3DXMATRIX& matrix,
                const D3DXVECTOR2& position,
                const D3DXVECTOR2& center,
                const Color& color);
        };
    private:
        typedef unsigned short BufferSize;

        static constexpr BufferSize maxVertexSprint = 4000;
        static constexpr BufferSize maxIndexSprint = 6000;

        LPDIRECT3DDEVICE9 device;
        LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
        LPDIRECT3DINDEXBUFFER9 indexBuffer;
        LPDIRECT3DVERTEXDECLARATION9 vertexDecl;
        TypedObjectReference<ShaderAsset> defaultTexturedSpriteShader;

        typedef std::vector<Object3D> Objects;
        Objects objects;
    private:
        ObjectManager* objectManager;
        CameraSystem* FindCameraSystem() const;
        FileSystem* FindFileSystem() const;
    private:
        void InitializeBuffers();
    };

    DX9GraphicsManager::Renderer2D::Renderer2D(ObjectManager& objectManager) : objectManager(&objectManager)
    {}

    DX9GraphicsManager::Renderer2D::~Renderer2D()
    {
        vertexBuffer->Release();
        indexBuffer->Release();
        vertexDecl->Release();
    }

    void DX9GraphicsManager::Renderer2D::Initialize(DX9GraphicsManager& handler, LPDIRECT3DDEVICE9 device)
    {
        this->device = device;
        InitializeBuffers();

        auto path = FindFileSystem()->Get()->ExePath().Append("Shaders\\TexturedSprite.fx").GetValue();

        auto shaderData = handler.CreateShaderData(path);
        defaultTexturedSpriteShader = objectManager->CreateObject<ShaderAsset>(path, std::move(shaderData));
    }

    void DX9GraphicsManager::Renderer2D::Start(ObjectsSize size)
    {
        if (size != 0)
            objects.reserve(size);
    }

    void DX9GraphicsManager::Renderer2D::Stop(const D3DXMATRIX* projection)
    {
        if (projection)
            Flush(*projection);

        objects.clear();
    }

    void DX9GraphicsManager::Renderer2D::Flush(const D3DXMATRIX& projection)
    {
        if (objects.empty())
            return;

        // Sort objects
        {
            auto sorter = [&](const Object3D &left, const Object3D &right)
            {
                const auto leftZ = left.z;
                const auto rightZ = right.z;
                if (leftZ == rightZ)
                {
                    const auto leftTex = left.tex;
                    const auto rightTex = right.tex;

                    if (leftTex == rightTex)
                        return left.shader.Get() < right.shader.Get();
                    else
                        return leftTex < rightTex;
                }
                else
                    return leftZ < rightZ;
            };

            std::sort(objects.begin(), objects.end(), sorter);
        }

        auto cameraSystem = FindCameraSystem();

        device->SetVertexDeclaration(vertexDecl);
        device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
        device->SetIndices(indexBuffer);

        LPDIRECT3DTEXTURE9 focusedTex = nullptr;
        TypedObjectReference<ShaderAsset> focusedShader;
        const ShaderAssetDataImplementation* focusedShaderImpl = nullptr;

        auto setTexture = [&](LPDIRECT3DTEXTURE9 set)
        {
            focusedTex = set;
            if (focusedTex && focusedShaderImpl)
                focusedShaderImpl->effect->SetTexture("g_Texture", focusedTex);
        };

        auto setShader = [&](TypedObjectReference<ShaderAsset> set)
        {
            focusedShader = set;
            focusedShaderImpl = focusedShader->DataAs<ShaderAssetDataImplementation>();
            focusedShaderImpl->effect->SetMatrix("g_Projection", &projection);

            auto& cameraSize = cameraSystem->GetSize();
            FLOAT screenSize[2] = { static_cast<FLOAT>(cameraSize.GetWidth()), static_cast<FLOAT>(cameraSize.GetHeight()) };
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
        BufferSize primCount = 0;

        auto drawPrims = [&](bool endOfDraw)
        {
            vertexBuffer->Unlock();
            indexBuffer->Unlock();

            const ShaderAssetData::PassT passCount = focusedShaderImpl->Begin();
            ShaderAssetData::PassT passCountCurrent = 0;

            do
            {
                focusedShaderImpl->BeginNextPass(passCountCurrent);
                ++passCountCurrent;

                device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, primCount);

                focusedShaderImpl->EndPass();
            } while (passCountCurrent < passCount);

            focusedShaderImpl->End();

            if (!endOfDraw)
            {
                vertexBuffer->Lock(0, 0, &vertexData, D3DLOCK_DISCARD);
                indexBuffer->Lock(0, 0, &indexData, D3DLOCK_DISCARD);
                vertexCount = 0;
                indexCount = 0;
                primCount = 0;
            }
        };

        for (auto loop = objects.begin(); loop != objects.end(); ++loop)
        {
            const size_t loopVerticesSize = loop->vertices.size();
            const size_t loopIndiciesSize = loop->indicies.size();

            LPDIRECT3DTEXTURE9 loopTexture = loop->tex;
            TypedObjectReference<ShaderAsset> loopShader = loop->shader;

            // Check if the sprint is done
            if ((vertexCount + loopVerticesSize) >= maxVertexSprint || (indexCount + loopIndiciesSize) >= maxIndexSprint)
                drawPrims(false);

            // Check if the texture AND shader is different
            if(focusedTex != loopTexture && focusedShader != loopShader)
            {
                if (vertexCount > 0)
                    drawPrims(false);

                setTexture(loopTexture);
                setShader(loopShader);
            }
            // Check if the texture is different
            else if (focusedTex != loopTexture)
            {
                if (vertexCount > 0)
                    drawPrims(false);

                setTexture(loopTexture);
            }
            // Check if the shader is different
            else if (focusedShader != loopShader)
            {
                if (vertexCount > 0)
                    drawPrims(false);

                setTexture(focusedTex);
                setShader(loopShader);
            }

            // Vertices
            memcpy(&reinterpret_cast<Vertex*>(vertexData)[vertexCount], loop->vertices.data(), loopVerticesSize * sizeof(Vertex));
            
            // Indicies
            for (size_t indexLoop = 0; indexLoop != loopIndiciesSize; ++indexLoop)
                loop->indicies[indexLoop] = vertexCount + loop->indicies[indexLoop];
            memcpy(&reinterpret_cast<Index*>(indexData)[indexCount], loop->indicies.data(), loopIndiciesSize * sizeof(Index));

            vertexCount += static_cast<BufferSize>(loopVerticesSize);
            indexCount += static_cast<BufferSize>(loopIndiciesSize);
            primCount += loop->primCount;
        }

        vertexBuffer->Unlock();
        indexBuffer->Unlock();

        objects.clear();
        if (vertexCount == 0)
            return;

        drawPrims(true);
    }

    void DX9GraphicsManager::Renderer2D::Draw(
        LPDIRECT3DTEXTURE9 tex,
        TypedObjectReference<ShaderAsset> shader,
        float X,
        float Y,
        float Z,
        const AxisBoundingBox2D& imageBounds,
        const Size2D& size,
        const Position2D& center,
        const Scalings& scalings,
        const Angle& rotation,
        const Color& color)
    {
        objects.push_back(Object3D(tex, shader, X, Y, Z, imageBounds, size, center, scalings, rotation, color));
    }

    void DX9GraphicsManager::Renderer2D::OnLostDevice()
    {
        vertexBuffer->Release();
        indexBuffer->Release();
        defaultTexturedSpriteShader->DataAs<ShaderAssetDataImplementation>()->effect->OnLostDevice();
    }

    void DX9GraphicsManager::Renderer2D::OnResetDevice()
    {
        InitializeBuffers();
        defaultTexturedSpriteShader->DataAs<ShaderAssetDataImplementation>()->effect->OnResetDevice();
    }

    TypedObjectReference<ShaderAsset> DX9GraphicsManager::Renderer2D::GetDefaultTexturedSpriteShader()
    {
        return defaultTexturedSpriteShader;
    }

    DX9GraphicsManager::Renderer2D::Object3D::Object3D(
        LPDIRECT3DTEXTURE9 tex,
        TypedObjectReference<ShaderAsset> shader,
        float X,
        float Y,
        float Z,
        const AxisBoundingBox2D& imageBounds,
        const Size2D& size,
        const Position2D& center,
        const Scalings& scalings,
        const Angle& rotation,
        const Color& color) :

        tex(tex), z(Z), shader(shader),
        vertices
    {
        Vertex(D3DXVECTOR2(0.0f, 0.0f),
            ColorToD3D(color),
            imageBounds.GetLeft(),
            imageBounds.GetTop()),
        Vertex(D3DXVECTOR2(size.GetWidth(), 0.0f),
            ColorToD3D(color),
            imageBounds.GetRight(),
            imageBounds.GetTop()),
        Vertex(D3DXVECTOR2(0.0f, size.GetHeight()),
            ColorToD3D(color),
            imageBounds.GetLeft(),
            imageBounds.GetBottom()),
        Vertex(D3DXVECTOR2(size.GetWidth(), size.GetHeight()),
            ColorToD3D(color),
            imageBounds.GetRight(),
            imageBounds.GetBottom())
    }
    {
        SetupQuad(X, Y, center, scalings, rotation.As<Radians>());
    }

    DX9GraphicsManager::Renderer2D::Object3D::Object3D(Object3D&& arg) :
        vertices(std::move(arg.vertices)), indicies(std::move(arg.indicies)), primCount(std::move(arg.primCount)),
        z(std::move(arg.z)), tex(std::move(arg.tex)), shader(std::move(arg.shader))
    {}

    DX9GraphicsManager::Renderer2D::Object3D& DX9GraphicsManager::Renderer2D::Object3D::operator=(Object3D&& arg)
    {
        vertices = std::move(arg.vertices);
        indicies = std::move(arg.indicies);
        primCount = std::move(arg.primCount);
        z = std::move(arg.z);
        tex = std::move(arg.tex);
        shader = std::move(arg.shader);
        return *this;
    }

    void DX9GraphicsManager::Renderer2D::Object3D::SetupQuad(
        float X,
        float Y,
        const Position2D& center,
        const Scalings& scalings,
        float rotation)
    {
        // Setup matrix and center
        D3DXVECTOR2 transformedCenter;
        D3DXMATRIX matrix;
        {
            D3DXVECTOR2 centerVector(center.GetX(), center.GetY());
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

        // Indicies
        // 0, 1, 2
        // 1, 3, 2

        indicies.resize(6);
        indicies[0] = 0;
        indicies[1] = 1;
        indicies[2] = 2;
        indicies[3] = 1;
        indicies[4] = 3;
        indicies[5] = 2;

        primCount = 2;
    }

    void DX9GraphicsManager::Renderer2D::Object3D::SetupRegularPolygon(
        float radius,
        unsigned int polyCount,
        float X,
        float Y,
        const Scalings& scalings,
        float rotation,
        const Color& color)
    {
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
                indicies[startIndex] = static_cast<Index>(currentPoly + 1);
                indicies[startIndex + 1] = static_cast<Index>(currentPoly + 2);
                indicies[startIndex + 2] = static_cast<Index>(0);
            };

            float currentAngle = FULL_CIRCLE_RADIANS<float> / 2.0f;
            vertices.resize(polyCount + 1);
            indicies.resize(polyCount * 3);
            SetupVertexCommon(vertices[0], matrix, D3DXVECTOR2(useRadius, useRadius), transformedCenter, color);

            for (size_t loopPoly = 0; loopPoly != polyCount; ++loopPoly)
            {
                auto vector = D3DXVECTOR2(useRadius + (std::cos(currentAngle) * useRadius), useRadius + (std::sin(currentAngle) * useRadius));

                SetupVertexCommon(vertices[loopPoly + 1], matrix, vector, transformedCenter, color);
                setIndex(loopPoly);
                currentAngle += angle;
            }

            // Fix the second-to-last index
            indicies[indicies.size() - 2] = 1;
        }

        primCount = polyCount;
    }

    void DX9GraphicsManager::Renderer2D::Object3D::SetupVertexCommon(
        Vertex& vertex,
        const D3DXMATRIX& matrix,
        const D3DXVECTOR2& position,
        const D3DXVECTOR2& center)
    {
        D3DXVec2TransformCoord(&vertex.position, &position, &matrix);

        vertex.center = center;
    }

    void DX9GraphicsManager::Renderer2D::Object3D::SetupVertexCommon(
        Vertex& vertex,
        const D3DXMATRIX& matrix,
        const D3DXVECTOR2& position,
        const D3DXVECTOR2& center,
        const Color& color)
    {
        D3DXVec2TransformCoord(&vertex.position, &position, &matrix);
        vertex.center = center;
        vertex.color = ColorToD3D(color);
    }

    CameraSystem* DX9GraphicsManager::Renderer2D::FindCameraSystem() const
    {
        return objectManager->FindSystem<CameraSystem>();
    }

    FileSystem* DX9GraphicsManager::Renderer2D::FindFileSystem() const
    {
        return objectManager->FindSystem<FileSystem>();
    }

    void DX9GraphicsManager::Renderer2D::InitializeBuffers()
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

        D3DVERTEXELEMENT9 vElements[] = {
            { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1 },
            { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };

        device->CreateVertexDeclaration(vElements, &vertexDecl);
    }

    DX9GraphicsManager::DX9GraphicsManager(ObjectManager& objectManager, HWND hwnd, const ScreenDimensions& backbuffer, bool fullscreen) :
        GraphicsManager(objectManager), hwnd(hwnd), renderer2D(new Renderer2D(objectManager))
    {
        ZeroMemory(&presentationParameters, sizeof(presentationParameters));

        SetMainDimensions(backbuffer);
        SetFullscreen(fullscreen);

        // Create the Direct3D interface
        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        presentationParameters.hDeviceWindow = hwnd;
        SetupPresentationParameters();

        d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentationParameters, &device);
        renderer2D->Initialize(*this, device);
        D3DXCreateLine(device, &lineInterface);

        SetRenderStates();

        device->GetRenderTarget(0, &mainSurface);
    }

    DX9GraphicsManager::~DX9GraphicsManager()
    {
        d3d->Release();
        device->Release();
        lineInterface->Release();
    }

    void DX9GraphicsManager::SetFullscreen(bool set)
    {
        (set) ? presentationParameters.Windowed = false : presentationParameters.Windowed = true;
    }

    void DX9GraphicsManager::ClearTarget(const Flags<Target>& target)
    {
        DWORD pass = 0;
        if (target.Get(Target::MAIN))
            pass |= D3DCLEAR_TARGET;

        if (target.Get(Target::STENCIL))
            pass |= D3DCLEAR_STENCIL;

        device->Clear(0, nullptr, pass, 0, 1.0f, 0);
    }

    void DX9GraphicsManager::ClearTarget(const Flags<Target>& target, const Color& color)
    {
        DWORD pass = 0;
        if (target.Get(Target::MAIN))
            pass |= D3DCLEAR_TARGET;

        if (target.Get(Target::STENCIL))
            pass |= D3DCLEAR_STENCIL;

        device->Clear(0, nullptr, pass, ColorToD3D(color), 1.0f, 0);
    }

    void DX9GraphicsManager::Flush()
    {
        renderer2D->Flush(projection);
    }

    void DX9GraphicsManager::SetRenderState(RenderState state, bool set)
    {
        D3DRENDERSTATETYPE pass;

        if (state == RenderState::STENCIL)
            pass = D3DRS_STENCILENABLE;

        DWORD tf;
        (set) ? tf = TRUE : tf = FALSE;

        device->SetRenderState(pass, tf);
    }

    bool DX9GraphicsManager::Start()
    {
        HRESULT hr = device->TestCooperativeLevel();

        // Device is lost but can't reset it
        if (hr == D3DERR_DEVICELOST)
            return false;

        // Device is lost, and can reset
        if (hr == D3DERR_DEVICENOTRESET)
            Reinitialize();

        device->BeginScene();
        return true;
    }

    void DX9GraphicsManager::End()
    {
        device->EndScene();
    }

    void DX9GraphicsManager::StartSprites(size_t spriteCount)
    {
        renderer2D->Start(spriteCount);
    }

    void DX9GraphicsManager::EndSprites()
    {
        renderer2D->Stop(&projection);
    }

    void DX9GraphicsManager::StartLines()
    {
        lineInterface->Begin();
    }

    void DX9GraphicsManager::EndLines()
    {
        lineInterface->End();
    }

    void DX9GraphicsManager::StartStencil()
    {
        device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NEVER);

        device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_REPLACE);
        device->SetRenderState(D3DRS_STENCILREF, 1);
        device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
    }

    void DX9GraphicsManager::StopStencil()
    {
        Flush();

        device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

        device->SetRenderState(D3DRS_STENCILREF, 0);
        device->SetRenderState(D3DRS_STENCILWRITEMASK, 0);
    }

    LPDIRECT3DDEVICE9& DX9GraphicsManager::GetDevice()
    {
        return device;
    }

    DX9GraphicsManager::Scalings::Scalings() : x(0.0f), y(0.0f)
    {}

    DX9GraphicsManager::Scalings::Scalings(float x, float y) : x(x), y(y)
    {}

    void DX9GraphicsManager::RenderObject(
        LPDIRECT3DTEXTURE9 tex,
        TypedObjectReference<ShaderAsset> shader,
        float X,
        float Y,
        float Z,
        const AxisBoundingBox2D& imageBounds,
        const Size2D& size,
        const Position2D& center,
        const Scalings& scalings,
        const Angle& rotation,
        const Color& color)
    {
        if (!shader)
            shader = renderer2D->GetDefaultTexturedSpriteShader();

        renderer2D->Draw(tex, shader, X, Y, Z, imageBounds, size, center, scalings, rotation, color);
    }

    void DX9GraphicsManager::ReinitializeImpl()
    {
        SetupPresentationParameters();

        // Destroy interfaces
        renderer2D->OnLostDevice();

        HRESULT hr = lineInterface->OnLostDevice();
        if (hr != S_OK)
            FindLoggingSystem()->Log(String("The DirectX line interface was not able to be released.") + DXGetErrorDescription(hr),
                LogType::ERROR_SEVERE);

        // Reset
        hr = device->Reset(&presentationParameters);
        if (hr == D3DERR_DEVICELOST)
            return;
        else if (hr != S_OK)
            FindLoggingSystem()->Log(String("The DirectX device failed when resetting.") + DXGetErrorDescription(hr),
                LogType::ERROR_SEVERE);

        // Recreate interfaces
        renderer2D->OnResetDevice();
        hr = lineInterface->OnResetDevice();
        if (hr != S_OK)
            FindLoggingSystem()->Log(String("The DirectX line interface was not able to be reset.") + DXGetErrorDescription(hr),
                LogType::ERROR_SEVERE);

        // Reset render states
        SetRenderStates();
    }

    void DX9GraphicsManager::SetMainDimensionsImpl(const ScreenDimensions& dimensions)
    {
        presentationParameters.BackBufferWidth = dimensions.width;
        presentationParameters.BackBufferHeight = dimensions.height;
        SetProjectionMatrix();
    }

    ScreenDimensions DX9GraphicsManager::GetMainDimensionsImpl() const
    {
        return ScreenDimensions(presentationParameters.BackBufferWidth, presentationParameters.BackBufferHeight);
    }

    std::unique_ptr<ImageAssetData> DX9GraphicsManager::CreateImageDataImpl(const FilePath& path)
    {
        D3DXIMAGE_INFO info;
        HRESULT hr = D3DXGetImageInfoFromFile(path.c_str(), &info);
        if (hr != S_OK)
            FindLoggingSystem()->Log("An image asset could not be created.",
                LogType::ERROR_SEVERE,
                LogNameValueVector{ NameValuePair("File Path", path.GetValue()) });

        LPDIRECT3DTEXTURE9 tex;
        hr = D3DXCreateTextureFromFileEx(
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
            &tex);
        if (hr != S_OK)
            FindLoggingSystem()->Log("An image asset could not be created.",
                LogType::ERROR_SEVERE,
                LogNameValueVector{ NameValuePair("File Path", path.GetValue()) });

        return std::make_unique<ImageAssetDataImplementation>(tex);
    }

    std::unique_ptr<ImageAssetData> DX9GraphicsManager::CreateImageDataImpl(void* buffer, std::int32_t size, const FileName& name)
    {
        D3DXIMAGE_INFO info;
        HRESULT hr = D3DXGetImageInfoFromFileInMemory(buffer, size, &info);
        if (hr != S_OK)
            FindLoggingSystem()->Log("An image asset could not be created.",
                LogType::ERROR_SEVERE,
                LogNameValueVector{ NameValuePair("File Name", name.GetValue()) });

        LPDIRECT3DTEXTURE9 tex;
        hr = D3DXCreateTextureFromFileInMemoryEx(
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
            &tex);
        if (hr != S_OK)
            FindLoggingSystem()->Log("An image asset could not be created.",
                LogType::ERROR_SEVERE,
                LogNameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return std::make_unique<ImageAssetDataImplementation>(tex);
    }

    std::unique_ptr<ShaderAssetData> DX9GraphicsManager::CreateShaderDataImpl(const FilePath& path)
    {
        LPD3DXEFFECT effect;
        HRESULT hr = D3DXCreateEffectFromFile(
            device,
            path.c_str(),
            nullptr,
            nullptr,
            D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER,
            nullptr,
            &effect,
            nullptr);
        if (hr != S_OK)
            FindLoggingSystem()->Log("A shader asset could not be created.",
                LogType::ERROR_SEVERE,
                LogNameValueVector{ NameValuePair("File Path", path.GetValue()) });

        return std::make_unique<ShaderAssetDataImplementation>(effect);
    }

    std::unique_ptr<ShaderAssetData> DX9GraphicsManager::CreateShaderDataImpl(void* buffer, std::int32_t size, const FileName& name)
    {
        LPD3DXEFFECT effect;
        HRESULT hr = D3DXCreateEffect(
            device,
            buffer,
            size,
            nullptr,
            nullptr,
            D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER,
            nullptr,
            &effect,
            nullptr);
        if (hr != S_OK)
            FindLoggingSystem()->Log("A shader asset could not be created.",
                LogType::ERROR_SEVERE,
                LogNameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return std::make_unique<ShaderAssetDataImplementation>(effect);
    }

    RenderSurface DX9GraphicsManager::CreateRenderSurfaceImpl(void* window)
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
            FindLoggingSystem()->Log("A render surface could not be created.",
                LogType::ERROR_SEVERE);

        LPDIRECT3DSURFACE9 backBuffer;
        hr = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
        if (hr != S_OK)
            FindLoggingSystem()->Log("A render surface could not be created.",
                LogType::ERROR_SEVERE);

        return RenderSurface(RenderSurface::DataPtr(new RenderSurfaceData(*this, *FindLoggingSystem(), swapChain, backBuffer)));
    }

    Canvas DX9GraphicsManager::CreateCanvasImpl(const ScreenDimensions& dimensions)
    {
        LPDIRECT3DTEXTURE9 tex;
        HRESULT hr = D3DXCreateTexture(
            device,
            dimensions.width,
            dimensions.height,
            D3DX_DEFAULT,
            0,
            D3DFMT_X8R8G8B8,
            D3DPOOL_MANAGED,
            &tex);
        if (hr != S_OK)
            FindLoggingSystem()->Log("A canvas could not be created.",
                LogType::ERROR_SEVERE);

        return Canvas(Canvas::DataPtr(new CanvasData(*this, *FindLoggingSystem(), tex)), dimensions.width, dimensions.height);
    }

    bool DX9GraphicsManager::CanMakeImageImpl(const FilePath& path) const
    {
        D3DXIMAGE_INFO info;
        return D3DXGetImageInfoFromFile(path.c_str(), &info) == S_OK;
    }

    bool DX9GraphicsManager::CanMakeImageImpl(void* buffer, std::int32_t size) const
    {
        D3DXIMAGE_INFO info;
        return D3DXGetImageInfoFromFileInMemory(buffer, size, &info) == S_OK;
    }

    void DX9GraphicsManager::ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions)
    {
        LPDIRECT3DTEXTURE9 tex;
        HRESULT hr = D3DXCreateTexture(
            device,
            dimensions.width,
            dimensions.height,
            D3DX_DEFAULT,
            D3DUSAGE_RENDERTARGET,
            D3DFMT_X8R8G8B8,
            D3DPOOL_DEFAULT,
            &tex);
        if (hr != S_OK)
            FindLoggingSystem()->Log("Resizing of a canvas was attempted and failed.",
                LogType::ERROR_SEVERE);

        canvas.GetData<CanvasData>()->tex->Release();
        canvas.GetData<CanvasData>()->tex = tex;
    }

    void DX9GraphicsManager::SetRenderTargetImpl(RenderSurface& set)
    {
        SetProjectionMatrix();
    }

    void DX9GraphicsManager::SetRenderTargetToMainImpl()
    {
        device->SetRenderTarget(0, mainSurface);
        SetProjectionMatrix();
    }

    void DX9GraphicsManager::ReleaseMainRenderTarget()
    {
        mainSurface->Release();
    }

    void DX9GraphicsManager::ResetMainRenderTarget()
    {
        HRESULT hr = device->GetRenderTarget(0, &mainSurface);
        if (hr != S_OK)
            FindLoggingSystem()->Log("The main render target is unretrievable.",
                LogType::ERROR_SEVERE);
    }

    void DX9GraphicsManager::PresentImpl()
    {
        HRESULT hr = device->Present(nullptr, nullptr, nullptr, nullptr);
        if (hr != S_OK)
            FindLoggingSystem()->Log("The frame was unpresentable.",
                LogType::ERROR_SEVERE);
    }

    void DX9GraphicsManager::PresentImpl(void* windowOverride)
    {
        HRESULT hr = device->Present(nullptr, nullptr, static_cast<HWND>(windowOverride), nullptr);
        if (hr != S_OK)
            FindLoggingSystem()->Log("The frame was unpresentable.",
                LogType::ERROR_SEVERE);
    }

    void DX9GraphicsManager::RenderSpriteImpl(SpriteReference sprite, float X, float Y)
    {
        auto material = sprite->material;
        if (!material)
            return;

        RenderObject(
            material->xVisual->DataAs<ImageAssetDataImplementation>()->tex,
            (sprite->patchShader) ? sprite->patchShader : material->shader,
            X,
            Y,
            sprite->position.z,
            AxisBoundingBox2D(
                sprite->primaryAssetSlice.Get().GetLeft() / sprite->material->width,
                sprite->primaryAssetSlice.Get().GetTop() / sprite->material->height,
                sprite->primaryAssetSlice.Get().GetRight() / sprite->material->width,
                sprite->primaryAssetSlice.Get().GetBottom() / sprite->material->height,
                false),
            sprite->primaryAssetSlice.Get().GetSize(),
            sprite->primaryAssetSlice.Get().GetCenter(),
            Scalings(sprite->size.widthScaler, sprite->size.heightScaler),
            sprite->size.xRotation.Get(),
            sprite->color);
    }

    void DX9GraphicsManager::RenderCanvasViewImpl(CanvasViewReference view, float X, float Y)
    {
        RenderObject(
            view->source->GetData<CanvasData>()->tex,
            renderer2D->GetDefaultTexturedSpriteShader(),
            X,
            Y,
            view->position.z,
            AxisBoundingBox2D(0.0f, 0.0f, 1.0f, 1.0f),
            Size2D(view->size.width, view->size.height),
            Position2D(view->size.width.Get() / 2, view->size.height.Get() / 2),
            Scalings(view->size.widthScaler, view->size.heightScaler),
            view->size.xRotation.Get(),
            Color(255, 255, 255, 255));
    }

    void DX9GraphicsManager::RenderUnknownFragmentImpl(RenderFragmentReference fragment, float X, float Y)
    {}

    void DX9GraphicsManager::RenderLineImpl(const LineRender& line)
    {
        if (line.GetWidth() != lineInterface->GetWidth())
        {
            EndLines();
            lineInterface->SetWidth(line.GetWidth());
            StartLines();
        }

        D3DXVECTOR2 points[] =
        {
            D3DXVECTOR2(static_cast<float>(line.GetFrom().GetX()), static_cast<float>(line.GetFrom().GetY())),
            D3DXVECTOR2(static_cast<float>(line.GetTo().GetX()), static_cast<float>(line.GetTo().GetY()))
        };
        lineInterface->SetWidth(line.GetWidth());
        lineInterface->Draw(points, 2, ColorToD3D(line.color));
    }

    void DX9GraphicsManager::SetupPresentationParameters()
    {
        // Discard old frames
        presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        presentationParameters.BackBufferCount = 1;
        // Set the back buffer format to 32-bit
        presentationParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
        presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        presentationParameters.MultiSampleQuality = 0;

        if (FindFpsSystem()->GetVSync())
            // Enable vsync
            presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
        else
            // Disable vsync
            presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    void DX9GraphicsManager::SetRenderStates()
    {
        HRESULT hr = device->SetRenderState(D3DRS_ZENABLE, false);
        if (hr != S_OK)
            FindLoggingSystem()->Log("DirectX9 Z buffer couldn't be set to false.",
                LogType::ERROR_SEVERE);
        hr = device->SetRenderState(D3DRS_LIGHTING, false);
        if (hr != S_OK)
            FindLoggingSystem()->Log("DirectX9 lighting couldn't be set to false.",
                LogType::ERROR_SEVERE);
        hr = device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
        if (hr != S_OK)
            FindLoggingSystem()->Log("DirectX9 alpha blend couldn't be set to true.",
                LogType::ERROR_SEVERE);
        hr = device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        if (hr != S_OK)
            FindLoggingSystem()->Log("DirectX9 source blend couldn't be set to source alpha.",
                LogType::ERROR_SEVERE);
        hr = device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        if (hr != S_OK)
            FindLoggingSystem()->Log("DirectX9 destination blend couldn't be set to inverse source alpha.",
                LogType::ERROR_SEVERE);
    }

    void DX9GraphicsManager::SetProjectionMatrix()
    {
        ScreenDimensions dimensions(GetCurrentDimensions());
        D3DXMatrixOrthoOffCenterLH(&projection, 0, static_cast<FLOAT>(dimensions.width), static_cast<FLOAT>(dimensions.height), 0, 0.0f, 1.0f);
    }

    void DX9GraphicsManager::OnResolutionChanged(const Agui::Resolution& arg)
    {
        SetMainDimensions(ScreenDimensions(static_cast<Dimension>(arg.GetWidth()), static_cast<Dimension>(arg.GetHeight())));
        Reinitialize();
    }

    FpsSystem* DX9GraphicsManager::FindFpsSystem() const
    {
        return objectManager->FindSystem<FpsSystem>();
    }

    LoggingSystem* DX9GraphicsManager::FindLoggingSystem() const
    {
        return objectManager->FindSystem<LoggingSystem>();
    }
}