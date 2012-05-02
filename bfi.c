/* Copyright (C) 2010, 2012 Philipp Winter (identity.function@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE	65536		/* amount of virtual memory */
#define PROG_SIZE	MEM_SIZE	/* available space for program code */

int mem[MEM_SIZE];		/* the virtual memory */
char prog[PROG_SIZE];	/* cached copy of the program */
int *dp = NULL;			/* global data pointer */

void init( const char *file ) {

	FILE *fp = NULL;
	size_t ret = 0;
	size_t offset = 0;

	memset(mem, 0, (size_t) MEM_SIZE * sizeof(int));
	memset(prog, 0, (size_t) PROG_SIZE * sizeof(char));
	dp = mem;

	fp = fopen(file, "r");
	do {
		if (offset >= PROG_SIZE) {
			fprintf(stderr, "Program buffer not large enough to hold code.\n");
			exit(1);
		}
		ret = fread(prog+offset, 1, 512, fp);
		offset += ret;
	} while (ret > 0);
	fclose(fp);
}

void interpret( const char *ip ) {

	int nest = 0;

#ifdef DEBUG
	printf("interpreting symbol %c [%p:+%ld]\n", *ip, ip, (ip - prog));
#endif

	while (1) {
		switch (*ip) {
			case '>':
				dp++;
				break;
			case '<':
				dp--;
				break;
			case '+':
				(*dp)++;
				break;
			case '-':
				(*dp)--;
				break;
			case '.':
				putchar(*dp);
				break;
			case ',':
				(*dp) = getchar();
				break;
			case '[':
				if (*dp != 0) { /* continue loop */
					interpret(ip+1);
					ip--;
				} else if (*dp == 0) { /* end loop */
					nest = 1;
					do {
						ip++;
						if (*ip == '[') nest++;
						if (*ip == ']') nest--;
					} while (nest > 0);
				}
				break;
			case ' ':
			case '\t':
			case '\n':
				break;
			case ']':
			case '\0':
				return;
			default:
				fprintf(stderr, "Invalid symbol: \\x%x.\n", *ip);
				exit(1);
		}
		ip++;
	}
}

int main( int argc, char **argv ) {

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		return 1;
	}

	init(argv[1]);
	interpret(prog);

	return 0;
}
