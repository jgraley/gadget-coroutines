/**
 * Tracing.h - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 */

#include "Tracing.h"

#include "Coroutine_arm.h"

#include <cstring>
#include <functional>
#include <cstdint>
#include "Arduino.h"

using namespace std;


void _gcoroutines_log(const char *message)
{
  void *cls = get_cls();
  set_cls(nullptr);
  Serial.println(message); 
  delay(100);
  set_cls(cls);
}

function< void(const char *) >  _gcoroutines_logger = _gcoroutines_log;


void gcoroutines_set_logger( function< void(const char *) > logger )
{
  _gcoroutines_logger = logger;
}


void _gcoroutines_trace( const char *file, int line, const char *sformat, const char *uformat, ... )
{
  va_list args;
  va_start( args, uformat );
  char message[256];
  const char *file_separator = strrchr( file, '/' );
  snprintf( message, sizeof(message), sformat, file_separator ? file_separator+1 : file, line );
  int l = strlen(message);
  vsnprintf( message+l, sizeof(message)-l, uformat, args );
  message[sizeof(message)-1] = '\0';
  _gcoroutines_logger(message);
  va_end( args );
}
