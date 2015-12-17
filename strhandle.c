#include "strhandle.h"
#include "rpnC.c"


int
querytok (char *from, const char *const what, int numchar)	/*search for 'rem' keyword */
{
  char little[8] = "";		/*initialize */
  unsigned int i = 0;
  for (; *from != '\0'; from++)
    {
      if (isalpha (*from))
	{
	  while (i != numchar)
	    {
	      little[i] = *(from + i);
	      i++;
	    }
	}
    }

  return strcmp (little, what);
}


unsigned int
cntitem (const char in[], char what)	/*declare it 'const' */
{
  unsigned int y = 0;
  size_t kount = 0;
  while (in[kount] != '\0')
    {
      if (in[kount] == what)
	{
	  y++;
	}
      kount++;
    }

  return y;
}

int
findOp (const char str[], const char *comp, const char *_goto_)
{
  char cpy[256] = "";
  strncpy (cpy, str, 256);
  char cpy2[256] = "";
  strncpy (cpy2, str, 256);
  char *tok_r = NULL;
  char *save_ptr[256];
  tok_r = strtok_r (cpy, " ", save_ptr);

  if (querytok (cpy2, _goto_, 4) == 0)
    {
      return -5;
    }

  while (strcmp (tok_r, "==") != 0 && strcmp (tok_r, ">") != 0
	 && strcmp (tok_r, "<") != 0)
    {
      tok_r = strtok_r (NULL, " ", save_ptr);
    }


  return strncmp (tok_r, comp, 2);
}


