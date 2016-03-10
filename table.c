/* Project: sttyl
 * Author:  Beth Boose
 * File:    table.c
 * Purpose: keep all required data structures in one place
 */

#include "table.h"
#include <stdlib.h>
#include <stdio.h>

/* array of allowable user input with required flag */
struct user_input v_args_arr[] = {
    {"erase",   1, 0, 0, 0, 'n'},
    {"kill",    1, 0, 0, 0, 'n'},
    {"echo",    0, 0, 0, 0, 'l'},
    {"echoe",   0, 0, 0, 0, 'l'},
    {"hupcl",   0, 0, 0, 0, 'c'},
    {"icrnl",   0, 0, 0, 0, 'i'},
    {"isig",    0, 0, 0, 0, 'l'},
    {"opost",   0, 0, 0, 0, 'o'},
    {"intr",    0, 0, 0, 0, 'n'},
    {"icanon",  0, 0, 0, 0, 'l'},
    {NULL,      0, 0, 0, 0,   0}
}; 

/**
 * print a single user_input struct
 * @param u_in
 */
static void printuserinput(struct user_input *u_in) {
    printf("param: %s ", u_in->param_name);
    printf("arg: %c ", u_in->p_arg);
    printf("req'd?: %d ", u_in->req_fl);
    printf("set?: %d ", u_in->set_fl);
    printf("set by usr?: %d\n ", u_in->set_by_usr);
}

/**
 * print an entire user_input array
 */
void printv_arr() {
    printf("v_args_arr:\n");
    for (int i=0; v_args_arr[i].param_name != NULL; i++){
        printuserinput(&v_args_arr[i]);
    }
}

/**
 * I want to get whatever value I need, so put all of them in a table
 * matched against what I might look for
 */