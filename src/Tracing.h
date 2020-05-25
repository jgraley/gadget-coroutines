/**
 * Tracing.h
 * gadget-coroutines
 * Stacked coroutines for the Arduino environment.
 * (C) 2020 John Graley; BSD license applies.
 *
 * Tracing macros
 */

#ifndef Tracing_h
#define Tracing_h

#include <cstring>
#include <functional>
#include <cstdint>

extern void _gcoroutines_log(const char *message);
extern void gcoroutines_set_logger( std::function< void(const char *) > logger );
extern void _gcoroutines_trace( const char *file, int line, const char *sformat, const char *uformat, ... );

#define TRACE( ARGS... ) do { _gcoroutines_trace( __FILE__, __LINE__, "%s:%d ", ARGS); } while(0)
#define ERROR( ARGS... ) do { _gcoroutines_trace( __FILE__, __LINE__, "ERROR %s:%d ", ARGS); abort(); } while(0)
#define ASSERT( COND, ARGS... ) do { if(!(COND)) ERROR(ARGS); } while(0)
#define DISABLED_TRACE( ARGS... ) do {} while(0)

#endif
