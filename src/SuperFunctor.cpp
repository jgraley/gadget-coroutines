

#include "SuperFunctor.h"
#include "Tracing.h"

using namespace std;
using namespace GC;

SuperFunctor::SuperFunctor() :
    entrypoint_fpt(nullptr)
{
    const pair<MachineInstruction *, int> assembly = GetAssembly();
    ASSERT( assembly.second <= SUPER_FUNCTOR_THUNK_ASSEMBLY_SIZE, "Thunk assemebly too long" );
    memcpy( entrypoint_thunk, assembly.first, assembly.second * sizeof(MachineInstruction) );     
    __builtin___clear_cache(entrypoint_thunk, entrypoint_thunk+sizeof(entrypoint_thunk)); // We've written some code into memory
    //TRACE("thunk at %p %x %x %x %x", entrypoint_thunk, entrypoint_thunk[0], entrypoint_thunk[1], entrypoint_thunk[2], entrypoint_thunk[3]);
}


SuperFunctor::operator EntryPointFP()
{
    if(!entrypoint_fpt)
    {
        // First fill in the FPT (Function Pointer with This arg) using 
        // hand-written version of the vcall algorithm. We do it here
        // because the object is not fully formed during our constructor.
        EntryPointMFP mfp = &SuperFunctor::operator();
        
        // alias an array of words over the member function pointer
        typedef array<uint32_t, sizeof(EntryPointMFP)/sizeof(uint32_t)> MFPWords;
        MFPWords &mfp_words = *(MFPWords *)&mfp;
        ASSERT( mfp_words.size() == member_function_pointer_size/4, "Unexpected size of member function pointer");    
        
        auto this_words = (uint32_t *)this;
        entrypoint_fpt = (EntryPointFPT)get_vcall_destination( mfp_words.data(), this_words );
    }
    
    // Convert the address of the thunk to a function pointer
    return (EntryPointFP)ptr_to_function_ptr(entrypoint_thunk);
}


pair<MachineInstruction *, int> SuperFunctor::GetAssembly()
{
    const int pipeline_overshoot = pipeline_overshoot_instructions * sizeof(MachineInstruction);

    volatile bool actually_run_it = false;
    if( actually_run_it )
    {
        BEGIN:
        asm( SUPER_FUNCTOR_THUNK_ASSEMBLY
              : : [this_to_PC_offset] "I" (offsetof(SuperFunctor, entrypoint_thunk) + pipeline_overshoot),
                  [this_to_entrypoint_offset] "M" (offsetof(SuperFunctor, entrypoint_fpt)) : );            
        END:
            for(;;);
    }
    
    auto begin = (MachineInstruction *)(&&BEGIN);
    auto end = (MachineInstruction *)(&&END);
    
    return make_pair( begin, end-begin );
}


#ifdef SUPERFUNCTOR_TESTS  
void SuperFunctor::TestPCRelative()
{
    uint8_t *pc_grab;
    asm( "mov %[result], pc" :  [result] "=r" (pc_grab)  : : );    
    TRACE("Grabbed PC: %p; function pointer:%p", pc_grab, &SuperFunctor::TestPCRelative);
    /* What I learned:
     * - Grabbed PC is ahead of the mov instruction by 4 bytes (actually 2 
     *   instructions, I think, so only correct in thumb mode)
     * - Function pointer has bit 0 set to 1, indicating that it points
     *   to a thumb instruction (if bit 0 were 0, a Cortex M0 would fault)
     */
}


void SuperFunctor::TestMemberFunctionPointer()
{
    void (SuperFunctor::* volatile mfp)() = &SuperFunctor::operator();
    TRACE("Size of member function pointer: %d", sizeof(mfp));
    TRACE("As uint32_t: %x %x", ((uint32_t *)&mfp)[0], ((uint32_t *)&mfp)[1] );
    SuperFunctor s;
    //(s.*mfp)();
    TRACE("Thises: %p %p", &s, (SuperFunctor *)&s );
    TRACE("entrypoint_thunk: %p", &(s.entrypoint_thunk) );

    /* What I learned:
     * - Size of MFP is 8 bytes: Updated:
     * If non-virtual: { FP, 0 }
     * If virtual: { vtable-offset, (object-offset<<1) | 1 }
     * 
     * To invoke function given { P, Q }:
     * if( Q.0 == 0 )
     *   call P
     * else
     *   call *( *(object+(Q>>1)) + P )
     */   
}


void SuperFunctor::TestUnpack()
{
    SuperFunctor s;
    EntryPointMFP mfp = &SuperFunctor::operator();
    EntryPointFPT fpt = s.UnpackMemberFunctionPointer( mfp ); 
    TRACE("Thises: %p %p", &s, (SuperFunctor *)&s );
    TRACE("operator() as uint32_t: %x %x", ((uint32_t *)&mfp)[0], ((uint32_t *)&mfp)[1] );
    TRACE("FPT as uint32_t: %x", ((uint32_t *)&fpt)[0] );
}
#endif
