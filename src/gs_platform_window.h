//
// File: gs_window.h
// Author: Peter Slattery
// Creation Date: 2020-03-30
//
#ifndef GS_WINDOW_H

typedef struct gs_window gs_window;

// TODO(Peter): More options
static gs_window gs_CreateWindow(char* Title, int Width, int Height);
static bool gs_WindowIsOpen(gs_window Window);
static void gs_CloseWindow(gs_window* Window);
static bool gs_TakeNextInputEvent(gs_window Window, gs_input_event* Event);

#if defined(GS_WINDOW_WIN32) || defined(GS_PLATFORM_WIN32)

#define gs_Win32PrintLastError() gs_Win32PrintLastError_(__FILE__, __LINE__)
static void
gs_Win32PrintLastError_(char* File, u32 Line)
{
    char DebugStringData[256];
    gs_string DebugString = MakeString(DebugStringData, 0, 256);
    u32 Error = GetLastError();
    PrintF(&DebugString, "%s Line %d: Error %d\n\0", File, Line, Error);
    OutputDebugStringA(DebugString.Str);
}

struct gs_window
{
    int Width, Height;
    HWND WindowHandle;
    HDC DeviceContext;
};

LRESULT CALLBACK
gs_Win32DefaultWindowEventHandler (HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = {0};
    
    switch (Message)
    {
        case WM_SIZE:
        case WM_DESTROY:
        case WM_PAINT:
        case WM_CLOSE:
        default:
        {
            Result = DefWindowProc(Handle, Message, WParam, LParam);
        }break;
    }
    
    return Result;
}

static HINSTANCE
gs_Win32GetModuleHandle()
{
    HINSTANCE Result = GetModuleHandle(NULL);
    if (Result == NULL)
    {
        gs_Win32PrintLastError();
    }
    return Result;
}

static gs_window
gs_Win32CreateWindow(char* Title, int Width, int Height, WNDPROC EventHandler)
{
    gs_window Result = {0};
    
    HINSTANCE HInstance = gs_Win32GetModuleHandle();
    
    WNDCLASS WindowClass = {0};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    if (EventHandler != 0)
    {
        WindowClass.lpfnWndProc = EventHandler;
    }
    else
    {
        WindowClass.lpfnWndProc = gs_Win32DefaultWindowEventHandler;
    }
    WindowClass.hInstance = HInstance;
    WindowClass.lpszClassName = Title;
    
    if (RegisterClass(&WindowClass))
    {
        Result.WindowHandle = CreateWindowEx(0,
                                             WindowClass.lpszClassName,
                                             Title,
                                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                             CW_USEDEFAULT,
                                             CW_USEDEFAULT,
                                             Width,
                                             Height,
                                             0,
                                             0,
                                             HInstance,
                                             0);
        Result.DeviceContext = GetDC(Result.WindowHandle);
        Result.Width = Width;
        Result.Height = Height;
    }
    else
    {
        gs_Win32PrintLastError();
    }
    
    return Result;
}

static gs_window
gs_CreateWindow(char* Title, int Width, int Height)
{
    gs_window Result = gs_Win32CreateWindow(Title, Width, Height, 0);
    return Result;
}

static bool
gs_WindowIsOpen(gs_window Window)
{
    bool Result = IsWindow(Window.WindowHandle);
    return Result;
}

static void
gs_CloseWindow(gs_window* Window)
{
    ReleaseDC(Window->WindowHandle, Window->DeviceContext);
    DestroyWindow(Window->WindowHandle);
    *Window = {0};
}


