/* Project: sttyl
 * Author:  Beth Boose
 * File:    sttyl.c
 * Purpose: duplicate some of the functionality of stty
 */
#include "table.h"
#include "debug.h"
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>


static int populate_dispf(int argc, char *argv[]);
static int matches(char *param, char *userinput);

int main(int argc, char *argv[]) {
/* get current termios setting */
    if (tcgetattr(STDOUT_FILENO, &sys_tty) == -1) { /* get info */
        perror("cannot get params about stdin");
        exit(EXIT_FAILURE);
    }
/* get winsize for rows/columns */
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &sys_win) == -1) {
        perror("cannot get rows/columns for stdin");
        exit(EXIT_FAILURE);
    }
/* need this for baud rate */ 
    if ((sys_speed = cfgetospeed(&sys_tty)) == (speed_t) - 1) {
        perror("cannot get baud rate for stdin");
        exit(EXIT_FAILURE);
    }

/* no args? print everything in dispf */
    if (argc < 2) {
        print_dispf();
        exit(EXIT_SUCCESS);
    }

/* validate user input, set ptrs for settings */
    if (populate_dispf(argc, argv) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

/* there was valid input, set it */
    if (tcsetattr(STDOUT_FILENO,TCSANOW, &sys_tty) == -1) {
        perror("unable to set tty attributes");
        exit(EXIT_FAILURE);
    }

    return 0;
}

/** 
 * validate and populate 
 * @param argc - number of cmd line args
 * @param argv - ptr to cmd line args
 * @return EXIT_SUCCESS if input is valid
 */
static int populate_dispf(int argc, char *argv[]) {
    if (argc == 1) { // no params to validate, go ahead
        return EXIT_SUCCESS;
    }
    
    int i = 1;
    while (i < argc) {
        int foundmatch = 0;
        for (int j = 0; !foundmatch && dispf[j].param_name != NULL; j++) {
            if ((foundmatch = matches(argv[i], dispf[j].param_name))) {
                switch (dispf[j].param_type) {
                    case REQD_CHAR:
                        if ((i + 1 >= argc)) { // no arg
                            //debug("no more items, argc= %d\n", numargs);
                            fprintf(stderr, "sttyl: missing argument to `%s'\n ", argv[i]);
                            return EXIT_FAILURE;
                        } else if (strlen(argv[i + 1]) > 1) { // too many args
                            fprintf(stderr, "sttyl: invalid integer argument `%s'\n ", argv[i + 1]);
                            return EXIT_FAILURE;
                        }
                        /* param matches, arg is right length, collect it, set i to skip, break */
                        *(cc_t *) dispf[j].sys_p = argv[i + 1][0];
                        i++; // increment so we skip the next arg
                        break;

                    case REQD_BIT: // its a matching sw, set the value according to sw 
                        if (argv[i][0] == '-') { // turn flag off
                            *(tcflag_t *) dispf[j].sys_p &= ~dispf[j].bitmask;
                        } else {
                            *(tcflag_t *) dispf[j].sys_p |= dispf[j].bitmask;
                        }
                        break;

                    case REQD_RC:
                        fprintf(stderr, "setting row/columns not implemented\n");
                        return EXIT_FAILURE;
                    case REQD_SPD:
                        fprintf(stderr,"setting speed is not implemented\n");
                        return EXIT_FAILURE;
                    default:
                        assert(0);
                }
            }
        }
        if (!foundmatch) { // this didn't match anything
            fprintf(stderr, "sttyl: invalid argument `%s'\n ", argv[i]);
            return EXIT_FAILURE;
        }
        i++;
    }
    return EXIT_SUCCESS;
}

/**
 * matches param against userinput, if userinput has a leading dash, strip
 * that first
 * @param param - 
 * @param userinput - 
 * @return 1 if param matches userinput
 */
static int matches(char *userinput, char *param) {
    int matches = 0;
    if (strcmp(param, userinput) == 0) {
        matches = 1;
    } else if (strlen(userinput) > 1 && userinput[0] == '-') {
        if (strcmp(&userinput[1], param) == 0) {
            matches = 1;
        }
    }
    return matches;
}

