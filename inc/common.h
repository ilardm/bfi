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
