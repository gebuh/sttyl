
/* Project: sttyl
 * Author:  Beth Boose
 * File:    table.h
 *  Purpose: keep all required data structures in one place
 */

#ifndef TABLE_H
#define TABLE_H

#include <termios.h>
#include <sys/ioctl.h>

/** our termios struct for display and/or setting */
extern struct termios sys_tty;

/** hold row/col data */
extern struct winsize sys_win;

/** hold speed data */
extern speed_t sys_speed; 

/**
 * use enum to decide how to determine dispf type
 */
enum disp_t {
    REQD_BIT,  /** termios values that are set by bits (on/off) type cc_t */
    REQD_CHAR, /** termios values with user input char values type tcflag_t */
    REQD_RC,    /* row column output - not implemented */
    REQD_SPD    /* baud rate output - not implemented */
};

/* defines a struct to contain all the info needed to get/set 
 * termios data 
 */
struct stty_fields {
    char *param_name;           /** name of valid cmdline param */
    enum disp_t param_type;     /** type that sys_p points to */
    void *sys_p;       /** field in struct termios etc that holds this param */
    tcflag_t bitmask;  /** which bit(s) in *sys_p (if req_fl indicates BIT) */
    int read_only;     /** parameter cannot be set; display only */ 
};

/** array of implemented input and/or output termios tty fields */
extern const struct stty_fields dispf[];

/**
 * print a single user_input struct use disp_t to determine type
 * and print accordingly
 * @param one_row a single stty_fields struct
 */
extern void print_dispf_row(const struct stty_fields *one_row);

/**
 * print the entire dispf
 */
extern void print_dispf();

#endif /* TABLE_H */
