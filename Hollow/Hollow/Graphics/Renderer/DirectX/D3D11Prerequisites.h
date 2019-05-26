#pragma once

#ifndef D3D11_LIBS
#define D3D11_LIBS
#pragma comment( lib, "d3d11.lib")
#pragma comment( lib, "D3DCompiler.lib")
#endif

#include <d3d11.h>
#include "Hollow/Platform.h"

class D3D11VertexBuffer;
class D3D11RenderTarget;
class D3D11DepthStencil;
class D3D11PixelShader;
class D3D11VertexShader;
class D3D11ConstantBuffer;
class D3D11BlendState;
class D3D11SamplerState;
class D3D11RasterizerState;
class D3D11Context;
class D3D11TextureManager;
class D3D11ShaderManager;
class D3D11HardwareBufferManager;
class D3D11RenderApi;
class D3D11Shader;
class D3D11WindowManager;
class D3D11BufferManager;
class D3D11RenderTargetManager;