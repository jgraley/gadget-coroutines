/*
  GCoroutines.h - Coroutines for Gadgets.
  Created by John Graley, 2020.
  (C) John Graley LGPL license applies.
*/

#include "GCoroutines.h"
#include <cstring>
#include <functional>
#include <csetjmp> 
#include <cstdint>

// TODO integrate with wiringpi yeild()
// TODO wait() on lambda with timeout object
// TODO micros() in log
// TODO tabs in log based on cls
// TODO GCoroutine(s) -> Coroutine(s)

using namespace std;

function< void(const char *) >  _gcoroutines_logger = [](const char *message)
{
    Serial.println(message); 
    delay(100);
};

void gcoroutines_set_logger( function< void(const char *) > logger )
{
    _gcoroutines_logger = logger;
}

void _gcoroutines_trace( const char *file, int line, const char *sformat, const char *uformat, ... )
{
    va_list args;
    va_start( args, uformat );
    char message[256];
    snprintf( message, sizeof(message), sformat, file, line );
    int l = strlen(message);
    vsnprintf( message+l, sizeof(message)-l, uformat, args );
    message[sizeof(message)-1] = '\0';
    _gcoroutines_logger(message);
    va_end( args );
}

#define TRACE( ARGS... ) do { _gcoroutines_trace( __FILE__, __LINE__, "%s:%d ", ARGS); } while(0)
#define FAIL( ARGS... ) do { _gcoroutines_trace( __FILE__, __LINE__, "Failed at %s:%d ", ARGS); abort(); } while(0)
#define ASSERT( COND, ARGS... ) do { if(!(COND)) FAIL(ARGS); } while(0)

constexpr uint32_t make_magic_le(const char *str)
{
    return str[0] | (str[1]<<8) | (str[2]<<16) | (str[3]<<24);
}

#define LE_CHR(a,b,c,d) (  )
static const uint32_t GCO_MAGIC = make_magic_le("GCo1");


#if defined(__arm__) || defined(__thumb__)

// We're assuming pointers and ints are the same size because the old
// C library functions we use seem to assume it.
static_assert( sizeof(int) == sizeof(void *) );

// Make sure the setjmp.h is the one we expect
static_assert( sizeof(jmp_buf) == sizeof(int[23]) );

static const int ARM_JMPBUF_INDEX_SP = 8;
inline void *get_jmp_buf_sp( jmp_buf env )
{
    return reinterpret_cast<byte *>( env[ARM_JMPBUF_INDEX_SP] );
}
inline void set_jmp_buf_sp( jmp_buf env, void *new_sp )
{
    env[ARM_JMPBUF_INDEX_SP] = reinterpret_cast<int>(new_sp);
}
inline void *get_frame_address()
{
    return reinterpret_cast<void *>( __builtin_frame_address(0) );
}
static const int ARM_JMPBUF_INDEX_TLS = 5;
inline void *get_jmp_buf_cls( jmp_buf env )
{
    return reinterpret_cast<byte *>( env[ARM_JMPBUF_INDEX_TLS] );
}
inline void set_jmp_buf_cls( jmp_buf env, void *new_cls )
{
    env[ARM_JMPBUF_INDEX_TLS] = reinterpret_cast<int>(new_cls);
}
inline void *get_cls()
{
    void *cls;
    asm( "mov %[result], r9" : [result] "=r" (cls) : : );
    return cls;
}
#endif

enum
{
    IMMEDIATE = 0, // Must be zero
    PARENT_TO_CHILD = 1, // All the others must be non-zero
    CHILD_TO_PARENT = 2,
    PARENT_TO_CHILD_STARTING = 3
};


