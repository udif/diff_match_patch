//
//  main.cpp
//  dmpdiff
//
//  Created by udif on 11/5/16.
//  Copyright © 2016 udif. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <locale>

#include "diff_match_patch.h"

std::wstring readFile(const char* filename)
{
    std::wifstream wif(filename);
    wif.imbue(std::locale::global(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>)));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}

//std::ostream& operator<<(std::ostream& os, const Diff& d)
static void dump_Diff(const Diff& d)
{
    switch(d.operation) {
        case INSERT:
            std::wcout << "+" << d.text;
            break;
        case DELETE:
            std::wcout << "-" << d.text;
            break;
        case EQUAL:
            std::wcout << "=" << d.text;
            break;
    }
}

static void dump_diff(std::list<Diff> ld)
{
    for (auto diff : ld) {
        dump_Diff(diff);
    }
}
int main(int argc, const char * argv[]) {

    /*
     * Sanity checking
     */
    if (argc != 3) {
        printf("Usage %s file1 file2\n", argv[0]);
        exit(64);
    }
    try {
        diff_match_patch dmp;
        std::wstring wstr1 = readFile(argv[1]);
        std::wstring wstr2 = readFile(argv[2]);
        
        std::list<Diff> result = dmp.diff_main(wstr1, wstr2);
        dump_diff(result);
        //dmp_diff_print_raw(stdout, diff);
        //dmp_diff_free(diff);
    }
    catch (int e) {
    }
    // insert code here...
    return 0;
}





