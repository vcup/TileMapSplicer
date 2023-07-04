#include <stdio.h>
#include "cmd_handle.h"


int main(int argc, char **argv) {
    void *args_table[] = {
        help = arg_litn("h", "help", 0, 1, "display this help and exit"),
        input_dir = arg_file0(NULL, NULL, "<input_dir>", "read tile image from dir, default: ./input"),
        output_dir = arg_file0(NULL, NULL, "<output_dir>", "write result , default: ./output"),
        end = arg_end(20),
    };
    int exitcode = init_cmd_handle(argc, argv, args_table);
    if (exitcode) goto exit;

    printf("Hello, World!\n");

    exit:
    arg_freetable(args_table, sizeof(args_table) / sizeof(args_table[0]));
    return exitcode;
}
