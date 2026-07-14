#ifndef GETOPT_H
#define GETOPT_H

#include <string.h>
#include <stdio.h>

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;

int getopt(int argc, char * const argv[], const char *optstring);

#endif // GETOPT_H