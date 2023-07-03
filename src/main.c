#include <stdio.h>
#include <argtable3.h>

struct arg_lit *help;
struct arg_file *input_dir, *output_dir;
struct arg_end *end;

int main(int argc, char **argv) {
    void *argtable[] = {
        help = arg_litn("h", "help", 0, 1, "display this help and exit"),
        input_dir = arg_file0(NULL, NULL, "<input_dir>", "read tile image from dir, default: ./input"),
        output_dir = arg_file0(NULL, NULL, "<output_dir>", "write result , default: ./output"),
        end = arg_end(0),
    };

    int err, exitcode = 0;
    err = arg_parse(argc, argv, argtable);
    if (help->count > 0) {
        printf("Usage: %s", argv[0]);
        arg_print_syntax(stdout, argtable, "\n");
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

    printf("Hello, World!\n");

    exit:
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return exitcode;
}
