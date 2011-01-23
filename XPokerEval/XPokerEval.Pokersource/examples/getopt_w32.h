#ifndef __GETOPT_W32_H__
#define __GETOPT_W32_H__

extern int optind;
extern char *optarg;
extern int opterr;

int getopt (int argc, char *argv[], const char *optstring);

#endif