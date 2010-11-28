#ifndef UTILS_H
#define UTILS_H

#include "common.h"

BOOL strStratsWith(UCHAR* _buffer, UCHAR* _start);
BOOL strEndsWith(UCHAR* _buffer, UCHAR* _end);

#if defined linux
void setColor(int _color);
#endif

#endif
