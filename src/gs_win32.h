#ifndef GS_WIN32_H

#include "gs_input.h"

#ifdef DEBUG
#define DEBUG_GET_MESSAGE_NAME(string, message) sprintf(string, message);
#else
#define DEBUG_GET_MESSAGE_NAME(string, message)
#endif

struct win32_state
{
    b32 Initialized;
    b32 Running;
};

struct win32_window_info
{
    char* Name;
    char* ClassName;
    s32 Width;
    s32 Height;
    WNDPROC WindowEventsHandler; // If this is left null, Win32HandleWindowsEvents will be used
};

struct win32_opengl_window_info
{
    s32 ColorBits;
    s32 AlphaBits;
    s32 DepthBits;
    HDC DeviceContext;
    HGLRC RenderContext;
};

struct win32_window
{
    win32_window_info Info;
    
    WNDCLASS Class;
    HWND Handle;
    
    // TODO(peter): Make this a union?
    win32_opengl_window_info OpenGLInfo;
};

struct handle_window_msg_result
{
    b32 NeedsUpdate;
#ifdef DEBUG
    char MessageType[128];
#endif
};

struct win32_memory_op_result
{
    u8* Base;
    s32 Size;
    b32 Success; /// TODO(Peter): turn this into an error code/enum?
};

global_variable win32_state GlobalWin32State;

// Windowing & Graphics
internal void         InitializeWin32();
internal win32_window CreateWin32Window (char* WindowName, char* WindowClassName, s32 Width, s32 Height);
LRESULT CALLBACK      Win32HandleWindowsEvents (HWND WindowHandle, UINT Msg, WPARAM wParam, LPARAM lParam);
internal handle_window_msg_result HandleWindowsMessage (HWND WindowHandle, MSG Message);

// Memory
internal win32_memory_op_result Win32Alloc(s32 Size);
internal b32                    Win32Free(u8* Base, s32 Size);

// File IO
internal win32_memory_op_result ReadEntireFile(char* Path);
internal b32                    WriteEntireFile(char* Path, u8* Contents, s32 Size);

internal void
InitializeWin32 ()
{
    GlobalWin32State = {};
    GlobalWin32State.Running = false;
    GlobalWin32State.Initialized = true;
}

internal win32_window
CreateWin32Window (HINSTANCE HInstance, win32_window_info Info)
{
    win32_window Result = {};
    Result.Info = Info;
    
    Result.Class = {};
    Result.Class.style = CS_HREDRAW | CS_VREDRAW;
    if (Info.WindowEventsHandler)
    {
        Result.Class.lpfnWndProc = Info.WindowEventsHandler;
    }
    else
    {
        Result.Class.lpfnWndProc = Win32HandleWindowsEvents;
    }
    Result.Class.hInstance = HInstance;
    Result.Class.lpszClassName = Info.ClassName;
    
    if (RegisterClass(&Result.Class))
    {
        Result.Handle = CreateWindowEx(
            0,
            Result.Class.lpszClassName,
            Info.Name, 
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            Info.Width,
            Info.Height,
            0,
            0, 
            HInstance,
            0);
    }
    
    return Result;
};

internal void
CreateOpenGLWindowContext (win32_opengl_window_info Info, win32_window* Window)
{
    Info.DeviceContext = GetDC(Window->Handle);
    
    // Setup pixel format
    {
        PIXELFORMATDESCRIPTOR PixelFormatDesc = { 0 };
        // TODO: Program seems to work perfectly fine without all other params except dwFlags.
        //       Can we skip other params for the sake of brevity?
        PixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        PixelFormatDesc.nVersion = 1;
        PixelFormatDesc.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        PixelFormatDesc.iPixelType = PFD_TYPE_RGBA;// TODO(Peter): include this in win32_opengl_window_info?
        PixelFormatDesc.cColorBits = Info.ColorBits;
        PixelFormatDesc.cAlphaBits = Info.AlphaBits;
        PixelFormatDesc.cDepthBits = Info.DepthBits;
        PixelFormatDesc.dwLayerMask = PFD_MAIN_PLANE; // TODO(Peter): include this in win32_opengl_window_info?
        //
        
        s32 PixelFormat = ChoosePixelFormat(Info.DeviceContext, &PixelFormatDesc);
        if (!PixelFormat) { InvalidCodePath; } // TODO: Log: Choose pixel format failed
        if (!SetPixelFormat(Info.DeviceContext, PixelFormat, &PixelFormatDesc)) { InvalidCodePath; } // TODO: Log: Set pixel format failed
    }
    
    // Create rendering context
    {
        // TODO: Create "proper" context?
        //       https://www.opengl.org/wiki/Creating_an_OpenGL_Context_(WGL)#Proper_Context_Creation
        
        Info.RenderContext = wglCreateContext(Info.DeviceContext);
        wglMakeCurrent(Info.DeviceContext, Info.RenderContext);
        
        // TODO(Peter): do we want this?
        /*
            glGetIntegerv(GL_MAJOR_VERSION, );
            glGetIntegerv(GL_MINOR_VERSION, );
            (char*)glGetString(GL_VENDOR);
            (char*)glGetString(GL_RENDERER);
            */
    }
    
    Window->OpenGLInfo = Info;
}

