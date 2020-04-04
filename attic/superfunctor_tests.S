000043ea <SuperFunctor::operator()()>:
    43ea:	4770      	bx	lr

000043ec <SuperFunctor::TestPCRelative()>:
    43ec:	b507      	push	{r0, r1, r2, lr}
    43ee:	467b      	mov	r3, pc
    43f0:	4a04      	ldr	r2, [pc, #16]	; (4404 <SuperFunctor::TestPCRelative()+0x18>)
    43f2:	9300      	str	r3, [sp, #0]
    43f4:	9201      	str	r2, [sp, #4]
    43f6:	4b04      	ldr	r3, [pc, #16]	; (4408 <SuperFunctor::TestPCRelative()+0x1c>)
    43f8:	4a04      	ldr	r2, [pc, #16]	; (440c <SuperFunctor::TestPCRelative()+0x20>)
    43fa:	211d      	movs	r1, #29
    43fc:	4804      	ldr	r0, [pc, #16]	; (4410 <SuperFunctor::TestPCRelative()+0x24>)
    43fe:	f000 f9ad 	bl	475c <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    4402:	bd07      	pop	{r0, r1, r2, pc}
    4404:	000043ed 	andeq	r4, r0, sp, ror #7
    4408:	000058d4 	ldrdeq	r5, [r0], -r4
    440c:	0000584f 	andeq	r5, r0, pc, asr #16
    4410:	000058fa 	strdeq	r5, [r0], -sl

00004414 <SuperFunctor::SuperFunctor()>:
    4414:	4770      	bx	lr
	...

00004418 <SuperFunctor::TestMemberFunctionPointer()>:
    4418:	b5f0      	push	{r4, r5, r6, r7, lr}
    441a:	4b15      	ldr	r3, [pc, #84]	; (4470 <SuperFunctor::TestMemberFunctionPointer()+0x58>)
    441c:	b08d      	sub	sp, #52	; 0x34
    441e:	2508      	movs	r5, #8
    4420:	9308      	str	r3, [sp, #32]
    4422:	2300      	movs	r3, #0
    4424:	002a      	movs	r2, r5
    4426:	a908      	add	r1, sp, #32
    4428:	a806      	add	r0, sp, #24
    442a:	9309      	str	r3, [sp, #36]	; 0x24
    442c:	f000 fad8 	bl	49e0 <memcpy>
    4430:	4f10      	ldr	r7, [pc, #64]	; (4474 <SuperFunctor::TestMemberFunctionPointer()+0x5c>)
    4432:	4e11      	ldr	r6, [pc, #68]	; (4478 <SuperFunctor::TestMemberFunctionPointer()+0x60>)
    4434:	003a      	movs	r2, r7
    4436:	0030      	movs	r0, r6
    4438:	9500      	str	r5, [sp, #0]
    443a:	4b10      	ldr	r3, [pc, #64]	; (447c <SuperFunctor::TestMemberFunctionPointer()+0x64>)
    443c:	2124      	movs	r1, #36	; 0x24
    443e:	f000 f98d 	bl	475c <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    4442:	9b06      	ldr	r3, [sp, #24]
    4444:	9c07      	ldr	r4, [sp, #28]
    4446:	9303      	str	r3, [sp, #12]
    4448:	9401      	str	r4, [sp, #4]
    444a:	9300      	str	r3, [sp, #0]
    444c:	003a      	movs	r2, r7
    444e:	4b0c      	ldr	r3, [pc, #48]	; (4480 <SuperFunctor::TestMemberFunctionPointer()+0x68>)
    4450:	0030      	movs	r0, r6
    4452:	2125      	movs	r1, #37	; 0x25
    4454:	f000 f982 	bl	475c <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    4458:	002a      	movs	r2, r5
    445a:	a906      	add	r1, sp, #24
    445c:	a80a      	add	r0, sp, #40	; 0x28
    445e:	f000 fabf 	bl	49e0 <memcpy>
    4462:	1064      	asrs	r4, r4, #1
    4464:	ab05      	add	r3, sp, #20
    4466:	1918      	adds	r0, r3, r4
    4468:	9b03      	ldr	r3, [sp, #12]
    446a:	4798      	blx	r3
    446c:	b00d      	add	sp, #52	; 0x34
    446e:	bdf0      	pop	{r4, r5, r6, r7, pc}
    4470:	000043eb 	andeq	r4, r0, fp, ror #7
    4474:	0000584f 	andeq	r5, r0, pc, asr #16
    4478:	000058fa 	strdeq	r5, [r0], -sl
    447c:	0000596b 	andeq	r5, r0, fp, ror #18
    4480:	00005990 	muleq	r0, r0, r9

