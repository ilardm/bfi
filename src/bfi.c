#include <string.h>
#include <malloc.h>
#include <memory.h>

#include "bfi.h"
#include "main.h"
#include "common.h"

BOOL validate(UCHAR* _buffer)
{
	int sz=strlen(_buffer);
	int i=0;
	int op=0;
	int cl=0;

	#if defined _DEBUG
	printf("+++ validate: (len=%d) '%s'\n", sz, _buffer);
	#endif

	for ( i=0; i<sz; i++)
	{
		#if defined _DEBUG
		printf("'%c'(0x%0x) ", _buffer[i], _buffer[i]);
		#endif

		if ( _buffer[i]!='+' &&
			_buffer[i]!='-' &&
			_buffer[i]!='>' &&
			_buffer[i]!='<' &&
			_buffer[i]!='[' &&
			_buffer[i]!=']' &&
			_buffer[i]!='.' &&
			_buffer[i]!=','
			)
		{
			return false;
		}

		if ( _buffer[i]=='[' )
			op++;
		if ( _buffer[i]==']' )
			cl++;
	}
	#if defined _DEBUG
	printf("\n");
	#endif

	return op==cl;
}

BOOL execute(UCHAR* _buffer)
{
	if ( !validate(_buffer) )
	{
		#if defined _DEBUG
		printf("\n--- execute: '%s' is invalid\n", _buffer);
		#endif
		return false;
	}

	#if defined _DEBUG
	printf("\n+++ execute: (@ 0x%X) '%s'\n", _buffer, _buffer);
	#endif

	int sz=strlen(_buffer);
	UCHAR* tmp_buffer;		// copy here body of loop cycle if any
	UCHAR* bf_sp=_buffer;

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

			if ( used_memory<bf_mp )
			{
				used_memory=bf_mp;
			}
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

			if ( used_memory<bf_mp )
			{
				used_memory=bf_mp;
			}
		}

		if ( *bf_sp=='>' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: >\n");
// 			#endif

			if ( bf_mp<bf_mem+BF_MEMORY_SIZE)
			{
				#if defined _DEBUG
				printf("0x%X -> ", bf_mp);
				bf_mp++;
				printf("0x%X\n", bf_mp);
				#else
				bf_mp++;
				#endif

				if ( used_memory<bf_mp )
				{
					used_memory=bf_mp;
				}
			}
			else
			{
				#if defined _DEBUG
				printf("--- execute>: out of memory bounds\n");
				#else
				printf("warning: out of memory bounds\n");
				#endif
			}
		}

		if ( *bf_sp=='<' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: <\n");
// 			#endif

			if ( bf_mp>bf_mem  )
			{
				#if defined _DEBUG
				printf("0x%X -> ", bf_mp);
				bf_mp--;
				printf("0x%X\n", bf_mp);
				#else
				bf_mp--;
				#endif
			}
			else
			{
				#if defined _DEBUG
				printf("--- execute<: out of memory bounds\n");
				#else
				printf("warning: out of memory bounds\n");
				#endif
			}
		}

		if ( *bf_sp=='.' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: .\n");
// 			#endif

			#if defined _DEBUG
// 			printf("%c (0x%0X)", *bf_mp, *bf_mp);
			memDump();
			#else
			printf("%c", *bf_mp);
			#endif

			if ( used_memory<bf_mp )
			{
				used_memory=bf_mp;
			}
		}

		if ( *bf_sp==',' )
		{
// 			#if defined _DEBUG
// 			printf("+++ execute: ,\n");
// 			#endif

			*bf_mp=(UCHAR)getchar();

			#if defined _DEBUG
			printf("+++ execute,: 0x%x\n", *bf_mp);
			#endif

			if ( used_memory<bf_mp )
			{
				used_memory=bf_mp;
			}
		}

		if ( *bf_sp=='[' )
		{
			#if defined _DEBUG
			printf("+++ execute[: '[' found @ 0x%X\n", bf_sp);
			#endif

			UCHAR* tmp_bf_sp=index(bf_sp, ']')+1;
			int tmp_sz=abs(bf_sp-tmp_bf_sp);
			tmp_buffer=(UCHAR*)calloc(tmp_sz, sizeof(char));
			if ( tmp_buffer )
			{
				strncpy(tmp_buffer, bf_sp, tmp_sz);
			}
			else
			{
				#if defined _DEBUG
				printf("--- execute[: tmp_buffer=NULL\n");
				#else
				printf("error: can't allocate memory\n");
				#endif
				return false;
			}
			
			while ( tmp_buffer && !validate(tmp_buffer) )
			{
				#if defined _DEBUG
				printf("+++ execute[::searchEndOfCycle: '%s'(%d) is invalid : ++\n", tmp_buffer, tmp_sz);
				#endif
				tmp_bf_sp=index(tmp_bf_sp, ']')+1;
				tmp_sz=abs(bf_sp-tmp_bf_sp);
				tmp_buffer=(UCHAR*)realloc(tmp_buffer, tmp_sz);

				if ( tmp_buffer )
				{
					strncpy(tmp_buffer, bf_sp, tmp_sz);
					tmp_buffer[tmp_sz]=0;
				}
				else
				{
					#if defined _DEBUG
					printf("--- execute[: tmp_buffer=NULL\n");
					#else
					printf("error: can't allocate memory\n");
					#endif
					return false;
				}
			}

			if ( tmp_buffer )
			{
				int sz=strlen(tmp_buffer);
				tmp_buffer[sz-1]=0;

				while ( *bf_mp )
				{
					#if defined _DEBUG
					printf("\n+++ execute: recursive calls left: %d\n", *bf_mp);
					#endif
					if ( !execute(tmp_buffer+1) )
					{
						#if defined _DEBUG
						printf("--- execute: error execute while recursive calls\n");
						memDump();
						#endif
						return false;
					}
				}
				#if defined _DEBUG
				printf("+++ execute: exit from recursive call\n\n");
				#endif
				free(tmp_buffer);
				bf_sp=tmp_bf_sp-1;
			}
		}

		bf_sp++;
	}

	return true;
}

void memDump()
{
	#if defined _DEBUG
	printf("\nmemDump():\n");
	#endif
	
	int i=0;
	int sz=abs(used_memory-bf_mem)+1;		// dump only used memory.

	int mp=abs(bf_mem-bf_mp);
	BOOL bmp=false;
	#if defined _DEBUG
	printf("+++ memDump: now bf_mp points to #%d celll\n", mp);
	#endif

	#if defined _DEBUG
	printf("+++ memDump: sz=%d (0x%X && 0x%X)\n", sz, bf_mem, used_memory);
	#endif

	for ( i=0; i<sz; i++ )
	{
		bmp= (i==mp) ? true : false;
		// TODO: highlight with color
		
		// line number
		if ( i%16==0 )
		{
			printf("%08X  ", bf_mem+i);
		}

		if ( i%2==0)
		{
			printf( bmp ? "*%02x*" : "%02x",
					bf_mem[i]);
		}
		else
		{
			if ( (i%16)%7==0 )
			{
				printf( bmp ? "*%02x*  ": "%02x  ",
						bf_mem[i]);
			}
			else
			{
				printf( bmp ? "*%02x* " : "%02x ",
						bf_mem[i]);
			}
		}

		if ( (i+1)%16==0)
		{
			printf("\n");
		}
	}
	printf("\n");
}