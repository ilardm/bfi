#ifndef MAIN_H
#define MAIN_H

#include "common.h"

int main(int argc, char* argv[]);
int findParam(char** _where, int _count, UCHAR* _param);

void help(char* _name);

UCHAR* bf_mem;
UCHAR* bf_sc;
UCHAR* bf_mp;
long bfo;

#endif
