
/* Project: sttyl
 * Author:  Beth Boose
 * File:    table.h
 */

#ifndef TABLE_H
#define TABLE_H

#include <termios.h>
#include <sys/ioctl.h>

extern struct termios sys_tty;
extern struct winsize sys_win;
extern speed_t sys_speed;


enum disp_t {
    REQD_BIT,
    REQD_CHAR,
    REQD_RC,
    REQD_SPD
};

/* for collecting user input, param name and arg flag */
struct all_fields {
    char *param_name;  /** name of valid cmdline param */
    enum disp_t req_fl;     /** type that sys_p points to */
    void *sys_p;       /** field in struct termios etc that holds this param */
    tcflag_t bitmask;  /** which bit(s) in *sys_p (if req_fl indicates BIT) */
    int read_only;     /** parameter cannot be set; display only */ 
};

extern struct all_fields dispf[];  //FIXME - less brittle
extern void print_field(struct all_fields *u_in);
extern void print_all_fields();
void printv_arr();
#endif /* TABLE_H */

