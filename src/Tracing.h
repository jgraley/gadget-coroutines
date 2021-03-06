/**
 * @file Tracing.h
 * ### `hopping-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 *
 * @brief Tracing macros
 */

#ifndef Tracing_h
#define Tracing_h

#include <cstring>
#include <functional>
#include <cstdint>

extern void _gcoroutines_log(const char *message);
extern void gcoroutines_set_logger( std::function< void(const char *) > logger );
extern void _gcoroutines_trace( const char *file, int line, const char *sformat, const char *uformat, ... );

#define HC_TRACE( ARGS... ) do { _gcoroutines_trace( __FILE__, __LINE__, "%s:%d ", ARGS); } while(0)
#define HC_ERROR( ARGS... ) do { _gcoroutines_trace( __FILE__, __LINE__, "HC_ERROR %s:%d ", ARGS); abort(); } while(0)
#define HC_ASSERT( COND, ARGS... ) do { if(!(COND)) HC_ERROR(ARGS); } while(0)
#define HC_DISABLED_TRACE( ARGS... ) do {} while(0)

#endif
