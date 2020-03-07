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
	
/// @TODO require C++11

void gcoroutines_set_logger( std::function< void(const char *) > logger );

class GCoroutine
{
public:
    typedef uint8_t byte;

    explicit GCoroutine( void (*child_main_function_)(GCoroutine *) ); // @TODO back to std::function if possible @TODO if we can use cls effectively, no need for the GCoroutine * argument
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
    
    [[ noreturn ]] void start_child();

    const int magic;
    void (* const child_main_function)(GCoroutine *);
    const int stack_size;
    byte * const child_stack_memory;
    ChildStatus child_status;
    jmp_buf parent_jmp_buf;
    jmp_buf child_jmp_buf;
    
    static const int default_stack_size = 1024;
};

#endif