struct handle_window_event_result
{
    LRESULT Result;
    b32 Handled;
};

internal handle_window_event_result
HandleWindowEventUnlessWouldUseDefault (HWND WindowHandle, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    handle_window_event_result Result = {};
    Result.Handled = false;
    
    switch (Msg)
    {
        case WM_SIZE:
        {
            //ResizeDIBSection();
            Result.Handled = true;
        }break;
        
        case WM_CLOSE:
        {
            Result.Result = DefWindowProc(WindowHandle, Msg, wParam, lParam);
            Result.Handled = true;
        }break;
        
        case WM_DESTROY:
        {
            GlobalWin32State.Running = false;
            Result.Handled = true;
        }break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT PaintStruct;
            HDC DeviceContext;
            b32 PaintResult;
            
            DeviceContext = BeginPaint(WindowHandle, &PaintStruct);
            PaintResult = EndPaint(WindowHandle, &PaintStruct);
            Result.Handled = true;
        }break;
    }
    
    return Result;
}

LRESULT CALLBACK
Win32HandleWindowsEvents (
HWND WindowHandle,
UINT Msg,
WPARAM wParam,
LPARAM lParam
)
{
    handle_window_event_result EventResult = HandleWindowEventUnlessWouldUseDefault(
        WindowHandle, 
        Msg, 
        wParam, 
        lParam);
    
    if (!EventResult.Handled)
    {
        EventResult.Result = DefWindowProc(WindowHandle, Msg, wParam, lParam);
    }
    
    return EventResult.Result;
}

