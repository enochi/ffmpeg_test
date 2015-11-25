/*
 * Copyright (C) 2008 SUYUN, Inc.
 * All rights reserved.
 *
 * Author      : XiaKQ
 * Version     : 1.0.0
 * Description : 操作系统移植用的头文件
 */

#ifndef __SYSTEM_PORT_H__
#define	__SYSTEM_PORT_H__

#define __OS_WIN32__    0
#define __OS_LINUX__    1

#if   __OS_WIN32__

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <process.h>
#include <iphlpapi.h>
#include <assert.h>

#elif __OS_LINUX__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <dlfcn.h>

typedef unsigned int UINT;
typedef int	SOCKET;
typedef enum
{
	FALSE = 0,
	TRUE,
} BOOL;

#define	closesocket close

#endif

#endif  /* __SYSTEM_PORT_H__ */

