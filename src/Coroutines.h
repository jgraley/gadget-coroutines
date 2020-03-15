/**
 * Coroutines.h - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 * 
 * Main Gadget Coroutine class
 */
#ifndef Coroutines_h
#define Coroutines_h

#include "Coroutines_ARM.h"

#include <functional>
#include <csetjmp> 
#include <cstdint>
#include "Arduino.h"

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

void gcoroutines_set_logger( std::function< void(const char *) > logger );

class Coroutine
{
public:
  explicit Coroutine( std::function<void()> child_function_ ); 
  ~Coroutine();
    
  void operator()();
  inline static Coroutine *get_current();
  inline static void yield( 
    std::function<void()> enabler = std::function<void()>(),
    std::function<void()> disabler = std::function<void()>() );

private:
  enum ChildStatus
  {
    READY,
    RUNNING,
    COMPLETE
  };
  
  enum LongJmpValue
  {
    IMMEDIATE = 0, // Must be zero
    PARENT_TO_CHILD = 1, // All the others must be non-zero
    CHILD_TO_PARENT = 2,
    PARENT_TO_CHILD_STARTING = 3
  };

  byte *prepare_child_stack( byte *frame_end, byte *stack_pointer );
  void prepare_child_jmp_buf( jmp_buf &child_jmp_buf, const jmp_buf &initial_jmp_buf, byte *parent_stack_pointer, byte *child_stack_pointer );
  [[ noreturn ]] void child_main_function();
  void jump_to_child();
  void yield_nonstatic( std::function<void()> enabler, std::function<void()> disabler );
  [[ noreturn ]] void jump_to_parent();
  
  const uint32_t magic;
  const std::function<void()> child_function; 
  const int stack_size;
  byte * const child_stack_memory;
  ChildStatus child_status;
  jmp_buf parent_jmp_buf;
  jmp_buf child_jmp_buf;
  std::function<void()> child_enabler;
  std::function<void()> child_disabler;
    
  static const int default_stack_size = 1024;
  static const uint32_t MAGIC;
};

///-- 
// Implement the inline functions here

Coroutine *Coroutine::get_current()
{
  return (Coroutine *)( get_cls() );
}


void Coroutine::yield( std::function<void()> enabler, std::function<void()> disabler )
{
  Coroutine * const that = get_current();
  if( that )
    that->yield_nonstatic( enabler, disabler );
}

#endif
