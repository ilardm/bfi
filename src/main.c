#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>

#include "main.h"

int main(int argc, char* argv[])
{
	#if defined _DEBUG
	printf("+++ start\n");

	int argi=0;
	printf("+++ argc: %d\n", argc);
	for ( argi; argi<argc; argi++)
	{
		printf("+++ argv[%d]='%s'\n", argi, argv[argi]);
	}
	#endif

	bf_mem=(unsigned char*)calloc(30000, sizeof(char));

	FILE* bf_sc_fd;
	if ( argc>1 )
	{
		bf_sc_fd=fopen(argv[argc-1], "r");
		if ( bf_sc_fd )
		{
			bf_sc=(unsigned char*)calloc(64, sizeof(char));
			int bf_sc_sz=0;
			int readb=0;
			while( !feof(bf_sc_fd) )
			{
				if ( bf_sc )
				{
					readb=fread(bf_sc+bf_sc_sz*64*sizeof(char), 64*sizeof(char), 1, bf_sc_fd);

// 					#if defined _DEBUG
// 					printf("bf_sc::fread #%d:\n%s (%d)\n", bf_sc_sz+1, bf_sc, readb);
// 					#endif

					if ( readb==1 )
					{
						bf_sc_sz+=1;
						bf_sc=(unsigned char*)realloc(bf_sc, (bf_sc_sz+1)*64*sizeof(char));
					}
				}
				else
				{
					printf("--- bf_sc=NULL\n");
					break;
				}
			}
			close(bf_sc_fd);

			#if defined _DEBUG
			if ( bf_sc )
			{
				printf("bf_sc (%d):\n'%s'\n", 64*(bf_sc_sz+1)*sizeof(char), bf_sc);
			}
			#endif

			if ( bf_sc )
			{
				int sz=strlen(bf_sc);
				#if defined _DEBUG
				printf("+++ bf_sc[%d]=0x%02X\n", sz-1, bf_sc[sz-1]);
				#endif

				// TODO: str replace ?
				if ( bf_sc[sz-1]==0x0a || bf_sc[sz-1]==0x0d )
					bf_sc[sz-1]=0;
				if ( bf_sc[sz-2]==0x0a || bf_sc[sz-2]==0x0a )
					bf_sc[sz-2]=0;

// 				printf("+++ validate is %d\n", validate(bf_sc));
				if ( validate(bf_sc) )
				{
					bf_mp=bf_mem;
					execute(bf_sc);
					printf("\n");
// 					unsigned char* result=execute(bf_sc);
// 					if ( result )
// 					{
// 						printf("result:\n'%s'\n", result);
// 					}
// 					else
// 					{
// 						printf("result is wrong\n");
// 					}
					free(bf_sc);
					free(bf_mem);
				}
				else
				{
					printf("code is invalid\n");
				}
			}
		}
		else
		{
			printf("can't open '%s'\n", argv[argc-1]);
		}
	}

	#if defined _DEBUG
	printf("+++ stop\n");
	#endif

	return 0;
};

bool validate(unsigned char* _buffer)
{
	int sz=strlen(_buffer);
	int i=0;
	int op=0;
	int cl=0;

	#if defined _DEBUG
	printf("+++ validate: (len=%d) '%s'\n", sz, _buffer);
	#endif
	
	// TODO: validate for other chars
	for ( i=0; i<sz; i++)
	{
		#if defined _DEBUG
		printf("\t '%c' - 0x%0x\n", _buffer[i], _buffer[i]);
		#endif
		
		if ( _buffer[i]=='[' )
			op++;
		if ( _buffer[i]==']' )
			cl++;
	}

	return op==cl;
}

