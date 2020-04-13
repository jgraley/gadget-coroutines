#ifndef SUPERFUNCTOR_H
#define SUPERFUNCTOR_H

#include <utility>
#include <cstddef>
#include <cstdint>

/*#include <cstdlib>
#include <algorithm>
*/
#define MAX_INSTRUCTIONS 4
#define SUPERFUNCTOR_TESTS

class SuperFunctor
{
public:
  SuperFunctor();
  virtual ~SuperFunctor();
  typedef void (*EntryPointFP)();
  EntryPointFP GetEntryPoint();

protected:  
  virtual void operator()();    

public:     // TODO private
  typedef void (SuperFunctor::* EntryPointMFP)();
  typedef void (*EntryPointFPT)( SuperFunctor *this_ );
  typedef uint16_t ThumbInstruction;
  
  static std::pair<ThumbInstruction *, int> GetAssembly();
  EntryPointFPT UnpackMemberFunctionPointer( EntryPointMFP mfp );

  ThumbInstruction entrypoint_thunk[MAX_INSTRUCTIONS];
  EntryPointFPT entrypoint_fpt; 

#ifdef SUPERFUNCTOR_TESTS  
public:
  static void TestPCRelative();
  static void TestMemberFunctionPointer(); 
  static void TestUnpack(); 
#endif
};

/*
#include "sam.h"

extern DeviceVectors exception_table;

class RWVectorTable
{
public:
    RWVectorTable() :
        aligned_size( GetAlignedSize() ),
        vectors( (DeviceVectors *)memalign( aligned_size, aligned_size ) ),
        previous_vectors( (DeviceVectors *)(SCB->VTOR & SCB_VTOR_TBLOFF_Msk) )
    {
        *vectors = exception_table;

        static_assert( __VTOR_PRESENT );
        SCB->VTOR = ((uint32_t) vectors & SCB_VTOR_TBLOFF_Msk);
    }
    
    ~RWVectorTable()
    {
        SCB->VTOR = ((uint32_t) previous_vectors & SCB_VTOR_TBLOFF_Msk);
        free( vectors );
    }
    
    DeviceVectors &operator()()
    {
        return *vectors;        
    }
    
private:
    static unsigned int GetAlignedSize()
    {
        // http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Ciheijba.html
        unsigned int sz = sizeof(DeviceVectors);
        int highest_bit = 31 - __builtin_clz(sz);
        unsigned int aligned_size = 1U << highest_bit;
        if( aligned_size < sz )
            aligned_size = aligned_size << 1;
        aligned_size = std::max( aligned_size, 32*sizeof(uint32_t) );
        return aligned_size;
    }
    const unsigned int aligned_size;
    DeviceVectors * const vectors;
    const DeviceVectors * const previous_vectors;
};
*/
#endif