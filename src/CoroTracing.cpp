/*
  Coroutines.h - Coroutines for Gadgets.
  Created by John Graley, 2020.
  (C) John Graley LGPL license applies.
*/

#include "CoroTracing.h"

#include <cstring>
#include <functional>
#include <cstdint>
#include "Arduino.h"

using namespace std;

function< void(const char *) >  _gcoroutines_logger = [](const char *message)
{
  Serial.println(message); 
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
