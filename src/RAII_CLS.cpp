/**
 * RAII_CLS.cc - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 * 
 * Set the CLS and allow it to be restored on exit
 */

#include "RAII_CLS.h"

#include "Coroutine_arm.h"

using namespace std;
using namespace GC;
using namespace Arm;

RAII_CLS::RAII_CLS( void *new_cls ) :
  previous_cls( get_tr() )
{
  set_tr( new_cls );
}


RAII_CLS::~RAII_CLS()
{
  set_tr( previous_cls );
}
