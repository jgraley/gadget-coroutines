/**
 * @file SubSketch.h
 * ### `hopping-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 * 
 * @brief Support for running a whole sketch inside another
 */
#ifndef SubSketch_h
#define SubSketch_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

#include "Coroutine.h"

#define HC_SUB_SKETCH_TASK_IMPL(NAMESPACE, CLASSEXT) \
class NAMESPACE##CLASSEXT : public HC::Coroutine \
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
 * `HC_SUB_SKETCH_TASK(my_namespace) my_task;`
 * 
 * You can then invoke the sub-sketch in the usual way eg via my_task().
 */
#define HC_SUB_SKETCH_TASK(NAMESPACE) \
HC_SUB_SKETCH_TASK_IMPL(NAMESPACE, _class_)


#endif
