/* Project: sttyl
 * Author:  Beth Boose
 * File:    table.c
 * Purpose: keep all required data structures in one place
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "table.h"

struct termios sys_tty;
struct winsize sys_win;
speed_t sys_speed;

static void show_baud(speed_t thespeed);

/* array of allowable user input with required flag */
const struct all_fields dispf[] = {
    {"erase", REQD_CHAR, &sys_tty.c_cc[VERASE], 0, 0},
    {"kill", REQD_CHAR, &sys_tty.c_cc[VKILL], 0, 0},
    {"eol", REQD_CHAR, &sys_tty.c_cc[VEOL], 0, 0},
    {"echo", REQD_BIT, &sys_tty.c_lflag, ECHO, 0},
    {"echoe", REQD_BIT, &sys_tty.c_lflag, ECHOE, 0},
    {"hupcl", REQD_BIT, &sys_tty.c_cflag, HUPCL, 0},
    {"icrnl", REQD_BIT, &sys_tty.c_iflag, ICRNL, 0},
    {"isig", REQD_BIT, &sys_tty.c_lflag, ISIG, 0},
    {"opost", REQD_BIT, &sys_tty.c_oflag, OPOST, 0},
    {"intr", REQD_CHAR, &sys_tty.c_cc[VINTR], 0, 0},
    {"icanon", REQD_BIT, &sys_tty.c_lflag, ICANON, 0},
    {"rows", REQD_RC, &sys_win.ws_row, 0, 1},
    {"columns", REQD_RC, &sys_win.ws_col, 0, 1},
    {"speed", REQD_SPD, &sys_speed, 0, 1},
    {NULL, 0, NULL, 0, 0}
};

void print_all_fields() {
    for(int i=0; dispf[i].param_name != NULL; i++) {
        print_field (&dispf[i]);
    } 
    printf("\n");
}
/**
 * print a single user_input struct
 * @param u_in
 */
void print_field(const struct all_fields *u_in) {
    char *dash = "-";
    int ch;
    
    switch (u_in->param_type) {
        case REQD_BIT:
            if (*(tcflag_t *) u_in->sys_p & u_in->bitmask) {
                dash = "";
            }
            printf("%s%s;", dash, u_in->param_name);
            break;

        case REQD_CHAR:
            ch = *(cc_t *) u_in->sys_p;
            printf("%s = ", u_in->param_name);
            if (ch > 0x7F) {
                printf("<non-ascii(0x%02X)>", ch);
            } else if (ch == '\0') {
                printf("<undef>");
            } else if (ch == 0x7F) {
                printf("^?");
            } else if (ch >= ' ' && ch <= '~') {
                printf("%c", ch);
            } else if (ch < ' ') {
                printf("^%c", 'A' + ch - 1);
            } else {
                printf("\%03o", ch);
            }
            printf(";");
            break;

        case REQD_RC:
            printf("%s %d;", u_in->param_name, (int) *(unsigned short int *) u_in->sys_p);
            break;

        case REQD_SPD:
            show_baud (*(speed_t *) u_in->sys_p);
            break;

        default:
            assert(0);
    }

    printf(" ");
    return;
}

/**
 * FIXME ADD MORE BAUDS
 */
static void show_baud(speed_t thespeed) {
    printf("speed ");
    switch (thespeed) {
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
        case B38400: printf("38400");
            break;
        default: printf("Fast");
            assert(0);
            break;
    }
    printf(" baud ");
}