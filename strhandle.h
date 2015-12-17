#ifndef STRHANDLE_H
#define STRHANDLE_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


int querytok (char *from, const char *const what, int numchar);
unsigned int cntitem (const char in[], char what);
int findOp (const char str[], const char *comp, const char *_goto_);
char **commatocmd (char **args);



#endif
