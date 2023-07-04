#ifndef CMD_HANDLE
#define CMD_HANDLE

#include <argtable3.h>
enum {
    TMS_ERR_PATH_STAT_ERR = 1,
    TMS_ERR_PATH_IS_NOT_DIR,
};


extern struct arg_lit *help;
extern struct arg_file *input_dir, *output_dir;
extern struct arg_end *end;

int init_cmd_handle(int argc, char **argv, void **args_table);
#endif