// @Cleanup: turn this into a lookup table
// @MetaCleanup: turn this into a lookup table that you generate
static gs_key
gs_TranslateWin32KeyCode(int Win32Key)
{
    gs_key Result = gs_Key_Invalid;
    
    if (Win32Key == VK_ESCAPE) { Result = gs_Key_Esc; }
    if (Win32Key == VK_SPACE)  { Result = gs_Key_Space; }
    
    if (Win32Key == VK_CAPITAL)     { Result = gs_Key_CapsLock; }
    else if (Win32Key == VK_TAB)         { Result = gs_Key_Tab; }
    else if (Win32Key == VK_LSHIFT)      { Result = gs_Key_LeftShift; }
    else if (Win32Key == VK_RSHIFT)      { Result = gs_Key_RightShift; }
    else if (Win32Key == VK_LCONTROL)    { Result = gs_Key_LeftCtrl; }
    else if (Win32Key == VK_RCONTROL)    { Result = gs_Key_RightCtrl; }
    
    // TODO(Peter): support the function key?
    //else if (Win32Key == VK_) { Result = gs_Key_Fn; }
    
    else if (Win32Key == VK_MENU)        { Result = gs_Key_Alt; }
    else if (Win32Key == VK_PRIOR)       { Result = gs_Key_PageUp; }
    else if (Win32Key == VK_NEXT)        { Result = gs_Key_PageDown; }
    else if (Win32Key == VK_BACK)        { Result = gs_Key_Backspace; }
    else if (Win32Key == VK_DELETE)      { Result = gs_Key_Delete; }
    else if (Win32Key == VK_RETURN)      { Result = gs_Key_Enter; }
    
    else if (Win32Key == VK_F1)  { Result = gs_Key_F1; }
    else if (Win32Key == VK_F2)  { Result = gs_Key_F2; }
    else if (Win32Key == VK_F3)  { Result = gs_Key_F3; }
    else if (Win32Key == VK_F4)  { Result = gs_Key_F4; }
    else if (Win32Key == VK_F5)  { Result = gs_Key_F5; }
    else if (Win32Key == VK_F6)  { Result = gs_Key_F6; }
    else if (Win32Key == VK_F7)  { Result = gs_Key_F7; }
    else if (Win32Key == VK_F8)  { Result = gs_Key_F8; }
    else if (Win32Key == VK_F9)  { Result = gs_Key_F9; }
    else if (Win32Key == VK_F10) { Result = gs_Key_F10; }
    else if (Win32Key == VK_F11) { Result = gs_Key_F11; }
    else if (Win32Key == VK_F12) { Result = gs_Key_F12; }
    
    if      (Win32Key == 0x30) { Result = gs_Key_0; }
    else if (Win32Key == 0x31) { Result = gs_Key_1; }
    else if (Win32Key == 0x32) { Result = gs_Key_2; }
    else if (Win32Key == 0x33) { Result = gs_Key_3; }
    else if (Win32Key == 0x34) { Result = gs_Key_4; }
    else if (Win32Key == 0x35) { Result = gs_Key_5; }
    else if (Win32Key == 0x36) { Result = gs_Key_6; }
    else if (Win32Key == 0x37) { Result = gs_Key_7; }
    else if (Win32Key == 0x38) { Result = gs_Key_8; }
    else if (Win32Key == 0x39) { Result = gs_Key_9; }
    
    else if (Win32Key == 0x41) { Result = gs_Key_A; }
    else if (Win32Key == 0x42) { Result = gs_Key_B; }
    else if (Win32Key == 0x43) { Result = gs_Key_C; }
    else if (Win32Key == 0x44) { Result = gs_Key_D; }
    else if (Win32Key == 0x45) { Result = gs_Key_E; }
    else if (Win32Key == 0x46) { Result = gs_Key_F; }
    else if (Win32Key == 0x47) { Result = gs_Key_G; }
    else if (Win32Key == 0x48) { Result = gs_Key_H; }
    else if (Win32Key == 0x49) { Result = gs_Key_I; }
    else if (Win32Key == 0x4A) { Result = gs_Key_J; }
    else if (Win32Key == 0x4B) { Result = gs_Key_K; }
    else if (Win32Key == 0x4C) { Result = gs_Key_L; }
    else if (Win32Key == 0x4D) { Result = gs_Key_M; }
    else if (Win32Key == 0x4E) { Result = gs_Key_N; }
    else if (Win32Key == 0x4F) { Result = gs_Key_O; }
    else if (Win32Key == 0x50) { Result = gs_Key_P; }
    else if (Win32Key == 0x51) { Result = gs_Key_Q; }
    else if (Win32Key == 0x52) { Result = gs_Key_R; }
    else if (Win32Key == 0x53) { Result = gs_Key_S; }
    else if (Win32Key == 0x54) { Result = gs_Key_T; }
    else if (Win32Key == 0x55) { Result = gs_Key_U; }
    else if (Win32Key == 0x56) { Result = gs_Key_V; }
    else if (Win32Key == 0x57) { Result = gs_Key_W; }
    else if (Win32Key == 0x58) { Result = gs_Key_X; }
    else if (Win32Key == 0x59) { Result = gs_Key_Y; }
    else if (Win32Key == 0x5A) { Result = gs_Key_Z; }
    
    else if      (Win32Key == VK_NUMPAD0) { Result = gs_Key_Num0; }
    else if (Win32Key == VK_NUMPAD1) { Result = gs_Key_Num1; }
    else if (Win32Key == VK_NUMPAD2) { Result = gs_Key_Num2; }
    else if (Win32Key == VK_NUMPAD3) { Result = gs_Key_Num3; }
    else if (Win32Key == VK_NUMPAD4) { Result = gs_Key_Num4; }
    else if (Win32Key == VK_NUMPAD5) { Result = gs_Key_Num5; }
    else if (Win32Key == VK_NUMPAD6) { Result = gs_Key_Num6; }
    else if (Win32Key == VK_NUMPAD7) { Result = gs_Key_Num7; }
    else if (Win32Key == VK_NUMPAD8) { Result = gs_Key_Num8; }
    else if (Win32Key == VK_NUMPAD9) { Result = gs_Key_Num9; }
    
    /*
    else if (Win32Key == VK_) { Result = gs_Key_Bang; }
    else if (Win32Key == VK_) { Result = gs_Key_At; }
    else if (Win32Key == VK_) { Result = gs_Key_Pound; }
    else if (Win32Key == VK_) { Result = gs_Key_Dollar; }
    else if (Win32Key == VK_) { Result = gs_Key_Percent; }
    else if (Win32Key == VK_) { Result = gs_Key_Carrot; }
    else if (Win32Key == VK_) { Result = gs_Key_Ampersand; }
    else if (Win32Key == VK_) { Result = gs_Key_Star; }
    else if (Win32Key == VK_) { Result = gs_Key_LeftParen; }
    else if (Win32Key == VK_) { Result = gs_Key_RightParen; }
    else if (Win32Key == VK_) { Result = gs_Key_Minus; }
    else if (Win32Key == VK_) { Result = gs_Key_Plus; }
    else if (Win32Key == VK_) { Result = gs_Key_Equals; }
    else if (Win32Key == VK_) { Result = gs_Key_Underscore; }
    else if (Win32Key == VK_) { Result = gs_Key_LeftBrace; }
    else if (Win32Key == VK_) { Result = gs_Key_RightBrace; }
    else if (Win32Key == VK_) { Result = gs_Key_LeftBracket; }
    else if (Win32Key == VK_) { Result = gs_Key_RightBracket; }
    else if (Win32Key == VK_) { Result = gs_Key_Colon; }
    else if (Win32Key == VK_) { Result = gs_Key_SemiColon; }
    else if (Win32Key == VK_) { Result = gs_Key_SingleQuote; }
    else if (Win32Key == VK_) { Result = gs_Key_DoubleQuote; }
    else if (Win32Key == VK_) { Result = gs_Key_ForwardSlash; }
    else if (Win32Key == VK_) { Result = gs_Key_Backslash; }
    else if (Win32Key == VK_) { Result = gs_Key_Pipe; }
    else if (Win32Key == VK_) { Result = gs_Key_Comma; }
    else if (Win32Key == VK_) { Result = gs_Key_Period; }
    else if (Win32Key == VK_) { Result = gs_Key_QuestionMark; }
    else if (Win32Key == VK_) { Result = gs_Key_LessThan; }
    else if (Win32Key == VK_) { Result = gs_Key_GreaterThan; }
*/
    else if (Win32Key == VK_OEM_3) { Result = gs_Key_Tilde; }
    //else if (Win32Key == VK_) { Result = gs_Key_BackQuote; }
    
    else if (Win32Key == VK_UP)    { Result = gs_Key_UpArrow; }
    else if (Win32Key == VK_DOWN)  { Result = gs_Key_DownArrow; }
    else if (Win32Key == VK_LEFT)  { Result = gs_Key_LeftArrow; }
    else if (Win32Key == VK_RIGHT) { Result = gs_Key_RightArrow; }
    
    return Result;
}

