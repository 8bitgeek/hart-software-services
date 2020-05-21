/*******************************************************************************
 * Copyright 2019 Microchip Corporation.
 *
 * SPDX-License-Identifier: MIT
 * 
 * MPFS HSS Embedded Software
 *
 */

/**
 * \file Stack Checking guard functions
 * \brief Stack Checking guard functions
 */

#include "config.h"
#include "hss_types.h"

#include "hss_debug.h"

unsigned long __stack_chk_guard;

void __stack_chk_guard_setup(void);
void __stack_chk_fail(void);

__attribute__((weak)) void __stack_chk_guard_setup(void)
{
    __stack_chk_guard = 0xDEAD0BAD;
}

__attribute__((weak)) void __stack_chk_fail(void)
{
    // Print a canary to know that we've had stack corruption...
    //
    // to help debug this, it might help to disable the print statement
    // once stack corruption is detected...
    mHSS_DEBUG_PUTS("__stack_chk_fail(): stack corruption detected!!" CRLF);

    asm("ebreak");
}

#ifdef __linux
// if using the Linux compiler, we might need __memset_chk() also
#include <string.h> // for memset
void* __memset_chk(void *dst, int c, size_t len, size_t dstlen);
__attribute__((weak)) void* __memset_chk(void *dst, int c, size_t len, size_t dstlen)
{
    if (dstlen < len) {
        mHSS_DEBUG_PUTS("__memset_chk(): dstlen < len!!" CRLF);
        while (1); // stack has been corrupted
    } else {
        return memset(dst, c, len);
    }
}
#endif
