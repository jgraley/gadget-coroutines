/**
 * RAII_CLS.h - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 * 
 * Set the CLS and allow it to be restored on exit
 */
#ifndef RAII_CLS_h
#define RAII_CLS_h

#include <atomic>

class RAII_CLS
{
public:    
    RAII_CLS( void *new_cls );
    ~RAII_CLS();
    
private:
    std::atomic<void *> const previous_cls;
};

#endif