char **
commatocmd (char **args)
{
  int i = 0;

  while (*(args + i) != NULL)
    {

      if (cntitem (*(args + i), ',') == 1)
	{
	  char *token = "";
	  char tempbuff[96] = "";
	  strncpy (tempbuff, (*(args + i)), 96);
	  token = strtok (tempbuff, " ");
	  sprintf (*(args + i), "%s", token);
	  strcat (*(args + i), " ");
	  token = strtok (NULL, " ");
	  char *command = malloc (24);
	  snprintf (command, 24, "%s", token);	/*save the command */
	  strcat (*(args + i), token);
	  strcat (*(args + i), " ");
	  token = strtok (NULL, " ");
	  while (token != NULL)
	    {
	      if (strncmp (token, ",", 1) != 0)
		{
		  strcat (*(args + i), token);
		  strcat (*(args + i), " ");
		}
	      else if (strncmp (token, ",", 1) == 0)
		{
		  strcat (*(args + i), command);
		  strcat (*(args + i), " ");
		}
	      token = strtok (NULL, " ");

	    }

	  free (command);
	}
      /*5 let x = 2 --------->> = 2 let x */
      else if (cntitem (*(args + i), '=') == 1)
	{

	  char cccp[96] = "";
	  strncpy (cccp, *(args + i), 96);
	  char saveLine[4];
	  strncpy (saveLine, *(args + i), 4);
	  char *tokeq = "";
	  char *firsttok = "";
	  firsttok = " = ";
	  tokeq = strstr (strtok (*(args + i), "="), "let");
	  char *savetok = NULL;
	  savetok = malloc (sizeof (char) * 48);
	  snprintf (savetok, 48, "%s", tokeq);
	  tokeq = strtok (NULL, "=");
	  sprintf (*(args + i), "%s", strtok (saveLine, " "));
	  if (querytok (cccp, "rem ", 4) == 0)
	    {
	      strcat (*(args + i), " ");
	      strcat (*(args + i), "rem ");
	    }
	  strcat (*(args + i), firsttok);
	  if (cntitem (cccp, STRTOKEN) == 0)
	    {
	      strcat (*(args + i), rpnConv (tokeq));
	    }
	  else if (cntitem (cccp, STRTOKEN) == 3)
	    {
	      strcat (*(args + i), tokeq);
	    }


	  strcat (*(args + i), " ");
	  strcat (*(args + i), savetok);	/*append the last part */
	  free (savetok);
	}

      /*########################################################################################################################## */

      else if (querytok (*(args + i), "if", 2) == 0)
	{			/*PROBLEMS WITH 'f' and 'i' */
	  char strsave[96] = "";
	  strncpy (strsave, *(args + i), 96);
	  char line[12] = "";
	  strncpy (line, strsave, 12);
	  char *nline = NULL;
	  nline = strtok (line, " ");
	  char *newLine[256] = { };
	  newLine[0] = nline;
	  newLine[1] = " if ";
	  /*############################################################################################# */
	  if (strstr (strsave, "==") != NULL && strstr (strsave, ">") == NULL
	      && strstr (strsave, "<") == NULL)
	    {			/*three instances:: == , > , < */
	      newLine[3] = "== ";
	      char *tock = NULL;
	      tock = strtok (strsave, "==");
	      char part1[96] = "";
	      strncpy (part1, tock, 96);
	      tock = strtok (NULL, "is");
	      tock++;
	      tock++;
	      char part2[96] = "";
	      strncpy (part2, tock, 96);
	      char expr1[96] = "";
	      tock = strtok (part1, "if");
	      tock = strtok (NULL, "if");
	      ++tock;
	      strcpy (expr1, tock);
	      char expr2[96] = "";
	      tock = strtok (part2, " ");
	      while (strncmp (tock, "goto", 4))
		{
		  /*BUG*/	/*################# */
		    strncat (expr2, tock, strlen (tock));
		  strcat (expr2, " ");
		  tock = strtok (NULL, " ");
		}


	      char *_goto_ = malloc (sizeof (char) * 24);
	      strcpy (_goto_, strstr (*(args + i), "goto"));
	      memset (*(args + i), 0, strlen (*(args + i)));
	      newLine[2] = rpnConv (expr1);
	      newLine[4] = rpnConv (expr2);
	      newLine[5] = _goto_;
	      int h = 0;

	      while (newLine[h] != NULL)
		{
		  strcat (*(args + i), newLine[h]);
		  h++;
		}
	      free (_goto_);
	    }

	  /*########################################################################## */
	  else if (strstr (strsave, ">") != NULL && strstr (strsave, "==") == NULL	/*do the same stuff */
		   && strstr (strsave, "<") == NULL)
	    {
	      newLine[3] = "> ";
	      char *tock = NULL;
	      tock = strtok (strsave, ">");
	      char part1[96] = "";
	      strncpy (part1, tock, 96);
	      tock = strtok (NULL, "is");
	      tock++;
	      char part2[96] = "";
	      strncpy (part2, tock, 96);
	      char expr1[96] = "";
	      tock = strtok (part1, "if");
	      tock = strtok (NULL, "if");
	      ++tock;
	      strcpy (expr1, tock);
	      char expr2[96] = "";
	      tock = strtok (part2, " ");
	      while (strncmp (tock, "goto", 4))
		{
		  strncat (expr2, tock, strlen (tock));
		  strcat (expr2, " ");
		  tock = strtok (NULL, " ");
		}

	      char *_goto_ = malloc (sizeof (char) * 24);
	      strcpy (_goto_, strstr (*(args + i), "goto"));
	      memset (*(args + i), 0, strlen (*(args + i)));
	      newLine[2] = rpnConv (expr1);
	      newLine[4] = rpnConv (expr2);
	      newLine[5] = _goto_;
	      int h = 0;

	      while (newLine[h] != NULL)
		{
		  strcat (*(args + i), newLine[h]);
		  h++;
		}

	      free (_goto_);
	    }


	  else if (strstr (strsave, "<") != NULL
		   && strstr (strsave, "==") == NULL
		   && strstr (strsave, ">") == NULL)
	    {
	      newLine[3] = "< ";
	      char *tock = NULL;
	      tock = strtok (strsave, "<");
	      char part1[96] = "";
	      strncpy (part1, tock, 96);
	      tock = strtok (NULL, "is");
	      tock++;
	      char part2[96] = "";
	      strncpy (part2, tock, 96);
	      char expr1[96] = "";
	      tock = strtok (part1, "if");
	      tock = strtok (NULL, "if");
	      ++tock;
	      strcpy (expr1, tock);
	      char expr2[96] = "";
	      tock = strtok (part2, " ");

	      while (strncmp (tock, "goto", 4))
		{
		  strncat (expr2, tock, strlen (tock));
		  strcat (expr2, " ");
		  tock = strtok (NULL, " ");
		}

	      char *_goto_ = malloc (sizeof (char) * 24);
	      strcpy (_goto_, strstr (*(args + i), "goto"));
	      memset (*(args + i), 0, strlen (*(args + i)));
	      newLine[2] = rpnConv (expr1);
	      newLine[4] = rpnConv (expr2);
	      newLine[5] = _goto_;
	      int h = 0;
	      while (newLine[h] != NULL)
		{
		  strcat (*(args + i), newLine[h]);
		  h++;
		}

	      free (_goto_);
	    }
	}


      else
	{
	  char cp[96] = "";
	  strcpy (cp, *(args + i));
	  char *t = NULL;
	  t = strtok (cp, " ");
	  sprintf (*(args + i), "%s", "");
	  while (t != NULL)
	    {
	      strcat (*(args + i), t);
	      strcat (*(args + i), " ");
	      t = strtok (NULL, " ");
	    }
	}


      i++;
    }

  i -= i;


  return args;
}
