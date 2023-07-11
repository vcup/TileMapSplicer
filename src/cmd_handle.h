#ifndef TILE_MAP_SPLICER_CMD_HANDLE_H
#define TILE_MAP_SPLICER_CMD_HANDLE_H

#include <argtable3.h>
#define DEFAULT_INPUT_DIR "input_dir"
#define DEFAULT_OUTPUT_DIR "output_dir"

enum {
    TMS_ERR_PATH_STAT_ERR = 1,
    TMS_ERR_PATH_IS_NOT_DIR,
};


extern struct arg_lit *help;
extern struct arg_file *input_dir, *output_dir;
extern struct arg_end *end;

void init_cmd_handle();
int invoke_cmd(int argc, char **argv);
#endif // TILE_MAP_SPLICER_CMD_HANDLE_H