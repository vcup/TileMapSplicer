#ifndef CMD_HANDLE
#define CMD_HANDLE

#include <argtable3.h>

extern struct arg_lit *help;
extern struct arg_file *input_dir, *output_dir;
extern struct arg_end *end;

int init_cmd_handle(int argc, char **argv, void **args_table);
#endif