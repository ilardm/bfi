#ifndef BFI_H
#define BFI_H

#include "main.h"

bool validate(unsigned char* _buffer);
bool execute(unsigned char* _buffer);

extern unsigned char* bf_mem;
extern unsigned char* bf_sc;
extern unsigned char* bf_mp;

#endif