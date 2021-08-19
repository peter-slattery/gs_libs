/* date = May 22nd 2021 2:28 pm */

#ifndef GS_GFX_WIN32_H
#define GS_GFX_WIN32_H


#ifdef __cplusplus
extern "C" {
#endif
  
#ifndef GS_GFX_DONT_FORCE_DISCRETE_GPU
  // Use discrete GPU by default.
  // http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
  __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
  
  // https://gpuopen.com/learn/amdpowerxpressrequesthighperformance/
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif
  
#ifdef __cplusplus
}
#endif

#define GFX_MAX_WINDOWS 32
typedef struct gs_window_context_win32
{
  HWND WindowHandles[GFX_MAX_WINDOWS];
  b8 WindowsShouldClose[GFX_MAX_WINDOWS];
  gs_window_dim WindowDims[GFX_MAX_WINDOWS];
  u64 WindowCount;
} gs_window_context_win32;

global gs_window_context_win32 Win32WindowCtx;

internal gs_window
Win32WindowCtxPushWindow(gs_window_context_win32* WCtx, HWND Handle)
{
  gs_window Result = {};
  if (WCtx->WindowCount < (GFX_MAX_WINDOWS + 1))
  {
    Result.WindowId = ++WCtx->WindowCount;
  }
  else
  {
    for (u64 i = 0; i < GFX_MAX_WINDOWS; i++)
    {
      if (WCtx->WindowHandles[i] == INVALID_HANDLE_VALUE)
      {
        Result.WindowId = i;
        break;
      }
    }
  }
  
  // NOTE(PS): if this fires, you need to increase the size of win32_max_windows
  // (or question why you have that many windows XD)
  Assert(Result.WindowId < GFX_MAX_WINDOWS);
  WCtx->WindowHandles[Result.WindowId] = Handle;
  
  return Result;
}

internal u64 
Win32WindowCtxGetWindowIdFromHWND(gs_window_context_win32* WCtx, HWND WindowHandle)
{
  u64 Result = 0;
  for (u64 i = 0; i <= WCtx->WindowCount; i++)
  {
    if (WCtx->WindowHandles[i] == WindowHandle)
    {
      Result = i;
      break;
    }
  }
  return Result;
}

LRESULT CALLBACK
Win32WindowEventHandler(HWND WindowHandle, UINT Msg, WPARAM WParam, LPARAM LParam)
{
  LRESULT Result = 0;
  
  u64 WindowId = Win32WindowCtxGetWindowIdFromHWND(&Win32WindowCtx, WindowHandle);
  
  switch (Msg)
  {
    case WM_NCCREATE:
    case WM_CREATE:
    case WM_ACTIVATE: // TODO(PS): we probably want to log this somewhere
    {
      Result = DefWindowProc(WindowHandle, Msg, WParam, LParam);
    }break;
    
    case WM_SIZE:
    {
      RECT ClientRect;
      GetClientRect(WindowHandle, &ClientRect);
      
      gs_window_dim NewDim = {
        .Width = ClientRect.right - ClientRect.left,
        .Height = ClientRect.bottom - ClientRect.top,
      };
      Win32WindowCtx.WindowDims[WindowId] = NewDim;
    }break;
    
    case WM_CLOSE:
    {
      Win32WindowCtx.WindowsShouldClose[WindowId] = true;
    }break;
    
    case WM_DESTROY:
    {
      Result = DefWindowProc(WindowHandle, Msg, WParam, LParam);
      
      Win32WindowCtx.WindowHandles[WindowId] = (HWND)INVALID_HANDLE_VALUE;
      Win32WindowCtx.WindowsShouldClose[WindowId] = false;
    }break;
    
    case WM_PAINT:
    {
      PAINTSTRUCT PaintStruct;
      HDC DeviceContext;
      b32 PaintResult;
      
      DeviceContext = BeginPaint(WindowHandle, &PaintStruct);
      PaintResult = EndPaint(WindowHandle, &PaintStruct);
    }break;
    
    default:
    {
      Result = DefWindowProc(WindowHandle, Msg, WParam, LParam);
    } break;
  }
  
  return Result;
}

