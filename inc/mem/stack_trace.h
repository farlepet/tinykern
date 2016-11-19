#ifndef MEM_STACK_TRACE_H
#define MEM_STACK_TRACE_H

#include <types.h>

void stack_trace(u32 max_frames, u32 *ebp, u32 saved_eip);

#endif
