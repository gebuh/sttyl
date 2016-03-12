/* Project: sttyl
 * Author:  Beth Boose
 * File:    table.c
 * Purpose: keep all required data structures in one place
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "table.h"

/** our termios struct for display and/or setting */
struct termios sys_tty;

/** hold row/col data */
struct winsize sys_win;

/** hold speed data */
speed_t sys_speed;

static void show_baud(speed_t thespeed);

/** array of implemented input and/or output termios tty fields */
const struct stty_fields dispf[] = {
    {"speed",   REQD_SPD,   &sys_speed,             0,      1},
    {"rows",    REQD_RC,    &sys_win.ws_row,        0,      1},
    {"columns", REQD_RC,    &sys_win.ws_col,        0,      1},      
    {"intr",    REQD_CHAR,  &sys_tty.c_cc[VINTR],   0,      0},
    {"erase",   REQD_CHAR,  &sys_tty.c_cc[VERASE],  0,      0},
    {"kill",    REQD_CHAR,  &sys_tty.c_cc[VKILL],   0,      0},
    {"eol",     REQD_CHAR,  &sys_tty.c_cc[VEOL],    0,      0},
    {"hupcl",   REQD_BIT,   &sys_tty.c_cflag,       HUPCL,  0},
    {"icrnl",   REQD_BIT,   &sys_tty.c_iflag,       ICRNL,  0},
    {"opost",   REQD_BIT,   &sys_tty.c_oflag,       OPOST,  0},
    {"isig",    REQD_BIT,   &sys_tty.c_lflag,       ISIG,   0},
    {"icanon",  REQD_BIT,   &sys_tty.c_lflag,       ICANON, 0},
    {"echo",    REQD_BIT,   &sys_tty.c_lflag,       ECHO,   0},
    {"echoe",   REQD_BIT,   &sys_tty.c_lflag,       ECHOE,  0},      
    {NULL,      0,          NULL,                   0,      0}
};

/**
 * print the entire dispf
 */
void print_dispf() {
    for(int i=0; dispf[i].param_name != NULL; i++) {
        print_dispf_row (&dispf[i]);
    } 
    printf("\n");
}

/**
 * print a single user_input struct use disp_t to determine type
 * and print accordingly
 * @param one_row a single stty_fields struct
 */
void print_dispf_row(const struct stty_fields *one_row) {
    char *dash = "-";
    int ch;
    
    switch (one_row->param_type) {
        case REQD_BIT: /* print name, add a dash depending on bitmask */
            if (*(tcflag_t *) one_row->sys_p & one_row->bitmask) {
                dash = "";
            }
            printf("%s%s;", dash, one_row->param_name);
            break;

        case REQD_CHAR: /* print name and char value */
            ch = *(cc_t *) one_row->sys_p;
            printf("%s = ", one_row->param_name);
            if (ch > 0x7F) {
                printf("<non-ascii(0x%02X)>", ch); // not recognized
            } else if (ch == '\0') {
                printf("<undef>");  // handle undefined
            } else if (ch == 0x7F) {
                printf("^?"); // get DEL
            } else if (ch >= ' ' && ch <= '~') {
                printf("%c", ch); // print if between 0-126 ascii
            } else if (ch < ' ') {
                printf("^%c", 'A' + ch - 1);
            } else {
                printf("\%03o", ch); // lets just print an octal value
            }
            printf(";");
            break;

        case REQD_RC:
            printf("%s %d;", one_row->param_name, 
                    (int) *(unsigned short int *) one_row->sys_p);
            break;

        case REQD_SPD:
            show_baud (*(speed_t *) one_row->sys_p);
            break;

        default:
            assert(0);
    }

    printf(" ");
    return;
}

/**
 * convert speeds to baud and print
 */
static void show_baud(speed_t thespeed) {
    printf("speed ");
    switch (thespeed) {
        case B50: printf("50");
            break;
        case B75: printf("75");
            break;
        case B110: printf("110");
            break;
        case B134: printf("134");
            break;
        case B150: printf("150");
            break;
        case B200: printf("200");
            break;
        case B300: printf("300");
            break;
        case B600: printf("600");
            break;
        case B1200: printf("1200");
            break;
        case B1800: printf("1800");
            break;
        case B2400: printf("2400");
            break;
        case B4800: printf("4800");
            break;
        case B9600: printf("9600");
            break;
        case B19200: printf("19200");
            break;
        case B38400: printf("38400");
            break;
        case B57600: printf("57600");
            break;
        case B115200: printf("115200");
            break;
        case B230400: printf("230400");
            break;
        default: printf("Unknown");
    }    
    printf(" baud;");
}