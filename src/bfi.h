#ifndef BFI_H
#define BFI_H

#include "main.h"
#include "common.h"

UCHAR* prepare(UCHAR* _buffer);
BOOL validate(UCHAR* _buffer);
BOOL execute(UCHAR* _buffer);

void memDump();
void printSource(UCHAR* _buffer, UCHAR* _sp);

UCHAR* used_memory;

extern UCHAR* bf_mem;
extern UCHAR* bf_sc;
extern UCHAR* bf_mp;

extern long bfo;

#endif