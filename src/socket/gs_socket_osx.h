/* date = August 25th 2021 0:06 am */

#ifndef GS_SOCKET_OSX_H
#define GS_SOCKET_OSX_H

gs_socket_handler
CreatePlatformSocketHandler(gs_allocator A)
{
  gs_socket_handler Result = {
    .UserData = 0
  };
  return Result;
}

void 
CloseSocket(s32 SocketFileDesc)
{
  close(SocketFileDesc);
}

#endif //GS_SOCKET_OSX_H
