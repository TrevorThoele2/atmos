
#include <array>
#include "DXGraphics.h"

#include <Windows.h>
#include <stdio.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
#include <DxErr.h>
#pragma comment(lib, "dxerr.lib")

#include "ImageAsset.h"

#include "Sprite.h"
#include "Shader.h"
#include "CanvasView.h"
#include "LineRender.h"
#include "LightFragment.h"

#include "Camera.h"
#include "FPS.h"
#include "Flags.h"
#include "SimpleFile.h"
#include "Environment.h"

#include "Logger.h"

#include <AGUI\System.h>

namespace Atmos
{
    D3DCOLOR ColorToD3D(const Color &col)
    {
        return D3DCOLOR_ARGB(col.alpha, col.red, col.green, col.blue);
    }

    class ImageAssetData : public ImageAsset::Data
    {
    public:
        LPDIRECT3DTEXTURE9 tex;

        ImageAssetData(LPDIRECT3DTEXTURE9 tex) : tex(tex)
        {}

        ~ImageAssetData()
        {
            tex->Release();
        }
    };

    class ShaderAssetData : public ShaderAsset::Data
    {
    public:
        LPD3DXEFFECT effect;

        ShaderAssetData(LPD3DXEFFECT effect) : effect(effect)
        {}

        ~ShaderAssetData()
        {
            effect->Release();
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

            ShaderAsset::PassT pass;
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
    private:
        D3DPRESENT_PARAMETERS presentationParameters;
        LPDIRECT3DSWAPCHAIN9 swapChain;
        LPDIRECT3DSURFACE9 backBuffer;
    public:
        RenderSurfaceData(LPDIRECT3DSURFACE9 backBuffer) : swapChain(nullptr), backBuffer(backBuffer)
        {}

        RenderSurfaceData(LPDIRECT3DSWAPCHAIN9 swapChain, LPDIRECT3DSURFACE9 backBuffer) : swapChain(swapChain), backBuffer(backBuffer)
        {
            swapChain->GetPresentParameters(&presentationParameters);
        }

        ~RenderSurfaceData()
        {
            Release();
        }

        void SetAsRenderTarget() override
        {
            Environment::GetGraphics<DX9GraphicsHandler>()->GetDevice()->SetRenderTarget(0, backBuffer);
        }

        void Present() override
        {
            auto hr = swapChain->Present(nullptr, nullptr, nullptr, nullptr, 0);
            if (hr != S_OK)
                Logger::Log(String("A swap chain failed when presenting.\n") + DXGetErrorDescription(hr),
                    Logger::Type::ERROR_SEVERE);
        }

        void Reset() override
        {
            presentationParameters.BackBufferWidth = 0;
            presentationParameters.BackBufferHeight = 0;

            HRESULT hr = Environment::GetGraphics<DX9GraphicsHandler>()->GetDevice()->CreateAdditionalSwapChain(&presentationParameters, &swapChain);
            if (hr != S_OK)
                Logger::Log(String("A DirectX device could not create an additional swap chain.\n") + DXGetErrorDescription(hr),
                    Logger::Type::ERROR_SEVERE);

            hr = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
            if (hr != S_OK)
                Logger::Log(String("A DirectX device could not retrieve the back bfufer from a swap chain.\n") + DXGetErrorDescription(hr),
                    Logger::Type::ERROR_SEVERE);
        }

        void Release() override
        {
            backBuffer->Release();
            backBuffer = nullptr;

            swapChain->Release();
            swapChain = nullptr;
        }

        RenderSurface::ScreenDimensions GetDimensions() override
        {
            RECT rect;
            GetClientRect(presentationParameters.hDeviceWindow, &rect);
            return RenderSurface::ScreenDimensions(rect.right, rect.bottom);
        }

        const D3DPRESENT_PARAMETERS& GetPresentationParameters() const
        {
            return presentationParameters;
        }
    };

    class CanvasData : public Canvas::Data
    {
    public:
        LPDIRECT3DTEXTURE9 tex;
        LPDIRECT3DSURFACE9 backBuffer;

        D3DLOCKED_RECT lockedRect;

        CanvasData(LPDIRECT3DTEXTURE9 tex) : tex(tex)
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
                Logger::Log("A canvas' back buffer could not be locked.",
                    Logger::Type::ERROR_LOW);
        }

        void StopPainting() override
        {
            if (backBuffer->UnlockRect() != S_OK)
                Logger::Log("A canvas' back buffer could not be unlocked.",
                    Logger::Type::ERROR_LOW);
        }

        void PaintPixel(const Canvas::PositionT &position, const Color &color, Canvas::Dimension height) override
        {
            DWORD *row = reinterpret_cast<DWORD*>(reinterpret_cast<char*>(lockedRect.pBits) + lockedRect.Pitch * position.second);
            row[position.first] = ColorToD3D(color);
        }