static int
Win32GetKeyIndex (int Win32VirtualKey, bool NumpadValid, bool TranslateToChar)
{
    int Result = -1;
    
    if (!TranslateToChar)
    {
        if      (Win32VirtualKey == VK_SPACE)       { Result = (int)KeyCode_Space; } 
        else if (Win32VirtualKey == VK_TAB)         { Result = (int)KeyCode_Tab; }
    }
    
    if (Win32VirtualKey == VK_CAPITAL)     { Result = (int)KeyCode_CapsLock; }
    else if (Win32VirtualKey == VK_LSHIFT)      { Result = (int)KeyCode_LeftShift; } 
    else if (Win32VirtualKey == VK_RSHIFT)      { Result = (int)KeyCode_RightShift; }
    else if (Win32VirtualKey == VK_LCONTROL)    { Result = (int)KeyCode_LeftCtrl; } 
    else if (Win32VirtualKey == VK_RCONTROL)    { Result = (int)KeyCode_RightCtrl; }
    
    // TODO(Peter): support the function key?
    //else if (Win32VirtualKey == VK_) { Result = (int)KeyCode_Fn; } 
    
    else if (Win32VirtualKey == VK_MENU)        { Result = (int)KeyCode_Alt; } 
    else if (Win32VirtualKey == VK_PRIOR)       { Result = (int)KeyCode_PageUp; } 
    else if (Win32VirtualKey == VK_NEXT)        { Result = (int)KeyCode_PageDown; }
    else if (Win32VirtualKey == VK_BACK)        { Result = (int)KeyCode_Backspace; } 
    else if (Win32VirtualKey == VK_DELETE)      { Result = (int)KeyCode_Delete; }
    else if (Win32VirtualKey == VK_RETURN)      { Result = (int)KeyCode_Enter; }
    
    else if (Win32VirtualKey == VK_F1)  { Result = (int)KeyCode_F1; } 
    else if (Win32VirtualKey == VK_F2)  { Result = (int)KeyCode_F2; } 
    else if (Win32VirtualKey == VK_F3)  { Result = (int)KeyCode_F3; } 
    else if (Win32VirtualKey == VK_F4)  { Result = (int)KeyCode_F4; } 
    else if (Win32VirtualKey == VK_F5)  { Result = (int)KeyCode_F5; } 
    else if (Win32VirtualKey == VK_F6)  { Result = (int)KeyCode_F6; } 
    else if (Win32VirtualKey == VK_F7)  { Result = (int)KeyCode_F7; }
    else if (Win32VirtualKey == VK_F8)  { Result = (int)KeyCode_F8; } 
    else if (Win32VirtualKey == VK_F9)  { Result = (int)KeyCode_F9; } 
    else if (Win32VirtualKey == VK_F10) { Result = (int)KeyCode_F10; } 
    else if (Win32VirtualKey == VK_F11) { Result = (int)KeyCode_F11; } 
    else if (Win32VirtualKey == VK_F12) { Result = (int)KeyCode_F12; } 
    
    if (!TranslateToChar)
    {
        if      (Win32VirtualKey == 0x30) { Result = (int)KeyCode_0; } 
        else if (Win32VirtualKey == 0x31) { Result = (int)KeyCode_1; } 
        else if (Win32VirtualKey == 0x32) { Result = (int)KeyCode_2; } 
        else if (Win32VirtualKey == 0x33) { Result = (int)KeyCode_3; } 
        else if (Win32VirtualKey == 0x34) { Result = (int)KeyCode_4; } 
        else if (Win32VirtualKey == 0x35) { Result = (int)KeyCode_5; } 
        else if (Win32VirtualKey == 0x36) { Result = (int)KeyCode_6; } 
        else if (Win32VirtualKey == 0x37) { Result = (int)KeyCode_7; }
        else if (Win32VirtualKey == 0x38) { Result = (int)KeyCode_8; } 
        else if (Win32VirtualKey == 0x39) { Result = (int)KeyCode_9; }
        
        else if (Win32VirtualKey == 0x41) { Result = (int)KeyCode_A; } 
        else if (Win32VirtualKey == 0x42) { Result = (int)KeyCode_B; } 
        else if (Win32VirtualKey == 0x43) { Result = (int)KeyCode_C; } 
        else if (Win32VirtualKey == 0x44) { Result = (int)KeyCode_D; } 
        else if (Win32VirtualKey == 0x45) { Result = (int)KeyCode_E; } 
        else if (Win32VirtualKey == 0x46) { Result = (int)KeyCode_F; } 
        else if (Win32VirtualKey == 0x47) { Result = (int)KeyCode_G; } 
        else if (Win32VirtualKey == 0x48) { Result = (int)KeyCode_H; }
        else if (Win32VirtualKey == 0x49) { Result = (int)KeyCode_I; } 
        else if (Win32VirtualKey == 0x4A) { Result = (int)KeyCode_J; } 
        else if (Win32VirtualKey == 0x4B) { Result = (int)KeyCode_K; } 
        else if (Win32VirtualKey == 0x4C) { Result = (int)KeyCode_L; } 
        else if (Win32VirtualKey == 0x4D) { Result = (int)KeyCode_M; } 
        else if (Win32VirtualKey == 0x4E) { Result = (int)KeyCode_N; } 
        else if (Win32VirtualKey == 0x4F) { Result = (int)KeyCode_O; } 
        else if (Win32VirtualKey == 0x50) { Result = (int)KeyCode_P; } 
        else if (Win32VirtualKey == 0x51) { Result = (int)KeyCode_Q; } 
        else if (Win32VirtualKey == 0x52) { Result = (int)KeyCode_R; } 
        else if (Win32VirtualKey == 0x53) { Result = (int)KeyCode_S; } 
        else if (Win32VirtualKey == 0x54) { Result = (int)KeyCode_T; } 
        else if (Win32VirtualKey == 0x55) { Result = (int)KeyCode_U; } 
        else if (Win32VirtualKey == 0x56) { Result = (int)KeyCode_V; } 
        else if (Win32VirtualKey == 0x57) { Result = (int)KeyCode_W; } 
        else if (Win32VirtualKey == 0x58) { Result = (int)KeyCode_X; } 
        else if (Win32VirtualKey == 0x59) { Result = (int)KeyCode_Y; } 
        else if (Win32VirtualKey == 0x5A) { Result = (int)KeyCode_Z; }
    }
    
    if (NumpadValid)
    {
        if      (Win32VirtualKey == VK_NUMPAD0) { Result = (int)KeyCode_Num0; } 
        else if (Win32VirtualKey == VK_NUMPAD1) { Result = (int)KeyCode_Num1; } 
        else if (Win32VirtualKey == VK_NUMPAD2) { Result = (int)KeyCode_Num2; } 
        else if (Win32VirtualKey == VK_NUMPAD3) { Result = (int)KeyCode_Num3; } 
        else if (Win32VirtualKey == VK_NUMPAD4) { Result = (int)KeyCode_Num4; } 
        else if (Win32VirtualKey == VK_NUMPAD5) { Result = (int)KeyCode_Num5; } 
        else if (Win32VirtualKey == VK_NUMPAD6) { Result = (int)KeyCode_Num6; } 
        else if (Win32VirtualKey == VK_NUMPAD7) { Result = (int)KeyCode_Num7; } 
        else if (Win32VirtualKey == VK_NUMPAD8) { Result = (int)KeyCode_Num8; } 
        else if (Win32VirtualKey == VK_NUMPAD9) { Result = (int)KeyCode_Num9; }
    }
    
    if (Win32VirtualKey == VK_UP)    { Result = (int)KeyCode_UpArrow; }
    else if (Win32VirtualKey == VK_DOWN)  { Result = (int)KeyCode_DownArrow; }
    else if (Win32VirtualKey == VK_LEFT)  { Result = (int)KeyCode_LeftArrow; }
    else if (Win32VirtualKey == VK_RIGHT) { Result = (int)KeyCode_RightArrow; }
    
    return Result;
}

