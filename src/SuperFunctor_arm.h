#ifndef SUPERFUNCTOR_ARM_H
#define SUPERFUNCTOR_ARM_H

#include <utility>
#include <cstddef>
#include <cstdint>

namespace GC
{
namespace Arm
{
// This file contains low-level stuff for ARM only
static_assert( __arm__==1 || __thumb__==1 );

static const int member_function_pointer_size = 8;

inline void *get_vcall_destination(uint32_t *mfp_words, uint32_t *this_words)
{
    if( mfp_words[1] & 1 )
    {
        int this_offset_bytes = mfp_words[1] >> 1;
        auto vtable = (void **)(this_words[this_offset_bytes>>2]);
        int vtable_offset_bytes = mfp_words[0];
        return vtable[vtable_offset_bytes>>2];
    }
    else
    {
        return (void *)(mfp_words[0]);        
    }        
}

// Only thumb-mode trampoline has been tested
static_assert( __thumb__==1 );
#define TRAMPOLINE_THUMB 1

#ifdef TRAMPOLINE_THUMB
typedef uint16_t MachineInstruction;
#else
typedef uint32_t MachineInstruction;
#endif

inline void *ptr_to_function_ptr( MachineInstruction *ptr )
{
    auto ptr_word = (uint32_t)ptr;
#ifdef TRAMPOLINE_THUMB
    ptr_word |= 1; // set the "thumb" bit to make a function pointer
#endif
    return (void *)ptr_word;
}

#define SUPER_FUNCTOR_TRAMPOLINE_SIZE 4
#define SUPER_FUNCTOR_TRAMPOLINE \
    "mov r0, pc\n\t" /* Get PC */ \
    "sub r0, %[this_to_PC_offset]\n\t" /* Calculate "this" pointer into r0 */ \
    "ldr r1, [r0, %[this_to_entrypoint_offset]]\n\t" /* Get entrypoint function pointer */ \
    "bx  r1" /* Jump to it (tail call) */
    
static const int pipeline_overshoot_instructions = 2;

} } // namespace
    
#endif