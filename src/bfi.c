#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>

#include "bfi.h"
#include "main.h"
#include "common.h"

UCHAR* prepare(UCHAR* _buffer)
{
	int sz=strlen(_buffer);
	int newsz=0;

	UCHAR* tmp_buffer=(UCHAR*)calloc(sz, sizeof(char));
	if ( !tmp_buffer )
	{
		#if defined _DEBUG
		printf("--- prepare: tmp_buffer=NULL\n");
		#else
		printf("can't allocate memory\n");
		#endif
		return NULL;
	}
	UCHAR* tmp_buffer_p=tmp_buffer;

	BOOL skip=false;
	int i=0;
	while (i<sz)
	{
		if ( _buffer[i]!=' ' &&
			_buffer[i]!='\t' &&
			_buffer[i]!='\r' &&
			_buffer[i]!='\n'
			)
		{
			if ( _buffer[i]=='(' )
			{
				skip=true;
				i++;
				continue;
			}
			if ( _buffer[i]==')' )
			{
				skip=false;
				i++;
				continue;
			}
// 			#if defined _DEBUG
// 			if ( !skip )
// 			{
// 				printf("+++ prerpare: cp %c(0x%x)\n", _buffer[i], _buffer[i]);
// 			}
// 			#endif

			if ( !skip )
			{
				*tmp_buffer_p=_buffer[i];

				tmp_buffer_p++;
			}
		}
		i++;
	}
	free(_buffer);
	newsz=strlen(tmp_buffer);

	#if defined _DEBUG
	printf("+++ prepare: sz=%d new sz=%d\n", sz, newsz);
	#endif

	return tmp_buffer;
}

BOOL validate(UCHAR* _buffer)
{
	int sz=strlen(_buffer);
	int i=0;
	int bracket=0;

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
			if ( bfo & BFO_QUICK )
			{
				if ( (_buffer[i]>=48 && _buffer[i]<=57) ||
					_buffer[i]=='@' )
				{
					continue;
				}
			}
			return false;
		}

		if ( _buffer[i]=='[' )
			bracket++;
		if ( _buffer[i]==']' )
			bracket--;
	}
	#if defined _DEBUG
	printf("\n");
	#endif

	return bracket==0;
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

	int repeat=0;
	UCHAR* repeat_start=0;
	UCHAR* repeat_stop=0;
	// TODO: crashes here
	/* with this sc
@
+3 [> +10 <-]> -3 .@
< +6 [> +10 <-]> +4 .@
< +6 (crashes here) [> -- -- -- - <-]> .@
< +6 [> +10 <-]> ..@
< +9 [> -9 <-]> - .@
< +6 [> +10 <-]> +4 .@
< +6 [> -7 <-]> - .@
+ < +6 [> +10 <-]> ..@
	*/
	UCHAR* repeat_buffer;
	repeat_buffer=(UCHAR*)calloc(BUFFER_SIZE, sizeof(char));
	if ( !repeat_buffer )
	{
		#if defined _DEBUG
		printf("--- execute: repeat_buffer=NULL\n");
		#else
		printf("error: can't allocate memory\n");
		#endif
		return false;
	}
	BOOL repeat_done=false;
	
	UCHAR* tmp_buffer;		// copy here body of loop cycle if any
	UCHAR* bf_sp=_buffer;

