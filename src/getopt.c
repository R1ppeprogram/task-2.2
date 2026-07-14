#include "getopt.h"
#include <stdlib.h>

char *optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = 0;

int getopt(int argc, char * const argv[], const char *optstring) {
    static int optpos = 1;
    char *optchar;
    
    if (optind >= argc) {
        return -1;
    }
    
    if (argv[optind][0] != '-' || argv[optind][1] == '\0') {
        return -1;
    }
    
    if (argv[optind][1] == '-' && argv[optind][2] == '\0') {
        optind++;
        return -1;
    }
    
    optchar = strchr(optstring, argv[optind][optpos]);
    if (optchar == NULL) {
        optopt = argv[optind][optpos];
        if (opterr) {
            fprintf(stderr, "Unknown option: -%c\n", optopt);
        }
        return '?';
    }
    
    if (optchar[1] == ':') {
        if (argv[optind][optpos + 1] != '\0') {
            optarg = &argv[optind][optpos + 1];
            optind++;
            optpos = 1;
        } else if (optind + 1 < argc) {
            optarg = argv[optind + 1];
            optind += 2;
            optpos = 1;
        } else {
            if (opterr) {
                fprintf(stderr, "Option -%c requires an argument\n", argv[optind][optpos]);
            }
            return '?';
        }
        return *optchar;
    }
    
    optopt = argv[optind][optpos];
    if (argv[optind][optpos + 1] != '\0') {
        optpos++;
    } else {
        optind++;
        optpos = 1;
    }
    
    return *optchar;
}