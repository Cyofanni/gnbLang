/*
 *
 * 
 *                                                     _________   
 * 												 	  | 		 \
 *                                                    |           \
 *           ______                __ __      ___     |           /       
 *          /  ______\            |   \ \     |  |    |         /      __      __
 *         /  /                   |  | \ \    |  |	  |        /       \ \    / /
 * 	      /  /                    |  |  \ \   |  |    |        \        \ \  / /
 * 		 /  /        _________    |  |   \ \  |  |    |         \        \ \/ /
 *       |  |        |__   __|    |  |    \ \ |  |    |         /        / /\ \ 
 *       |  |__________|  |       |  |     \ \   |    |        /        / /  \ \
 *       |________________| 	  |__|      \____|    |_______/        /_/ 	  \_\ 	    
 * 
 *    ------------------>>>>>> A MACHINE SIMULATOR FOR GNB PROGRAMMING LANGUAGE COMPILED CODE
 */


#include <signal.h>
#include "rpnC.c"
#include "tables.h"
#include "machineinstr.h"

int *commands = NULL;		/*use an int array for commands */
float *vars_and_consts = NULL;	/*use a float array for variables and constants */
char **str_vars = NULL;

float ACCUMULATOR = 0;
int **ASCII_ACCUMULATOR = NULL;


int
main (int argc, char **argv)
{
  exec (argv[1]);

  exit (EXIT_SUCCESS);
}

int
int_cmp (const void *const a, const void *const b)
{
  return (*(unsigned int *) a - *(unsigned int *) b);
}

