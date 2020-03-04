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

/// @TODO up-to-date exception specs

class GCoroutine
{
public:
    explicit GCoroutine( std::function coroutine_function );
    ~GCoroutine();
    
    void run_iteration();
    void yield();

private:
    enum ChildStatus
    {
        READY,
        RUNNING,
        COMPLETE
    }

    // @TODO Some kind of magic to protect all methods
    std::byte * const child_stack_memory;
    ChildStatus child_status;
    jmp_buf child_jmp_buf;
    
    static const int default_stack_size = 1024;
};

#endif
