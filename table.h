
/* Project: sttyl
 * Author:  Beth Boose
 * File:    table.h
 */

#ifndef TABLE_H
#define TABLE_H

/* for collecting user input, param name and arg flag */
struct user_input {
    char *param_name;  /* name of valid cmdline param */
    int req_fl;         /* set if param_name should have an argument */
    char p_arg;       /* argument to params that require them */
    int set_fl;         /* on/off sw for set params */
    int set_by_usr;      /* on if set by user */
    char type_fl;        /* is this control, local or input fl? */ 
};

extern struct user_input v_args_arr[11]; //FIXME - less brittle
void printv_arr();
#endif /* TABLE_H */

