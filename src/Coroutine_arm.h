/*
  Coroutine_arm.h - Coroutines for Gadgets.
  Created by John Graley, 2020.
  (C) John Graley LGPL license applies.
*/
#ifndef Coroutine_arm_h
#define Coroutine_arm_h

#include <csetjmp> 
#include <cstring>

namespace GC
{
namespace Arm
{

// This file contains low-level stuff for ARM only
static_assert( __arm__==1 || __thumb__==1 );

// We're assuming pointers and ints are the same size because the old
// C library functions we use seem to assume it.
static_assert( sizeof(int) == sizeof(void *) );

// Make sure the setjmp.h is the one we expect
static_assert( sizeof(jmp_buf) == sizeof(int[23]) );

// Jmp buf only holds "callee-save" registers
static const int FIRST_CALLEE_SAVE = 4;

// See http://infocenter.arm.com/help/topic/com.arm.doc.espc0002/ATPCS.pdf
static const int JMPBUF_INDEX_SP = 12 - FIRST_CALLEE_SAVE; // r12
static const int JMPBUF_INDEX_TR =  9 - FIRST_CALLEE_SAVE; // r9
#if defined(__thumb__)
// ATPCS says any of r4-r7 which would be problematic for us. But gcc
// uses r7 by default, so this will work until you configure it otherwise.
static const int JMPBUF_INDEX_FP =  7 - FIRST_CALLEE_SAVE; // r7
#elif defined(__arm__)
static const int JMPBUF_INDEX_FP = 11 - FIRST_CALLEE_SAVE; // r11
#endif 

inline void *get_jmp_buf_sp( const jmp_buf &env )
{
  return reinterpret_cast<void *>( env[JMPBUF_INDEX_SP] );
}

inline void set_jmp_buf_sp( jmp_buf &env, void *new_sp )
{
  env[JMPBUF_INDEX_SP] = reinterpret_cast<int>(new_sp);
}

inline void *get_jmp_buf_tr( const jmp_buf &env )
{
  return reinterpret_cast<void *>( env[JMPBUF_INDEX_TR] );
}

inline void set_jmp_buf_tr( jmp_buf &env, void *new_cls )
{
  env[JMPBUF_INDEX_TR] = reinterpret_cast<int>(new_cls);
}

inline void *get_jmp_buf_fp( const jmp_buf &env )
{
  return reinterpret_cast<void *>( env[JMPBUF_INDEX_FP] );
}

inline void set_jmp_buf_fp( jmp_buf &env, void *new_fp )
{
  env[JMPBUF_INDEX_FP] = reinterpret_cast<int>(new_fp);
}

inline void copy_jmp_buf( jmp_buf &dest, const jmp_buf &src )
{
    memcpy( dest, src, sizeof(jmp_buf) );
}


inline void *get_tr()
{
    void *cls;
    asm( "mov %[result], r9" : [result] "=r" (cls) : : );
    return cls;
}

inline void set_tr( void *cls )
{
    asm( "mov r9, %[value]" : : [value] "r" (cls) : );
}

inline void *get_sp()
{
    void *sp;
    asm( "mov %[result], sp" : [result] "=r" (sp) : : );
    return sp;
}

typedef int *jmp_buf_ptr;

} } // namespace

#endif
