00004348 <Coroutine::Coroutine(std::function<void ()>)>:
    4348:       b5b0            push    {r4, r5, r7, lr}
    434a:       4b38            ldr     r3, [pc, #224]  ; (442c <Coroutine::Coroutine(std::function<void ()>)+0xe4>)
    434c:       b09e            sub     sp, #120        ; 0x78
    434e:       af04            add     r7, sp, #16
    4350:       6078            str     r0, [r7, #4]
    4352:       c008            stmia   r0!, {r3}
    4354:       2300            movs    r3, #0
    4356:       687a            ldr     r2, [r7, #4]
    4358:       000c            movs    r4, r1
    435a:       60d3            str     r3, [r2, #12]
    435c:       688b            ldr     r3, [r1, #8]
    435e:       2b00            cmp     r3, #0
    4360:       d006            beq.n   4370 <Coroutine::Coroutine(std::function<void ()>)+0x28>
    4362:       2202            movs    r2, #2
    4364:       4798            blx     r3
    4366:       687b            ldr     r3, [r7, #4]
    4368:       68e2            ldr     r2, [r4, #12]
    436a:       611a            str     r2, [r3, #16]
    436c:       68a2            ldr     r2, [r4, #8]
    436e:       60da            str     r2, [r3, #12]
    4370:       2080            movs    r0, #128        ; 0x80
    4372:       687b            ldr     r3, [r7, #4]
    4374:       00c0            lsls    r0, r0, #3
    4376:       6158            str     r0, [r3, #20]
    4378:       f000 f892       bl      44a0 <operator new[](unsigned int)>
    437c:       687b            ldr     r3, [r7, #4]
    437e:       687a            ldr     r2, [r7, #4]
    4380:       6198            str     r0, [r3, #24]
    4382:       2300            movs    r3, #0
    4384:       3410            adds    r4, #16
    4386:       7713            strb    r3, [r2, #28]
    4388:       4829            ldr     r0, [pc, #164]  ; (4430 <Coroutine::Coroutine(std::function<void ()>)+0xe8>)
    438a:       466b            mov     r3, sp
    438c:       9200            str     r2, [sp, #0]
    438e:       9301            str     r3, [sp, #4]
    4390:       9402            str     r4, [sp, #8]
    4392:       4b28            ldr     r3, [pc, #160]  ; (4434 <Coroutine::Coroutine(std::function<void ()>)+0xec>)
    4394:       4a28            ldr     r2, [pc, #160]  ; (4438 <Coroutine::Coroutine(std::function<void ()>)+0xf0>)
    4396:       2121            movs    r1, #33 ; 0x21
    4398:       f000 f958       bl      464c <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    439c:       230c            movs    r3, #12
    439e:       18f8            adds    r0, r7, r3
    43a0:       f000 fb74       bl      4a8c <setjmp>
    43a4:       2800            cmp     r0, #0
    43a6:       d023            beq.n   43f0 <Coroutine::Coroutine(std::function<void ()>)+0xa8>
    43a8:       2803            cmp     r0, #3
    43aa:       d11b            bne.n   43e4 <Coroutine::Coroutine(std::function<void ()>)+0x9c>
    43ac:       466b            mov     r3, sp
    43ae:       464c            mov     r4, r9
    43b0:       4d1f            ldr     r5, [pc, #124]  ; (4430 <Coroutine::Coroutine(std::function<void ()>)+0xe8>)
    43b2:       9302            str     r3, [sp, #8]
    43b4:       687b            ldr     r3, [r7, #4]
    43b6:       9401            str     r4, [sp, #4]
    43b8:       9300            str     r3, [sp, #0]
    43ba:       4a1f            ldr     r2, [pc, #124]  ; (4438 <Coroutine::Coroutine(std::function<void ()>)+0xf0>)
    43bc:       4b1f            ldr     r3, [pc, #124]  ; (443c <Coroutine::Coroutine(std::function<void ()>)+0xf4>)
    43be:       2135            movs    r1, #53 ; 0x35
    43c0:       0028            movs    r0, r5
    43c2:       f000 f943       bl      464c <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    43c6:       2c00            cmp     r4, #0
    43c8:       d109            bne.n   43de <Coroutine::Coroutine(std::function<void ()>)+0x96>
    43ca:       2136            movs    r1, #54 ; 0x36
    43cc:       0028            movs    r0, r5
    43ce:       687b            ldr     r3, [r7, #4]
    43d0:       4a1b            ldr     r2, [pc, #108]  ; (4440 <Coroutine::Coroutine(std::function<void ()>)+0xf8>)
    43d2:       9300            str     r3, [sp, #0]
    43d4:       4b1b            ldr     r3, [pc, #108]  ; (4444 <Coroutine::Coroutine(std::function<void ()>)+0xfc>)
    43d6:       f000 f939       bl      464c <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    43da:       f000 fa2b       bl      4834 <abort>
    43de:       0020            movs    r0, r4
    43e0:       f7ff ff8c       bl      42fc <Coroutine::child_main_function()>
    43e4:       9000            str     r0, [sp, #0]
    43e6:       4b18            ldr     r3, [pc, #96]   ; (4448 <Coroutine::Coroutine(std::function<void ()>)+0x100>)
    43e8:       4a15            ldr     r2, [pc, #84]   ; (4440 <Coroutine::Coroutine(std::function<void ()>)+0xf8>)
    43ea:       213b            movs    r1, #59 ; 0x3b
    43ec:       4810            ldr     r0, [pc, #64]   ; (4430 <Coroutine::Coroutine(std::function<void ()>)+0xe8>)
    43ee:       e7f2            b.n     43d6 <Coroutine::Coroutine(std::function<void ()>)+0x8e>
    43f0:       6afc            ldr     r4, [r7, #44]   ; 0x2c
    43f2:       466b            mov     r3, sp
    43f4:       9301            str     r3, [sp, #4]
    43f6:       687b            ldr     r3, [r7, #4]
    43f8:       9703            str     r7, [sp, #12]
    43fa:       9402            str     r4, [sp, #8]
    43fc:       9300            str     r3, [sp, #0]
    43fe:       4a0e            ldr     r2, [pc, #56]   ; (4438 <Coroutine::Coroutine(std::function<void ()>)+0xf0>)
    4400:       4b12            ldr     r3, [pc, #72]   ; (444c <Coroutine::Coroutine(std::function<void ()>)+0x104>)
    4402:       2129            movs    r1, #41 ; 0x29
    4404:       480a            ldr     r0, [pc, #40]   ; (4430 <Coroutine::Coroutine(std::function<void ()>)+0xe8>)
    4406:       f000 f921       bl      464c <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    440a:       0022            movs    r2, r4
    440c:       0039            movs    r1, r7
    440e:       6878            ldr     r0, [r7, #4]
    4410:       f7ff fea6       bl      4160 <Coroutine::prepare_child_stack(unsigned char*, unsigned char*)>
    4414:       220c            movs    r2, #12
    4416:       6879            ldr     r1, [r7, #4]
    4418:       0003            movs    r3, r0
    441a:       18ba            adds    r2, r7, r2
    441c:       6878            ldr     r0, [r7, #4]
    441e:       3124            adds    r1, #36 ; 0x24
    4420:       f7ff febc       bl      419c <Coroutine::prepare_child_jmp_buf(int (&) [23], int const (&) [23], unsigned char*)>
    4424:       46bd            mov     sp, r7
    4426:       6878            ldr     r0, [r7, #4]
    4428:       b01a            add     sp, #104        ; 0x68
    442a:       bdb0            pop     {r4, r5, r7, pc}
    442c:       316f4347        cmncc   pc, r7, asr #6
    4430:       000057a1        andeq   r5, r0, r1, lsr #15
    4434:       000056ee        andeq   r5, r0, lr, ror #13
    4438:       0000579a        muleq   r0, sl, r7
    443c:       00005740        andeq   r5, r0, r0, asr #14
    4440:       00005794        muleq   r0, r4, r7
    4444:       00005756        andeq   r5, r0, r6, asr r7
    4448:       00005650        andeq   r5, r0, r0, asr r6
    444c:       0000570e        andeq   r5, r0, lr, lsl #14