internal handle_window_msg_result
HandleWindowsMessage (
HWND WindowHandle,
MSG Message)
{
    handle_window_msg_result Result = {};
    Result.NeedsUpdate = 0;
    
    switch (Message.message)
    {
        case WM_HOTKEY:
        {
            DEBUG_GET_MESSAGE_NAME(Result.MessageType, "WM_HOTKEY ");
        }break;
        
        case WM_MOUSEWHEEL:
        {
            DEBUG_GET_MESSAGE_NAME(Result.MessageType, "WM_MOUSEWHEEL ");
            int MouseWheel = GET_WHEEL_DELTA_WPARAM(Message.wParam);
            /*
            Input.New->MouseScroll = MouseWheel;
            Result.NeedsUpdate = true;
            */
        }break;
        
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        {
            DEBUG_GET_MESSAGE_NAME(Result.MessageType, "WM_MOUSEBUTTON ");
            
            /*
            Input.New->KeyStates[KeyCode_MouseLeftButton]   = (GetKeyState(VK_LBUTTON) & (1 << 15)) != 0;
            Input.New->KeyStates[KeyCode_MouseMiddleButton] = (GetKeyState(VK_MBUTTON) & (1 << 15)) != 0;
            Input.New->KeyStates[KeyCode_MouseRightButton]  = (GetKeyState(VK_RBUTTON) & (1 << 15)) != 0;
            // NOTE(Peter): If you decide to support extra mouse buttons, on windows the key codes are
            // VK_XBUTTON1 and VK_XBUTTON2
            
            if (KeyTransitionedDown(KeyCode_MouseLeftButton, Input))
            {
                Input.MouseDownX = Input.New->MouseX;
                Input.MouseDownY = Input.New->MouseY;
            }
            Result.NeedsUpdate = true;*/
        }break;
        
        case WM_MOUSEMOVE:
        {
            DEBUG_GET_MESSAGE_NAME(Result.MessageType, "WM_MOUSEMOVE ");
            POINT MousePos;
            GetCursorPos(&MousePos);
            ScreenToClient(WindowHandle, &MousePos);
            
            /*
            Input.New->MouseX = MousePos.x;
            Input.New->MouseY = App.WindowHeight - MousePos.y;
            
            Result.NeedsUpdate = true;
            */
        }break;
        
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            int VirtualKey = (int)Message.wParam;
            bool KeyDown = (Message.lParam & (1 << 31)) == 0;
            int KeyIndex = Win32GetKeyIndex(VirtualKey, true, true);
            /*
            if (KeyIndex >= 0)
            {
                DEBUG_GET_MESSAGE_NAME(Result.MessageType, "WM_KEYEvent ");
                Input.New->KeyStates[KeyIndex] = KeyDown;
                Result.NeedsUpdate = true;
            }
            else
            {
                if (Input.TranslateInputToCharValues && KeyDown)
                {
                    // NOTE(Peter): Took this out b/c we're translating the WM_CHAR messages
                    // in the message pump, and if we do it here as well, character producing
                    // key messages get put on the message queue twice
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                else
                {
                    DEBUG_GET_MESSAGE_NAME(Result.MessageType, "WM_KEYEvent ");
                    // NOTE(Peter): This is so that when you lift up a key that was generating a WM_CHAR,
                    // the app still has a chance to respond to it.
                    Result.NeedsUpdate = true;
                }
            }
            */
        }break;
        
        case WM_CHAR:
        {
            DEBUG_GET_MESSAGE_NAME(Result.MessageType, "WM_CHAR ");
            /*
            char TranslatedChar = (char)Message.wParam;
            int KeyIndex = GetKeyIndexFromChar(TranslatedChar);
            
            if (KeyIndex >= 0)
            {
                // NOTE(Peter): Always setting this to true becuase windows is stupid and doesn't
                // pass the press/release bit through correctly. So now the KEYDOWN/KEYUP Messages above
                // only translate the message to a WM_CHAR message if its a key down. Since we clear all
                // keystates to false at the beginning of an input frame, this will make transitions 
                // get registered correctly.
                Input.New->KeyStates[KeyIndex] = true;
                Result.NeedsUpdate = true;
            }
            else
            {
                printf("Translated Char Not Recognized: %c\n", TranslatedChar); 
                //InvalidCodePath;
            }
            */
        }break;
        
        default:
        {
            DEBUG_GET_MESSAGE_NAME(Result.MessageType, "Unhandled WM Event ");
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }break;
    }
    
    return Result;
}

