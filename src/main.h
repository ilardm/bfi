#ifndef MAIN_H
#define MAIN_H

#define bool char
#define true 1
#define false 0

int main(int argc, char* argv[]);

unsigned char* bf_mem;
unsigned char* bf_sc;
unsigned char* bf_mp;

bool validate(unsigned char* _buffer);
bool execute(unsigned char* _buffer);

#endif