internal gs_window
Win32WindowOpen(gs_window_desc* Desc, u8* UserData)
{
  gs_window Result = {};
  
  HINSTANCE HInstance = GetModuleHandle(NULL);
  
  WNDCLASSEX WindowClass = {
    .cbSize = sizeof(WindowClass),
    .style = CS_HREDRAW | CS_VREDRAW,
    .lpfnWndProc = Win32WindowEventHandler,
    .hInstance = HInstance,
    .lpszClassName = Desc->WindowName
  };
  
  if (RegisterClassEx(&WindowClass))
  {
    DWORD ExtendedStyleFlags = 0;
    
    DWORD Style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    if (HasFlag(Desc->StyleFlags, WindowStyle_Maximized)) Style |= WS_MAXIMIZE;
    
    s32 X = CW_USEDEFAULT;
    s32 Y = CW_USEDEFAULT;
    
    HWND ParentWindowHandle = 0;
    HMENU MenuHandle = 0;
    LPVOID Param = 0;
    
    HWND WindowHandle = CreateWindowEx(ExtendedStyleFlags, WindowClass.lpszClassName, Desc->WindowName, Style, X, Y, Desc->Width, Desc->Height, ParentWindowHandle, MenuHandle, HInstance, Param);
    if (WindowHandle != NULL)
    {
      Result = Win32WindowCtxPushWindow((gs_window_context_win32*)UserData, WindowHandle);
      
      gs_window_dim NewDim = {
        .Width = (s32)Desc->Width,
        .Height = (s32)Desc->Height,
      };
      Win32WindowCtx.WindowDims[Result.WindowId] = NewDim;
    }
    else
    {
      u32 Error = GetLastError();
      switch (Error)
      {
        InvalidDefaultCase;
      }
    }
  }
  else
  {
    u32 Error = GetLastError();
    switch (Error)
    {
      InvalidDefaultCase;
    }
  }
  
  return Result;
}

internal bool
Win32WindowClose(gs_window* Window, u8* UserData)
{
  gs_window_context_win32* WCtx = (gs_window_context_win32*)(UserData);
  HWND WindowHandle = WCtx->WindowHandles[Window->WindowId];
  
  bool Result = DestroyWindow(WindowHandle);
  if (!Result)
  {
    u32 Error = GetLastError();
    switch (Error)
    {
      InvalidDefaultCase;
    }
  }
  return Result;
}

internal bool
Win32WindowShouldClose(gs_window Window, u8* UserData)
{
  gs_window_context_win32* WCtx = (gs_window_context_win32*)(UserData);
  bool Result = WCtx->WindowsShouldClose[Window.WindowId];
  return Result;
}

internal gs_window_dim
Win32WindowGetDim(gs_window Window, u8* UserData)
{
  gs_window_context_win32* WCtx = (gs_window_context_win32*)(UserData);
  gs_window_dim Result = WCtx->WindowDims[Window.WindowId];
  return Result;
}

