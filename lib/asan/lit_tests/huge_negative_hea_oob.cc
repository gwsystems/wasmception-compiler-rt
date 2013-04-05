// RUN: %clangxx_asan -m64  %s -o %t && %t 2>&1 | %symbolize > %t.out
// Check that we can find huge buffer overflows to the left.
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv) {
  char *x = (char*)malloc(1 << 20);
  memset(x, 0, 10);
  int res = x[-argc * 4000];  // BOOOM
  // CHECK: is located 4000 bytes to the left of
}
