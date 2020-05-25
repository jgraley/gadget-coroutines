/**
 * RAII_TR.h - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 * 
 * Set the TR (thread register) and allow it to be restored on exit
 */
#ifndef RAII_TR_h
#define RAII_TR_h

#include <atomic>

namespace GC
{

class RAII_TR
{
public:    
    RAII_TR( void *new_tr );
    ~RAII_TR();
    
private:
    std::atomic<void *> const previous_tr;
};

} // namespace


#endif