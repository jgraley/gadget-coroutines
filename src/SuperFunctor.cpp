

#include "SuperFunctor.h"
#include "CoroTracing.h"

using namespace std;

SuperFunctor::SuperFunctor() :
   entrypoint_mfp( &SuperFunctor::operator() )
{
    const pair<ThumbInstruction *, int> assembly = GetAssembly();
    ASSERT( assembly.second <= MAX_INSTRUCTIONS, "Thunk assmebly too long" );
    memcpy( entrypoint_thunk, assembly.first, assembly.second * sizeof(ThumbInstruction) );     
}


void SuperFunctor::SetEntrypoint( EntryPoint ep )
{
    // TODO deconstruct ep into a normal funciton pointer using "this",
    // since we declare the virtual operator()
    // See if we can just reference operator() directly (we're not in the 
    // constructor, so it might work) 
    entrypoint_mfp = ep;

    // TODO and then, for efficiency, modify the thunk code with a direct branch
}


void TestClient::operator()()
{
}


pair<SuperFunctor::ThumbInstruction *, int> SuperFunctor::GetAssembly()
{
    volatile bool actually_run_it = false;
    if( actually_run_it )
    {
        BEGIN:
        asm( "mov r0, pc\n\t" // Get PC into "this" pointer
             "sub r0, %[this_PC_offset]\n\t" // Correct for pipeline effect in PC
             "ldr r1, [r0, %[this_entry_offset]]\n\t" // Get function pointer after this code
             "bx  r1" // Jump to it (tail call)
              : : [this_PC_offset] "I" (offsetof(SuperFunctor, entrypoint_thunk)+4),
                  [this_entry_offset] "M" (offsetof(SuperFunctor, entrypoint_mfp)) : );            
        END:
            for(;;);
    }
    
    ThumbInstruction *begin = (ThumbInstruction *)(&&BEGIN);
    ThumbInstruction *end = (ThumbInstruction *)(&&END);
    
    return make_pair( begin, end-begin );
}

#ifdef SUPERFUNCTOR_TESTS  
void TestClient::TestPCRelative()
{
    uint8_t *pc_grab;
    asm( "mov %[result], pc" :  [result] "=r" (pc_grab)  : : );    
    TRACE("Grabbed PC: %p; function pointer:%p", pc_grab, &TestClient::TestPCRelative);
    /* What I learned:
     * - Grabbed PC is ahead of the mov instruction by 4 bytes (actually 2 
     *   instructions, I think, so only correct in thumb mode)
     * - Function pointer has bit 0 set to 1, indicating that it points
     *   to a thumb instruction (if bit 0 were 0, a Cortex M0 would fault)
     */
}


void TestClient::TestMemberFunctionPointer()
{
    void (TestClient::* volatile mfp)() = &TestClient::operator();
    TRACE("Size of member function pointer: %d", sizeof(mfp));
    TRACE("As uint32_t: %x %x", ((uint32_t *)&mfp)[0], ((uint32_t *)&mfp)[1] );
    TestClient s;
    (s.*mfp)();
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
#endif