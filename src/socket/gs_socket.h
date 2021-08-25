/* date = August 24th 2021 11:16 pm */

#ifndef GS_SOCKET_H
#define GS_SOCKET_H

#if !defined(Assert)
# define Assert(c)
#endif

#if !defined(InvalidCodePath)
# define InvalidCodePath
#endif

#if !defined(GS_SOCKET_PROFILE_FUNC)
# define GS_SOCKET_PROFILE_FUNC
#endif

typedef struct
{
  s32 FD;
} gs_socket_fd;

typedef struct
{
  u8* UserData;
} gs_socket_handler;

gs_socket_handler CreatePlatformSocketHandler ();
void CloseSocket(s32 SocketFileDesc);

gs_socket_fd
CreateSocket(struct addrinfo Hints, char* Node, char* Port)
{
  gs_socket_fd Result = {0};
  
  struct addrinfo* ServInfo;
  s32 RV = getaddrinfo(Node, Port, &Hints, &ServInfo);
  if (RV != 0) {
    printf("Error: getaddrinfo: %s\n", gai_strerror(RV));
    return Result;
  }
  
  struct addrinfo* P = 0;
  s32 SockFD = 0;
  for (P = ServInfo; P != NULL; P = P->ai_next) 
  {
    SockFD = socket(P->ai_family, P->ai_socktype, P->ai_protocol);
    if (SockFD == -1)
    {
      printf("Error: socket\n");
      continue;
    }
    
    s32 Yes = 1;
    RV = setsockopt(SockFD, 
                    SOL_SOCKET, 
                    SO_REUSEADDR, 
                    (char*)&Yes, 
                    sizeof(s32));
    if (RV == -1)
    {
      printf("Error: setsockopt\n");
      return Result;
    }
    
    RV = bind(SockFD, P->ai_addr, P->ai_addrlen);
    if (RV == -1) 
    {
      CloseSocket(SockFD);
      printf("Error: bind\n");
      continue;
    }
    
    break;
  }
  
  if (P == NULL)
  {
    printf("Error: server: failed to bind\n");
    return Result;
  }
  
  freeaddrinfo(ServInfo);
  
  Result.FD = SockFD;
  return Result;
}

# if PLATFORM_WINDOWS
#  include "gs_socket_win32.h"
# elif PLATFORM_OSX
#  include "gs_socket_osx.h"
# elif PLATFORM_LINUX
#  include "gs_socket_linux.h"
# else
#  error "Trying to include gs_socket on a platform without an impl"
# endif

#endif //GS_SOCKET_H
