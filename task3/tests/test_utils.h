#pragma once

#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

#include <string.h>
#include <stdio.h>

#define TEST(name) void test_##name()

#define RUN_TEST(name)                      \
do {                                        \
    printf("------------------------\n");   \
    printf("Running test " #name "...\n");  \
    test_##name();                          \
    printf("Success!\n");                   \
    printf("------------------------\n");   \
} while(0)

#endif
