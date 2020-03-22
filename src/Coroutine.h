/**
 * Coroutine.h - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 * 
 * Main Gadget Coroutine class
 */
#ifndef Coroutine_h
#define Coroutine_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

#include "Coroutine_arm.h"
#include "Task.h"

#include <functional>
#include <csetjmp> 
#include <cstdint>
#include "Arduino.h"

class Coroutine : public Task
{
public:
  explicit Coroutine( std::function<void()> child_function_ ); 
  ~Coroutine();
    
  void operator()();
  inline static Coroutine *get_current();
  inline static void yield( 
    std::function<void()> hop_ = std::function<void()>() );

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
  void yield_nonstatic( std::function<void()> hop_ );
  [[ noreturn ]] void jump_to_parent();
  
  const uint32_t magic;
  const std::function<void()> child_function; 
  const int stack_size;
  byte * const child_stack_memory;
  ChildStatus child_status;
  jmp_buf parent_jmp_buf;
  jmp_buf child_jmp_buf;
  std::function<void()> hop;
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


void Coroutine::yield( std::function<void()> hop )
{
  Coroutine * const that = get_current();
  if( that )
    that->yield_nonstatic( hop );
}

#endif
