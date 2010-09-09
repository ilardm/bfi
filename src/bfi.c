#include <string.h>
#include <malloc.h>
#include <memory.h>

#include "bfi.h"
#include "main.h"

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

bool execute(unsigned char* _buffer)
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

			if ( bf_mp<bf_mem+BF_MEMORY_SIZE)
			{
				#if defined _DEBUG
				printf("0x%X -> ", bf_mp);
				bf_mp++;
				printf("0x%X\n", bf_mp);
				#else
				bf_mp++;
				#endif
			}
			else
			{
				#if defined _DEBUG
				printf("--- execute>: out of memory range\n");
				#else
				printf("error: out of memory\n");
				#endif
				return false;
// 				break;
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
				printf("--- execute<: out of memory range\n");
				#else
				printf("error: out of memory\n");
				#endif
				return false;
// 				break;
			}
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

			unsigned char* tmp_bf_sp=index(bf_sp, ']')+1;
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
				tmp_bf_sp=index(tmp_bf_sp, ']')+1;
				tmp_sz=abs(bf_sp-tmp_bf_sp);
				tmp_buffer=(unsigned char*)realloc(tmp_buffer, tmp_sz);

				if ( tmp_buffer )
				{
					strncpy(tmp_buffer, bf_sp, tmp_sz);
					tmp_buffer[tmp_sz]=0;
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
