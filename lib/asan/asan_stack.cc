//===-- asan_stack.cc -----------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a part of AddressSanitizer, an address sanity checker.
//
// Code for ASan stack trace.
//===----------------------------------------------------------------------===//
#include "asan_internal.h"
#include "asan_flags.h"
#include "asan_stack.h"
#include "sanitizer_common/sanitizer_flags.h"

namespace __asan {

void PrintStack(const uptr *trace, uptr size) {
  StackTrace::PrintStack(trace, size);
}

void PrintStack(StackTrace *stack) {
  PrintStack(stack->trace, stack->size);
}

}  // namespace __asan

// ------------------ Interface -------------- {{{1

extern "C" {
SANITIZER_INTERFACE_ATTRIBUTE
void __sanitizer_print_stack_trace() {
  using namespace __asan;
  PRINT_CURRENT_STACK();
}
}  // extern "C"
