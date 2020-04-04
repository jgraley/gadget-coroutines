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
    43fa:	2131      	movs	r1, #49	; 0x31
    43fc:	4804      	ldr	r0, [pc, #16]	; (4410 <SuperFunctor::TestPCRelative()+0x24>)
    43fe:	f000 f9f1 	bl	47e4 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    4402:	bd07      	pop	{r0, r1, r2, pc}
    4404:	000043ed 	andeq	r4, r0, sp, ror #7
    4408:	0000594c 	andeq	r5, r0, ip, asr #18
    440c:	000058c3 	andeq	r5, r0, r3, asr #17
    4410:	00005970 	andeq	r5, r0, r0, ror r9

00004414 <SuperFunctor::GetAssembly()>:
    4414:	b082      	sub	sp, #8
    4416:	466b      	mov	r3, sp
    4418:	2200      	movs	r2, #0
    441a:	3307      	adds	r3, #7
    441c:	701a      	strb	r2, [r3, #0]
    441e:	781b      	ldrb	r3, [r3, #0]
    4420:	4293      	cmp	r3, r2
    4422:	d004      	beq.n	442e <SuperFunctor::GetAssembly()+0x1a>
    4424:	4678      	mov	r0, pc
    4426:	3808      	subs	r0, #8
    4428:	68c1      	ldr	r1, [r0, #12]
    442a:	4708      	bx	r1
    442c:	e7fe      	b.n	442c <SuperFunctor::GetAssembly()+0x18>
    442e:	4a04      	ldr	r2, [pc, #16]	; (4440 <SuperFunctor::GetAssembly()+0x2c>)
    4430:	4b04      	ldr	r3, [pc, #16]	; (4444 <SuperFunctor::GetAssembly()+0x30>)
    4432:	6002      	str	r2, [r0, #0]
    4434:	1a9b      	subs	r3, r3, r2
    4436:	105b      	asrs	r3, r3, #1
    4438:	6043      	str	r3, [r0, #4]
    443a:	b002      	add	sp, #8
    443c:	4770      	bx	lr
    443e:	46c0      	nop			; (mov r8, r8)
    4440:	00004424 	andeq	r4, r0, r4, lsr #8
    4444:	0000442c 	andeq	r4, r0, ip, lsr #8

00004448 <SuperFunctor::SuperFunctor()>:
    4448:	4b0e      	ldr	r3, [pc, #56]	; (4484 <SuperFunctor::SuperFunctor()+0x3c>)
    444a:	b513      	push	{r0, r1, r4, lr}
    444c:	3308      	adds	r3, #8
    444e:	6003      	str	r3, [r0, #0]
    4450:	2300      	movs	r3, #0
    4452:	60c3      	str	r3, [r0, #12]
    4454:	3301      	adds	r3, #1
    4456:	6103      	str	r3, [r0, #16]
    4458:	0004      	movs	r4, r0
    445a:	4668      	mov	r0, sp
    445c:	f7ff ffda 	bl	4414 <SuperFunctor::GetAssembly()>
    4460:	9a01      	ldr	r2, [sp, #4]
    4462:	2a04      	cmp	r2, #4
    4464:	dd07      	ble.n	4476 <SuperFunctor::SuperFunctor()+0x2e>
    4466:	4b08      	ldr	r3, [pc, #32]	; (4488 <SuperFunctor::SuperFunctor()+0x40>)
    4468:	4a08      	ldr	r2, [pc, #32]	; (448c <SuperFunctor::SuperFunctor()+0x44>)
    446a:	210c      	movs	r1, #12
    446c:	4808      	ldr	r0, [pc, #32]	; (4490 <SuperFunctor::SuperFunctor()+0x48>)
    446e:	f000 f9b9 	bl	47e4 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    4472:	f000 faab 	bl	49cc <abort>
    4476:	1d20      	adds	r0, r4, #4
    4478:	0052      	lsls	r2, r2, #1
    447a:	9900      	ldr	r1, [sp, #0]
    447c:	f000 faf4 	bl	4a68 <memcpy>
    4480:	0020      	movs	r0, r4
    4482:	bd16      	pop	{r1, r2, r4, pc}
    4484:	00005a30 	andeq	r5, r0, r0, lsr sl
    4488:	00005a18 	andeq	r5, r0, r8, lsl sl
    448c:	000058bd 			; <UNDEFINED> instruction: 0x000058bd
    4490:	00005970 	andeq	r5, r0, r0, ror r9

00004494 <SuperFunctor::TestMemberFunctionPointer()>:
    4494:	2300      	movs	r3, #0
    4496:	b5f0      	push	{r4, r5, r6, r7, lr}
    4498:	b08f      	sub	sp, #60	; 0x3c
    449a:	9305      	str	r3, [sp, #20]
    449c:	3301      	adds	r3, #1
    449e:	9306      	str	r3, [sp, #24]
    44a0:	aa03      	add	r2, sp, #12
    44a2:	ab05      	add	r3, sp, #20
    44a4:	cb03      	ldmia	r3!, {r0, r1}
    44a6:	c203      	stmia	r2!, {r0, r1}
    44a8:	2308      	movs	r3, #8
    44aa:	4e13      	ldr	r6, [pc, #76]	; (44f8 <SuperFunctor::TestMemberFunctionPointer()+0x64>)
    44ac:	4f13      	ldr	r7, [pc, #76]	; (44fc <SuperFunctor::TestMemberFunctionPointer()+0x68>)
    44ae:	0030      	movs	r0, r6
    44b0:	9300      	str	r3, [sp, #0]
    44b2:	003a      	movs	r2, r7
    44b4:	4b12      	ldr	r3, [pc, #72]	; (4500 <SuperFunctor::TestMemberFunctionPointer()+0x6c>)
    44b6:	213e      	movs	r1, #62	; 0x3e
    44b8:	f000 f994 	bl	47e4 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    44bc:	9d04      	ldr	r5, [sp, #16]
    44be:	9c03      	ldr	r4, [sp, #12]
    44c0:	4b10      	ldr	r3, [pc, #64]	; (4504 <SuperFunctor::TestMemberFunctionPointer()+0x70>)
    44c2:	003a      	movs	r2, r7
    44c4:	213f      	movs	r1, #63	; 0x3f
    44c6:	0030      	movs	r0, r6
    44c8:	9501      	str	r5, [sp, #4]
    44ca:	9400      	str	r4, [sp, #0]
    44cc:	f000 f98a 	bl	47e4 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    44d0:	a809      	add	r0, sp, #36	; 0x24
    44d2:	f7ff ffb9 	bl	4448 <SuperFunctor::SuperFunctor()>
    44d6:	ab07      	add	r3, sp, #28
    44d8:	001a      	movs	r2, r3
    44da:	2301      	movs	r3, #1
    44dc:	a903      	add	r1, sp, #12
    44de:	c941      	ldmia	r1!, {r0, r6}
    44e0:	c241      	stmia	r2!, {r0, r6}
    44e2:	1068      	asrs	r0, r5, #1
    44e4:	aa09      	add	r2, sp, #36	; 0x24
    44e6:	1810      	adds	r0, r2, r0
    44e8:	421d      	tst	r5, r3
    44ea:	d001      	beq.n	44f0 <SuperFunctor::TestMemberFunctionPointer()+0x5c>
    44ec:	6803      	ldr	r3, [r0, #0]
    44ee:	591c      	ldr	r4, [r3, r4]
    44f0:	47a0      	blx	r4
    44f2:	b00f      	add	sp, #60	; 0x3c
    44f4:	bdf0      	pop	{r4, r5, r6, r7, pc}
    44f6:	46c0      	nop			; (mov r8, r8)
    44f8:	00005970 	andeq	r5, r0, r0, ror r9
    44fc:	000058c3 	andeq	r5, r0, r3, asr #17
    4500:	000059e1 	andeq	r5, r0, r1, ror #19
    4504:	00005a05 	andeq	r5, r0, r5, lsl #20
