/*
  brainf*k interpreter written in C with little extension -- QBF
  Copyright (C) 2011  Ilya Arefiev <arefiev.id@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

	if ( argc>1 )
	{
		if ( findParam(argv, argc, "-h")>0 ||
			findParam(argv, argc, "--help")>0
			)
		{
			help(argv[0]);
			return RET_SUCCES;
		}
	}
	else
	{
		printf("error: not enought params\n");
		help(argv[0]);
		return RET_ERR_PARAMS;
	}

	bfo=0;
	if ( findParam(argv, argc, "-d")>0 ||
		findParam(argv, argc, "--debug")>0
		)
	{
		bfo |= BFO_DEBUG;
		#if defined _DEBUG
		printf("+++ main: in debug BF mode: bfo: 0x%X\n", bfo);
		#endif
	}

	bf_mem=(UCHAR*)calloc(BF_MEMORY_SIZE, sizeof(char));
	if ( !bf_mem )
	{
		#if defined _DEBUG
		printf("--- main: bf_mem=NULL\n");
		#else
		printf("error: can't allocate memory for BF cells\n");
		#endif
		return RET_ERR_MEM;
	}

	FILE* bf_sc_fd;
	if ( argc>1 && strEndsWith(argv[argc-1], "bf") )
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
						memset(bf_sc+(BUFFER_SIZE*bf_sc_sz), 0, BUFFER_SIZE);
					}
				}
				else
				{
					#if defined _DEBUG
					printf("--- bf_sc=NULL\n");
					#endif
					return RET_ERR_MEM;
				}
			}
			close(bf_sc_fd);

			if ( bf_sc )
			{
				#if defined _DEBUG
				printf("bf_sc @ 0x%X (%d):\n'%s'\n", bf_sc,
					   BUFFER_SIZE*(bf_sc_sz+1)*sizeof(char),
					   bf_sc);
				#endif

				if ( strEndsWith(argv[argc-1], ".qbf") ||
					findParam(argv, argc, "-q")>0 ||
					findParam(argv, argc, "--quick")>0
					)
				{
					bfo |= BFO_QUICK;
					#if defined _DEBUG
					printf("+++ main: in quick BF mode: bfo: 0x%X\n", bfo);
					#endif
					
					#if defined _DEBUG
					printf("+++ main: prepare sc\n");
					#endif
					
					bf_sc=prepare(bf_sc);
					if ( bf_sc )
					{
						#if defined _DEBUG
						printf("bf_sc @ 0x%X (%d):\n'%s'\n", bf_sc,
							BUFFER_SIZE*(bf_sc_sz+1)*sizeof(char),
							bf_sc);
						#endif
					}
					else
					{
						#if defined _DEBUG
						printf("--- main: can't prepare SC\n");
						#endif
						return RET_ERR_SC;
					}
				}
			}

			if ( findParam(argv, argc, "-sc")>0 ||
				findParam(argv, argc, "--show-code")>0
				)
			{
				printf("Source code:\n'%s'\n", bf_sc);
			}

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
		printf("'%s' seems to be not a BF source\n", argv[argc-1]);
	}

	#if defined _DEBUG
	printf("+++ stop\n");
	#endif

	return RET_SUCCES;
};


int findParam(char** _where, int _count, UCHAR* _param)
{
	#if defined _DEBUG
	printf("+++ findParam: count(%d) param(%s)\n", _count, _param);
	#endif
	int i=0;
	for ( i=1; i<_count; i++ )
	{
		#if defined _DEBUG
		printf("+++ findParam: compare #%d %s && %s\n", i, _where[i], _param);
		#endif
		
		if ( strcmp(_where[i], _param)==0 )
		{
			#if defined _DEBUG
			printf("+++ findParam: %s found @ %d\n", _param, i);
			#endif
			return i;
		}
	}

	return -1;
}

void help(char* _name)
{
	printf("quick BrainF*k interpreter v%02.2f\n\n", VERSION);
	
	printf("Copyright (C) 2011  Ilya Arefiev <ilardm@gmail.com>\n");
	printf("This program licensed under GPLv3 and comes with ABSOLUTELY NO WARRANTY;\n");
    printf("This is free software, and you are welcome to redistribute it\n");
    printf("under certain conditions.\n");
	printf("For details visit <http://www.gnu.org/licenses/gpl.html>\n\n");

	if ( _name )
	{
		printf("Usage: %s [options] <path_to_BF_source>\n\n", _name);
	}
	else
	{
		#if defined _DEBUG
		printf("--- help: _name=NULL\n");
		#endif
		printf("Usage: bfi [options] <path_to_BF_source>\n\n");
	}
	
	printf("Options:\n");
	printf("\t-h, --help\tshow this help\n");
	printf("\t-q, --quick\tto interpreter BF source as quick BrainF*k\n");
	printf("\t-d, --debug\tto debug(stop on breakpoints) BF source\n");
	printf("\t-sc, --show-code\tto show BF source (which will be executed) before execute it\n");

	printf("\n");

	printf("<path_to_BF_source> must ends with '.bf' to execute it as pure BrainF*k code\n");
	printf("<path_to_BF_source> must ends with '.qbf' to execute it as quick BrainF*k code\n");
}
