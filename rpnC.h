#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tables.h"
											/*link with -lm for powf() */
#define NO_MORE_ELEMENTS "NONE"
#define DEFAULT 0

typedef enum
{ false, true } bool;


struct stackNode
{
  char *data;			/*a strings, not a char */
  struct stackNode *nextPtr;
};


typedef struct stackNode StackNode;
typedef StackNode *StackNodePtr;

struct precedence
{
  int prec;
  char *op;
} priority[] =
{
  {
  0, "+"},
  {
  0, "-"},
  {
  1, "*"},
  {
  1, "/"},
  {
  1, "%"},
  {
  2, "^"}
};

typedef struct precedence PRIOR;

#define _opr_size sizeof(priority)/sizeof(PRIOR)


int comp (const void *const k1, const void *const k2);
int comp1 (const void *const k1, const void *const k2);
bool isOper (char item);

void push (StackNodePtr * topPtr, char *info);
char *pop (StackNodePtr * topPtr);
char *top (StackNodePtr * topPtr);
int isEmpty (StackNodePtr topPtr);
void printStack (StackNodePtr currentPtr);
				  /*written due to problems when linking with -lm */
char *rpnConv (char *infix);
bool isConst (char *tok);

float rpnEval (char *postfix);