static bool
gs_TakeNextInputEvent(gs_window Window, gs_input_event* Event)
{
    bool Result = false;
    
    MSG Message;
    Result = PeekMessageA(&Message, Window.WindowHandle, 0, 0, PM_REMOVE);
    if (Result)
    {
        switch (Message.message)
        {
            case WM_HOTKEY:
            {
            }break;
            
            case WM_MOUSEWHEEL:
            {
            }break;
            
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            {
                
            }break;
            
            case WM_MOUSEMOVE:
            {
            }break;
            
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                int VirtualKey = (int)Message.wParam;
                Event->Key = gs_TranslateWin32KeyCode(VirtualKey);
                
                if (Event->Key != gs_Key_Invalid)
                {
                    bool KeyWasDown = (Message.lParam & (1 << 30)) != 0;
                    bool KeyIsDown = (Message.lParam & (1 << 31)) == 0;
                    if (KeyIsDown && !KeyWasDown)
                    {
                        Event->Type = gs_EventType_KeyPressed;
                    }
                    else if (!KeyIsDown && KeyWasDown)
                    {
                        Event->Type = gs_EventType_KeyReleased;
                    }
                    else
                    {
                        Event->Type = gs_EventType_NoEvent;
                    }
                }
                else
                {
                    Event->Type = gs_EventType_NoEvent;
                }
                
            }break;
            
            case WM_CHAR:
            {
            }break;
            
            default:
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
                gs_EventType_NoEvent;
            }break;
        }
    }
    else
    {
        Event->Type = gs_EventType_NoMoreEvents;
    }
    
    return Result;
}

