/* date = August 24th 2021 11:16 pm */

#ifndef GS_SOCKET_WIN32_H
#define GS_SOCKET_WIN32_H

typedef struct
{
  WSADATA WSAData;
} gs_socket_handler_win32;

gs_socket_handler
CreatePlatformSocketHandler(gs_allocator A)
{
  gs_socket_handler_win32* Win32Data = AllocStruct(A, gs_socket_handler_win32, "Win32 Socket Handler");
  
  gs_socket_handler Result = {
    .UserData = (u8*)Win32Data
  };
  
  WSAStartup(MAKEWORD(2, 2), &Win32Data->WSAData);
  
  return Result;
}

void 
CloseSocket(s32 SocketFileDesc)
{
  closesocket(SocketFileDesc);
}

#endif //GS_SOCKET_WIN32_H