        void Clear(const Color &color) override
        {
            std::memset(lockedRect.pBits, ColorToD3D(color), lockedRect.Pitch * GetOwner()->GetHeight());
        }

        void Release() override
        {
            tex->Release();
        }

        void Reset(Canvas::Dimension width, Canvas::Dimension height) override
        {
            D3DXCreateTexture(Environment::GetGraphics<DX9GraphicsHandler>()->GetDevice(), width, height, D3DX_DEFAULT, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &tex);
            tex->GetSurfaceLevel(0, &backBuffer);
        }
    };

    class DX9GraphicsHandler::Renderer2D
    {
    private:
        struct Vertex
        {
            D3DXVECTOR2 position;
            D3DXVECTOR2 center;
            D3DCOLOR color;
            FLOAT u, v;
            Vertex() : position(0, 0), color(0), u(0.0f), v(0.0f) {}
            Vertex(D3DXVECTOR2 &&position, D3DCOLOR color, FLOAT u, FLOAT v) : position(std::move(position)), center(0.0f, 0.0f), color(color), u(u), v(v) {}
        };

        typedef unsigned short Index;

        class Object3D
        {
        private:
            void SetupQuad(float X, float Y, const Position2D &center, const Join2<float> &scaling, float rotation);
            void SetupRegularPolygon(float radius, unsigned int polyCount, float X, float Y, const Join2<float> &scaling, float rotation, const Color &color);

            static void SetupVertexCommon(Vertex &vertex, const D3DXMATRIX &matrix, const D3DXVECTOR2 &position, const D3DXVECTOR2 &center);
            static void SetupVertexCommon(Vertex &vertex, const D3DXMATRIX &matrix, const D3DXVECTOR2 &position, const D3DXVECTOR2 &center, const Color &color);
        public:
            std::vector<Vertex> vertices;
            std::vector<Index> indicies;
            unsigned int primCount;
            float z;
            LPDIRECT3DTEXTURE9 tex;
            const ShaderAsset *shader;

            //Object3D(const Sprite &sprite, float X, float Y, float Z, const ShaderAsset &shader);
            //Object3D(const CanvasView &view, float X, float Y, float Z, const ShaderAsset &shader);
            Object3D(LPDIRECT3DTEXTURE9 tex, const ShaderAsset &shader, float X, float Y, float Z, const AxisBoundingBox2D &imageBounds, const Size2D &size, const Position2D &center, const Join2<float> &scaling, const Angle &rotation, const Color &color);
            Object3D(Object3D &&arg);
            Object3D& operator=(Object3D &&arg);
        };
    private:
        typedef unsigned short BufferSize;

        static constexpr BufferSize maxVertexSprint = 4000;
        static constexpr BufferSize maxIndexSprint = 6000;

        LPDIRECT3DDEVICE9 device;
        LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
        LPDIRECT3DINDEXBUFFER9 indexBuffer;
        LPDIRECT3DVERTEXDECLARATION9 vertexDecl;
        std::unique_ptr<ShaderAsset> defaultTexturedSpriteShader;
        std::unique_ptr<ShaderAsset> defaultLightShader;

        typedef std::vector<Object3D> Objects;
        Objects objects;

        void InitBuffers();
    public:
        Renderer2D() = default;

        ~Renderer2D();
        void Init(DX9GraphicsHandler &handler, LPDIRECT3DDEVICE9 device);

        void Start(Objects::size_type size);
        // If the projection exists, then this will flush the sprites
        void Stop(const D3DXMATRIX *projection);
        void Flush(const D3DXMATRIX &projection);

        //void Draw(const Sprite &sprite, float X, float Y);
        //void Draw(const CanvasView &view, float X, float Y);
        void Draw(LPDIRECT3DTEXTURE9 tex, const ShaderAsset &shader, float X, float Y, float Z, const AxisBoundingBox2D &imageBounds, const Size2D &size, const Position2D &center, const Join2<float> &scaling, const Angle &rotation, const Color &color);

        void OnLostDevice();
        void OnResetDevice();

        ShaderAsset& GetDefaultTexturedSpriteShader();
    };

