//
//  main.cpp
//  dmpdiff
//
//  Created by udif on 11/5/16.
//  Copyright © 2016 udif. All rights reserved.
//

#include <iostream>
extern "C" {
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
}
#include "diff_match_patch.h"


const int DMP_MAX_FILE_SIZE=100000000;

enum {
    DMP_GFS_NO_FILE,
    DMP_GFS_FILE_TOO_LARGE,
    DMP_GFS_FILENAME_TOO_LONG,
    DMP_GFS_MMAP_FAILED,
    DMP_GFS_STAT_FAILED
};

typedef struct {
    off_t len;
    int fd;
    char *m;
} diff_file;

void cleanup (diff_file *df)
{
    if (df->fd > 0) {
        df->fd = 0;
        close(df->fd);
    }
}

int get_file_size(diff_file *df, const char *name)
{
    struct stat st;
    
    if (strnlen(name, FILENAME_MAX) == FILENAME_MAX) {
        return DMP_GFS_FILENAME_TOO_LONG;
    }
    df->fd = open(name, O_RDONLY);
    if (df->fd < 0)
        return DMP_GFS_NO_FILE;
    if (fstat(df->fd, &st) < 0)
        return DMP_GFS_STAT_FAILED;
    df->len = st.st_size; // lseek(df->fd, (off_t)0, SEEK_END);
    if (df->len > DMP_MAX_FILE_SIZE) {
        return DMP_GFS_FILE_TOO_LARGE;
    };
    printf("len:%d\n", (int)df->len);
    df->m = (char *)mmap (0, (size_t)df->len, PROT_READ, MAP_PRIVATE, df->fd, 0);
    if (df->m == MAP_FAILED) {
        std::cout << "mmap failed: \n";
        return DMP_GFS_MMAP_FAILED;
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    diff_match_patch dmp;
    diff_file f1, f2;
    int r1, r2;

    /*
     * Sanity checking
     */
    if (argc != 3) {
        printf("Usage %s file1 file2\n", argv[0]);
        exit(64);
    }
    if ((r1 = get_file_size(&f1, argv[1])) || (r2 = get_file_size(&f2, argv[2]))) {
        printf("r1=%d r2=%d!\n", r1, r2);
        exit(66);
    }
    dmp.diff_main(f1.m, (uint32_t)f1.len, f2.m, (uint32_t)f2.len);
    dmp_diff_print_raw(stdout, diff);
    dmp_diff_free(diff);
    cleanup(&f1);
    cleanup(&f2);
    // insert code here...
    return 0;
}



