#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "common.h"

BOOL strStratsWith(UCHAR* _buffer, UCHAR* _start)
{
	int sz=strlen(_start);
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
	from-=sz;

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
