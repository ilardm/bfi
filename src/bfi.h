#ifndef BFI_H
#define BFI_H

#include "main.h"
#include "common.h"

BOOL validate(UCHAR* _buffer);
BOOL execute(UCHAR* _buffer);
void memDump();

UCHAR* used_memory;

extern UCHAR* bf_mem;
extern UCHAR* bf_sc;
extern UCHAR* bf_mp;

#endif