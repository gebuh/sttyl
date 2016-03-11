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
/**
 **	showtty	
 **	displays some of current tty settings
 **/


void show_some_flags(struct termios *ttyp);

static int validate_input(int argc, char *argv[]);
static int matches(char *param, char *userinput);
static void show_baud(speed_t thespeed);


int main(int argc, char *argv[]) {

    /* Get the current terminal attributes. */
    
    if (tcgetattr(STDOUT_FILENO, &sys_tty) == -1) { /* get info */
        perror("cannot get params about stdin");
        exit(EXIT_FAILURE);
    }

    if (ioctl( STDOUT_FILENO, TIOCGWINSZ, &sys_win) == -1) {
        perror("cannot get rows/columns for stdin");
        exit(EXIT_FAILURE);
    }

    if (cfgetospeed (&sys_tty) == (unsigned int)-1) {
        perror("cannot get baud rate for stdin");
        exit(EXIT_FAILURE);
    }
    
    // no args? print everything
    if (argc < 2) {
        print_all_fields();
        exit(EXIT_SUCCESS);
    }
    
    /* collect user input and validate or fail with message */
    if (validate_input(argc, argv) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }
    
   
//    // set whatever's in user set array
//    if (tcsetattr(FILENO_STDIN, &sys_tty) == -1) {
//        perror("unable to set tty attributes");
//        exit(EXIT_FAILURE);
//    }

    //printv_arr();

    return 0;
}


/** 
 * @param argc - number of cmd line args
 * @param argv - ptr to cmd lin args
 * @return EXIT_SUCCESS if input is valid
 */
static int validate_input(int numargs, char *argv[]) {
//    if (numargs == 1) { // no params to validate, go ahead
//        return EXIT_SUCCESS;
//    }
//
//    /* check each param, if it matches erase, kill, 
//     * it should be followed by a single char
//     * if it matches anything else, collect it
//     * other wise return fail
//     */
//    int i = 1;
//    while (i < numargs) {
//        int foundmatch = 0;
//        for (int j = 0; dispf[j].param_name != NULL; j++) {
//            if ((foundmatch = matches(argv[i], dispf[j].param_name))) { // param name matches
//                // does it require an arg?
//                if (dispf[j].req_fl) {
//                  // debug(" %s requires an arg\n", v_args_arr[j].param_name);
//                    // next item should exist and be a single char
//                    if ((i + 1 >= numargs)) { // no arg
//                        //debug("no more items, argc= %d\n", numargs);
//                        fprintf(stderr, "sttyl: missing argument to `%s'\n ", argv[i]);
//                        return EXIT_FAILURE;
//                    } else if (strlen(argv[i + 1]) > 1) { // too many args
//                        fprintf(stderr, "sttyl: invalid integer argument `%s'\n ", argv[i + 1]); 
//                        return EXIT_FAILURE;
//                    }
//                    /* param matches, arg is right length, collect it, set i to skip, break */
//                    //debug(" %s setting arg to %s\n", v_args_arr[j].param_name, argv[i + 1]);
//                    dispf[j].p_arg = argv[i + 1][0];
//                    dispf[j].set_by_usr = 1;
//                    i++; // increment so we skip the next arg
//                    break;
//
//                } else { // its a matching sw, set the value according to sw
//                    dispf[j].set_by_usr = 1;
//                    dispf[j].set_fl = 1;
//                    if (argv[i][0] == '-') { // turn flag off
//                        dispf[j].set_fl = 0;
//                    }
//                    break;
//                }
//            }
//        }
//        if (!foundmatch) { // this didn't match anything
//            fprintf(stderr, "sttyl: invalid argument `%s'\n ", argv[i]);
//            return EXIT_FAILURE;
//        }
//        i++;
//    }
    return EXIT_SUCCESS;
}

/**
 * matches param against user_input struct, with or
 * without leading dash
 * @param param
 * @param userinput
 * @return 1 if param matches userinput
 */
static int matches(char *param, char *userinput) {
    int matches = 0;
    if (strcmp(param, userinput) == 0) {
        //debug("%s matches %s\n", param, userinput);
        matches = 1;
    } else if (strlen(param) > 1 && param[0] == '-') {
        if (strcmp(&param[1], userinput) == 0) {
            matches = 1;
            //debug("%s matches %s\n", param, userinput);
        }
    } else {
        matches = 0;
        //debug("%s doesn't match %s\n", param, userinput);
    }
    return matches;
}

void show_baud(speed_t thespeed)
/*
 *	prints the speed in english
 */ {
    printf("speed ");
    switch (thespeed) {
        case B300: printf("300 baud ");
            break;
        case B600: printf("600 baud ");
            break;
        case B1200: printf("1200 baud ");
            break;
        case B1800: printf("1800 baud ");
            break;
        case B2400: printf("2400 baud ");
            break;
        case B4800: printf("4800 baud ");
            break;
        case B9600: printf("9600 baud ");
            break;
        default: printf("Fast baud ");
            break;
    }
}

struct flaginfo {
    tcflag_t fl_value;
    char *fl_name;
};

struct flaginfo input_flags[] = {

    { IGNBRK, "Ignore the break condition"},
    { BRKINT, "Signal interrupt on break"},
    { IGNPAR, "Ignore chars with parity errors"},
    { PARMRK, "Mark parity errors"},
    { INPCK, "Enable input parity check"},
    { ISTRIP, "Strip character"},
    { INLCR, "Map NL to CR on input"},
    { IGNCR, "Ignore CR"},
    { ICRNL, "Map CR to NL on input"},
    { IXON, "Enable start/stop output control"},
    { IXOFF, "Enable start/stop input control"},
    { 0, NULL}
};

struct flaginfo local_flags[] = {
    { ISIG, "Enable signals"},
    { ICANON, "Canonical input (erase and kill)"},
    { ECHO, "Enable echo"},
    { ECHOE, "Echo ERASE as BS-SPACE-BS"},
    {ECHOK, "Echo KILL by starting new line"},
    {0, NULL}
};

void show_flagset(int thevalue, struct flaginfo thebitnames[]);

void show_some_flags(struct termios *ttyp)
/*
 *	show the values of two of the flag sets_: c_iflag and c_lflag
 *	adding c_oflag and c_cflag is pretty routine - just add new
 *	tables above and a bit more code below.
 */ {
    show_flagset(ttyp->c_iflag, input_flags);
    show_flagset(ttyp->c_lflag, local_flags);
}

void show_flagset(int thevalue, struct flaginfo thebitnames[])
/*
 * check each bit pattern and display descriptive title
 */ {
    int i;

    for (i = 0; thebitnames[i].fl_value != 0; i++) {
        if (thevalue & thebitnames[i].fl_value)
            printf("ON  ");
        else
            printf("OFF ");
        printf("  %s\n", thebitnames[i].fl_name);
    }
}


