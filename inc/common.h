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

#ifndef COMMON_H
#define COMMON_H

#include "rev.h"

enum
{
	RET_SUCCES=0,
	RET_ERR_PARAMS=1,
	RET_ERR_MEM=2,
	RET_ERR_SC=4
};

enum
{
	BFO_QUICK=1,
	BFO_DEBUG=2
};

enum
{
	CL_NONE=0,
	CL_YELLOW=33
};

typedef char BOOL;
#define true 1
#define false 0

#define BUFFER_SIZE 64
#define BF_MEMORY_SIZE 29999

typedef unsigned char UCHAR;

#endif
