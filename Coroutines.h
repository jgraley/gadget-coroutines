/*
  Coroutines.h - Coroutines for Gadgets.
  Created by John Graley, 2020.
  (C) John Graley LGPL license applies.
*/
#ifndef Coroutines_h
#define Coroutines_h

#include "Arduino.h"
#include <functional>
#include <csetjmp> 
#include <cstdint>
	
#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

void gcoroutines_set_logger( std::function< void(const char *) > logger );

class Coroutine
{
public:
    typedef uint8_t byte;

    explicit Coroutine( std::function<void()> child_main_function_ ); 
    ~Coroutine();
    
    void run_iteration();
    inline static void yield();
    inline static Coroutine *get_current();

private:
    enum ChildStatus
    {
        READY,
        RUNNING,
        COMPLETE
    };
    
    [[ noreturn ]] void start_child();
    void yield_ns();

    const uint32_t magic;
    std::function<void()> child_main_function;
    const int stack_size;
    byte * const child_stack_memory;
    ChildStatus child_status;
    jmp_buf parent_jmp_buf;
    jmp_buf child_jmp_buf;
    
    static const int default_stack_size = 1024;
};

#endif