bool execute(unsigned char* _buffer)
{
	if ( !validate(_buffer) )
	{
		#if defined _DEBUG
		printf("--- execute: '%s' is invalid\n", _buffer);
		#endif
		return false;
	}
	
	#if defined _DEBUG
	printf("+++ execute: (@ 0x%X) '%s'\n", _buffer, _buffer);
	#endif
	
	// TODO: implement
	int sz=strlen(_buffer);
	unsigned char* tmp_buffer;
	unsigned char* bf_sp=_buffer;
	
	while ( bf_sp<_buffer+sz )
	{
		#if defined _DEBUG
		printf("+++ execute: parse '%c'@ 0x%X\n", *bf_sp, bf_sp);
		#endif
		if ( *bf_sp=='+' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: +\n");
// 			#endif

			#if defined _DEBUG
			printf("%d (0x%0X) @ 0x%X -> ", *bf_mp, *bf_mp, bf_mp);
			(*bf_mp)++;
			printf("%d (0x%0X) @ 0x%X\n", *bf_mp, *bf_mp, bf_mp);
			#else
			(*bf_mp)++;
			#endif
		}

		if ( *bf_sp=='-' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: -\n");
// 			#endif
			
			#if defined _DEBUG
			printf("%d (0x%0X) @ 0x%X -> ", *bf_mp, *bf_mp, bf_mp);
			(*bf_mp)--;
			printf("%d (0x%0X) @ 0x%X\n", *bf_mp, *bf_mp, bf_mp);
			#else
			(*bf_mp)--;
			#endif
		}

		if ( *bf_sp=='>' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: >\n");
// 			#endif

			#if defined _DEBUG
			printf("0x%X -> ", bf_mp);
			bf_mp++;
			printf("0x%X\n", bf_mp);
			#else
			bf_mp++;
			#endif
		}

		if ( *bf_sp=='<' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: <\n");
// 			#endif

			#if defined _DEBUG
			printf("0x%X -> ", bf_mp);
			bf_mp--;
			printf("0x%X\n", bf_mp);
			#else
			bf_mp--;
			#endif
		}

		if ( *bf_sp=='.' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: .\n");
// 			#endif

			#if defined _DEBUG
			printf("%c (0x%0X)", *bf_mp, *bf_mp);
			#else
			printf("%c", *bf_mp);
			#endif
		}

		if ( *bf_sp==',' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: ,\n");
// 			#endif

			*bf_mp=(unsigned char)getchar();
		}

		if ( *bf_sp=='[' )
		{
			#if defined _DEBUG
			printf("+++ execute: '[' found @ 0x%X\n", bf_sp);
			#endif

			unsigned char* tmp_bf_sp=bf_sp+1;
			int tmp_sz=abs(bf_sp-tmp_bf_sp);
			tmp_buffer=(unsigned char*)calloc(tmp_sz, sizeof(char));
			if ( tmp_buffer )
			{
				strncpy(tmp_buffer, bf_sp, tmp_sz);
			}
			while ( tmp_buffer && !validate(tmp_buffer) )
			{
				#if defined _DEBUG
				printf("+++ execute::searchEndOfCycle: '%s'(%d) is invalid : ++\n", tmp_buffer, tmp_sz);
				#endif
				tmp_bf_sp++;
				tmp_sz=abs(bf_sp-tmp_bf_sp);
				tmp_buffer=(unsigned char*)realloc(tmp_buffer, tmp_sz);
// 				#if defined _DEBUG
// 				int i=0;
// 				printf("+++ execute::realloc\n");
// 				for ( i=0; i< tmp_sz; i++ )
// 				{
// 					printf("\t0x%X\n", tmp_buffer[i]);
// 				}
// 				printf("\n");
// 				#endif
				if ( tmp_buffer )
				{
					strncpy(tmp_buffer, bf_sp, tmp_sz);
					tmp_buffer[tmp_sz]=0;
				}
			}

			if ( tmp_buffer )
			{
				int sz=strlen(tmp_buffer);
// 				#if defined _DEBUG
// 				int i=0;
// 				printf("+++ execute::execute\n");
// 				for ( i=0; i< tmp_sz; i++ )
// 				{
// 					printf("\t0x%X\n", tmp_buffer[i]);
// 				}
// 				printf("\n");
// 				#endif
				tmp_buffer[sz-1]=0;
				
				while ( *bf_mp )
				{
					#if defined _DEBUG
					printf("+++ execute: recursive calls left: %d\n", *bf_mp);
					#endif
					if ( !execute(tmp_buffer+1) )
					{
						return false;
					}
				}
				#if defined _DEBUG
				printf("+++ execute: exit from recursive call\n");
				#endif
				free(tmp_buffer);
				bf_sp=tmp_bf_sp-1;
			}
		}

		bf_sp++;
	}

	return true;
}
