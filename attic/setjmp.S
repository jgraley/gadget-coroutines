00004868 <setjmp>:
    4868:       c0f0            stmia   r0!, {r4, r5, r6, r7}
    486a:       4641            mov     r1, r8
    486c:       464a            mov     r2, r9
    486e:       4653            mov     r3, sl
    4870:       465c            mov     r4, fp
    4872:       466d            mov     r5, sp
    4874:       4676            mov     r6, lr
    4876:       c07e            stmia   r0!, {r1, r2, r3, r4, r5, r6}
    4878:       3828            subs    r0, #40 ; 0x28
    487a:       c8f0            ldmia   r0!, {r4, r5, r6, r7}
    487c:       2000            movs    r0, #0
    487e:       4770            bx      lr

00004880 <longjmp>:
    4880:       3010            adds    r0, #16
    4882:       c87c            ldmia   r0!, {r2, r3, r4, r5, r6}
    4884:       4690            mov     r8, r2
    4886:       4699            mov     r9, r3
    4888:       46a2            mov     sl, r4
    488a:       46ab            mov     fp, r5
    488c:       46b5            mov     sp, r6
    488e:       c808            ldmia   r0!, {r3}
    4890:       3828            subs    r0, #40 ; 0x28
    4892:       c8f0            ldmia   r0!, {r4, r5, r6, r7}
    4894:       1c08            adds    r0, r1, #0
    4896:       d100            bne.n   489a <longjmp+0x1a>
    4898:       2001            movs    r0, #1
    489a:       4718            bx      r3
