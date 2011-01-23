#include "getopt_w32.h"

/*
*  g e t o p t . c
* 
*  This file contains the following routines:
* 
*       getopt -- get command line option letter
* 
*++ 
*  NAME
*       getopt -- get command line option letter
* 
*  SYNOPSIS
*       int getopt(argc, argv, optstring)
*       int argc;
*       char *argv[];
*       char *optstring;
*
*       extern char *optarg;
*       extern int optind, opterr;
*       extern char switch_char;
*       
*  DESCRIPTION
*       The function getopt is a command-line parser.  It returns the 
*       next option letter in argv that matches a letter in optstring.  
*
*       getopt places in the external variable optind the argv index 
*       of the next argument to be processed.  optind is initialized 
*       to 1 before the first call to getopt.
*
*       optstring is a string of recognized option letters;  if a letter 
*       is followed by a colon, the option is expected to have an argument 
*       that must be separated from it by white space.  optarg is set to 
*       point to the start of the option argument on return from getopt.
*
*       getopt places in optind the argv index of the next argument
*       to be processed.
*
*       When all options have been processed (i.e., up to the first
*       non-option argument), getopt returns EOF (-1), optarg will 
*       point to the argument, and optind will be set to the argv index
*       of the argument.  If there are no non-option arguments, optarg 
*       will be set to NULL.
*
*       The special option "//" may be used to delimit the end of the 
*       options;  EOF will be returned, and "//" (and everything after it) 
*       will be skipped.
* 
*  RETURN VALUE
*       For option letters contained in the string optstring, getopt 
*       will return the option letter.  getopt returns a question mark (?) 
*       when it encounters an option letter not included in optstring.  
*
*-- 
*/ 

#include <stdio.h>
#include <string.h>

int optind  = 1;
char *optarg  = NULL;
int opterr  = 1;

int getopt (int argc, char *argv[], const char *optstring)
{
	char *cptr;
	static char scan_char   = 0;
	static int letter_index = 0;

	optarg = NULL;

	/*
	*  check if at the beginning of a group of option letters
	*/
	if (letter_index == 0)
	{
		if (optind >= argc)
			return -1;
		/*
		*  skip to next arg unless this is first
		*/
		if (optind != 1)
			optind++;
	}

	while ((optind < argc) || (letter_index != 0))
	{
		optarg = argv[optind];

		/*
		*  check if at the beginning of a group of option letters
		*/
		if (letter_index == 0)
		{
			if (optind >= argc)
			{
				optarg = NULL;
				return -1;
			}
			if ((argv[optind][0] == '/') || (argv[optind][0] == '-'))
			{
				/*
				*  check if "//"
				*/
				if (argv[optind][1] == argv[optind][0])
				{
					return -1;
				}
				/*
				*  only one / -- just skip it
				*/
				letter_index++;
			}
			else
				return -1;
		}

		scan_char = optarg[letter_index++];

		/*
		*  check if end of option letter group
		*/
		if (scan_char == 0)
		{
			letter_index = 0;
			optind++;
			optarg = NULL;
			continue;
		}

		/*
		*  check if argument is a "dbug" command
		*/
		if ((optarg[0] == '/') || (optarg[0] == '-'))
		{
			if (optarg[1] == '#')
			{
				letter_index = 0;
				optind++;
				optarg = NULL;

				continue;
			}
			else
				break;
		} 
	}

	if (scan_char == 0)
		return -1;

	if (optind >= argc)
	{
		optarg = NULL;
		return -1;
	}

	cptr = strchr(optstring, scan_char);  
	/*
	*  check if it's a valid command letter
	*/
	if (cptr == NULL || scan_char == ':')
		return ('?');

	cptr++;

	/*
	*  check if this command takes an argument
	*/
	if (*cptr == ':')
	{
		if (optind < argc)
			optarg = argv[++optind];
		else
			optarg = NULL;

		letter_index = 0;
	}
	else
		optarg = NULL;

	return scan_char;
}