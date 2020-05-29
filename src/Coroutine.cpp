/**
 * @file Coroutine.cpp
 * ### `hopping-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 */

#include "Coroutine.h"

#include "Coroutine_arm.h"
#include "Tracing.h"
#include "Integration.h"

#include <cstring>
#include <functional>
#include <csetjmp> 
#include <cstdint>
#include "Arduino.h"

using namespace std;
using namespace GC;
using namespace Arm;

// Only enable when constructing after system initialisation, eg in setup()
#define CONSTRUCTOR_TRACE DISABLED_TRACE

Coroutine::Coroutine( function<void()> child_function_ ) :
  child_function( child_function_ ),
  stack_size( default_stack_size ),
  child_stack_memory( (byte *)calloc(default_stack_size, 1) ),
  child_status( READY )
{    
  ASSERT(child_function, "NULL child function was supplied");
  jmp_buf initial_jmp_buf;
  int val;
  CONSTRUCTOR_TRACE("this=%p sp=%p", this, get_sp());
  CONSTRUCTOR_TRACE("last arg begins %p ends %p", &child_function_, &child_function_ + 1);
  CONSTRUCTOR_TRACE("first local begins %p ends %p", &initial_jmp_buf, &initial_jmp_buf + 1);
  switch( val = setjmp(initial_jmp_buf) ) { 
    case IMMEDIATE: {
      // Get current stack pointer and frame address 
      // taking care that it will have a different stack frame
      byte *frame_end = (byte *)(&child_function_ + 1);
      byte *stack_pointer = (byte *)( get_jmp_buf_sp(initial_jmp_buf) );  
      
      CONSTRUCTOR_TRACE("this=%p sp=%p, stack_pointer=%p, frame_end=%p", this, get_sp(), stack_pointer, frame_end);

      // Get the child's stack ready                   
      byte *child_stack_pointer = prepare_child_stack( frame_end, stack_pointer );
      
      // Create the child's jump buf
      prepare_child_jmp_buf( child_jmp_buf, initial_jmp_buf, stack_pointer, child_stack_pointer );
      break;
    }
    case PARENT_TO_CHILD_STARTING: {
      CONSTRUCTOR_TRACE("this=%p me=%p sp=%p", this, me(), get_sp());
      child_main_function();            
    }
    default: {
      // This setjmp call was only to get the stack pointer. 
      ERROR("unexpected longjmp value: %d", val);
    }
  }
}


Coroutine::~Coroutine()
{
  check_valid_this();
  ASSERT( child_status == COMPLETE, "destruct when child was not complete, status %d", (int)child_status );
  delete[] child_stack_memory;
  bring_in_Integration();
}


void Coroutine::wait( std::function<bool()> test )
{
    do
    {
        yield();
    }
    while( test()==false );
}


void Coroutine::set_hop_lambda( std::function<void()> hop )
{
  Task::set_hop_lambda( [=]
  {
    RAII_TR tr(this);
    hop();
  } );
}


pair<const byte *, const byte *> Coroutine::get_child_stack_bounds()
{
    return make_pair(child_stack_memory, child_stack_memory+stack_size);
}


int Coroutine::estimate_stack_peak_usage()
{
    byte *p = child_stack_memory + cls_heap_top;
    while( *p==0 && p < child_stack_memory+stack_size )
      p++;
    return child_stack_memory+stack_size-p;      
}


int Coroutine::get_cls_usage()
{
    return cls_heap_top;
}


byte *Coroutine::prepare_child_stack( byte *frame_end, byte *stack_pointer )
{
  // Decide how much stack to keep (basically the current frame, i.e. the 
  // stack frame of this invocation of this function) and copy it into the 
  // new stack, at the bottom.
  // Note: stacks usually begin at the highest address and work down
  int bytes_to_retain = frame_end - stack_pointer;
  byte *child_stack_pointer = child_stack_memory + stack_size - bytes_to_retain;      
  CONSTRUCTOR_TRACE("moving %d from %p to %p", bytes_to_retain, stack_pointer, child_stack_pointer );
  memmove( child_stack_pointer, stack_pointer, bytes_to_retain );
  return child_stack_pointer;
}