// 	if ( bfo & BFO_QUICK )
// 	{
// 		free(repeat_buffer);
// 		printf("not implemented qbf yet\n");
// 		return false;
// 	}
	
	while ( bf_sp<_buffer+sz )
	{
		repeat_done=false;
// 		#if defined _DEBUG
// 		printf("+++ execute: parse '%c'@ 0x%X\n", *bf_sp, bf_sp);
// 		#endif

		if ( *bf_sp=='@' )
		{
			#if defined _DEBUG
			printf("\n+++ executeq: breakpoint reached\n");
			memDump();
			printSource(_buffer, bf_sp);
			getchar();
			#else
			if ( bfo & BFO_DEBUG )
			{
				printf("\nstop on breakpoint\n");
				memDump();
				printSource(_buffer, bf_sp);
				getchar();
			}
			#endif
		}

		if ( *bf_sp=='+' )
		{
			#if defined _DEBUG
			printf("+++ execute: +\n");
			#endif

			if ( (bfo & BFO_QUICK) && bf_sp<_buffer+sz+1 )
			{
				if ( *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
				{
					repeat_start= ++bf_sp;
					while ( bf_sp<_buffer+sz+1 && *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
					{
						bf_sp++;
					}
					repeat_stop=bf_sp+1;

					memcpy(repeat_buffer, repeat_start, abs(repeat_start-repeat_stop));
					#if defined _DEBUG
					printf("+++ executeq: repeat %s(%d) times\n", repeat_buffer, atoi(repeat_buffer));
					#endif

					(*bf_mp)+=atoi(repeat_buffer);
					repeat_done=true;

					memset(repeat_buffer, 0, BUFFER_SIZE);
				}
				else
				{
					#if defined _DEBUG
					printf("--- executeq: '%c' NAN\n", *(bf_sp+1));
					#endif
				}
			}

			if ( !repeat_done )
			{
				#if defined _DEBUG
				printf("%d (0x%0X) @ 0x%X -> ", *bf_mp, *bf_mp, bf_mp);
				(*bf_mp)++;
				printf("%d (0x%0X) @ 0x%X\n", *bf_mp, *bf_mp, bf_mp);
				#else
				(*bf_mp)++;
				#endif
			}

			if ( used_memory<bf_mp )
			{
				used_memory=bf_mp;
			}
		}

		if ( *bf_sp=='-' )
		{
			#if defined _DEBUG
			printf("+++ execute: -\n");
			#endif

			if ( (bfo & BFO_QUICK) && bf_sp<_buffer+sz+1 )
			{
				if ( *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
				{
					repeat_start= ++bf_sp;
					while ( bf_sp<_buffer+sz+1 && *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
					{
						bf_sp++;
					}
					repeat_stop=bf_sp+1;

					memcpy(repeat_buffer, repeat_start, abs(repeat_start-repeat_stop));
					#if defined _DEBUG
					printf("+++ executeq: repeat %s(%d) times\n", repeat_buffer, atoi(repeat_buffer));
					#endif

					(*bf_mp)-=atoi(repeat_buffer);
					repeat_done=true;

					memset(repeat_buffer, 0, BUFFER_SIZE);
				}
				else
				{
					#if defined _DEBUG
					printf("--- executeq: '%c' NAN\n", *(bf_sp+1));
					#endif
				}
			}

			if ( !repeat_done )
			{
				#if defined _DEBUG
				printf("%d (0x%0X) @ 0x%X -> ", *bf_mp, *bf_mp, bf_mp);
				(*bf_mp)--;
				printf("%d (0x%0X) @ 0x%X\n", *bf_mp, *bf_mp, bf_mp);
				#else
				(*bf_mp)--;
				#endif
			}

			if ( used_memory<bf_mp )
			{
				used_memory=bf_mp;
			}
		}

		if ( *bf_sp=='>' )
		{
			#if defined _DEBUG
			printf("+++ execute: >\n");
			#endif

			repeat=1;

			if ( (bfo & BFO_QUICK) && bf_sp<_buffer+sz+1 )
			{
				if ( *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
				{
					repeat_start= ++bf_sp;
					while ( bf_sp<_buffer+sz+1 && *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
					{
						bf_sp++;
					}
					repeat_stop=bf_sp+1;

					memcpy(repeat_buffer, repeat_start, abs(repeat_start-repeat_stop));
					#if defined _DEBUG
					printf("+++ executeq: repeat %s(%d) times\n", repeat_buffer, atoi(repeat_buffer));
					#endif

					repeat=atoi(repeat_buffer);

					memset(repeat_buffer, 0, BUFFER_SIZE);
				}
				else
				{
					#if defined _DEBUG
					printf("--- executeq: '%c' NAN\n", *(bf_sp+1));
					#endif
				}
			}

			if ( bf_mp+repeat<=bf_mem+BF_MEMORY_SIZE)
			{
				#if defined _DEBUG
				printf("0x%X -> ", bf_mp);
				bf_mp+=repeat;
				printf("0x%X\n", bf_mp);
				#else
				bf_mp+=repeat;
				#endif
			}
			else
			{
				#if defined _DEBUG
				printf("--- execute>: out of memory bounds\n");
				#else
				printf("warning: out of memory bounds\n");
				#endif
				if ( bfo & BFO_QUICK )
				{
					bf_mp=bf_mem+BF_MEMORY_SIZE;
				}
			}

			if ( used_memory<bf_mp )
			{
				used_memory=bf_mp;
			}
		}

		if ( *bf_sp=='<' )
		{
			#if defined _DEBUG
			printf("+++ execute: <\n");
			#endif

			repeat=1;

			if ( (bfo & BFO_QUICK) && bf_sp<_buffer+sz+1 )
			{
				if ( *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
				{
					repeat_start= ++bf_sp;
					while ( bf_sp<_buffer+sz+1 && *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
					{
						bf_sp++;
					}
					repeat_stop=bf_sp+1;

					memcpy(repeat_buffer, repeat_start, abs(repeat_start-repeat_stop));
					#if defined _DEBUG
					printf("+++ executeq: repeat %s(%d) times\n", repeat_buffer, atoi(repeat_buffer));
					#endif

					repeat=atoi(repeat_buffer);

					memset(repeat_buffer, 0, BUFFER_SIZE);
				}
				else
				{
					#if defined _DEBUG
					printf("--- executeq: '%c' NAN\n", *(bf_sp+1));
					#endif
				}
			}

			if ( bf_mp-repeat>=bf_mem  )
			{
				#if defined _DEBUG
				printf("0x%X -> ", bf_mp);
				bf_mp-=repeat;
				printf("0x%X\n", bf_mp);
				#else
				bf_mp-=repeat;
				#endif
			}
			else
			{
				#if defined _DEBUG
				printf("--- execute<: out of memory bounds\n");
				#else
				printf("warning: out of memory bounds\n");
				#endif
				if ( bfo & BFO_QUICK )
				{
					bf_mp=bf_mem;
				}
			}
		}

		if ( *bf_sp=='.' )
		{
			#if defined _DEBUG
			printf("+++ execute: .\n");
			#endif

			#if defined _DEBUG
			memDump();
			#else
			putchar(*bf_mp);
			#endif

			if ( used_memory<bf_mp )
			{
				used_memory=bf_mp;
			}
		}

		if ( *bf_sp==',' )
		{
			#if defined _DEBUG
			printf("+++ execute: ,\n");
			#endif

			if ( (bfo & BFO_QUICK) && bf_sp<_buffer+sz+1 )
			{
				if ( *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
				{
					repeat_start= ++bf_sp;
					while ( bf_sp<_buffer+sz+1 && *(bf_sp+1)>=48 && *(bf_sp+1)<=57 )
					{
						bf_sp++;
					}
					repeat_stop=bf_sp+1;

					memcpy(repeat_buffer, repeat_start, abs(repeat_start-repeat_stop));
					#if defined _DEBUG
					printf("+++ executeq: repeat %s(%d) times\n", repeat_buffer, atoi(repeat_buffer));
					#endif

					repeat=atoi(repeat_buffer);

					memset(repeat_buffer, 0, BUFFER_SIZE);
				}
				else
				{
					#if defined _DEBUG
					printf("--- executeq: '%c' NAN\n", *(bf_sp+1));
					#endif
					repeat=1;
				}
			}

			while ( repeat )
			{
				*bf_mp=(UCHAR)getchar();
				#if defined _DEBUG
				printf("+++ execute,: 0x%x\n", *bf_mp);
				#endif

				if ( bf_mp<bf_mem+BF_MEMORY_SIZE)
				{
					bf_mp++;
				}
				else
				{
					#if defined _DEBUG
					printf("--- executeq: out of memory bounds. input into last cell (%d left)\n", repeat);
					#else
					printf("warning: out of memory bounds. input into last cell\n");
					#endif
				}
				repeat--;
			}

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
				memcpy(tmp_buffer, bf_sp, tmp_sz);
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
			
			while ( tmp_buffer && !validate(tmp_buffer) )
			{
				#if defined _DEBUG
				printf("+++ execute[::searchEndOfCycle: '%s'(%d) is invalid : ++\n", tmp_buffer, tmp_sz);
				#endif
				tmp_bf_sp=index(tmp_bf_sp, ']')+1;
				tmp_sz=abs(bf_sp-tmp_bf_sp);
				tmp_buffer=(UCHAR*)realloc(tmp_buffer, tmp_sz);
				memset(tmp_buffer, 0, tmp_sz);

				if ( tmp_buffer )
				{
					memcpy(tmp_buffer, bf_sp, tmp_sz);
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

	free(repeat_buffer);

	return true;
}

void memDump()
{
	#if defined _DEBUG
	printf("\nmemDump():\n");
	#endif

	if ( used_memory==0 )
	{
		return;
	}
	
	int i=0;
	int sz=abs(used_memory-bf_mem)+1;		// dump only used memory.

	int mp=abs(bf_mem-bf_mp);
	BOOL bmp=false;
	#if defined _DEBUG
	printf("+++ memDump: now bf_mp points to #%d cell\n", mp);
	#endif

	#if defined _DEBUG
	printf("+++ memDump: sz=%d (0x%X && 0x%X)\n", sz, bf_mem, used_memory);
	#endif

	for ( i=0; i<sz; i++ )
	{
		bmp= (i==mp) ? true : false;

		// line number
		if ( i%16==0 )
		{
			printf("%08X  ", bf_mem+i);
		}

		if ( i%2==0)
		{
			#if defined linux
			if ( bmp )
			{
				setColor(CL_YELLOW);
				printf("%02x", bf_mem[i]);
				setColor(CL_NONE);
			}
			else
			{
				printf("%02x", bf_mem[i]);
			}
			#else
			printf( bmp ? "*%02x*" : "%02x",
					bf_mem[i]);
			#endif
		}
		else
		{
			if ( (i%16)%7==0 )
			{
				#if defined linux
				if ( bmp )
				{
					setColor(CL_YELLOW);
					printf("%02x  ", bf_mem[i]);
					setColor(CL_NONE);
				}
				else
				{
					printf("%02x  ", bf_mem[i]);
				}
				#else
				printf( bmp ? "*%02x*  ": "%02x  ",
						bf_mem[i]);
				#endif
			}
			else
			{
				#if defined linux
				if ( bmp )
				{
					setColor(CL_YELLOW);
					printf("%02x ", bf_mem[i]);
					setColor(CL_NONE);
				}
				else
				{
					printf("%02x ", bf_mem[i]);
				}
				#else
				printf( bmp ? "*%02x* " : "%02x ",
						bf_mem[i]);
				#endif
			}
		}

		if ( (i+1)%16==0)
		{
			printf("\n");
		}
	}
	printf("\n");
}

void printSource(UCHAR* _buffer, UCHAR* _sp)
{
	#if defined _DEBUG
	printf("+++ printSource: '%s' -> '%c'(%d)\n", _buffer, *_sp, abs(_buffer-_sp));
	#else
	printf("current source: ");
	#endif

	int delta=15;
	
	UCHAR* start=0;
	UCHAR* stop=0;
	int sz=strlen(_buffer);

	if ( _sp-delta>=_buffer )
	{
		start=_sp-delta;
	}
	else
	{
		start=_buffer;
	}
	
	if ( _sp+delta<=_buffer+sz )
	{
		stop=_sp+delta;
	}
	else
	{
		stop=_buffer+sz;
	}

	UCHAR* i=0;
	for ( i=start; i<stop; i++ )
	{
		if ( i==_sp )
		{
			#if defined linux
			setColor(CL_YELLOW);
			printf("%c", *i);
			setColor(CL_NONE);
			#else
			printf("**%c**", *i);
			#endif
		}
		else
		{
			printf("%c", *i);
		}
	}
	printf("\n");
}