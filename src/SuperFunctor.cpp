

#include "SuperFunctor.h"
#include "CoroTracing.h"

SuperFunctor::SuperFunctor()
{
}


void SuperFunctor::operator()()
{
}


void SuperFunctor::ReferenceAssmebly()
{
    asm( "mov r0, pc\n\t"
         "ldr r1, [r0, #8]\n\t"
         "bx  r1"
          : : : );            
}


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
    (s.*mfp)();
    /* What I learned:
     * - Size of MFP is 8 bytes, made up of regular function pointer 
     *   followed by 0. From the assembly, I suspect undefined. Again,
     *   bit 0 of the FP has been set to 1.
     * - MFP invoked using blx instruction directly on the FP part of 
     *   the MFP
     */
    
}
