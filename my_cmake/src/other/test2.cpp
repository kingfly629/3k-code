#include "Rectange.h"
#include "myfriend.h"
#include <memory>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <exception>
#include <algorithm>
#include <vector>
using namespace kkk;

int main(void) {
    try {
        char *string, *stopstring;
        double x;
        int base;
        string = "10110134932";
        printf("string = %s\n", string);
        /*Convertstringusingbase2,4,and8:*/
        for (base = 2; base <= 8; base *= 2) {
            /*Convertthestring:*/
            ul = strtoul(string, &stopstring, base);
            printf("strtol = %ld(base %d)\n", ul, base);
            printf("Stopped scan at: %s\n", stopstring);
        }
        return 0;
    } catch (const std::exception &ex) {
        std::cerr << "catch Exception: " << ex.what() << std::endl;
        fprintf(stderr, "catch Exception : %s", ex.what());
    }
    return 0;
}
