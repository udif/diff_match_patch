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

enum class df {
    DMP_GFS_OK = 0,
    DMP_GFS_NO_FILE,
    DMP_GFS_FILE_TOO_LARGE,
    DMP_GFS_FILENAME_TOO_LONG,
    DMP_GFS_MMAP_FAILED,
    DMP_GFS_STAT_FAILED
};

class diff_file {
    std::string str;
    int fd;
public:
    ~diff_file() {
        if (fd > 0) {
            fd = 0;
            close(fd);
        }
    }
    diff_file(const char *name) {
        struct stat st;
    
        if (strnlen(name, FILENAME_MAX) == FILENAME_MAX) {
            throw df::DMP_GFS_FILENAME_TOO_LONG;
        }
        fd = open(name, O_RDONLY);
        if (fd < 0)
            throw df::DMP_GFS_NO_FILE;
        if (fstat(fd, &st) < 0)
            throw df::DMP_GFS_STAT_FAILED;
        if (st.st_size > DMP_MAX_FILE_SIZE) {
            throw df::DMP_GFS_FILE_TOO_LARGE;
        };
        str.resize(st.st_size); // lseek(df->fd, (off_t)0, SEEK_END);
        std::cout << "len:" << st.st_size << "\n";
        if (read(fd, &str[0], st.st_size) < 0) {
            std::cout << "read() failed: \n";
            throw df::DMP_GFS_MMAP_FAILED;
        }
    }
};

int main(int argc, const char * argv[]) {
    int r1, r2;

    /*
     * Sanity checking
     */
    if (argc != 3) {
        printf("Usage %s file1 file2\n", argv[0]);
        exit(64);
    }
    try {
        diff_match_patch dmp;
        diff_file f1(argv[1]), f2(argv[2]);
        dmp.diff_main(&f1.str, &f2.str);
        //dmp_diff_print_raw(stdout, diff);
        //dmp_diff_free(diff);
    }
    catch (int e) {
    }
    // insert code here...
    return 0;
}



