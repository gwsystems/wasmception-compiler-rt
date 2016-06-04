// RUN: %clang_cl_asan -O0 %s -Fe%t
// RUN: not %run %t 2>&1 | FileCheck %s

#include <stdio.h>
#include <string.h>
#include <malloc.h>

int main() {
  char *ptr = _strdup("Hello");
  int subscript = 1;
  ptr[subscript] = '3';
  printf("%s\n", ptr);
  fflush(0);
// CHECK: H3llo

  subscript = -1;
  ptr[subscript] = 42;
// CHECK: AddressSanitizer: heap-buffer-overflow on address [[ADDR:0x[0-9a-f]+]]
// CHECK: WRITE of size 1 at [[ADDR]] thread T0
// CHECK:   {{#0 .* main .*}}intercept_strdup.cc:[[@LINE-3]]
// CHECK: [[ADDR]] is located 1 bytes to the left of 6-byte region
// CHECK: allocated by thread T0 here:
// CHECK:   #0 {{.*strdup}}
// CHECK:   #1 {{.* main .*}}intercept_strdup.cc:[[@LINE-15]]
// CHECK:   #2 {{.*foobar}}
// CHECK:   #3 {{.*foobar}}
// CHECK:   #4 {{.*foobar}}
// CHECK:   #5 {{.*foobar}}
// CHECK:   #6 {{.*foobar}}
// CHECK:   #7 {{.*foobar}}
// CHECK:   #8 {{.*foobar}}
// CHECK:   #9 {{.*foobar}}
  free(ptr);
}
