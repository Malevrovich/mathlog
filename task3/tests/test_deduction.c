#include "test_utils.h"

#include "deduction.h"

// #define DISABLED

TEST(simple) {
    FILE *out = fopen("deduction.txt", "w+");
    process_proof(stdin, out);
    fclose(out);
}

int main() {
    #ifndef DISABLED
    RUN_TEST(simple);
    #else
    printf("TEST DISABLED\n");
    #endif
}