/*
  CoroTracing.cpp - Coroutines for Gadgets.
  Created by John Graley, 2020.
  (C) John Graley LGPL license applies.
*/

#ifndef CoroTracing_h
#define CoroTracing_h

#include <cstring>
#include <functional>
#include <cstdint>

void gcoroutines_set_logger( std::function< void(const char *) > logger );
void _gcoroutines_trace( const char *file, int line, const char *sformat, const char *uformat, ... );

#define TRACE( ARGS... ) do { _gcoroutines_trace( __FILE__, __LINE__, "%s:%d ", ARGS); } while(0)
#define FAIL( ARGS... ) do { _gcoroutines_trace( __FILE__, __LINE__, "Failed at %s:%d ", ARGS); abort(); } while(0)
#define ASSERT( COND, ARGS... ) do { if(!(COND)) FAIL(ARGS); } while(0)

#endif
