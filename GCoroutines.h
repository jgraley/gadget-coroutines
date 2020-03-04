/*
  GCoroutines.h - Coroutines for Gadgets.
  Created by John Graley, 2020.
  (C) John Graley LGPL license applies.
*/
#ifndef GCoroutines_h
#define GCoroutines_h

#include "Arduino.h"
#include <functional>
#include <csetjmp> /// @TODO use std::any to break dependency
#include <cstdint>
#include <stdexcept>
	
/// @TODO add modern exception specs. Currently getting warnings around ~GCoroutines() like 
/// warning: throw will always call terminate() [-Wterminate]
/// note: in C++11 destructors default to noexcept


class GCoroutine_internal_error : public std::runtime_error
{
public:
    explicit GCoroutine_internal_error( const std::string& what_arg );
};


class GCoroutine_bad_longjmp_value : public GCoroutine_internal_error
{
public:
    explicit GCoroutine_bad_longjmp_value(const char *file, int line, int val);
};


class GCoroutine_TODO : public GCoroutine_internal_error
{
public:
    explicit GCoroutine_TODO(std::string what);
};



class GCoroutine
{
public:
    typedef uint8_t byte;

    explicit GCoroutine( std::function< void(GCoroutine *) > child_function );
    ~GCoroutine();
    
    void run_iteration();
    void yield();

private:
    enum ChildStatus
    {
        READY,
        RUNNING,
        COMPLETE
    };

    // @TODO Some kind of magic to protect all methods
    const int stack_size;
    byte * const child_stack_memory;
    ChildStatus child_status;
    jmp_buf child_jmp_buf;
    
    static const int default_stack_size = 1024;
};

#endif