#endif

#if defined(GS_WINDOW_OSX) || defined(GS_PLATFORM_OSX)

@interface gsosx_ApplicationDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate> @end

struct gs_window
{
    int Width, Height;
    
    NSApplication* Application;
    gsosx_ApplicationDelegate* Delegate;
    NSWindow* Window;
};


@implementation gsosx_ApplicationDelegate : NSObject
- (void)
applicationDidFinishLaunching: (NSNotification *)notification
{
    [NSApp stop: nil];
    NSAutoreleasePool* Pool = [[NSAutoreleasePool alloc] init];
    
    [Pool drain];
}

- (NSApplicationTerminateReply)
applicationShouldTerminate: (NSApplication*) sender
{
    return NO;
}

- (void)
dealloc
{
    [super dealloc];
}
@end

@interface gsosx_WindowDelegate: NSObject<NSWindowDelegate> @end
@implementation gsosx_WindowDelegate : NSObject
- (BOOL)
windowShouldClose: (id)sender
{
    // TODO(Peter): Stop Application Running?
    NSLog(@"Close button pressed");
    return NO;
}

- (void)
windowDidBecomeKey: (NSNotification*)notification
{
    // TODO: ???
}

- (void)
windowDisResignKey: (NSNotification*)notification
{
    // TODO: ???
}
@end

static NSApplication*
gs_OSXCreateApplication(NSObject* ApplicationDelegate)
{
    NSApplication* Application = [NSApplication sharedApplication];
	[Application setActivationPolicy: NSApplicationActivationPolicyRegular];
    [Application setDelegate: ApplicationDelegate];
    return Application;
}

static gs_window
gs_OSXCreateWindow(char* Title, int Width, int Height, NSWindowDelegate* WindowDelegate)
{
    gs_window Result = {0};
    int WindowStyleMask = NSWindowStyleMaskClosable;
	NSRect WindowRect = NSMakeRect(0, 0, Width, Height);
	
    Result.Window = [[NSWindow alloc] initWithContentRect: WindowRect styleMask: WindowStyleMask backing: NSBackingStoreBuffered defer: YES];
	Window.styleMask = NSWindowStyleMaskResizable | NSWindowStyleMaskTitled | NSWindowStyleMaskClosable;
    
    [Window setOpaque: YES];
	[Window setDelegate: WindowDelegate];
	[Window setTitle: Title];
    
}

static gs_window
gs_CreateWindow(char* Title, int Width, int Height)
{
	gs_window Result = {0};
    
    gsosx_ApplicationDelegate* Delegate = [[gsosx_ApplicationDelegate alloc] init];
    NSApplication* Application = gs_OSXCreateApplication(Delegate);
	
    gsosx_WindowDelegate* WindowDelegate = [[gsosx_WindowDelegate alloc] init];
    Result = gs_OSXCreateWindow(Title, Width, Height, WindowDelegate);
	
	NSMenu* MenuBar = [NSMenu alloc];
	NSMenuItem* AppMenuItem = [NSMenuItem alloc];
	[MenuBar addItem: AppMenuItem];
	[App setMainMenu: MenuBar];
    
	NSMenu* AppMenu = [NSMenu alloc];
	id QuitTitle = [@"Quit " stringByAppendingString: Title];
	id QuitMenuItem = [[NSMenuItem alloc] initWithTitle: QuitTitle action: @selector(terminate:) keyEquivalent: @"q"];
	[AppMenu addItem: QuitMenuItem];
	[AppMenuItem setSubmenu: AppMenu];
    
	[Application activateIgnoringOtherApps: YES];
    
    Result.Application = Application;
    Result.Delegate = ApplicationDelegate;
	return Window;
}

#endif

#define GS_WINDOW_H
#endif // GS_WINDOW_H