/*
  Coroutines_ARM.h - Coroutines for Gadgets.
  Created by John Graley, 2020.
  (C) John Graley LGPL license applies.
*/
#ifndef Coroutines_ARM_h
#define Coroutines_ARM_h

#include <csetjmp> 
#include <cstring>

// This file contains low-level stuff for ARM only
static_assert( __arm__==1 || __thumb__==1 );

// We're assuming pointers and ints are the same size because the old
// C library functions we use seem to assume it.
static_assert( sizeof(int) == sizeof(void *) );

// Make sure the setjmp.h is the one we expect
static_assert( sizeof(jmp_buf) == sizeof(int[23]) );

static const int ARM_JMPBUF_INDEX_SP = 8;
static const int ARM_JMPBUF_INDEX_CLS = 5;

inline void *get_jmp_buf_sp( const jmp_buf &env )
{
  return reinterpret_cast<void *>( env[ARM_JMPBUF_INDEX_SP] );
}

inline void set_jmp_buf_sp( jmp_buf &env, void *new_sp )
{
  env[ARM_JMPBUF_INDEX_SP] = reinterpret_cast<int>(new_sp);
}

inline void *get_frame_address()
{
  return reinterpret_cast<void *>( __builtin_frame_address(0) );
}

inline void *get_jmp_buf_cls( const jmp_buf &env )
{
  return reinterpret_cast<void *>( env[ARM_JMPBUF_INDEX_CLS] );
}

inline void set_jmp_buf_cls( jmp_buf &env, void *new_cls )
{
  env[ARM_JMPBUF_INDEX_CLS] = reinterpret_cast<int>(new_cls);
}

inline void copy_jmp_buf( jmp_buf &dest, const jmp_buf &src )
{
    memcpy( dest, src, sizeof(jmp_buf) );
}

inline void *get_cls()
{
    void *cls;
    asm( "mov %[result], r9" : [result] "=r" (cls) : : );
    return cls;
}

inline void set_cls( void *cls )
{
    asm( "mov r9, %[value]" : : [value] "r" (cls) : );
}

typedef int *jmp_buf_ptr;

#endif
