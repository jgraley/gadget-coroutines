/**
 * Coroutine.cpp - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 */

#include "Coroutine.h"

#include "Coroutine_arm.h"
#include "CoroTracing.h"
#include "CoroIntegration.h"
#include "RAII_CLS.h"

#include <cstring>
#include <functional>
#include <csetjmp> 
#include <cstdint>
#include "Arduino.h"

using namespace std;

// Only enable when constructing after system initialisation, eg in setup()
#define CONSTRUCTOR_TRACE DISABLED_TRACE

Coroutine::Coroutine( function<void()> child_function_ ) :
  child_function( child_function_ ),
  stack_size( default_stack_size ),
  child_stack_memory( new byte[stack_size] ),
  child_status(READY)
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
  bring_in_CoroIntegration();
}


pair<const byte *, const byte *> Coroutine::get_child_stack_bounds()
{
    return make_pair(child_stack_memory, child_stack_memory+stack_size);
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
  CONSTRUCTOR_TRACE("initial jmp_buf has cls=%08x sl=%08x fp=%08x sp=%08x lr=%08x", 
      initial_jmp_buf[5], initial_jmp_buf[6], initial_jmp_buf[7], 
      initial_jmp_buf[8], initial_jmp_buf[9] );
  byte *parent_frame_pointer = (byte *)(get_jmp_buf_fp(initial_jmp_buf));
  byte *child_frame_pointer = parent_frame_pointer + (child_stack_pointer-parent_stack_pointer);;
  copy_jmp_buf( child_jmp_buf, initial_jmp_buf );
  set_jmp_buf_sp( child_jmp_buf, child_stack_pointer);
  set_jmp_buf_fp( child_jmp_buf, child_frame_pointer);
  set_jmp_buf_cls( child_jmp_buf, this);
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


void Coroutine::yield_nonstatic( function<void()> hop )
{
  check_valid_this();
  ASSERT( child_status == RUNNING, "yield when child was not running, status %d", (int)child_status );
  
  if( hop )
  {
    set_hop_lambda( [=]
    {
        RAII_CLS raii_cls(this);
        hop();
    } );
  }

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