void
exec (const char *arg)
{
  char postfix[96] = "";
  int read (const char *file, int *const intArray, float *const floatArray);
  commands = calloc (MAX_SIZE_PROG - 100, sizeof (int));
  vars_and_consts = calloc (MAX_SIZE_PROG, sizeof (float));
  str_vars = calloc (MAX_SIZE_PROG - 100, sizeof (char *));
  ASCII_ACCUMULATOR = calloc (MAX_SIZE_PROG - 100, sizeof (int *));	/*FREE IT */


  int countchar = 0;

  for (countchar = 0; countchar < MAX_SIZE_PROG - 100; countchar++)
    {
      commands[countchar] = 0;	/*initialize */
      vars_and_consts[countchar] = 0;
    }

  read (arg, commands, vars_and_consts);
  int exec;
  int **const ptr = &commands;
  float **varconstPtr = &vars_and_consts;
  int array_counter = 0;




  countchar -= countchar;
  if (!ptr || !*ptr)
    {
      perror ("Memory not available");
    }

  int currentPos = 0;

  for (exec = 0; *(*ptr + exec) != '\0'; exec++)
    {
      if ((*(*ptr + exec) > BASESIZE_CMD) && (*(*ptr + exec) < UPPERSIZE_CMD))
	{
	  switch ((*(*ptr + exec)) / 100)
	    {

	    case READ:		/*display a numeric value on the screen */
	      fscanf (stdin, "%f", &(*(*varconstPtr + (*(*ptr + exec) % 100))));	/*NOT ONLY MOD 100 */
	      break;

	    case WRITE:
	      fprintf (stdout, "%.2f", (*(*varconstPtr + (*(*ptr + exec) % 100))));	/*NOT ONLY MOD 100 */
	      break;

	    case PUTCHAR:
	      fputc ((char) ((*(*ptr + exec)) % 100), stdout);
	      break;

	    case LOAD:
	      ACCUMULATOR = (*(*varconstPtr + (*(*ptr + exec) % 100)));
	      break;

	    case STRLOAD:
	      *(ASCII_ACCUMULATOR + array_counter) = malloc (100 * sizeof (int));	/*FREE IT */
	      while (((*(*ptr + exec)) % 100) != toascii ('$'))
		{
		  ASCII_ACCUMULATOR[array_counter][countchar] =
		    (*(*ptr + exec) % 1000);
		  exec++;
		  countchar++;
		}
	      ASCII_ACCUMULATOR[array_counter][countchar + 1] = -9999;
	      countchar = 0;
	      ++array_counter;
	      break;

	    case RPNLOAD:
	      while (*(*ptr + exec) / 100 == RPNLOAD)
		{
		  if ((*(*ptr + exec) % 100) < _PLUS_)
		    {		/*only for readability */

		      char float_to_str[24] = "";	/*vars and consts */
		      snprintf (float_to_str, 24, "%.2f",
				vars_and_consts[(*(*ptr + exec) % 100)]);
		      strcat (postfix, float_to_str);
		      strcat (postfix, " ");
		    }
		  exec++;
		}
	      exec--;		/*decrease it */

	      break;

	    case STRSTORE:
	      (*(str_vars + (*(*ptr + exec)) % 100)) = NULL;
	      (*(str_vars + (*(*ptr + exec)) % 100)) =
		malloc (sizeof (char) * 156);

	      while (ASCII_ACCUMULATOR[array_counter - 1][countchar] != -9999)
		{
		  (*(str_vars + (*(*ptr + exec)) % 1000))[countchar] =
		    (char) ASCII_ACCUMULATOR[array_counter - 1][countchar];
		  putchar ((*(str_vars + (*(*ptr + exec)) % 1000))
			   [countchar]);
		  ++countchar;
		}
	      countchar = 0;

	      break;

	    case STORE:
	      ACCUMULATOR = rpnEval (postfix);
	      (*(*varconstPtr + (*(*ptr + exec) % 100))) = ACCUMULATOR;
	      snprintf (postfix, sizeof (postfix), "%s", " ");	/*do the job here */
	      break;


	    case ADD:
	      ACCUMULATOR += (*(*varconstPtr + (*(*ptr + exec) % 100)));
	      break;

	    case SUB:		/*only for readability *//*###################################### */
	      ACCUMULATOR = rpnEval (postfix);
	      sprintf (postfix, "%s", " ");
	      exec++;
	      if (*(*ptr + exec) / 100 == RPNLOAD
		  || *(*ptr + exec) / 1000 == RPNLOAD)
		{
		  while (*(*ptr + exec) / 100 == RPNLOAD
			 || *(*ptr + exec) / 1000 == RPNLOAD)
		    {
		      if ((*(*ptr + exec) / 100) == RPNLOAD)
			{	/*only for readability */
			  char float_to_str[24] = "";	/*vars and consts */
			  snprintf (float_to_str, 24, "%.2f",
				    vars_and_consts[(*(*ptr + exec) % 100)]);
			  strcat (postfix, float_to_str);
			  strcat (postfix, " ");
			}
		      else if ((*(*ptr + exec) / 1000) == RPNLOAD
			       && (*(*ptr + exec) % 1000) >= _PLUS_)
			{
			  Math key, *value;
			  qsort (Operators, num_oper, sizeof (Math), int_cmp);
			  key.ident = (*(*ptr + exec) % 1000);
			  value =
			    bsearch (&key, Operators, 5, sizeof (Math),
				     int_cmp);
			  strcat (postfix, value->oper);
			  strcat (postfix, " ");
			}
		      else if ((*(*ptr + exec) / 1000) == RPNLOAD
			       && (*(*ptr + exec) % 1000) < _PLUS_)
			{
			  char float_to_str[24] = "";	/*vars and consts */
			  snprintf (float_to_str, 24, "%.2f",
				    vars_and_consts[(*(*ptr + exec)) % 1000]);
			  strcat (postfix, float_to_str);
			  strcat (postfix, " ");
			}
		      exec++;
		    }
		  ACCUMULATOR -= rpnEval (postfix);
		  sprintf (postfix, "%s", " ");
		  exec--;	/*###################################### */
		}

	      break;

	    case MUL:
	      ACCUMULATOR *= (*(*varconstPtr + (*(*ptr + exec) % 100)));
	      break;

	    case DIV:
	      ACCUMULATOR /= (*(*varconstPtr + (*(*ptr + exec) % 100)));
	      break;

	    case JUMP:		/*############################# */
	      exec = ((*(*ptr + exec)) % 100) - 1;	/*jump to the one before */
	      break;

	    case JUMPZERO:
	      if (ACCUMULATOR == 0)
		{
		  exec = ((*(*ptr + exec)) % 100) - 1;	/*############### */
		}
	      break;

	    case JUMPNEG:
	      if (ACCUMULATOR < 0)
		{
		  exec = ((*(*ptr + exec)) % 100) - 1;	/*########### */
		}
	      break;

	    case JUMPPOS:
	      if (ACCUMULATOR > 0)
		{
		  exec = ((*(*ptr + exec)) % 100) - 2;	/*########### */
		}
	      break;

	    case BEG_ROUTINE:

	      while ((*(*ptr + exec)) / 100 != RETURN)
		{
		  ++exec;
		}
	      break;

	    case CALL_ROUTINE:
	      currentPos = exec;
	      exec = (*(*ptr + exec) % 100);
	      break;

	    case RETURN:
	      exec = currentPos;
	      break;
	    }
	}

      if (*(*ptr + exec) > UPPERSIZE_CMD)
	{
	  /*do the same stuff */
	  switch ((*(*ptr + exec)) / 1000)
	    {

	    case READ:		/*display a numeric value on the screen */
	      fscanf (stdin, "%f", &(*(*varconstPtr + (*(*ptr + exec) % 1000))));	/*NOT ONLY MOD 100 */
	      break;

	    case WRITE:
	      fprintf (stdout, "%.2f", (*(*varconstPtr + (*(*ptr + exec) % 1000))));	/*NOT ONLY MOD 100 */
	      break;

	    case PUTCHAR:
	      fputc ((char) ((*(*ptr + exec)) % 1000), stdout);
	      break;

	    case LOAD:
	      ACCUMULATOR = (*(*varconstPtr + (*(*ptr + exec) % 1000)));
	      break;

	    case STRLOAD:
	      *(ASCII_ACCUMULATOR + array_counter) = malloc (100 * sizeof (int));	/*FREE IT */
	      while (((*(*ptr + exec)) % 100) != toascii ('$'))
		{
		  ASCII_ACCUMULATOR[array_counter][countchar] =
		    (*(*ptr + exec) % 1000);
		  exec++;
		  countchar++;
		}
	      ASCII_ACCUMULATOR[array_counter][countchar + 1] = -9999;
	      countchar = 0;
	      ++array_counter;
	      break;

	    case STRSTORE:
	      (*(str_vars + (*(*ptr + exec)) % 1000)) = NULL;
	      (*(str_vars + (*(*ptr + exec)) % 1000)) =
		malloc (sizeof (char) * 156);


	      while (ASCII_ACCUMULATOR[array_counter - 1][countchar] != -9999)
		{
		  (*(str_vars + (*(*ptr + exec)) % 1000))[countchar] =
		    (char) ASCII_ACCUMULATOR[array_counter - 1][countchar];
		  ++countchar;
		}
	      countchar = 0;

	      break;

	      /*############################################## */
	    case RPNLOAD:
	      while (*(*ptr + exec) / 1000 == RPNLOAD)
		{
		  if ((*(*ptr + exec) % 1000) < _PLUS_)
		    {		/*only for readability */

		      char float_to_str[24] = "";	/*vars and consts */
		      snprintf (float_to_str, 24, "%.2f",
				vars_and_consts[(*(*ptr + exec)) % 1000]);
		      strcat (postfix, float_to_str);
		      strcat (postfix, " ");
		    }

		  else if ((*(*ptr + exec) % 1000) >= _PLUS_)
		    {
		      Math key, *value;
		      qsort (Operators, num_oper, sizeof (Math), int_cmp);
		      key.ident = (*(*ptr + exec) % 1000);
		      value =
			bsearch (&key, Operators, 5, sizeof (Math), int_cmp);
		      strcat (postfix, value->oper);
		      strcat (postfix, " ");
		    }
		  exec++;
		}
	      exec--;
	      /*DECREASE THE COUNTER */
	      break;
	      /*################################################# */

	    case STORE:	/*only for the sake of readability */

	      (*(*varconstPtr + (*(*ptr + exec) % 1000))) = ACCUMULATOR;
	      break;


	    case ADD:
	      ACCUMULATOR += (*(*varconstPtr + (*(*ptr + exec) % 1000)));
	      break;

	      /*###################################################### */
	      /*SIB DISCARDED */
	      /*###################################################### */

	    case MUL:
	      ACCUMULATOR *= (*(*varconstPtr + (*(*ptr + exec) % 1000)));
	      break;

	    case DIV:
	      ACCUMULATOR /= (*(*varconstPtr + (*(*ptr + exec) % 1000)));
	      break;

	    case JUMP:
	      exec = (*(*ptr + exec)) % 1000;
	      break;

	    case JUMPZERO:
	      if (ACCUMULATOR == 0)
		{
		  exec = (*(*ptr + exec)) % 1000;
		}
	      break;

	    case JUMPNEG:
	      if (ACCUMULATOR < 0)
		{
		  exec = (*(*ptr + exec)) % 1000;
		}
	      break;

	    case JUMPPOS:
	      if (ACCUMULATOR > 0)
		{
		  exec = (*(*ptr + exec)) % 1000;
		}
	      break;


	    case BEG_ROUTINE:
	      while ((*(*ptr + exec)) % 1000 != RETURN)
		{
		  ++exec;
		}
	      break;

	    case CALL_ROUTINE:
	      currentPos = exec;
	      exec = (*(*ptr + exec) % 1000);
	      break;

	    case RETURN:
	      exec = currentPos;
	      break;


	    case READSTR:	/*read a string on a location of str_vars array */
	      (*(str_vars + (*(*ptr + exec)) % 1000)) = NULL;
	      (*(str_vars + (*(*ptr + exec)) % 1000)) =
		malloc (sizeof (char) * 156);
	      char readTemp[124] = "";
	      fgets (readTemp, sizeof (readTemp), stdin);
	      snprintf ((*(str_vars + (*(*ptr + exec)) % 1000)), 156, "%s",
			readTemp);
	      break;

	    case WRITESTR:
	      if (*(str_vars + (*(*ptr + exec)) % 1000))
		{		/*do not attempt to display it without previous memory allocation!! */
		  fprintf (stdout, "%s",
			   *(str_vars + (*(*ptr + exec)) % 1000));
		}
	      else
		{
		  /*nothing to be done */
		}
	      break;
	    }

	}
    }


  for (exec = 0; str_vars[exec] != NULL; exec++)
    {
      free (str_vars[exec]);
      free (ASCII_ACCUMULATOR[exec]);
    }

  free (ASCII_ACCUMULATOR);
  free (str_vars);
  free (commands);
  free (vars_and_consts);
}


