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
