/**
 * Coroutines.cpp - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 */

#include "Coroutines.h"

#include "CoroTracing.h"

#include <cstring>
#include <functional>
#include <csetjmp> 
#include <cstdint>
#include "Arduino.h"

#define TEST_REENTRANCY

using namespace std;


Coroutine::Coroutine( function<void()> child_function_ ) :
  magic( MAGIC ),
  child_function( child_function_ ),
  stack_size( default_stack_size ),
  child_stack_memory( new byte[stack_size] ),
  child_status(READY)
{    
  jmp_buf initial_jmp_buf;
  int val;
  switch( val = setjmp(initial_jmp_buf) ) { 
    case IMMEDIATE: {
      // Get current stack pointer and frame address 
      // taking care that it will have a different stack frame
      byte *frame_pointer = (byte *)( get_frame_address() );
      byte *stack_pointer = (byte *)( get_jmp_buf_sp(initial_jmp_buf) );  
      
      // Get the child's stack ready                   
      byte *child_stack_pointer = prepare_child_stack( frame_pointer, stack_pointer );
      
      // Create the child's jump buf
      prepare_child_jmp_buf( initial_jmp_buf, child_stack_pointer );
      break;
    }
    case PARENT_TO_CHILD_STARTING: {
      // Warning: no this pointer
      Coroutine * const that = get_current();
      ASSERT( that, "still in baseline" );
      that->child_main_function();            
    }
    default: {
      // This setjmp call was only to get the stack pointer. 
      FAIL("unexpected longjmp value: %d", val);
    }
  }
}


Coroutine::~Coroutine()
{
  ASSERT( magic == MAGIC, "bad this pointer or object corrupted: %p", this );
  ASSERT( child_status == COMPLETE, "destruct when child was not complete, status %d", (int)child_status );
  delete[] child_stack_memory;
}


byte *Coroutine::prepare_child_stack( byte *frame_pointer, byte *stack_pointer )
{
  // Decide how much stack to keep (basically the current frame, i.e. the 
  // stack frame of this invocation of this function) and copy it into the 
  // new stack, at the bottom.
  // Note: stacks usually begin at the highest address and work down
  int bytes_to_retain = frame_pointer - stack_pointer;
  byte *child_stack_pointer = child_stack_memory + stack_size - bytes_to_retain;      
  memmove( child_stack_pointer, stack_pointer, bytes_to_retain );
  return child_stack_pointer;
}


void Coroutine::prepare_child_jmp_buf( const jmp_buf &initial_jmp_buf, byte *child_stack_pointer )
{
  // Prepare a jump buffer for the child and point it to the new stack
  copy_jmp_buf( child_jmp_buf, initial_jmp_buf );
  set_jmp_buf_sp(child_jmp_buf, child_stack_pointer);
  set_jmp_buf_cls(child_jmp_buf, this);
}


[[ noreturn ]] void Coroutine::child_main_function()
{
  ASSERT( magic == MAGIC, "bad this pointer or object corrupted: %p", this ); 
  child_status = RUNNING;
    
  // Invoke the child. We take the view that this is enough to give
  // it its first "timeslice"
  child_function();
    
  // If we get here, child returned without yielding (i.e. like a normal function).
  child_status = COMPLETE;
  
  // Let the parent run
  jump_to_parent();
}


void Coroutine::operator()()
{
  ASSERT( magic == MAGIC, "bad this pointer or object corrupted: %p", this );

  // Save the current next parent jump buffer
  jmp_buf_ptr saved_next_parent_jmp_buf = next_parent_jmp_buf;
  
  // Run the coroutine
  run_iteration();
  
  // Restore parent jmp buf pointer. This is a "re-enterer saves" model
  // - the preentpting operator() makes sure it leaves that pointer as 
  // it was before it started. Thus the re-entered invocation resumes
  // as if nothing happend (though the child state machine may have 
  // advanced).
  next_parent_jmp_buf = saved_next_parent_jmp_buf;
}
  
  
void Coroutine::run_iteration()
{
  jmp_buf parent_jmp_buf;
  int val;
  switch( val = setjmp(parent_jmp_buf) ) {                    
    case IMMEDIATE: {
      next_parent_jmp_buf = parent_jmp_buf;
      jump_to_child();
    }
       
    case CHILD_TO_PARENT: {
      // Warning: no this pointer #18
      return; 
    }
                    
    default: {
      FAIL("unexpected longjmp value: %d", val);
    }
  }
}        
        
        
void Coroutine::jump_to_child()
{
  // This is where we cease to be reentrant because we're starting to 
  // access member variables relating to child state
  
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
  ASSERT( magic == MAGIC, "bad this pointer or object corrupted: %p", this );
  ASSERT( child_status == RUNNING, "yield when child was not running, status %d", (int)child_status );
  
  int val;
  switch( val = setjmp( child_jmp_buf ) ) {                    
    case IMMEDIATE: {
      jump_to_parent();
    }
    case PARENT_TO_CHILD: {
      // If the child has ever yielded, its context will come back to here
      // Warning: no this pointer #18
      return; 
    }    
    default: {
      FAIL("unexpected longjmp value: %d", val);
    }
  }    
}


void Coroutine::jump_to_parent()
{
  // From here on, I believe the we can be re-entered via run_iteration().
  // The correct run_iteration calls will return in the correct order because
  // we are stacking the parent jmp bufs. 
  // From child's POV, each more-nested reentry will just be a successive 
  // iteration (because child jump buf and status are just the member ones
  // Note: we're reentrant into run_iteration() but not recursive, since
  // this is the child's context.
  
  longjmp( next_parent_jmp_buf, CHILD_TO_PARENT );
}


constexpr uint32_t make_magic_le(const char *str)
{
  return str[0] | (str[1]<<8) | (str[2]<<16) | (str[3]<<24);
}


const uint32_t Coroutine::MAGIC = make_magic_le("GCo1");

