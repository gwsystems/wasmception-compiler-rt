//=-- lsan.h --------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is a part of LeakSanitizer.
// Private header for standalone LSan RTL.
//
//===----------------------------------------------------------------------===//

#include "lsan_thread.h"
#include "sanitizer_common/sanitizer_flags.h"
#include "sanitizer_common/sanitizer_stacktrace.h"

#define GET_STACK_TRACE(max_size, fast)                       \
  __sanitizer::BufferedStackTrace stack;                      \
  GetStackTrace(&stack, max_size, StackTrace::GetCurrentPc(), \
                GET_CURRENT_FRAME(), nullptr, fast);

#define GET_STACK_TRACE_FATAL \
  GET_STACK_TRACE(kStackTraceMax, common_flags()->fast_unwind_on_fatal)

#define GET_STACK_TRACE_MALLOC                                      \
  GET_STACK_TRACE(__sanitizer::common_flags()->malloc_context_size, \
                  common_flags()->fast_unwind_on_malloc)

#define GET_STACK_TRACE_THREAD GET_STACK_TRACE(kStackTraceMax, true)

namespace __lsan {

void InitializeInterceptors();
void ReplaceSystemMalloc();

#define ENSURE_LSAN_INITED do {   \
  CHECK(!lsan_init_is_running);   \
  if (!lsan_inited)               \
    __lsan_init();                \
} while (0)

// Get the stack trace with the given pc and bp.
// The pc will be in the position 0 of the resulting stack trace.
// The bp may refer to the current frame or to the caller's frame.
ALWAYS_INLINE
void GetStackTrace(__sanitizer::BufferedStackTrace *stack,
                   __sanitizer::uptr max_depth, __sanitizer::uptr pc,
                   __sanitizer::uptr bp, void *context, bool fast) {
  uptr stack_top = 0, stack_bottom = 0;
  ThreadContext *t;
  if (__sanitizer::StackTrace::WillUseFastUnwind(fast) &&
      (t = CurrentThreadContext())) {
    stack_top = t->stack_end();
    stack_bottom = t->stack_begin();
  }
  if (!SANITIZER_MIPS || IsValidFrame(bp, stack_top, stack_bottom)) {
    if (StackTrace::WillUseFastUnwind(fast))
      stack->Unwind(max_depth, pc, bp, nullptr, stack_top, stack_bottom, true);
    else
      stack->Unwind(max_depth, pc, 0, context, 0, 0, false);
  }
}

}  // namespace __lsan

extern bool lsan_inited;
extern bool lsan_init_is_running;

extern "C" SANITIZER_INTERFACE_ATTRIBUTE void __lsan_init();