// Memory
internal win32_memory_op_result 
Win32Alloc(s32 Size)
{
    win32_memory_op_result Result = {};
    Result.Success = false;
    
    Result.Base = (u8*)VirtualAlloc(NULL, Size, 
                                    MEM_COMMIT | MEM_RESERVE, 
                                    PAGE_EXECUTE_READWRITE);
    if (Result.Base)
    {
        Result.Size = Size;
        Result.Success = true;
    }
    
    return Result;
}

internal b32
Win32Free(u8* Base, s32 Size)
{
    b32 Result = VirtualFree(Base, Size, MEM_RELEASE);
    return Result;
}

// File IO
internal win32_memory_op_result 
ReadEntireFile(char* Path)
{
    win32_memory_op_result Result = {};
    Result.Success = false;
    
    HANDLE FileHandle = CreateFileA (
        Path,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD FileSize = GetFileSize(FileHandle, NULL);
        Result.Base = (u8*)VirtualAlloc(NULL, FileSize, MEM_COMMIT | MEM_RESERVE,
                                        PAGE_EXECUTE_READWRITE);
        if (Result.Base) 
        {
            Result.Size = FileSize;
            
            s32 BytesRead = 0;
            if (ReadFile(FileHandle, (LPVOID)Result.Base, FileSize, (LPDWORD)(&BytesRead), NULL))
            {
                Result.Success = true;
            }
            else
            {
                Result.Size = 0;
            }
        }
        CloseHandle(FileHandle);
    }
    else
    {
        // TODO(Peter): failure
    }
    
    return Result;
}

internal b32
WriteEntireFile (char* Path, u8* Contents, s32 Size)
{
    b32 Result = false;
    HANDLE FileHandle = CreateFileA (
        Path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD BytesWritten = 0;
        
        b32 WriteSuccess = WriteFile(FileHandle,
                                     Contents, Size,
                                     &BytesWritten,
                                     NULL);
        
        if (WriteSuccess && BytesWritten == (u32)Size)
        {
            CloseHandle(FileHandle);
            Result = true;
        }
        else
        {
            Result = false;
        }
    }
    else
    {
        Result = false;
    }
    
    return Result;
}

#define GS_WIN32_H
#endif // GS_WIN32_H