GCoroutine::GCoroutine( void (*child_main_function_)() ) :
    magic( GCO_MAGIC ),
    child_main_function( child_main_function_ ),
    stack_size( default_stack_size ),
    child_stack_memory( new byte[stack_size] ),
    child_status(READY)
{
    byte *frame_pointer = static_cast<byte *>( get_frame_address() );
    jmp_buf initial_jmp_buf;
    int val;
    switch( val = setjmp(initial_jmp_buf) )
    { 
        case IMMEDIATE:
        {
            // Get current stack pointer and frame address @TODO bring this bit out into its own function
            // taking care that it will have a different stack frame
            byte *stack_pointer = static_cast<byte *>( get_jmp_buf_sp(initial_jmp_buf) );            
            
            // Decide how much stack to keep (basically the current frame, i.e. the 
            // stack frame of this invocation of this function) and copy it into the 
            // new stack, at the bottom.
            // Note: stacks usually begin at the highest address and work down
            int bytes_to_retain = frame_pointer - stack_pointer;
            byte *child_stack_pointer = child_stack_memory + stack_size - bytes_to_retain;      
            memmove( child_stack_pointer, stack_pointer, bytes_to_retain );
            
            // Prepare a jump buffer for the child and point it to the new stack
            memcpy( &child_jmp_buf, &initial_jmp_buf, sizeof(jmp_buf) );
            set_jmp_buf_sp(child_jmp_buf, child_stack_pointer);
            set_jmp_buf_cls(child_jmp_buf, this);
            break;
        }
        case PARENT_TO_CHILD_STARTING:
        {
            // Warning: no this pointer
            GCoroutine * const that = static_cast<GCoroutine *>(get_cls());
            that->start_child();            
        }
        default:
        {
            // This setjmp call was only to get the stack pointer. 
            FAIL("unexpected longjmp value: %d", val);
        }
    }
}

GCoroutine::~GCoroutine()
{
    ASSERT( child_status == COMPLETE, "destruct when child was not complete, status %d", static_cast<int>(child_status) );
    delete[] child_stack_memory;
}

        
[[ noreturn ]] void GCoroutine::start_child()
{
    ASSERT( magic==GCO_MAGIC, "bad this pointer or object corrupted: %p", this ); // @TODO many more of these checks
    child_status = RUNNING;
    
    // Invoke the child. We take the view that this is enough to give
    // it its first "timeslice"
    (*child_main_function)();
    
    // If we get here, child returned without yielding (i.e. like a normal function).
    child_status = COMPLETE;
    longjmp(child_jmp_buf, CHILD_TO_PARENT);
    // No break required: longjump does not return
}


void GCoroutine::run_iteration()
{
    int val;
    switch( val = setjmp(parent_jmp_buf) )
    {                    
        case IMMEDIATE:
        {
            switch( child_status )
            {
                case READY:
                {
                    longjmp(child_jmp_buf, PARENT_TO_CHILD_STARTING);
                    // No break required: longjump does not return
                }
                case RUNNING:
                {
                    longjmp(child_jmp_buf, PARENT_TO_CHILD);
                    // No break required: longjump does not return
                }
                case COMPLETE:
                {
                    return; // TODO get the stored completed message
                }
            }   
        }
        
        case CHILD_TO_PARENT:
        {
            // Warning: no this pointer
            return; // TODO child's stored waiting or completed message
        }
                    
        default:
        {
            FAIL("unexpected longjmp value: %d", val);
        }
    }
}        
        
        
void GCoroutine::yield()
{
    GCoroutine * const that = static_cast<GCoroutine *>(get_cls());
    // @TODO check we're in the correct stack. If not then (a) we're the
    // wrong child or (b) we overflowed or underflowed. Using r9 to track 
    // current child could prevent (a) and guard/fence zones could detect (b)
    // Think on...
    int val;
    switch( val = setjmp( that->child_jmp_buf ) )
    {                    
        case IMMEDIATE:
        {
            // Run the main routine
            longjmp( that->parent_jmp_buf, CHILD_TO_PARENT );
            // No break required: longjump does not return
        }
        case PARENT_TO_CHILD:
        {
            // If the child has ever yielded, its context will come back to here
            // Warning: no this pointer
            return; 
        }    
        default:
        {
            FAIL("unexpected longjmp value: %d", val);
        }
    }    
}
