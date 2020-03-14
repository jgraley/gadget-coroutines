00004348 <Coroutine::Coroutine(std::function<void ()>)>:
    4348:	b5b0      	push	{r4, r5, r7, lr}
    434a:	4b3a      	ldr	r3, [pc, #232]	; (4434 <Coroutine::Coroutine(std::function<void ()>)+0xec>)
    434c:	b09e      	sub	sp, #120	; 0x78
    434e:	af04      	add	r7, sp, #16
    4350:	6078      	str	r0, [r7, #4]
    4352:	c008      	stmia	r0!, {r3}
    4354:	2300      	movs	r3, #0
    4356:	687a      	ldr	r2, [r7, #4]
    4358:	000c      	movs	r4, r1
    435a:	60d3      	str	r3, [r2, #12]
    435c:	688b      	ldr	r3, [r1, #8]
    435e:	2b00      	cmp	r3, #0
    4360:	d006      	beq.n	4370 <Coroutine::Coroutine(std::function<void ()>)+0x28>
    4362:	2202      	movs	r2, #2
    4364:	4798      	blx	r3
    4366:	687b      	ldr	r3, [r7, #4]
    4368:	68e2      	ldr	r2, [r4, #12]
    436a:	611a      	str	r2, [r3, #16]
    436c:	68a2      	ldr	r2, [r4, #8]
    436e:	60da      	str	r2, [r3, #12]
    4370:	2080      	movs	r0, #128	; 0x80
    4372:	687b      	ldr	r3, [r7, #4]
    4374:	00c0      	lsls	r0, r0, #3
    4376:	6158      	str	r0, [r3, #20]
    4378:	f000 f896 	bl	44a8 <operator new[](unsigned int)>
    437c:	687b      	ldr	r3, [r7, #4]
    437e:	687a      	ldr	r2, [r7, #4]
    4380:	6198      	str	r0, [r3, #24]
    4382:	2300      	movs	r3, #0
    4384:	7713      	strb	r3, [r2, #28]
    4386:	0022      	movs	r2, r4
    4388:	466b      	mov	r3, sp
    438a:	9301      	str	r3, [sp, #4]
    438c:	687b      	ldr	r3, [r7, #4]
    438e:	3210      	adds	r2, #16
    4390:	9203      	str	r2, [sp, #12]
    4392:	9300      	str	r3, [sp, #0]
    4394:	4828      	ldr	r0, [pc, #160]	; (4438 <Coroutine::Coroutine(std::function<void ()>)+0xf0>)
    4396:	4b29      	ldr	r3, [pc, #164]	; (443c <Coroutine::Coroutine(std::function<void ()>)+0xf4>)
    4398:	9402      	str	r4, [sp, #8]
    439a:	4a29      	ldr	r2, [pc, #164]	; (4440 <Coroutine::Coroutine(std::function<void ()>)+0xf8>)
    439c:	2122      	movs	r1, #34	; 0x22
    439e:	f000 f959 	bl	4654 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    43a2:	230c      	movs	r3, #12
    43a4:	18f8      	adds	r0, r7, r3
    43a6:	f000 fb75 	bl	4a94 <setjmp>
    43aa:	2800      	cmp	r0, #0
    43ac:	d023      	beq.n	43f6 <Coroutine::Coroutine(std::function<void ()>)+0xae>
    43ae:	2803      	cmp	r0, #3
    43b0:	d11b      	bne.n	43ea <Coroutine::Coroutine(std::function<void ()>)+0xa2>
    43b2:	466b      	mov	r3, sp
    43b4:	464c      	mov	r4, r9
    43b6:	4d20      	ldr	r5, [pc, #128]	; (4438 <Coroutine::Coroutine(std::function<void ()>)+0xf0>)
    43b8:	9302      	str	r3, [sp, #8]
    43ba:	687b      	ldr	r3, [r7, #4]
    43bc:	9401      	str	r4, [sp, #4]
    43be:	9300      	str	r3, [sp, #0]
    43c0:	4a1f      	ldr	r2, [pc, #124]	; (4440 <Coroutine::Coroutine(std::function<void ()>)+0xf8>)
    43c2:	4b20      	ldr	r3, [pc, #128]	; (4444 <Coroutine::Coroutine(std::function<void ()>)+0xfc>)
    43c4:	2136      	movs	r1, #54	; 0x36
    43c6:	0028      	movs	r0, r5
    43c8:	f000 f944 	bl	4654 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    43cc:	2c00      	cmp	r4, #0
    43ce:	d109      	bne.n	43e4 <Coroutine::Coroutine(std::function<void ()>)+0x9c>
    43d0:	2137      	movs	r1, #55	; 0x37
    43d2:	0028      	movs	r0, r5
    43d4:	687b      	ldr	r3, [r7, #4]
    43d6:	4a1c      	ldr	r2, [pc, #112]	; (4448 <Coroutine::Coroutine(std::function<void ()>)+0x100>)
    43d8:	9300      	str	r3, [sp, #0]
    43da:	4b1c      	ldr	r3, [pc, #112]	; (444c <Coroutine::Coroutine(std::function<void ()>)+0x104>)
    43dc:	f000 f93a 	bl	4654 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    43e0:	f000 fa2c 	bl	483c <abort>
    43e4:	0020      	movs	r0, r4
    43e6:	f7ff ff89 	bl	42fc <Coroutine::child_main_function()>
    43ea:	9000      	str	r0, [sp, #0]
    43ec:	4b18      	ldr	r3, [pc, #96]	; (4450 <Coroutine::Coroutine(std::function<void ()>)+0x108>)
    43ee:	4a16      	ldr	r2, [pc, #88]	; (4448 <Coroutine::Coroutine(std::function<void ()>)+0x100>)
    43f0:	213c      	movs	r1, #60	; 0x3c
    43f2:	4811      	ldr	r0, [pc, #68]	; (4438 <Coroutine::Coroutine(std::function<void ()>)+0xf0>)
    43f4:	e7f2      	b.n	43dc <Coroutine::Coroutine(std::function<void ()>)+0x94>
    43f6:	6afc      	ldr	r4, [r7, #44]	; 0x2c
    43f8:	466b      	mov	r3, sp
    43fa:	9301      	str	r3, [sp, #4]
    43fc:	687b      	ldr	r3, [r7, #4]
    43fe:	9703      	str	r7, [sp, #12]
    4400:	9402      	str	r4, [sp, #8]
    4402:	9300      	str	r3, [sp, #0]
    4404:	4a0e      	ldr	r2, [pc, #56]	; (4440 <Coroutine::Coroutine(std::function<void ()>)+0xf8>)
    4406:	4b13      	ldr	r3, [pc, #76]	; (4454 <Coroutine::Coroutine(std::function<void ()>)+0x10c>)
    4408:	212a      	movs	r1, #42	; 0x2a
    440a:	480b      	ldr	r0, [pc, #44]	; (4438 <Coroutine::Coroutine(std::function<void ()>)+0xf0>)
    440c:	f000 f922 	bl	4654 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    4410:	0022      	movs	r2, r4
    4412:	0039      	movs	r1, r7
    4414:	6878      	ldr	r0, [r7, #4]
    4416:	f7ff fea3 	bl	4160 <Coroutine::prepare_child_stack(unsigned char*, unsigned char*)>
    441a:	220c      	movs	r2, #12
    441c:	6879      	ldr	r1, [r7, #4]
    441e:	0003      	movs	r3, r0
    4420:	18ba      	adds	r2, r7, r2
    4422:	6878      	ldr	r0, [r7, #4]
    4424:	3124      	adds	r1, #36	; 0x24
    4426:	f7ff feb9 	bl	419c <Coroutine::prepare_child_jmp_buf(int (&) [23], int const (&) [23], unsigned char*)>
    442a:	46bd      	mov	sp, r7
    442c:	6878      	ldr	r0, [r7, #4]
    442e:	b01a      	add	sp, #104	; 0x68
    4430:	bdb0      	pop	{r4, r5, r7, pc}
    4432:	46c0      	nop			; (mov r8, r8)
    4434:	316f4347 	cmncc	pc, r7, asr #6
    4438:	000057b3 			; <UNDEFINED> instruction: 0x000057b3
    443c:	000056f6 	strdeq	r5, [r0], -r6
    4440:	000057ac 	andeq	r5, r0, ip, lsr #15
    4444:	00005752 	andeq	r5, r0, r2, asr r7
    4448:	000057a6 	andeq	r5, r0, r6, lsr #15
    444c:	00005768 	andeq	r5, r0, r8, ror #14
    4450:	00005658 	andeq	r5, r0, r8, asr r6
    4454:	00005720 	andeq	r5, r0, r0, lsr #14
