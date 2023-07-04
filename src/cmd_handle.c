#include <sys/stat.h>

#include "cmd_handle.h"

struct arg_lit *help;
struct arg_file *input_dir, *output_dir;
struct arg_end *end;

int valid_dir(void *parent) {
    struct arg_file *path = parent;
    if (path->count == 0) return 0;

    struct stat dirStat;
    int ret = stat(path->filename[0], &dirStat);
    if (ret < 0) {
        printf("Cannot open file or dir %s", path->filename[0]);
        return ret;
    }
    if (S_ISDIR(dirStat.st_mode)) return 0;

    printf("%s is not a dir", path->filename[0]);
    return 2;
}

int init_cmd_handle(int argc, char **argv, void **args_table) {
    input_dir->hdr.checkfn = valid_dir;
    output_dir->hdr.checkfn = valid_dir;

    int err, exitcode = 0;
    err = arg_parse(argc, argv, args_table);
    if (help->count > 0) {
        printf("Usage: %s", argv[0]);
        arg_print_syntax(stdout, args_table, "\n");
        goto exit;
    }
    if (err > 0) {
        arg_print_errors(stdout, end, argv[0]);
        printf("Try '%s --help' for more information.\n", argv[0]);
        exitcode = 1;
        goto exit;
    }
    if (input_dir->count == 0) {
        input_dir->count = 1;
        input_dir->filename[0] = "input_dir/";
    }
    if (output_dir->count == 0) {
        output_dir->count = 1;
        output_dir->filename[0] = "output_dir/";
    }

    exit:
    return exitcode;
}