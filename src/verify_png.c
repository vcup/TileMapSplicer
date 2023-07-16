#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <png.h>

#include "verify_png.h"

int verify_all_png(const char *dir_path) {
    DIR *dir;
    struct dirent *ent;
    struct stat path_stat;
    FILE *fp;
    png_structp png;
    png_infop info;
    unsigned long long dir_path_len = strlen(dir_path);
    char file_full_path[dir_path_len + FILENAME_MAX];
    strcpy(file_full_path, dir_path);
    file_full_path[dir_path_len] = '/';

    dir = opendir(dir_path);
    while ((ent = readdir(dir))) {
        if (ent->d_namlen < 4 || strcasecmp(ent->d_name + ent->d_namlen - 4, ".png")) continue;
        strcpy(file_full_path + dir_path_len + 1, ent->d_name);
        stat(file_full_path, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) continue;

        fp = fopen(file_full_path, "rb");
        png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        info = png_create_info_struct(png);

        if(setjmp(png_jmpbuf(png))) {
            printf("Error during reading PNG file: %s\n", file_full_path);
            continue;
        }

        png_init_io(png, fp);
        png_read_info(png, info);

        unsigned int height = png_get_image_height(png, info);
        for (int i = 0; i < height; ++i) {
            png_read_row(png, NULL, NULL);
        }

        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
    }
    return 0;
}
