#include <time.h>
#include "strhandle.c"
#include "tables.h"
#include "machineinstr.h"
#include "err.h"



/*LOAD SOURCE CODE ON THIS ARRAY*/


cpvars safevars[100];

cptable safetable[100];

static int jumpcount = 0;
static int *jumps = NULL;
static int countTok = 0;
static int constcount = 0;
static int back_to_env[256] = { };

static int env_counter = 0;
static struct consts **constants = NULL;

int
main (int argc, char **argv)
{
  if (!argv[1])
    {
      FATAL ("***HALT!\n");
      FATAL ("***no input files***\n");
      FATAL ("---->compilation terminated\n");
    }
  else
    {
      parsimonious (argv[1], argv[2]);
      still_parsimonious (argv[2]);
    }
  return EXIT_SUCCESS;
}


void
parsimonious (char *file, char *obj)
{
  int compare (const void *const k1, const void *const k2);

  char **prog = NULL;
  unsigned int num_lines = 0;
  constants = malloc (256 * sizeof (struct consts *));

  int t;
  int i;
  char line[96] = " ";

  FILE *read;
  FILE *wPtr;
  read = fopen (file, "r");

  wPtr = fopen (obj, "w");

  file_err (read);
  file_err (wPtr);


  for (t = 0; t < 100; t++)
    {
      constants[t] = malloc (sizeof (struct consts));
    }

  qsort (Keyword, nr_instr, sizeof (struct keywords), compare);	/*qsort before a binary search */

  prog = malloc (sizeof (char *) * 256);

  for (i = 0; i < 256; i++)
    {				/*keep trace of the previous word */
      *(prog + i) = NULL;
      *(prog + i) = malloc (sizeof (char) * 96);
    }


  i -= i;

  while (!feof (read))
    {				/*read the file */
      num_lines++;
      strncpy (prog[i], line, 96);
      fgets (line, sizeof (line), read);
      prog[i][strlen (prog[i]) - 1] = ' ';	/*IS THE SPACE CAUSING PROBLEMS?? *//*\n to ' ' */
      i++;
    }

  i -= i;

  commatocmd (prog);		/*make it more compiler readable */

  /*######END OF FIRST PART############## */
  int j = 1;
  char *tok;

  int countLoc = 0;		/*############################### */
  unsigned int countVar = 99;
  unsigned int countStr = 199;	/*do the same stuff with strings */
  unsigned int countConst = 100;

  char strsave[BUFSIZ] = "";

  int tmpcount = 0;

  int varcount = 0;		/*count new variables */

  int funkcount = 0;


  for (j = 0; j < 256; j++)
    {
      back_to_env[j] = 0;
    }


  int cpcounter = 0;
  char **toktok = NULL;
  toktok = malloc (sizeof (char *) * 256);

  int k = 0;

  for (k = 0; k < 256; k++)
    {
      *(toktok + k) = NULL;
      *(toktok + k) = malloc (sizeof (char) * 32);
    }

  k -= k;


  jumps = malloc (256 * sizeof (int));




  for (j = 1; j != (num_lines); j++)
    {				/*use more counters */
      strncpy (strsave, *(prog + j), BUFSIZ);
      tok = strtok (*(prog + j), " ");
      table[countTok].symbol = tok;	/*use another counter, independent from j */
      table[countTok].type = 'L';

      safetable[countTok].symbol = malloc (sizeof (char) * 96);
      snprintf (safetable[countTok].symbol, 96, "%s", tok);
      safetable[countTok].type = 'L';


      if (querytok (strsave, "rem", 4) == 0)
	{			/*write a python-like function to get items from a string */
	  table[countTok].location = countLoc;	/*ASSIGN HERE LOCATION */
	  fprintf (wPtr, "-1\n");	/* is the machine code for comments (must be a number!!!) */
	}

      else if (querytok (strsave, "rem", 4) != 0)
	{
	  table[countTok].location = countLoc;	/*AND HERE OBVIOUSLY *//*assign a machine language location to every uncommented line */
	  safetable[countTok].location = countLoc;	/*#########!!!!!!!!!!!!!! */
	}

      countTok++;
      tmpcount++;

      char cpstr[96];

      while (tok != NULL)
	{
	  snprintf (*(toktok + k), 32, "%s", tok);
	  struct keywords key, *res;
	  key.instrName = tok;

	  res =
	    bsearch (&key, Keyword,
		     (sizeof (Keyword) / sizeof (struct keywords)),
		     sizeof (struct keywords), compare);

	  if (res == NULL)
	    {
	      /*error handling:: THERE MUST BE AT LEAST A KEYWORD */
	    }

	  else if (res != NULL && strncmp (res->instrName, "rem", 3) != 0)
	    {
	      switch (cntitem (strsave, STRTOKEN))
		{		/*how many $??? */
		case 0:
		  if ((res->code) != RPNLOAD)
		    {		/*if RPNLOAD find a better solution (if , let ...) */
		      if (res->code == SUB)
			{
			  fprintf (wPtr, "%d00\n", res->code);	/*if : '5 input $a input $b' ????????? */
			}
		      else if (res->code == JUMP)
			{	/*"if...goto" statement has three instances */
			  switch (findOp (strsave, "==", "goto"))
			    {
			    case -1:
			      fprintf (wPtr, "%d", (res->code) + 2);
			      break;

			    case 1:
			      fprintf (wPtr, "%d", (res->code) + 3);
			      break;

			    case 0:
			      fprintf (wPtr, "%d", (res->code) + 1);
			      break;

			    case -5:
			      fprintf (wPtr, "%d", (res->code));
			      break;
			    }
			}
		      else
			{
			  if (strcmp (tok, "next") == 0)
			    {
			      fprintf (wPtr, "%d\n", res->code);
			      countLoc++;
			    }
			  else
			    {
			      fprintf (wPtr, "%d", res->code);
			    }
			}
		    }

		  break;

		case 1:	/*if there is a string variable */
		  fprintf (wPtr, "%d", (res->code) + 3);
		  countLoc++;
		  break;
		  /*how to deal with 'print $print a if input let a = 3 * 1$' */
		case 2:
		  if (strsave[strlen (strsave) - 2] == STRTOKEN)
		    {		/*print $Hello world$ and print $a , $b are different!!!! */
		      strncpy (cpstr, strsave, 96);	/*if after print there is a printable string like $Hello world$ */
		      char *movptr = strstr (cpstr, "$");
		      ++movptr;

		      while (*movptr != STRTOKEN)
			{	/*case print $Hello world$ */
			  if (*movptr != '%')
			    {
			      fprintf (wPtr, "%d", (res->code) + 4);	/*PUTCHAR(15)::get ASCII code for each character */
			    }
			  switch (*movptr)
			    {
			    case '%':
			      /*nothing to be done */
			      break;

			    case '/':
			      fprintf (wPtr, "%d\n", 10);
			      countLoc++;
			      break;
			    }

			  if (*movptr != '/' && *movptr != '%')
			    {
			      fprintf (wPtr, "%d\n", toascii (*movptr));
			      countLoc++;
			    }

			  ++movptr;
			  //      countLoc++;
			}
		      memcpy (cpstr, "", 96);
		    }
		  else if (isalnum (*(strsave + strlen (strsave) - 2)))
		    {		/*case 'print $a print $b' */
		      fprintf (wPtr, "%d", (res->code) + 3);
		      countLoc++;
		    }
		  break;

		case 3:	/*if: '= $Hello World$' let $a */

		  strncpy (cpstr, strsave, 96);
		  char *movptr = strstr (cpstr, "$");
		  ++movptr;
		  while (*movptr != STRTOKEN)
		    {
		      if ((res->code) + 3 == STRLOAD)
			{
			  fprintf (wPtr, "%d", (res->code) + 3);
			  fprintf (wPtr, "%d\n", toascii (*movptr));
			}
		      ++movptr;
		      countLoc++;
		    }
		  if ((res->code) + 3 == STRSTORE)
		    {
		      fprintf (wPtr, "%d", (res->code) + 3);
		    /*STRSTORE*/}


		  break;
		}
	    }


	  if (strncmp (tok, "rem", 3) == 0)
	    {
	      break;
	    }


	  /*USE TWO DISTINCT STRUCTURES FOR LINES AND VARIABLES */
		  /*= a b * let a*/
	  if (strlen (tok) == 1 && isalpha (*tok))
	    {
	      qsort (vars, varcount, sizeof (struct var), compare);
	      struct var find, *value;
	      find.name = malloc (sizeof (char) * 12);
	      sprintf (find.name, "%s", tok);
	      value =
		bsearch (&find, vars, varcount, sizeof (struct var), compare);
	      tmpcount++;
	      /*################################################################################## */
	      /*this shall be increased at any var name */
	      if (value == NULL)
		{		/*you can insert a variable only if it is the firs time it appears */
		  vars[varcount].id = countVar;	/*PAY ATTENTION TO SIGSEGV CAUSED BY ERROR IN GNB SOURCE CODE */
		  vars[varcount].name = tok;	/*save this values in a static safe place */
		  vars[varcount].type = 'I';

		  safevars[cpcounter].id = countVar;	/*this is a safe place */
		  safevars[cpcounter].name = calloc (96, 1);	/*FREE IT */
		  snprintf (safevars[cpcounter].name, 96, "%s", tok);
		  safevars[cpcounter].type = 'I';
		  if (strcmp (toktok[1], "=") == 0
		      && strcmp (toktok[k - 1], "let") != 0)
		    {		/*case '5 = 4 * 3 * a let b' */
		      fprintf (wPtr, "%u", RPNLOAD);
		      fprintf (wPtr, "%d\n", vars[varcount].id);
		      countLoc++;
		    }
		  else if (strcmp (toktok[1], "if") == 0)
		    {		/*case '5 if 3 * 65 == a - 3 goto 15' */
		      fprintf (wPtr, "%u", RPNLOAD);
		      fprintf (wPtr, "%d\n", vars[varcount].id);
		      countLoc++;
		    }
		  else
		    {
		      fprintf (wPtr, "%d\n", vars[varcount].id);
		      countVar--;
		      cpcounter++;
		      free (find.name);
		      countLoc++;
		    }

		}

	      else
		{		/*back here */
		  srand (time (NULL));
		  vars[varcount].id = rand () % RAND_MAX;	/*filling the gaps in order to perform a successful quicksort */
		  vars[varcount].name = "None";
		  vars[varcount].type = 'I';
		  qsort (vars, varcount, sizeof (struct var), compare);
		  struct var ky1, *vl1;
		  ky1.name = tok;
		  vl1 =
		    bsearch (&ky1, vars, varcount, sizeof (struct var),
			     compare);
		  if (vl1 == NULL)
		    {
		      MEM_NOT_AVAIL ("vl1", file);
		    }
		  else
		    {
		      if (strcmp (toktok[1], "=") == 0
			  && strcmp (toktok[k - 1], "let") != 0)
			{
			  fprintf (wPtr, "%u", RPNLOAD);
			  fprintf (wPtr, "%d\n", vl1->id);
			  countLoc++;
			}
		      else if (strcmp (toktok[1], "if") == 0)
			{
			  fprintf (wPtr, "%u", RPNLOAD);
			  fprintf (wPtr, "%d\n", vl1->id);
			  countLoc++;
			}
		      else
			{
			  fprintf (wPtr, "%d\n", vl1->id);
			  countLoc++;
			}

		    }

		  free (find.name);
		}
	      varcount++;	/*STAY UNCOMMENTED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	    }
	  /*###################################################################### */


	  /*PAY ATTENTION TO A POSSIBLE $I am a$ */
	  if (strlen (tok) == 2 && *tok == STRTOKEN
	      && (strcmp (toktok[k - 1], "let") == 0
		  || strcmp (toktok[k - 1], "input") == 0
		  || strcmp (toktok[k - 1], "print") == 0))
	    {			/*STRING VARIABLE CASE */
	      qsort (vars, varcount, sizeof (struct var), compare);
	      struct var s, *fPtr;
	      struct var *sPtr = &s;
	      if (!sPtr)
		{
		  MEM_NOT_AVAIL ("sPtr", file);
		}
	      sPtr->name = calloc (12, sizeof (char));
	      snprintf (sPtr->name, 12, "%s", tok);
	      fPtr =
		bsearch (sPtr, vars, varcount, sizeof (struct var), compare);

	      if (fPtr == NULL)
		{
		  vars[varcount].id = countStr;
		  vars[varcount].name = tok;
		  vars[varcount].type = STRTOKEN;

		  safevars[cpcounter].id = countStr;	/*this is a safe place */
		  safevars[cpcounter].name = calloc (96, 1);	/*FREE IT */
		  snprintf (safevars[cpcounter].name, 96, "%s", tok);
		  safevars[cpcounter].type = STRTOKEN;

		  fprintf (wPtr, "%d\n", vars[varcount].id);
		  countStr--;
		  cpcounter++;
		  free (sPtr->name);
		}

	      else
		{
		  srand (time (NULL));
		  vars[varcount].id = rand () % RAND_MAX;
		  vars[varcount].name = "None";
		  vars[varcount].type = STRTOKEN;

		  qsort (vars, varcount, sizeof (struct var), compare);
		  struct var q, *w;
		  struct var *qPtr = &q;
		  if (!qPtr)
		    {
		      MEM_NOT_AVAIL ("qPtr", file);
		    }
		  qPtr->name = calloc (12, sizeof (char));
		  snprintf (qPtr->name, 12, "%s", tok);
		  w =
		    bsearch (qPtr, vars, varcount, sizeof (struct var),
			     compare);

		  if (!w)
		    {
		      MEM_NOT_AVAIL ("w", file);
		    }
		  else
		    {
		      fprintf (wPtr, "%d\n", w->id);
		    }

		  free (qPtr->name);
		  free (sPtr->name);
		}
	      varcount++;
	    }
	  /*get operators */
	  if (strlen (tok) == 1 && isOper (*tok))
	    {
	      qsort (Operators, num_oper, sizeof (Math), compare);
	      Math where, *here;
	      where.oper = calloc (2, sizeof (char));
	      snprintf (where.oper, 2, "%s", tok);

	      here =
		bsearch (&where, Operators, num_oper, sizeof (Math), compare);
	      if (!here)
		{
		  /*nothing to be done */
		}
	      else
		{
		  fprintf (wPtr, "%d", RPNLOAD);
		  fprintf (wPtr, "%u\n", here->ident);
		}
	      countLoc++;
	    }


	  /*NO LINES AFTER GOTO */
	  /*WHERE CAN WE FIND NUMERIC CONSTANTS?? */
	  if ((k > 0 && cntitem (tok, STRTOKEN) == 0 && atof (tok) != 0
	       && strcmp (toktok[k - 1], "goto") != 0) || (k > 0
							   &&
							   strcmp (toktok
								   [k - 1],
								   "if") == 0
							   && atof (tok) != 0)
	      || (k > 0 && strcmp (toktok[k - 1], "==") == 0
		  && atof (tok) != 0))
	    {

	      if (strcmp (toktok[1], "=") == 0
		  || strcmp (toktok[1], "if") == 0)
		{
		  (*(constants + constcount))->value = atof (tok);	/*to be written on the file */
		  (*(constants + constcount))->id = countConst;
		  fprintf (wPtr, "%u", RPNLOAD);
		  fprintf (wPtr, "%u\n", countConst++);	/*written constant position */
		  constcount++;

		}
	      countLoc++;
	    }

	  if (k > 0 && strcmp (toktok[k - 1], "function") == 0)
	    {			/*search for functions */
	      fprintf (wPtr, "%d\n", BEG_ROUTINE);
	      funktable[funkcount].namefunc = calloc (96, sizeof (char));	/*FREE IT */
	      snprintf (funktable[funkcount].namefunc, 96, "%s", tok);
	      funktable[funkcount].pos = countLoc;
	      funkcount++;
	      countLoc++;
	    }

	  if (k > 0 && strcmp (toktok[k - 1], "call") == 0)
	    {
	      qsort (funktable, funkcount, sizeof (Funk), compare);
	      Funk x, *y;
	      Funk *xPtr = &x;
	      xPtr->namefunc = calloc (96, sizeof (char));
	      snprintf (xPtr->namefunc, 96, "%s", tok);
	      y =
		bsearch (xPtr, funktable, funkcount,
			 sizeof (struct functions), compare);
	      if (!y)
		{
		  /*ERROR HANDLING, FUNCTIONS MUST BE DEFINED BEFORE CALLING THEM */
		}
	      else
		{
		  int putzero = 0;
		  if ((y->pos) < 10)
		    {
		      fprintf (wPtr, "%d%d\n", putzero, (y->pos));
		    }
		  else if ((y->pos) >= 10)
		    {
		      fprintf (wPtr, "%d\n", (y->pos));
		    }
		}
	      countLoc++;
	      back_to_env[env_counter] = countLoc;	/*############################# */
	      env_counter++;
	    }


	  if (k > 0 && strcmp (toktok[k - 1], "goto") == 0
	      && (strlen (tok) == 1 || strlen (tok) == 2)
	      && atoi (tok) % 5 == 0)
	    {			/*binary search */
	      countLoc++;
	      qsort (table, countTok, sizeof (struct tableEntry), compare);	/*SAVE ALSO TABLE IN A SAFE PLACE */
	      struct tableEntry kq, *v;
	      kq.symbol = tok;
	      v =
		bsearch (&kq, table, countTok, sizeof (struct tableEntry),
			 compare);
	      if (v == NULL)
		{
		  fprintf (wPtr, "@@\n");	/*@@ is the flag for to-be-completed jumps:  it can be a string cause it'll be modified before execution */
		  jumps[jumpcount] = atoi (tok);	/*this array holds the jumps */
		  ++jumpcount;
		  countLoc++;
		}
	      else if (v != NULL)
		{
		  if ((v->location) >= 10)	/*need to write for instance '4003' and not '403'for execution */
		    {
		      fprintf (wPtr, "%d\n", (v->location));
		      countLoc++;

		    }
		  else if ((v->location) < 10)
		    {
		      int writezero = 0;
		      fprintf (wPtr, "%d%d\n", writezero, (v->location));
		      countLoc++;
		    }
		}

	    }

	  k++;
	  tok = strtok (NULL, " ");
	}

      k = 0;			/*reset it */
      memset (strsave, 0, BUFSIZ);
    }


  k -= k;

  for (k = 0; k < 256; k++)
    {				/*when free a string array, loop and free each string */
      free (prog[k]);
      free (toktok[k]);

    }

  free (prog);
  free (toktok);

  fclose (read);
  fclose (wPtr);

  return;
  /*END OF FIRST PASSAGE: MACHINE LANGUAGE FILE ALREADY CREATED AND HALF-WRITTEN */
}

void
still_parsimonious (char *still_obj)
{
  int compare (const void *const k1, const void *const k2);
  FILE *rPtr;
  rPtr = fopen (still_obj, "r+");
  char **machinearray = NULL;
  machinearray = malloc (256 * sizeof (char *));

  if (!rPtr)
    {
      MEM_NOT_AVAIL ("rPtr", still_obj);
    }

  int x;
  for (x = 0; x < 256; x++)
    {
      *(machinearray + x) = NULL;
      *(machinearray + x) = malloc (132 * sizeof (char));
    }

  rewind (rPtr);

  char readbuff[132] = "";
  int machinecount = 0;
  char **newprog = NULL;
  newprog = malloc (sizeof (char *) * 256);

  for (x = 0; x < 256; x++)
    {
      *(newprog + x) = malloc (sizeof (char) * 132);
    }

  jumpcount -= jumpcount;
  env_counter -= env_counter;

  do
    {
      strncpy (machinearray[machinecount], readbuff, 132);
      if (strcmp (machinearray[machinecount], "40@@\n") != 0
	  && strcmp (machinearray[machinecount], "41@@\n") != 0
	  && strcmp (machinearray[machinecount], "42@@\n") != 0
	  && strcmp (machinearray[machinecount], "43@@\n") != 0
	  && strcmp (machinearray[machinecount], "8866\n") != 0)
	{			/*consider also newline escape char *///   
	  snprintf (*(newprog + machinecount), 132, "%s",
		    machinearray[machinecount]);
	}
      else if (strcmp (machinearray[machinecount], "40@@\n") == 0
	       || strcmp (machinearray[machinecount], "41@@\n") == 0
	       || strcmp (machinearray[machinecount], "42@@\n") == 0
	       || strcmp (machinearray[machinecount], "43@@\n") == 0)
	{			/*allow also 42 and 43 *//*if there is a jump... */
	  char two_char[5] = "";
	  two_char[0] = *readbuff;	/*get the first two characters */
	  two_char[1] = *(readbuff + 1);
	  qsort (&safetable, countTok, sizeof (cptable), compare);
	  cptable line, *lPtr;	/*remember -1 *//*control jumps array */
	  char *cvinttostr = NULL;
	  cvinttostr = malloc (sizeof (char) * 24);
	  snprintf (cvinttostr, 24, "%d", jumps[jumpcount]);
	  line.symbol = cvinttostr;
	  lPtr =
	    bsearch (&line, safetable, countTok, sizeof (cptable), compare);
	  if (!lPtr)
	    {
	      /*nothing to be done */
	    }
	  else if ((lPtr->location) < 10)
	    {
	      if (strcmp (two_char, "40") == 0)
		{
		  snprintf (*(newprog + machinecount), 132, "%s0%d\n",
			    two_char, (lPtr->location) - 1);
		}
	      else
		{
		  snprintf (*(newprog + machinecount), 132, "%s0%d\n",
			    two_char, (lPtr->location));
		}
	    }
	  else if ((lPtr->location) >= 10)
	    {
	      if (strcmp (two_char, "40") == 0)
		{
		  snprintf (*(newprog + machinecount), 132, "%s%d\n",
			    two_char, (lPtr->location) - 1);
		}
	      else
		{
		  snprintf (*(newprog + machinecount), 132, "%s%d\n",
			    two_char, (lPtr->location));
		}

	    }

	  fseek (rPtr, ftell (rPtr) - 5, SEEK_SET);	/*################ *//*rewind a bit */
	  fprintf (rPtr, "%s", *(newprog + machinecount));	/*overwrite (of course the pointer will shift by itself) */
	  /*copy newprog buffer to a file */
	  jumpcount++;
	  free (cvinttostr);
	}
      else if (strcmp (machinearray[machinecount], "8866\n") == 0)
	{
	  fseek (rPtr, ftell (rPtr) - 5, SEEK_SET);
	  fprintf (rPtr, "88%d", (back_to_env[env_counter] + 1));
	  env_counter++;
	}

      fgets (readbuff, sizeof (readbuff), rPtr);
      machinecount++;
    }
  while (!feof (rPtr));


  fprintf (rPtr, "%s\n", "4400");

  x -= x;

  while ((*(safevars + x)).name != NULL)
    {				/*ADD DEFAULT SPACE FOR VARIABLES */
      fputs ("0000\n", rPtr);
      ++x;
    }
  x -= x;

  while (x != constcount)
    {				/*ADD DEFAULT SPACE FOR VARIABLES */
      char float_to_str[24];
      snprintf (float_to_str, 24, "%.2f\n", (*(constants + x))->value);
      fputs (float_to_str, rPtr);
      ++x;
    }


  x -= x;


  for (x = 0; x < 256; x++)
    {
      free (machinearray[x]);
      free (newprog[x]);
      free (constants[x]);

    }

  x -= x;

  for (x = 0; x < countTok; x++)
    {
      free (safetable[x].symbol);
      free (safevars[x].name);
    }


  free (constants);
  free (machinearray);
  free (newprog);
  free (jumps);
  fclose (rPtr);

  return;
}


int
compare (const void *const k1, const void *const k2)
{				/*written in order to perform qsort */
  struct keywords *kPtr1 = (struct keywords *) k1;
  struct keywords *kPtr2 = (struct keywords *) k2;

  return strcmp (kPtr1->instrName, kPtr2->instrName);
}
