#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "common.h"

BOOL strStratsWith(UCHAR* _buffer, UCHAR* _start)
{
	int sz=strlen(_start);
	int bsz=strlen(_buffer);
	if ( bsz<sz )
	{
		#if defined _DEBUG
		printf("--- strStratsWith: bsz(%d)<=sz(%d)\n", bsz, sz);
		#endif
		return false;
	}
		
	if ( strncmp(_buffer, _start, sz)>=0 )
	{
		return true;
	}
	else
	{
		return  false;
	}
}

BOOL strEndsWith(UCHAR* _buffer, UCHAR* _end)
{
	#if defined _DEBUG
	printf("+++ strEndsWith\n");
	#endif
	
	int sz=strlen(_end);
	int from=strlen(_buffer);
	if ( from>sz )
	{
		from-=sz;
	}
	else
	{
		#if defined _DEBUG
		printf("--- strEndsWith: from(%d)<=sz(%d)\n", from, sz);
		#endif
		return false;
	}

	#if defined _DEBUG
	printf("+++ strEndsWith: compare %s && %s\n", _buffer+from, _end);
	#endif

	if ( strncmp(_buffer+from, _end, sz)==0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

#if defined linux
void setColor(int _color)
{
	putchar(0x1b);
	printf("[%dm", _color);
}
#endif