    void DX9GraphicsHandler::Renderer2D::Object3D::SetupQuad(float X, float Y, const Position2D &center, const Join2<float> &scaling, float rotation)
    {
        // Setup matrix and center
        D3DXVECTOR2 transformedCenter;
        D3DXMATRIX matrix;
        {
            D3DXVECTOR2 centerVector(center.GetX(), center.GetY());
            D3DXVECTOR2 positionVector(X, Y);
            D3DXVECTOR2 scalingVector(scaling.first, scaling.second);

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

    void DX9GraphicsHandler::Renderer2D::Object3D::SetupRegularPolygon(float radius, unsigned int polyCount, float X, float Y, const Join2<float> &scaling, float rotation, const Color &color)
    {
        const float angle = FULL_CIRCLE_RADIANS<float> / polyCount;
        const float useRadius = radius / std::cos(angle / 2);

        // Setup matrix and center
        D3DXVECTOR2 transformedCenter;
        D3DXMATRIX matrix;
        {
            D3DXVECTOR2 centerVector(useRadius, useRadius);
            D3DXVECTOR2 positionVector(X - useRadius, Y - useRadius);
            D3DXVECTOR2 scalingVector(scaling.first, scaling.second);

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
                SetupVertexCommon(vertices[loopPoly + 1], matrix, D3DXVECTOR2(useRadius + (std::cos(currentAngle) * useRadius), useRadius + (std::sin(currentAngle) * useRadius)), transformedCenter, color);
                setIndex(loopPoly);
                currentAngle += angle;
            }

            // Fix the second-to-last index
            indicies[indicies.size() - 2] = 1;
        }

        primCount = polyCount;
    }

    void DX9GraphicsHandler::Renderer2D::Object3D::SetupVertexCommon(Vertex &vertex, const D3DXMATRIX &matrix, const D3DXVECTOR2 &position, const D3DXVECTOR2 &center)
    {
        D3DXVec2TransformCoord(&vertex.position, &position, &matrix);

        vertex.center = center;
    }

    void DX9GraphicsHandler::Renderer2D::Object3D::SetupVertexCommon(Vertex &vertex, const D3DXMATRIX &matrix, const D3DXVECTOR2 &position, const D3DXVECTOR2 &center, const Color &color)
    {
        D3DXVec2TransformCoord(&vertex.position, &position, &matrix);
        vertex.center = center;
        vertex.color = ColorToD3D(color);
    }

    /*
    DX9GraphicsHandler::Renderer2D::Object3D::Object3D(const Sprite &sprite, float X, float Y, float Z, const ShaderAsset &shader) : z(Z), tex(sprite.GetMaterial()->GetXVisual()->GetData<ImageAssetData>()->tex), shader(&shader),
        vertices
        {
            Vertex(D3DXVECTOR2(0.0f, 0.0f),
                ColorToD3D(sprite.color),
                static_cast<float>(sprite.GetPrimaryAssetSlice().GetLeft() / sprite.GetPrimaryAssetSlice().GetWidth()),
                static_cast<float>(sprite.GetPrimaryAssetSlice().GetTop() / sprite.GetPrimaryAssetSlice().GetHeight())),
            Vertex(D3DXVECTOR2(sprite.GetPrimaryAssetSlice().GetWidth(), 0.0f),
                ColorToD3D(sprite.color),
                static_cast<float>(sprite.GetPrimaryAssetSlice().GetRight() / sprite.GetPrimaryAssetSlice().GetWidth()),
                static_cast<float>(sprite.GetPrimaryAssetSlice().GetTop() / sprite.GetPrimaryAssetSlice().GetHeight())),
            Vertex(D3DXVECTOR2(0.0f, sprite.GetPrimaryAssetSlice().GetHeight()),
                ColorToD3D(sprite.color),
                static_cast<float>(sprite.GetPrimaryAssetSlice().GetLeft() / sprite.GetPrimaryAssetSlice().GetWidth()),
                static_cast<float>(sprite.GetPrimaryAssetSlice().GetBottom() / sprite.GetPrimaryAssetSlice().GetHeight())),
            Vertex(D3DXVECTOR2(sprite.GetPrimaryAssetSlice().GetWidth(), sprite.GetPrimaryAssetSlice().GetHeight()),
                ColorToD3D(sprite.color),
                static_cast<float>(sprite.GetPrimaryAssetSlice().GetRight() / sprite.GetPrimaryAssetSlice().GetWidth()),
                static_cast<float>(sprite.GetPrimaryAssetSlice().GetBottom() / sprite.GetPrimaryAssetSlice().GetHeight()))
        }
    {
        SetupQuad(X, Y, sprite.GetPrimaryAssetSlice().GetCenter(), Join2<float>(sprite.GetSize().GetXScaler(), sprite.GetSize().GetYScaler()), sprite.GetXRotation().AsRadians());
    }

    DX9GraphicsHandler::Renderer2D::Object3D::Object3D(const CanvasView &view, float X, float Y, float Z, const ShaderAsset &shader) : z(Z), tex(view.GetCanvas().GetData<CanvasData>()->tex), shader(&shader),
        vertices
        {
            Vertex(D3DXVECTOR2(0.0f, 0.0f),
                D3DCOLOR_ARGB(255, 255, 255, 255),
                0.0f,
                0.0f),
            Vertex(D3DXVECTOR2(view.GetWidth(), 0.0f),
                D3DCOLOR_ARGB(255, 255, 255, 255),
                1.0f,
                0.0f),
            Vertex(D3DXVECTOR2(0.0f, view.GetHeight()),
                D3DCOLOR_ARGB(255, 255, 255, 255),
                0.0f,
                1.0f),
            Vertex(D3DXVECTOR2(view.GetWidth(), view.GetHeight()),
                D3DCOLOR_ARGB(255, 255, 255, 255),
                1.0f,
                1.0f)
        }
    {
        SetupQuad(X, Y, Position2D(view.GetWidth() / 2, view.GetHeight() / 2), Join2<float>(1.0f, 1.0f), 0.0f);
    }
    */

    DX9GraphicsHandler::Renderer2D::Object3D::Object3D(LPDIRECT3DTEXTURE9 tex, const ShaderAsset &shader, float X, float Y, float Z, const AxisBoundingBox2D &imageBounds, const Size2D &size, const Position2D &center, const Join2<float> &scaling, const Angle &rotation, const Color &color) : tex(tex), z(Z), shader(&shader),
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
        SetupQuad(X, Y, center, scaling, rotation.AsRadians());
    }

