#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>

#include "main.h"
#include "common.h"
#include "bfi.h"

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

	bf_mem=(UCHAR*)calloc(BF_MEMORY_SIZE, sizeof(char));

	FILE* bf_sc_fd;
	if ( argc>1 && strEndsWith(argv[1], ".bf") )
	{
		bf_sc_fd=fopen(argv[argc-1], "r");
		if ( bf_sc_fd )
		{
			bf_sc=(UCHAR*)calloc(BUFFER_SIZE, sizeof(char));
			int bf_sc_sz=0;
			int readb=0;
			while( !feof(bf_sc_fd) )
			{
				if ( bf_sc )
				{
					readb=fread(bf_sc+bf_sc_sz*BUFFER_SIZE*sizeof(char),
								BUFFER_SIZE*sizeof(char),
								1,
								bf_sc_fd);

// 					#if defined _DEBUG
// 					printf("bf_sc::fread #%d:\n%s (%d)\n", bf_sc_sz+1, bf_sc, readb);
// 					#endif

					if ( readb==1 )
					{
						bf_sc_sz+=1;
						bf_sc=(UCHAR*)realloc(bf_sc,
													  (bf_sc_sz+1)*BUFFER_SIZE*sizeof(char));
					}
				}
				else
				{
					#if defined _DEBUG
					printf("--- bf_sc=NULL\n");
					#endif
					break;
				}
			}
			close(bf_sc_fd);

			#if defined _DEBUG
			if ( bf_sc )
			{
				printf("bf_sc (%d):\n'%s'\n", BUFFER_SIZE*(bf_sc_sz+1)*sizeof(char), bf_sc);
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

				if ( validate(bf_sc) )
				{
					bf_mp=bf_mem;
					execute(bf_sc);
					printf("\n");
					
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
	else
	{
		printf("'%s' seems to be not a BF source\n", argv[1]);
	}

	#if defined _DEBUG
	printf("+++ stop\n");
	#endif

	return 0;
};
