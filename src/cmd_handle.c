#include <sys/stat.h>
#include <errno.h>

#include "cmd_handle.h"

struct arg_lit *help;
struct arg_file *input_dir, *output_dir;
struct arg_end *end;

int valid_dir(struct arg_file *path) {
    if (path->count == 0) return 0;

    struct stat dirStat;
    int ret = stat(path->filename[0], &dirStat);
    if (ret < 0) return TMS_ERR_PATH_STAT_ERR;
    if (S_ISDIR(dirStat.st_mode)) return 0;

    return TMS_ERR_PATH_IS_NOT_DIR;
}

void dir_error(struct arg_file *path, arg_dstr_t ds, int error, const char* argval, const char* progname) {
    const char* shortopts = path->hdr.shortopts;
    const char* longopts = path->hdr.longopts;
    const char* datatype = path->hdr.datatype;

    argval = path->filename[0];

    arg_dstr_catf(ds, "%s: ", progname);
    switch (error) {
        case TMS_ERR_PATH_STAT_ERR:
            arg_print_option_ds(ds, shortopts, longopts, datatype, ": ");
            {
                struct stat path_stat;
                stat(argval, &path_stat);
            }
            arg_dstr_catf(ds, "%s: \"%s\"\n", strerror(errno), argval);
            break;
        case TMS_ERR_PATH_IS_NOT_DIR:
            arg_print_option_ds(ds, shortopts, longopts, datatype, ": ");
            arg_dstr_catf(ds, "\"%s\" is not a dir\n", argval);
            break;
        default:
            break;
    }
}

int init_cmd_handle(int argc, char **argv, void **args_table) {
    input_dir->hdr.checkfn = (arg_checkfn*)valid_dir;
    input_dir->hdr.errorfn = (arg_errorfn*)dir_error;
    output_dir->hdr.checkfn = (arg_checkfn*)valid_dir;
    output_dir->hdr.errorfn = (arg_errorfn*)dir_error;

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