/**
 * SubSketch.h 
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 * 
 * Support for running a whole sketch inside another
 */
#ifndef SubSketch_h
#define SubSketch_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

#include "Coroutine.h"

#define GC_SUB_SKETCH_TASK_IMPL(NAMESPACE, CLASSEXT) \
class NAMESPACE##CLASSEXT : public GC::Coroutine \
{ \
public:  \
  NAMESPACE##CLASSEXT() : \
    Coroutine([] \
    { \
      NAMESPACE::setup(); \
      while(1) \
        NAMESPACE::loop(); \
    }) \
    { \
    } \
}

/** 
 * This macro expands to a class type, that will run the sub-sketch 
 * located in the specified namespace. Use like eg
 * 
 * `GC_SUB_SKETCH_TASK(my_namespace) my_task;`
 * 
 * You can then iterate the sub-sketch in the usual way eg via my_task().
 */
#define GC_SUB_SKETCH_TASK(NAMESPACE) \
GC_SUB_SKETCH_TASK_IMPL(NAMESPACE, _class_)


#endif