internal gs_key_code
Win32TranslateKeyCode(int VKey)
{
  gs_key_code Result = KeyCode_Invalid;
  
  switch (VKey)
  {
    case VK_ESCAPE:      { Result = KeyCode_Esc; } break;
    case VK_SPACE:       { Result = KeyCode_Space; } break;
    case VK_CAPITAL:     { Result = KeyCode_CapsLock; } break;
    case VK_TAB:         { Result = KeyCode_Tab; } break;
    case VK_LSHIFT:      { Result = KeyCode_LeftShift; } break;
    case VK_RSHIFT:      { Result = KeyCode_RightShift; } break;
    case VK_LCONTROL:    { Result = KeyCode_LeftCtrl; } break;
    case VK_RCONTROL:    { Result = KeyCode_RightCtrl; } break;
    
    // TODO(Peter:: support the function key?
    //case VK_: { Result = KeyCode_Fn; } break;
    
    case VK_MENU:        { Result = KeyCode_Alt; } break;
    case VK_PRIOR:       { Result = KeyCode_PageUp; } break;
    case VK_NEXT:        { Result = KeyCode_PageDown; } break;
    case VK_BACK:        { Result = KeyCode_Backspace; } break;
    case VK_DELETE:      { Result = KeyCode_Delete; } break;
    case VK_RETURN:      { Result = KeyCode_Enter; } break;
    
    case VK_F1:  { Result = KeyCode_F1; } break;
    case VK_F2:  { Result = KeyCode_F2; } break;
    case VK_F3:  { Result = KeyCode_F3; } break;
    case VK_F4:  { Result = KeyCode_F4; } break;
    case VK_F5:  { Result = KeyCode_F5; } break;
    case VK_F6:  { Result = KeyCode_F6; } break;
    case VK_F7:  { Result = KeyCode_F7; } break;
    case VK_F8:  { Result = KeyCode_F8; } break;
    case VK_F9:  { Result = KeyCode_F9; } break;
    case VK_F10: { Result = KeyCode_F10; } break;
    case VK_F11: { Result = KeyCode_F11; } break;
    case VK_F12: { Result = KeyCode_F12; } break;
    
    case 0x30: { Result = KeyCode_0; } break;
    case 0x31: { Result = KeyCode_1; } break;
    case 0x32: { Result = KeyCode_2; } break;
    case 0x33: { Result = KeyCode_3; } break;
    case 0x34: { Result = KeyCode_4; } break;
    case 0x35: { Result = KeyCode_5; } break;
    case 0x36: { Result = KeyCode_6; } break;
    case 0x37: { Result = KeyCode_7; } break;
    case 0x38: { Result = KeyCode_8; } break;
    case 0x39: { Result = KeyCode_9; } break;
    
    case 0x41: { Result = KeyCode_A; } break;
    case 0x42: { Result = KeyCode_B; } break;
    case 0x43: { Result = KeyCode_C; } break;
    case 0x44: { Result = KeyCode_D; } break;
    case 0x45: { Result = KeyCode_E; } break;
    case 0x46: { Result = KeyCode_F; } break;
    case 0x47: { Result = KeyCode_G; } break;
    case 0x48: { Result = KeyCode_H; } break;
    case 0x49: { Result = KeyCode_I; } break;
    case 0x4A: { Result = KeyCode_J; } break;
    case 0x4B: { Result = KeyCode_K; } break;
    case 0x4C: { Result = KeyCode_L; } break;
    case 0x4D: { Result = KeyCode_M; } break;
    case 0x4E: { Result = KeyCode_N; } break;
    case 0x4F: { Result = KeyCode_O; } break;
    case 0x50: { Result = KeyCode_P; } break;
    case 0x51: { Result = KeyCode_Q; } break;
    case 0x52: { Result = KeyCode_R; } break;
    case 0x53: { Result = KeyCode_S; } break;
    case 0x54: { Result = KeyCode_T; } break;
    case 0x55: { Result = KeyCode_U; } break;
    case 0x56: { Result = KeyCode_V; } break;
    case 0x57: { Result = KeyCode_W; } break;
    case 0x58: { Result = KeyCode_X; } break;
    case 0x59: { Result = KeyCode_Y; } break;
    case 0x5A: { Result = KeyCode_Z; } break;
    
    case VK_NUMPAD0: { Result = KeyCode_Num0; } break;
    case VK_NUMPAD1: { Result = KeyCode_Num1; } break;
    case VK_NUMPAD2: { Result = KeyCode_Num2; } break;
    case VK_NUMPAD3: { Result = KeyCode_Num3; } break;
    case VK_NUMPAD4: { Result = KeyCode_Num4; } break;
    case VK_NUMPAD5: { Result = KeyCode_Num5; } break;
    case VK_NUMPAD6: { Result = KeyCode_Num6; } break;
    case VK_NUMPAD7: { Result = KeyCode_Num7; } break;
    case VK_NUMPAD8: { Result = KeyCode_Num8; } break;
    case VK_NUMPAD9: { Result = KeyCode_Num9; } break;
    
    case VK_UP:    { Result = KeyCode_UpArrow; } break;
    case VK_DOWN:  { Result = KeyCode_DownArrow; } break;
    case VK_LEFT:  { Result = KeyCode_LeftArrow; } break;
    case VK_RIGHT: { Result = KeyCode_RightArrow; } break;
    
    InvalidDefaultCase;
  }
  
  return Result;
}

