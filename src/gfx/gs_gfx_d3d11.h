/* date = May 22nd 2021 10:35 pm */

#ifndef GS_GFX_D3D11_H
#define GS_GFX_D3D11_H

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxguid.lib")

internal void
D3D11WindowCreateGraphicsContext(gs_window Window, gs_gfx_context_desc* Desc, gs_gfx_handler GfxHandler)
{
    gs_window_context_win32* WCtx = (gs_window_context_win32*)GfxHandler.WindowUserData;
    HWND WindowHandle = WCtx->WindowHandles[Window.WindowId];
    HDC DeviceContext = GetDC(WindowHandle);
    
    // Create a reference device
    gs_window_dim WindowDim = WindowGetDim(GfxHandler, Window);
    
    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {
        .BufferCount = 1,
        .BufferDesc = {
            .Width = WindowDim.Width,
            .Height = WindowDim.Height,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .RefreshRate = {
                .Numerator = 60,
                .Denominator = 1,
            },
        },
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .OutputWindow = WindowHandle,
        .SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
        .SampleDesc = {
            .Count = 1,
            .Quality = 0,
        },
        .Windowed = true,
    };
    
    UINT CreateFlags = 0;
    AddFlag(CreateFlags, D3D11_CREATE_DEVICE_SINGLETHREADED);
#if DEBUG
    AddFlag(CreateFlags, D3D11_CREATE_DEVICE_DEBUG);
#endif
    
    // TODO(PS): I think we're going to want to hold onto these somewhere
    D3D_FEATURE_LEVEL FeatureLevel;
    IDXGISwapChain* SwapChain;
    ID3D11Device* D3DDevice;
    ID3D11DeviceContext* D3DDeviceContext;
    
    HRESULT HResult = D3D11CreateDeviceAndSwapChain(NULL, // pAdapter (use default)
                                                    D3D_DRIVER_TYPE_HARDWARE, // Drivertype
                                                    NULL, // Software
                                                    CreateFlags, // Flags
                                                    NULL, // pFeatureLevels
                                                    0, // FeatureLevels
                                                    D3D11_SDK_VERSION,
                                                    &SwapChainDesc,
                                                    &SwapChain,
                                                    &D3DDevice,
                                                    &FeatureLevel,
                                                    &D3DDeviceContext);
    Assert(SUCCEEDED(HResult));
    Assert(SwapChain && D3DDevice && D3DDeviceContext);
    
    // Create Render Target
    
    return;
}

internal void
GfxHandlerGetBackendCmds(gs_gfx_handler* GfxHandler)
{
    GfxHandler->PWindowCreateGraphicsContext = D3D11WindowCreateGraphicsContext;
    GfxHandler->BackendUserData = 0;
}

#endif //GS_GFX_D3D11_H