void Coroutine::prepare_child_jmp_buf( jmp_buf &child_jmp_buf, const jmp_buf &initial_jmp_buf, byte *parent_stack_pointer, byte *child_stack_pointer )
{
  // Prepare a jump buffer for the child and point it to the new stack
  CONSTRUCTOR_TRACE("initial jmp_buf has tr=%08x sl=%08x fp=%08x sp=%08x lr=%08x", 
      initial_jmp_buf[5], initial_jmp_buf[6], initial_jmp_buf[7], 
      initial_jmp_buf[8], initial_jmp_buf[9] );
  byte *parent_frame_pointer = (byte *)(get_jmp_buf_fp(initial_jmp_buf));
  byte *child_frame_pointer = parent_frame_pointer + (child_stack_pointer-parent_stack_pointer);;
  copy_jmp_buf( child_jmp_buf, initial_jmp_buf );
  set_jmp_buf_sp( child_jmp_buf, child_stack_pointer);
  set_jmp_buf_fp( child_jmp_buf, child_frame_pointer);
  set_jmp_buf_tr( child_jmp_buf, this);
}


[[ noreturn ]] void Coroutine::child_main_function()
{
  check_valid_this();
  child_status = RUNNING;
    
  // Invoke the child. We take the view that this is enough to give
  // it its first "timeslice"
  child_function();
    
  // If we get here, child returned without yielding (i.e. like a normal function).
  child_status = COMPLETE;
  
  // Let the parent run
  jump_to_parent();
}


void Coroutine::run_iteration()
{
  check_valid_this();
  
  // Save the current next parent jump buffer
  int val;
  switch( val = setjmp(parent_jmp_buf) ) {                    
    case IMMEDIATE: {
      jump_to_child();
    }
       
    case CHILD_TO_PARENT: {
      break;
    }
                    
    default: {
      ERROR("unexpected longjmp value: %d", val);
    }
  }  
}
        
        
void Coroutine::jump_to_child()
{
  switch( child_status ) {
    case READY: {
      longjmp(child_jmp_buf, PARENT_TO_CHILD_STARTING);
      // No break required: longjump does not return
    }
    case RUNNING: {
      longjmp(child_jmp_buf, PARENT_TO_CHILD);
      // No break required: longjump does not return
    }
    case COMPLETE: {
      // All finished, nothing to do except return
    }
  }   
}


void Coroutine::yield_nonstatic()
{
  check_valid_this();
  ASSERT( child_status == RUNNING, "yield when child was not running, status %d", (int)child_status );
  
  int val;
  switch( val = setjmp( child_jmp_buf ) ) {                    
    case IMMEDIATE: {
      jump_to_parent();
    }
    case PARENT_TO_CHILD: {
      // If the child has ever yielded, its context will come back to here
      break; 
    }    
    default: {
      ERROR("unexpected longjmp value: %d", val);
    }
  }    
}


void Coroutine::jump_to_parent()
{
  longjmp( parent_jmp_buf, CHILD_TO_PARENT );
}


extern void *__HeapLimit;
void *Coroutine::get_cls_address(void *obj)
{
    __emutls_object * const euo = (__emutls_object *)obj;   
    const Coroutine *me = ::me();
    if( euo->loc.offset==0 )
    {
      // The first time a TLS item is accessed, regardless of context, we
      // give it an offset.  
      if( cls_heap_top==0 )
        cls_heap_top++; // If we put 0 into euo.loc.offset, it will be indistinguishable from nullptr
      euo->loc.offset = (cls_heap_top + euo->align - 1) & ~(euo->align - 1);
      cls_heap_top = euo->loc.offset + euo->size;
    }
    
    byte *cls_heap;
    if( me )
    {
      // TLS data accessed in a coroutine  
      cls_heap = me->child_stack_memory;
    }
    else
    {
      // TLS data accessed outside of any coroutine
      // The first time this happens, we'll have to allocate a block of memory
      if( !cls_foreground_heap )
        cls_foreground_heap = (byte *)calloc(default_stack_size, 1);
      cls_heap = cls_foreground_heap;
    }
    return cls_heap + euo->loc.offset;
}


int Coroutine::cls_heap_top = 0;
byte *Coroutine::cls_foreground_heap = 0;


// This makes sure the TR is a NULL pointer for the foreground
// context (i.e. when outside any coroutine)
void __attribute__ ((constructor)) init_baseline_tr()
{
  set_tr( nullptr );
}
