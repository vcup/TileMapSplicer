#include <sys/stat.h>
#include <errno.h>

#include "cmd_handle.h"

struct arg_lit *help;
struct arg_file *input_dir, *output_dir;
struct arg_end *end;

int valid_input_dir(struct arg_file *path) {
    if (path->count == 0) return 0;

    struct stat path_stat;
    int ret = stat(path->filename[0], &path_stat);
    if (ret < 0) return TMS_ERR_PATH_STAT_ERR;
    if (S_ISDIR(path_stat.st_mode)) return 0;

    return TMS_ERR_PATH_IS_NOT_DIR;
}
int valid_output_dir(struct arg_file *path) {
    if (path->count == 0) return 0;

    struct stat path_stat;
    int ret = stat(path->filename[0], &path_stat);
    if (ret < 0 && errno != ENOENT) return TMS_ERR_PATH_STAT_ERR;
    if (errno == ENOENT || S_ISDIR(path_stat.st_mode)) return 0;

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

int handle_cmd_help_and_err(arg_dstr_t res, int argc, char **argv, void **cmd){
    int err = arg_parse(argc, argv, cmd);
    if (help->count > 0) {
        arg_dstr_catf(res, "Usage: %s", argv[0]);
        arg_print_syntax_ds(res, cmd, "\n");
        return 0;
    }
    if (err > 0) {
        arg_print_errors_ds(res, end, argv[0]);
        arg_dstr_catf(res, "Try '%s --help' for more information.\n", argv[0]);
        return 1;
    }

    return 0;
}

int normal_cmd_proc(int argc, char *argv[], arg_dstr_t res) {
    void *cmd[] = {
        help,
        input_dir,
        output_dir,
        end,
    };

    int exitcode = handle_cmd_help_and_err(res, argc, argv, cmd);
    if (exitcode) goto exit;

    exit:
    return 0;
}

int valid_cmd_proc(int argc, char *argv[], arg_dstr_t res) {
    void *cmd[] = {
        help,
        input_dir,
        end,
    };

    int exitcode = handle_cmd_help_and_err(res, argc, argv, cmd);
    if (exitcode) goto exit;

    exit:
    return 0;
}

void init_cmd_handle() {
    input_dir->hdr.checkfn = (arg_checkfn*) valid_input_dir;
    input_dir->hdr.errorfn = (arg_errorfn*)dir_error;
    output_dir->hdr.checkfn = (arg_checkfn*)valid_output_dir;
    output_dir->hdr.errorfn = (arg_errorfn*)dir_error;

    arg_cmd_init();
    arg_cmd_register("", normal_cmd_proc, "handle input tile, splicer as XYZ format");
    arg_cmd_register("valid", valid_cmd_proc, "verify input tile png image format only");

    if (input_dir->count == 0) {
        input_dir->count = 1;
        input_dir->filename[0] = DEFAULT_INPUT_DIR;
    }
    if (output_dir->count == 0) {
        output_dir->count = 1;
        output_dir->filename[0] = DEFAULT_OUTPUT_DIR;
        mkdir(DEFAULT_OUTPUT_DIR);
    } else mkdir(output_dir->filename[0]);
}

int invoke_cmd(int argc, char **argv) {
    arg_dstr_t res = arg_dstr_create();
    int exitcode;
    const char *subcmd;
    int subcmd_idx = 1;
    for (; subcmd_idx < argc; subcmd_idx++) {
        subcmd = argv[subcmd_idx];
        if (subcmd[0] != '-') break;
    }
    const int is_main_cmd = subcmd_idx == argc || (
        strcmp(subcmd, "valid")
    );
    if (is_main_cmd) exitcode = arg_cmd_dispatch("", argc, argv, res);
    else {
        char *argv_shifted[argc];
        for (int i = 0; i < subcmd_idx; ++i) {
            argv_shifted[i] = argv[i];
        }
        for (int i = subcmd_idx + 1; i < argc; i++) {
            argv_shifted[i - 1] = argv[i];
        }

        exitcode = arg_cmd_dispatch(subcmd, --argc, argv_shifted, res);
    }

    return exitcode;
}