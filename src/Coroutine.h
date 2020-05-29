/**
 * @file Coroutine.h
 * ### `hopping-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 * 
 * @brief Main Gadget Coroutine class
 */
#ifndef Coroutine_h
#define Coroutine_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

#include "Coroutine_arm.h"
#include "Task.h"
#include "Integration.h"

#include <functional>
#include <csetjmp> 
#include <cstdint>
#include <atomic>
#include "Arduino.h"

namespace HC
{

class Coroutine : public Task
{
public:
  explicit Coroutine( std::function<void()> child_function_ ); 
  ~Coroutine();
    
  inline static Coroutine *me();
  inline static void yield();

  static void wait( std::function<bool()> test );
  void set_hop_lambda( std::function<void()> hop );
  std::pair<const byte *, const byte *> get_child_stack_bounds();
  int estimate_stack_peak_usage();
  int get_cls_usage();
  
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

  // from libgcc/emutls.c
  typedef unsigned int word __attribute__((mode(word)));
  typedef unsigned int pointer __attribute__((mode(pointer)));
  struct __emutls_object
  {
    word size;
    word align;
    union {
      pointer offset;
      void *ptr;
    } loc;
    void *templ;
  };

  class RAII_TR
  {
  public:    
    inline RAII_TR( void *new_tr );
    inline ~RAII_TR();
        
  private:
    std::atomic<void *> const previous_tr;
  };

  byte *prepare_child_stack( byte *frame_end, byte *stack_pointer );
  void prepare_child_jmp_buf( jmp_buf &child_jmp_buf, const jmp_buf &initial_jmp_buf, byte *parent_stack_pointer, byte *child_stack_pointer );
  [[ noreturn ]] void child_main_function();
  void run_iteration();
  void jump_to_child();
  void yield_nonstatic();
  [[ noreturn ]] void jump_to_parent();
  static void *get_cls_address(void *obj) asm ("__emutls_get_address");
  
  const std::function<void()> child_function; 
  const int stack_size;
  byte * const child_stack_memory;
  ChildStatus child_status;
  jmp_buf parent_jmp_buf;
  jmp_buf child_jmp_buf;
    
  static int cls_heap_top;
  static byte *cls_foreground_heap;
    
  static const int default_stack_size = 2048;
};

///-- 
// Implement the inline functions here

Coroutine *Coroutine::me()
{
  return (Coroutine *)( Arm::get_tr() );
}


void Coroutine::yield()
{
  Coroutine * const me_value = me();
  if( me_value )
    me_value->yield_nonstatic();
}


Coroutine::RAII_TR::RAII_TR( void *new_tr ) :
  previous_tr( Arm::get_tr() )
{
  Arm::set_tr( new_tr );
}


Coroutine::RAII_TR::~RAII_TR()
{
  Arm::set_tr( previous_tr );
}

} // namespace

inline HC::Coroutine *me()
{
  return HC::Coroutine::me();
}

/** 
 * \example flashing.ino
 * \example hopping.ino
 * \example dimmer.ino
 */
 
#endif