    DX9GraphicsHandler::Renderer2D::Object3D::Object3D(Object3D &&arg) : vertices(std::move(arg.vertices)), indicies(std::move(arg.indicies)), primCount(std::move(arg.primCount)), z(std::move(arg.z)), tex(std::move(arg.tex)), shader(std::move(arg.shader))
    {}

    DX9GraphicsHandler::Renderer2D::Object3D& DX9GraphicsHandler::Renderer2D::Object3D::operator=(Object3D &&arg)
    {
        vertices = std::move(arg.vertices);
        indicies = std::move(arg.indicies);
        primCount = std::move(arg.primCount);
        z = std::move(arg.z);
        tex = std::move(arg.tex);
        shader = std::move(arg.shader);
        return *this;
    }

    void DX9GraphicsHandler::Renderer2D::InitBuffers()
    {
        device->CreateVertexBuffer(maxVertexSprint * sizeof(Vertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vertexBuffer, nullptr);
        device->CreateIndexBuffer(maxIndexSprint * sizeof(Index), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &indexBuffer, nullptr);

        D3DVERTEXELEMENT9 vElements[] = {
            { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1 },
            { 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            D3DDECL_END()
        };

        device->CreateVertexDeclaration(vElements, &vertexDecl);
    }

    DX9GraphicsHandler::Renderer2D::~Renderer2D()
    {
        vertexBuffer->Release();
        indexBuffer->Release();
        vertexDecl->Release();
    }

    void DX9GraphicsHandler::Renderer2D::Init(DX9GraphicsHandler &handler, LPDIRECT3DDEVICE9 device)
    {
        this->device = device;
        InitBuffers();

        defaultTexturedSpriteShader.reset(new ShaderAsset(std::move(handler.CreateShader(Environment::GetFileSystem()->GetExePath().Append("Shaders\\TexturedSprite.fx").GetValue()))));
        defaultLightShader.reset(new ShaderAsset(std::move(handler.CreateShader(Environment::GetFileSystem()->GetExePath().Append("Shaders\\Light.fx").GetValue()))));
    }

    void DX9GraphicsHandler::Renderer2D::Start(Objects::size_type size)
    {
        if (size != 0)
            objects.reserve(size);
    }

    void DX9GraphicsHandler::Renderer2D::Stop(const D3DXMATRIX *projection)
    {
        if (projection)
            Flush(*projection);

        objects.clear();
    }

    void DX9GraphicsHandler::Renderer2D::Flush(const D3DXMATRIX &projection)
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
                        return left.shader < right.shader;
                    else
                        return leftTex < rightTex;
                }
                else
                    return leftZ < rightZ;
            };

