/**
 * RAII_TR.cc - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 */

#include "RAII_TR.h"

#include "Coroutine_arm.h"

using namespace std;
using namespace GC;
using namespace Arm;

RAII_TR::RAII_TR( void *new_tr ) :
  previous_tr( get_tr() )
{
  set_tr( new_tr );
}


RAII_TR::~RAII_TR()
{
  set_tr( previous_tr );
}
