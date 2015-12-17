#ifndef MACHINEINSTR_H
#define MACHINEINSTR_H

#include <ctype.h>

#define MAX_SIZE_PROG 512

#define BASESIZE_CMD 1000
#define UPPERSIZE_CMD (BASESIZE_CMD*10)


#define NOTHING 0

#define READ 10
#define READSTR (READ+3)	/*13, but it is easier in this way */
#define WRITE 11
#define WRITESTR (WRITE+3)	/*14, but... */
#define PUTCHAR 15

#define LOAD 20
#define STRLOAD (LOAD+3)
#define RPNLOAD (LOAD+2)
#define STORE 21
#define STRSTORE (STORE+3)

#define ADD 30
#define SUB 31
#define MUL 32
#define DIV 33
#define POW 34

#define _PLUS_ 500
#define _MINUS_ 501
#define _AND_ 502
#define _FRACT_ 503
#define _RAISE_ 504



#define JUMP 40
#define JUMPZERO (JUMP+1)
#define JUMPNEG (JUMP+2)
#define JUMPPOS (JUMP+3)

#define CALL_ROUTINE 46
#define BEG_ROUTINE 77
#define RETURN 88
#define BACK 66

#define HALTPROGEXEC 44



int *commands;			/*use an int array for commands */
float *vars_and_consts;		/*use a float array for variables and constants */
char **str_vars;
				     /*use a string array for string IO */
float ACCUMULATOR;
int **ASCII_ACCUMULATOR;

void exec (const char *arg);


float makePowf (double *a, double *b);

int int_cmp (const void *const a, const void *const b);


#endif