            std::sort(objects.begin(), objects.end(), sorter);
        }

        device->SetVertexDeclaration(vertexDecl);
        device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
        device->SetIndices(indexBuffer);

        LPDIRECT3DTEXTURE9 focusedTex = nullptr;
        const ShaderAsset *focusedShader = nullptr;
        const ShaderAssetData *focusedShaderImpl = nullptr;

        auto setTexture = [&](LPDIRECT3DTEXTURE9 set)
        {
            focusedTex = set;
            if (focusedTex && focusedShaderImpl)
                focusedShaderImpl->effect->SetTexture("g_Texture", focusedTex);
        };

        auto setShader = [&](const ShaderAsset *set)
        {
            focusedShader = set;
            focusedShaderImpl = focusedShader->GetData<ShaderAssetData>();
            focusedShaderImpl->effect->SetMatrix("g_Projection", &projection);

            FLOAT screenSize[2] = { static_cast<FLOAT>(Camera::GetSize().GetWidth()), static_cast<FLOAT>(Camera::GetSize().GetHeight()) };
            focusedShaderImpl->effect->SetFloatArray("g_ScreenSize", screenSize, 2);

            if (focusedTex && focusedShaderImpl)
                focusedShaderImpl->effect->SetTexture("g_Texture", focusedTex);
        };

        setShader(objects.begin()->shader);
        setTexture(objects.begin()->tex);

        void *vertexData = nullptr;
        void *indexData = nullptr;
        vertexBuffer->Lock(0, 0, &vertexData, D3DLOCK_DISCARD);
        indexBuffer->Lock(0, 0, &indexData, D3DLOCK_DISCARD);
        
        BufferSize vertexCount = 0;
        BufferSize indexCount = 0;
        BufferSize primCount = 0;

        auto drawPrims = [&](bool endOfDraw)
        {
            vertexBuffer->Unlock();
            indexBuffer->Unlock();

            const ShaderAsset::PassT passCount = focusedShaderImpl->Begin();
            ShaderAsset::PassT passCountCurrent = 0;

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
            const ShaderAsset* loopShader = loop->shader;

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

    /*
    void DX9GraphicsHandler::Renderer2D::Draw(const Sprite &sprite, float X, float Y)
    {
        
    }

    void DX9GraphicsHandler::Renderer2D::Draw(const CanvasView &view, float X, float Y)
    {
        
    }
    */

    void DX9GraphicsHandler::Renderer2D::Draw(LPDIRECT3DTEXTURE9 tex, const ShaderAsset &shader, float X, float Y, float Z, const AxisBoundingBox2D &imageBounds, const Size2D &size, const Position2D &center, const Join2<float> &scaling, const Angle &rotation, const Color &color)
    {
        objects.push_back(Object3D(tex, shader, X, Y, Z, imageBounds, size, center, scaling, rotation, color));
    }

    void DX9GraphicsHandler::Renderer2D::OnLostDevice()
    {
        vertexBuffer->Release();
        indexBuffer->Release();
        defaultTexturedSpriteShader->GetData<ShaderAssetData>()->effect->OnLostDevice();
        defaultLightShader->GetData<ShaderAssetData>()->effect->OnLostDevice();
    }

    void DX9GraphicsHandler::Renderer2D::OnResetDevice()
    {
        InitBuffers();
        defaultTexturedSpriteShader->GetData<ShaderAssetData>()->effect->OnResetDevice();
        defaultLightShader->GetData<ShaderAssetData>()->effect->OnResetDevice();
    }

    ShaderAsset& DX9GraphicsHandler::Renderer2D::GetDefaultTexturedSpriteShader()
    {
        return *defaultTexturedSpriteShader.get();
    }

    void DX9GraphicsHandler::ReinitializeImpl()
    {
        SetupPresentationParameters();

        // Destroy interfaces
        renderer2D->OnLostDevice();

        HRESULT hr = lineInterface->OnLostDevice();
        if(hr != S_OK)
            Logger::Log(String("The DirectX line interface was not able to be released.\n") + DXGetErrorDescription(hr),
                Logger::Type::ERROR_SEVERE);

        // Reset
        hr = device->Reset(&presentationParameters);
        if (hr == D3DERR_DEVICELOST)
            return;
        else if (hr != S_OK)
            Logger::Log(String("The DirectX device failed when resetting.\n") + DXGetErrorDescription(hr),
                Logger::Type::ERROR_SEVERE);

        // Recreate interfaces
        renderer2D->OnResetDevice();
        hr = lineInterface->OnResetDevice();
        if (hr != S_OK)
            Logger::Log(String("The DirectX line interface was not able to be reset.\n") + DXGetErrorDescription(hr),
                Logger::Type::ERROR_SEVERE);
        
        // Reset render states
        SetRenderStates();
    }

    void DX9GraphicsHandler::SetMainDimensionsImpl(const ScreenDimensions &dimensions)
    {
        presentationParameters.BackBufferWidth = dimensions.first;
        presentationParameters.BackBufferHeight = dimensions.second;
        SetProjectionMatrix();
    }

    DX9GraphicsHandler::ScreenDimensions DX9GraphicsHandler::GetMainDimensionsImpl() const
    {
        return ScreenDimensions(presentationParameters.BackBufferWidth, presentationParameters.BackBufferHeight);
    }

    ImageAsset DX9GraphicsHandler::CreateImageImpl(const FilePath &path, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        D3DXIMAGE_INFO info;
        HRESULT hr = D3DXGetImageInfoFromFile(path.c_str(), &info);
        if (hr != S_OK)
            Logger::Log("An image asset could not be created.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Path", path.GetValue()) });

        LPDIRECT3DTEXTURE9 tex;
        hr = D3DXCreateTextureFromFileEx(device, path.c_str(), info.Width, info.Height, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &tex);
        if (hr != S_OK)
            Logger::Log("An image asset could not be created.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Path", path.GetValue()) });

        return ImageAsset(new ImageAssetData(tex), path.GetFileName(), cols, rows, info.Width, info.Height);
    }

    ImageAsset DX9GraphicsHandler::CreateImageImpl(void *buffer, std::int32_t size, const FileName &name, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        D3DXIMAGE_INFO info;
        HRESULT hr = D3DXGetImageInfoFromFileInMemory(buffer, size, &info);
        if (hr != S_OK)
            Logger::Log("An image asset could not be created.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });

        LPDIRECT3DTEXTURE9 tex;
        hr = D3DXCreateTextureFromFileInMemoryEx(device, buffer, size, info.Width, info.Height, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &tex);
        if (hr != S_OK)
            Logger::Log("An image asset could not be created.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return ImageAsset(new ImageAssetData(tex), name, cols, rows, info.Width, info.Height);
    }

    ShaderAsset DX9GraphicsHandler::CreateShaderImpl(const FilePath &path)
    {
        LPD3DXEFFECT effect;
        HRESULT hr = D3DXCreateEffectFromFile(device, path.c_str(), nullptr, nullptr, D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER, nullptr, &effect, nullptr);
        if (hr != S_OK)
            Logger::Log("A shader asset could not be created.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Path", path.GetValue()) });

        return ShaderAsset(new ShaderAssetData(effect), path.GetFileName());
    }

    ShaderAsset DX9GraphicsHandler::CreateShaderImpl(void *buffer, std::int32_t size, const FileName &name)
    {
        LPD3DXEFFECT effect;
        HRESULT hr = D3DXCreateEffect(device, buffer, size, nullptr, nullptr, D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER, nullptr, &effect, nullptr);
        if (hr != S_OK)
            Logger::Log("A shader asset could not be created.",
                Logger::Type::ERROR_SEVERE,
                Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });

        return ShaderAsset(new ShaderAssetData(effect), name);
    }

    RenderSurface DX9GraphicsHandler::CreateRenderSurfaceImpl(void *window)
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
            Logger::Log("A render surface could not be created.",
                Logger::Type::ERROR_SEVERE);

        LPDIRECT3DSURFACE9 backBuffer;
        hr = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
        if (hr != S_OK)
            Logger::Log("A render surface could not be created.",
                Logger::Type::ERROR_SEVERE);

        return RenderSurface(new RenderSurfaceData(swapChain, backBuffer));
    }

    Canvas DX9GraphicsHandler::CreateCanvasImpl(const ScreenDimensions &dimensions)
    {
        LPDIRECT3DTEXTURE9 tex;
        HRESULT hr = D3DXCreateTexture(device, dimensions.first, dimensions.second, D3DX_DEFAULT, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &tex);
        if (hr != S_OK)
            Logger::Log("A canvas could not be created.",
                Logger::Type::ERROR_SEVERE);

        return Canvas(new CanvasData(tex), dimensions.first, dimensions.second);
    }

    bool DX9GraphicsHandler::CanMakeImageImpl(const FilePath &path) const
    {
        D3DXIMAGE_INFO info;
        return D3DXGetImageInfoFromFile(path.c_str(), &info) == S_OK;
    }

    bool DX9GraphicsHandler::CanMakeImageImpl(void *buffer, std::int32_t size) const
    {
        D3DXIMAGE_INFO info;
        return D3DXGetImageInfoFromFileInMemory(buffer, size, &info) == S_OK;
    }

    void DX9GraphicsHandler::ResizeCanvasImpl(Canvas &canvas, const ScreenDimensions &dimensions)
    {
        LPDIRECT3DTEXTURE9 tex;
        HRESULT hr = D3DXCreateTexture(device, dimensions.first, dimensions.second, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &tex);
        if (hr != S_OK)
            Logger::Log("Resizing of a canvas was attempted and failed.",
                Logger::Type::ERROR_SEVERE);

        canvas.GetData<CanvasData>()->tex->Release();
        canvas.GetData<CanvasData>()->tex = tex;
    }

    void DX9GraphicsHandler::SetRenderTargetImpl(RenderSurface &set)
    {
        SetProjectionMatrix();
    }

    void DX9GraphicsHandler::SetRenderTargetToMainImpl()
    {
        device->SetRenderTarget(0, mainSurface);
        SetProjectionMatrix();
    }

    void DX9GraphicsHandler::ReleaseMainRenderTarget()
    {
        mainSurface->Release();
    }

    void DX9GraphicsHandler::ResetMainRenderTarget()
    {
        HRESULT hr = device->GetRenderTarget(0, &mainSurface);
        if (hr != S_OK)
            Logger::Log("The main render target is unretrievable.",
                Logger::Type::ERROR_SEVERE);
    }

    void DX9GraphicsHandler::PresentImpl()
    {
        HRESULT hr = device->Present(nullptr, nullptr, nullptr, nullptr);
        if (hr != S_OK)
            Logger::Log("The frame was unpresentable.",
                Logger::Type::ERROR_SEVERE);
    }

    void DX9GraphicsHandler::PresentImpl(void *windowOverride)
    {
        HRESULT hr = device->Present(nullptr, nullptr, static_cast<HWND>(windowOverride), nullptr);
        if (hr != S_OK)
            Logger::Log("The frame was unpresentable.",
                Logger::Type::ERROR_SEVERE);
    }

    void DX9GraphicsHandler::RenderSpriteImpl(const Sprite &sprite, float X, float Y)
    {
        AssetReference<Material> material(sprite.GetMaterial());
        if (!material)
            return;

        RenderObject(
            material->GetXVisual()->GetData<ImageAssetData>()->tex,
            (sprite.GetShaderPatch()) ? sprite.GetShaderPatch() : material->GetShader(),
            X,
            Y,
            sprite.GetZ(),
            AxisBoundingBox2D(sprite.GetPrimaryAssetSlice().GetLeft() / sprite.GetMaterial()->GetXVisual()->GetWidth(),
                sprite.GetPrimaryAssetSlice().GetTop() / sprite.GetMaterial()->GetXVisual()->GetHeight(),
                sprite.GetPrimaryAssetSlice().GetRight() / sprite.GetMaterial()->GetXVisual()->GetWidth(),
                sprite.GetPrimaryAssetSlice().GetBottom() / sprite.GetMaterial()->GetXVisual()->GetHeight(),
                false),
            sprite.GetPrimaryAssetSlice().GetSize(),
            sprite.GetPrimaryAssetSlice().GetCenter(),
            Join2<float>(sprite.GetScalers().first, sprite.GetScalers().second),
            sprite.GetXRotation(),
            sprite.color);
    }

    void DX9GraphicsHandler::RenderCanvasViewImpl(const CanvasView &view, float X, float Y)
    {
        RenderObject(
            view.GetCanvas().GetData<CanvasData>()->tex,
            &renderer2D->GetDefaultTexturedSpriteShader(),
            X,
            Y,
            view.GetZ(),
            AxisBoundingBox2D(0.0f, 0.0f, 1.0f, 1.0f),
            Size2D(view.GetWidth(), view.GetHeight()),
            Position2D(view.GetWidth() / 2, view.GetHeight() / 2),
            Join2<float>(view.GetScalers().first, view.GetScalers().second),
            view.GetXRotation(),
            Color(255, 255, 255, 255));
    }

    void DX9GraphicsHandler::RenderUnknownFragmentImpl(const RenderFragment &fragment, float X, float Y)
    {}

    void DX9GraphicsHandler::RenderLineImpl(const LineRender &line)
    {
        if (line.GetWidth() != lineInterface->GetWidth())
        {
            EndLines();
            lineInterface->SetWidth(line.GetWidth());
            StartLines();
        }

        D3DXVECTOR2 points[] = { D3DXVECTOR2(static_cast<float>(line.GetFrom().GetX()), static_cast<float>(line.GetFrom().GetY())), D3DXVECTOR2(static_cast<float>(line.GetTo().GetX()), static_cast<float>(line.GetTo().GetY())) };
        lineInterface->SetWidth(line.GetWidth());
        lineInterface->Draw(points, 2, ColorToD3D(line.color));
    }

    void DX9GraphicsHandler::SetupPresentationParameters()
    {
        // Discard old frames
        presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        presentationParameters.BackBufferCount = 1;
        // Set the back buffer format to 32-bit
        presentationParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
        presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        presentationParameters.MultiSampleQuality = 0;

        if (FpsHandler::GetVSync())
            // Enable vsync
            presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
        else
            // Disable vsync
            presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    void DX9GraphicsHandler::SetRenderStates()
    {
        HRESULT hr = device->SetRenderState(D3DRS_ZENABLE, false);
        if (hr != S_OK)
            Logger::Log("DirectX9 Z buffer couldn't be set to false.",
                Logger::Type::ERROR_MODERATE);
        hr = device->SetRenderState(D3DRS_LIGHTING, false);
        if (hr != S_OK)
            Logger::Log("DirectX9 lighting couldn't be set to false.",
                Logger::Type::ERROR_MODERATE);
        hr = device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
        if (hr != S_OK)
            Logger::Log("DirectX9 alpha blend couldn't be set to true.",
                Logger::Type::ERROR_MODERATE);
        hr = device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        if (hr != S_OK)
            Logger::Log("DirectX9 source blend couldn't be set to false.",
                Logger::Type::ERROR_MODERATE);
        hr = device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        if (hr != S_OK)
            Logger::Log("DirectX9 destination blend couldn't be set to false.",
                Logger::Type::ERROR_MODERATE);
    }

    void DX9GraphicsHandler::SetProjectionMatrix()
    {
        ScreenDimensions dimensions(GetCurrentDimensions());
        D3DXMatrixOrthoOffCenterLH(&projection, 0, static_cast<FLOAT>(dimensions.first), static_cast<FLOAT>(dimensions.second), 0, 0.0f, 1.0f);
    }

    void DX9GraphicsHandler::OnResolutionChanged(const agui::Resolution &arg)
    {
        SetMainDimensions(ScreenDimensions(static_cast<Dimension>(arg.GetWidth()), static_cast<Dimension>(arg.GetHeight())));
        Reinitialize();
    }

    void DX9GraphicsHandler::RenderObject(LPDIRECT3DTEXTURE9 tex, const AssetReference<ShaderAsset> &shader, float X, float Y, float Z, const AxisBoundingBox2D &imageBounds, const Size2D &size, const Position2D &center, const Join2<float> &scaling, const Angle &rotation, const Color &color)
    {
        AssetReference<ShaderAsset> useShader(shader);
        if (!useShader)
            useShader = renderer2D->GetDefaultTexturedSpriteShader();

        renderer2D->Draw(tex, *useShader, X, Y, Z, imageBounds, size, center, scaling, rotation, color);
    }

    DX9GraphicsHandler::DX9GraphicsHandler(HWND hwnd, const Join2<UINT> &backbuffer, bool fullscreen, bool subscribeEvents) : hwnd(hwnd), renderer2D(new Renderer2D())
    {
        ZeroMemory(&presentationParameters, sizeof(presentationParameters));

        SetMainDimensions(backbuffer);
        SetFullscreen(fullscreen);
        if (subscribeEvents)
            agui::System::Instance().eventResolutionChanged.Subscribe(std::bind(&DX9GraphicsHandler::OnResolutionChanged, this, std::placeholders::_1));

        // Create the Direct3D interface
        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        presentationParameters.hDeviceWindow = hwnd;
        SetupPresentationParameters();

        d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentationParameters, &device);
        renderer2D->Init(*this, device);
        D3DXCreateLine(device, &lineInterface);

        SetRenderStates();

        device->GetRenderTarget(0, &mainSurface);
    }

    DX9GraphicsHandler::~DX9GraphicsHandler()
    {
        d3d->Release();
        device->Release();
        lineInterface->Release();
    }

    void DX9GraphicsHandler::SetFullscreen(bool set)
    {
        (set) ? presentationParameters.Windowed = false : presentationParameters.Windowed = true;
    }

    void DX9GraphicsHandler::ClearTarget(const Flags<Target> &target)
    {
        DWORD pass = 0;
        if (target.Get(Target::MAIN))
            pass |= D3DCLEAR_TARGET;

        if (target.Get(Target::STENCIL))
            pass |= D3DCLEAR_STENCIL;

        device->Clear(0, nullptr, pass, 0, 1.0f, 0);
    }

    void DX9GraphicsHandler::ClearTarget(const Flags<Target> &target, const Color &color)
    {
        DWORD pass = 0;
        if (target.Get(Target::MAIN))
            pass |= D3DCLEAR_TARGET;

        if (target.Get(Target::STENCIL))
            pass |= D3DCLEAR_STENCIL;

        device->Clear(0, nullptr, pass, ColorToD3D(color), 1.0f, 0);
    }

    void DX9GraphicsHandler::Flush()
    {
        renderer2D->Flush(projection);
    }

    void DX9GraphicsHandler::SetRenderState(RenderState state, bool set)
    {
        D3DRENDERSTATETYPE pass;

        if (state == RenderState::STENCIL)
            pass = D3DRS_STENCILENABLE;

        DWORD tf;
        (set) ? tf = TRUE : tf = FALSE;

        device->SetRenderState(pass, tf);
    }

    bool DX9GraphicsHandler::Start()
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

    void DX9GraphicsHandler::End()
    {
        device->EndScene();
    }

    void DX9GraphicsHandler::StartSprites(size_t spriteCount)
    {
        renderer2D->Start(spriteCount);
    }

    void DX9GraphicsHandler::EndSprites()
    {
        renderer2D->Stop(&projection);
    }

    void DX9GraphicsHandler::StartLines()
    {
        lineInterface->Begin();
    }

    void DX9GraphicsHandler::EndLines()
    {
        lineInterface->End();
    }

    void DX9GraphicsHandler::StartStencil()
    {
        device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NEVER);

        device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_REPLACE);
        device->SetRenderState(D3DRS_STENCILREF, 1);
        device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
    }

    void DX9GraphicsHandler::StopStencil()
    {
        Flush();

        device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

        device->SetRenderState(D3DRS_STENCILREF, 0);
        device->SetRenderState(D3DRS_STENCILWRITEMASK, 0);
    }

    LPDIRECT3DDEVICE9& DX9GraphicsHandler::GetDevice()
    {
        return device;
    }
}