int
read (const char *file, int *const intArray, float *const floatArray)
{
  unsigned int i = 0;
  FILE *cPtr = NULL;
  char temp[24] = "";
  cPtr = fopen (file, "r");
  float *const floatPtr = floatArray;
  int begVar = 99;		/*THESE ARE VERY LIKELY TO BE CHANGED */
  int begConst = 100;

  if (!cPtr)
    {
      perror (" ");		/*no such file or directory */
      raise (SIGTERM);
    }


  while (!feof (cPtr))
    {
      fscanf (cPtr, "%s", temp);
      intArray[i] = atoi (temp);
      i++;
      if (strcmp (temp, "4400") == 0)
	{
	  break;
	}
    }


  while (!feof (cPtr))
    {
      fscanf (cPtr, "%s", temp);
      if (strncmp (temp, "0000", 4) == 0)
	{
	  *(floatPtr + begVar) = atof (temp);
	  --begVar;
	}
      else if (strncmp (temp, "0000", 4) != 0)
	{
	  *(floatPtr + begConst) = atof (temp);
	  ++begConst;
	}

    }

  return i;
}

/*RUNTIME EVALUATION*/
float
rpnEval (char *postfix)
{
  StackNodePtr stackPtr;
  char *tokPtr = NULL;
  tokPtr = strtok (postfix, " ");
  push (&stackPtr, NO_MORE_ELEMENTS);
  float result = 0;
  char strres[12];
  double dx;
  double dy;

  while (tokPtr != NULL)
    {

      if (isConst (tokPtr))
	{			/*if current token is a number */
	  push (&stackPtr, tokPtr);
	}



      else if (isOper (*tokPtr))
	{
	  char *x = top (&stackPtr);	/*still strings; then float values */
	  pop (&stackPtr);
	  char *y = top (&stackPtr);
	  pop (&stackPtr);

	  switch (*tokPtr)
	    {			/*which operator?? */
	    case '+':
	      result = atof (y) + atof (x);
	      snprintf (strres, 12, "%2.f", result);
	      push (&stackPtr, strres);

	      break;
	    case '-':
	      result = atof (y) - atof (x);
	      snprintf (strres, 12, "%.2f", result);
	      push (&stackPtr, strres);
	      break;
	    case '*':
	      result = atof (y) * atof (x);
	      snprintf (strres, 12, "%.2f", result);
	      push (&stackPtr, strres);
	      break;
	    case '/':
	      result = atof (y) / atof (x);
	      snprintf (strres, 12, "%.2f", result);
	      push (&stackPtr, strres);
	      break;
	    case '%':
	      result = atoi (y) % atoi (x);
	      snprintf (strres, 12, "%.2f", result);
	      push (&stackPtr, strres);
	      break;
	    case '^':
	      dy = atof (y);
	      dx = atof (x);
	      result = makePowf (&dy, &dx);
	      snprintf (strres, 12, "%.2f", result);
	      push (&stackPtr, strres);
	      break;
	    }

	}
      else
	{
	  /*nothing to be done */
	}

      tokPtr = strtok (NULL, " ");
    }

  float cptop = 0;
  cptop = atof (top (&stackPtr));	/*save top of stack */

  while (strncmp (top (&stackPtr), NO_MORE_ELEMENTS, 4) != 0)
    {
      pop (&stackPtr);
    }

  return cptop;
}


float
makePowf (double *a, double *b)
{				/*written due to problems when linking with -lm */
  int count = 0;
  double result = *a;

  while (count < (*b - 1))
    {
      result *= *a;
      count++;
    }

  return result;
}
