/* date = May 10th 2021 11:13 pm */

#ifndef GS_DEBUG_BREAK_H
#define GS_DEBUG_BREAK_H

// Function signature for debug_break, no matter the platform
#define DEBUG_BREAK() \
__attribute__((always_inline)) \
__inline__ static void debug_break(void)

#if defined(_MSC_VER)

# define debug_break __debugbreak

#elif defined(__i386__) || defined(__x86_64__)
DEBUG_BREAK()
{
	__asm__ volatile(".inst 0xde01");
}
#elif defined(__arm__) && !defined(__thumb__)
DEBUG_BREAK()
{
	__asm__ volatile(".inst 0xe7f001f0");
}
#elif defined(__aarch64__) && defined(__APPLE__)

# define debug_break __buitin_trap

#elif defined(__aarch64__)
DEBUG_BREAK()
{
    __asm__ volatile(".inst 0xd4200000");
}
#elif defined(__powerpc__)
DEBUG_BREAK()
{
    __asm__ volatile(".4byte 0x7d821008");
}
#elif defined(__riscv)
DEBUG_BREAK()
{
    __asm__ volatile(".4byte 0x00100073");
}
#else
DEBUG_BREAK()
{
    raise(SIGTRAP);
}
#endif

#endif //GS_DEBUG_BREAK_H