internal void
Win32HandleWindowMessage (gs_window_context_win32* WCtx, gs_window Window, HWND WindowHandle, MSG Message, gs_window_events* Events)
{
  switch (Message.message)
  {
    case WM_MOUSEWHEEL:
    {
      Events->MouseScroll = (s32)GET_WHEEL_DELTA_WPARAM(Message.wParam);
    }break;
    
    case WM_LBUTTONDOWN:
    {
      InputButtonSetDown(Events->MouseButtonStates, MouseButton_Left);
      
      // :Win32MouseEventCapture
      // NOTE(Peter): We capture events when the mouse goes down so that
      // if the user drags outside the window, we still get the mouse up
      // event and can process it. Otherwise, we can get into cases where
      // an event was started, didn't end, but the user can click again and
      // try to start the event again.
      // We relase event capture on mouse up.
      SetCapture(WindowHandle);
    }break;
    
    case WM_MBUTTONDOWN:
    {
      InputButtonSetDown(Events->MouseButtonStates, MouseButton_Middle);
      SetCapture(WindowHandle); // :Win32MouseEventCapture
    }break;
    
    case WM_RBUTTONDOWN:
    {
      InputButtonSetDown(Events->MouseButtonStates, MouseButton_Right);
      SetCapture(WindowHandle); // :Win32MouseEventCapture
    }break;
    
    case WM_LBUTTONUP:
    {
      InputButtonSetUp(Events->MouseButtonStates, MouseButton_Left);
      ReleaseCapture(); // :Win32MouseEventCapture
    }break;
    
    case WM_MBUTTONUP:
    {
      InputButtonSetUp(Events->MouseButtonStates, MouseButton_Middle);
      ReleaseCapture(); // :Win32MouseEventCapture
    }break;
    
    case WM_RBUTTONUP:
    {
      InputButtonSetUp(Events->MouseButtonStates, MouseButton_Right);
      ReleaseCapture(); // :Win32MouseEventCaptureb32
    }break;
    
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    {
      int VirtualKey = (int)Message.wParam;
      gs_key_code Key = Win32TranslateKeyCode(VirtualKey);
      
      b32 KeyWasDown = (Message.lParam & (1 << 30)) != 0;
      b32 KeyIsDown = (Message.lParam & (1 << 31)) == 0;
      
      if (!KeyWasDown && KeyIsDown)
      {
        InputButtonSetDown(Events->KeyStates, Key);
      }
      else if (KeyWasDown && KeyIsDown)
      {
        InputButtonSetUp(Events->KeyStates, Key);
      }
      
      // NOTE(PS): translate the key presses anyways because 
      // it allows us to construct a textual representation of
      // what the user has typed, using the os to handle the 
      // keyboard->text translation
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    }break;
    
    case WM_CHAR:
    {
      char CharTyped = (char)Message.wParam;
      InputAppendTypedChar(Events, CharTyped);
    }break;
    
    default:
    {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    }break;
  }
}

internal bool
Win32WindowPollEvents (gs_window Window, gs_window_events* Events, u8* UserData)
{
  gs_window_context_win32* WCtx = (gs_window_context_win32*)(UserData);
  HWND WindowHandle = WCtx->WindowHandles[Window.WindowId];
  
  MSG Message;
  while (PeekMessageA(&Message, WindowHandle, 0, 0, PM_REMOVE))
  {
    Win32HandleWindowMessage(WCtx, Window, WindowHandle, Message, Events);
  }
  
  return true;
}

internal void
Win32WindowSwapBuffers(gs_window Window, u8* UserData)
{
  gs_window_context_win32* WCtx = (gs_window_context_win32*)(UserData);
  HWND WindowHandle = Win32WindowCtx.WindowHandles[Window.WindowId];
  HDC DeviceContext = GetDC(WindowHandle);
  SwapBuffers(DeviceContext);
  ReleaseDC(WindowHandle, DeviceContext);
}

internal void
GfxHandlerGetPlatformWindowCmds(gs_gfx_handler* GfxHandler, gs_allocator A)
{
  GfxHandler->PWindowOpen = Win32WindowOpen;
  GfxHandler->PWindowClose = Win32WindowClose;
  GfxHandler->PWindowShouldClose = Win32WindowShouldClose;
  GfxHandler->PWindowGetDim = Win32WindowGetDim;
  GfxHandler->PWindowPollEvents = Win32WindowPollEvents;
  GfxHandler->PWindowSwapBuffers = Win32WindowSwapBuffers;
  GfxHandler->WindowUserData = (u8*)&Win32WindowCtx;
}

#endif //GS_GFX_WIN32_H
