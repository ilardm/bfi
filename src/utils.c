/*
  brainf*k interpreter written in C with little extension -- QBF
  Copyright (C) 2011  Ilya Arefiev <arefiev.id@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
