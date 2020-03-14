00004348 <Coroutine::Coroutine(std::function<void ()>)>:
    4348:	b5f0      	push	{r4, r5, r6, r7, lr}
    434a:	4b44      	ldr	r3, [pc, #272]	; (445c <Coroutine::Coroutine(std::function<void ()>)+0x114>)
    434c:	b09f      	sub	sp, #124	; 0x7c
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
    4378:	f000 f8ae 	bl	44d8 <operator new[](unsigned int)>
    437c:	687b      	ldr	r3, [r7, #4]
    437e:	687a      	ldr	r2, [r7, #4]
    4380:	6198      	str	r0, [r3, #24]
    4382:	2300      	movs	r3, #0
    4384:	4e36      	ldr	r6, [pc, #216]	; (4460 <Coroutine::Coroutine(std::function<void ()>)+0x118>)
    4386:	4d37      	ldr	r5, [pc, #220]	; (4464 <Coroutine::Coroutine(std::function<void ()>)+0x11c>)
    4388:	7713      	strb	r3, [r2, #28]
    438a:	2122      	movs	r1, #34	; 0x22
    438c:	466b      	mov	r3, sp
    438e:	9200      	str	r2, [sp, #0]
    4390:	9301      	str	r3, [sp, #4]
    4392:	0032      	movs	r2, r6
    4394:	4b34      	ldr	r3, [pc, #208]	; (4468 <Coroutine::Coroutine(std::function<void ()>)+0x120>)
    4396:	0028      	movs	r0, r5
    4398:	f000 f974 	bl	4684 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    439c:	0023      	movs	r3, r4
    439e:	3310      	adds	r3, #16
    43a0:	9301      	str	r3, [sp, #4]
    43a2:	9400      	str	r4, [sp, #0]
    43a4:	4b31      	ldr	r3, [pc, #196]	; (446c <Coroutine::Coroutine(std::function<void ()>)+0x124>)
    43a6:	0032      	movs	r2, r6
    43a8:	2123      	movs	r1, #35	; 0x23
    43aa:	0028      	movs	r0, r5
    43ac:	f000 f96a 	bl	4684 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    43b0:	230c      	movs	r3, #12
    43b2:	2208      	movs	r2, #8
    43b4:	18fc      	adds	r4, r7, r3
    43b6:	18ba      	adds	r2, r7, r2
    43b8:	3354      	adds	r3, #84	; 0x54
    43ba:	18d3      	adds	r3, r2, r3
    43bc:	9301      	str	r3, [sp, #4]
    43be:	0028      	movs	r0, r5
    43c0:	9400      	str	r4, [sp, #0]
    43c2:	4b2b      	ldr	r3, [pc, #172]	; (4470 <Coroutine::Coroutine(std::function<void ()>)+0x128>)
    43c4:	0032      	movs	r2, r6
    43c6:	2124      	movs	r1, #36	; 0x24
    43c8:	f000 f95c 	bl	4684 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    43cc:	0020      	movs	r0, r4
    43ce:	f000 fb79 	bl	4ac4 <setjmp>
    43d2:	2800      	cmp	r0, #0
    43d4:	d023      	beq.n	441e <Coroutine::Coroutine(std::function<void ()>)+0xd6>
    43d6:	2803      	cmp	r0, #3
    43d8:	d11b      	bne.n	4412 <Coroutine::Coroutine(std::function<void ()>)+0xca>
    43da:	466b      	mov	r3, sp
    43dc:	464c      	mov	r4, r9
    43de:	4d21      	ldr	r5, [pc, #132]	; (4464 <Coroutine::Coroutine(std::function<void ()>)+0x11c>)
    43e0:	9302      	str	r3, [sp, #8]
    43e2:	687b      	ldr	r3, [r7, #4]
    43e4:	9401      	str	r4, [sp, #4]
    43e6:	9300      	str	r3, [sp, #0]
    43e8:	4a1d      	ldr	r2, [pc, #116]	; (4460 <Coroutine::Coroutine(std::function<void ()>)+0x118>)
    43ea:	4b22      	ldr	r3, [pc, #136]	; (4474 <Coroutine::Coroutine(std::function<void ()>)+0x12c>)
    43ec:	2138      	movs	r1, #56	; 0x38
    43ee:	0028      	movs	r0, r5
    43f0:	f000 f948 	bl	4684 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    43f4:	2c00      	cmp	r4, #0
    43f6:	d109      	bne.n	440c <Coroutine::Coroutine(std::function<void ()>)+0xc4>
    43f8:	2139      	movs	r1, #57	; 0x39
    43fa:	0028      	movs	r0, r5
    43fc:	687b      	ldr	r3, [r7, #4]
    43fe:	4a1e      	ldr	r2, [pc, #120]	; (4478 <Coroutine::Coroutine(std::function<void ()>)+0x130>)
    4400:	9300      	str	r3, [sp, #0]
    4402:	4b1e      	ldr	r3, [pc, #120]	; (447c <Coroutine::Coroutine(std::function<void ()>)+0x134>)
    4404:	f000 f93e 	bl	4684 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    4408:	f000 fa30 	bl	486c <abort>
    440c:	0020      	movs	r0, r4
    440e:	f7ff ff75 	bl	42fc <Coroutine::child_main_function()>
    4412:	9000      	str	r0, [sp, #0]
    4414:	4b1a      	ldr	r3, [pc, #104]	; (4480 <Coroutine::Coroutine(std::function<void ()>)+0x138>)
    4416:	4a18      	ldr	r2, [pc, #96]	; (4478 <Coroutine::Coroutine(std::function<void ()>)+0x130>)
    4418:	213e      	movs	r1, #62	; 0x3e
    441a:	4812      	ldr	r0, [pc, #72]	; (4464 <Coroutine::Coroutine(std::function<void ()>)+0x11c>)
    441c:	e7f2      	b.n	4404 <Coroutine::Coroutine(std::function<void ()>)+0xbc>
    441e:	6afc      	ldr	r4, [r7, #44]	; 0x2c
    4420:	466b      	mov	r3, sp
    4422:	9301      	str	r3, [sp, #4]
    4424:	687b      	ldr	r3, [r7, #4]
    4426:	9703      	str	r7, [sp, #12]
    4428:	9402      	str	r4, [sp, #8]
    442a:	9300      	str	r3, [sp, #0]
    442c:	4a0c      	ldr	r2, [pc, #48]	; (4460 <Coroutine::Coroutine(std::function<void ()>)+0x118>)
    442e:	4b15      	ldr	r3, [pc, #84]	; (4484 <Coroutine::Coroutine(std::function<void ()>)+0x13c>)
    4430:	212c      	movs	r1, #44	; 0x2c
    4432:	480c      	ldr	r0, [pc, #48]	; (4464 <Coroutine::Coroutine(std::function<void ()>)+0x11c>)
    4434:	f000 f926 	bl	4684 <_gcoroutines_trace(char const*, int, char const*, char const*, ...)>
    4438:	0022      	movs	r2, r4
    443a:	0039      	movs	r1, r7
    443c:	6878      	ldr	r0, [r7, #4]
    443e:	f7ff fe8f 	bl	4160 <Coroutine::prepare_child_stack(unsigned char*, unsigned char*)>
    4442:	220c      	movs	r2, #12
    4444:	6879      	ldr	r1, [r7, #4]
    4446:	0003      	movs	r3, r0
    4448:	18ba      	adds	r2, r7, r2
    444a:	6878      	ldr	r0, [r7, #4]
    444c:	3124      	adds	r1, #36	; 0x24
    444e:	f7ff fea5 	bl	419c <Coroutine::prepare_child_jmp_buf(int (&) [23], int const (&) [23], unsigned char*)>
    4452:	46bd      	mov	sp, r7
    4454:	6878      	ldr	r0, [r7, #4]
    4456:	b01b      	add	sp, #108	; 0x6c
    4458:	bdf0      	pop	{r4, r5, r6, r7, pc}
    445a:	46c0      	nop			; (mov r8, r8)
    445c:	316f4347 	cmncc	pc, r7, asr #6
    4460:	000057f9 	strdeq	r5, [r0], -r9
    4464:	00005800 	andeq	r5, r0, r0, lsl #16
    4468:	00005726 	andeq	r5, r0, r6, lsr #14
    446c:	00005734 	andeq	r5, r0, r4, lsr r7
    4470:	0000574f 	andeq	r5, r0, pc, asr #14
    4474:	0000579f 	muleq	r0, pc, r7	; <UNPREDICTABLE>
    4478:	000057f3 	strdeq	r5, [r0], -r3
    447c:	000057b5 			; <UNDEFINED> instruction: 0x000057b5
    4480:	00005688 	andeq	r5, r0, r8, lsl #13
    4484:	0000576d 	andeq	r5, r0, sp, ror #14
