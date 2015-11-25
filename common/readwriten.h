#ifndef READWRITEN_H
#define READWRITEN_H

#include <unistd.h>

#ifdef __cplusplus
#include <cerrno>
using namespace std;
#else
#include <errno.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

	inline ssize_t readn(int fd, void *buf, size_t count)
	{
		char *strtmp;
		ssize_t reval, realcount=0;
		strtmp = (char *)buf;

		while (count>0)
		{
			reval = read(fd, strtmp, count);
			if (reval<0)
				{if (errno == EINTR)
					continue;
				else return -1;
				}
			else if (reval>0)
			{
				count -= reval;
				strtmp += reval;
				realcount += reval;
				continue;
			}
			else break;
		}

		return realcount;
	}

	inline ssize_t writen(int fd, const void *buf, size_t count)
	{
		char *strtmp;
		ssize_t reval, realcount=count;
		strtmp = (char *)buf;

		while(count>0)
		{
			reval = write(fd, strtmp, count);
			if (reval < 0)
				{if (errno == EINTR)
					continue;
				else return -1;
				}
			count -= reval;
			strtmp += reval;
		}

		return realcount;
	}

	inline ssize_t readline(int fd, void *buf, int size)
	{
		char *strtmp;
		ssize_t reval, realcount=0;
		strtmp = (char *)buf;

		while(size>1)
		{
			reval = read(fd, strtmp, 1);
			if (reval<0)
				if (errno == EINTR)
					continue;
				else return -1;
			else if (reval == 0)
				break;
			else
			{
				realcount++;
				size--;
				if (*strtmp++ =='\n')
					break;
			}
		}
		*strtmp='\0';
		return realcount;
	}

#ifdef __cplusplus
}
#endif

#endif
