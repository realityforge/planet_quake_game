	.file	"ai_dmnet.c"
	.text
	.p2align 4,,15
.globl BotResetNodeSwitches
	.type	BotResetNodeSwitches, @function
BotResetNodeSwitches:
.LFB69:
	.cfi_startproc
	movl	$0, numnodeswitches(%rip)
	ret
	.cfi_endproc
.LFE69:
	.size	BotResetNodeSwitches, .-BotResetNodeSwitches
	.p2align 4,,15
.globl BotSelectActivateWeapon
	.type	BotSelectActivateWeapon, @function
BotSelectActivateWeapon:
.LFB87:
	.cfi_startproc
	movl	4984(%rdi), %ecx
	testl	%ecx, %ecx
	jle	.L4
	movl	5036(%rdi), %edx
	movl	$2, %eax
	testl	%edx, %edx
	jle	.L4
.L5:
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	movl	4980(%rdi), %eax
	testl	%eax, %eax
	jle	.L6
	movl	5032(%rdi), %r11d
	movl	$3, %eax
	testl	%r11d, %r11d
	jg	.L5
.L6:
	movl	5004(%rdi), %r10d
	testl	%r10d, %r10d
	jle	.L7
	movl	5044(%rdi), %r9d
	movl	$8, %eax
	testl	%r9d, %r9d
	jg	.L5
.L7:
	movl	4996(%rdi), %r8d
	testl	%r8d, %r8d
	jle	.L8
	movl	5048(%rdi), %esi
	movl	$6, %eax
	testl	%esi, %esi
	jg	.L5
.L8:
	movl	5028(%rdi), %ecx
	testl	%ecx, %ecx
	jle	.L9
	movl	5072(%rdi), %edx
	movl	$13, %eax
	testl	%edx, %edx
	jg	.L5
.L9:
	movl	5020(%rdi), %r11d
	testl	%r11d, %r11d
	jle	.L10
	movl	5064(%rdi), %r10d
	movl	$11, %eax
	testl	%r10d, %r10d
	jg	.L5
.L10:
	movl	5000(%rdi), %r9d
	testl	%r9d, %r9d
	jle	.L11
	movl	5056(%rdi), %r8d
	movl	$7, %eax
	testl	%r8d, %r8d
	jg	.L5
.L11:
	movl	4992(%rdi), %esi
	testl	%esi, %esi
	jle	.L12
	movl	5052(%rdi), %ecx
	movl	$5, %eax
	testl	%ecx, %ecx
	jg	.L5
.L12:
	movl	5012(%rdi), %edx
	testl	%edx, %edx
	jle	.L13
	movl	5060(%rdi), %eax
	testl	%eax, %eax
	jle	.L13
	movl	$9, %eax
	ret
.L13:
	orl	$-1, %eax
	.p2align 4,,1
	ret
	.cfi_endproc
.LFE87:
	.size	BotSelectActivateWeapon, .-BotSelectActivateWeapon
	.p2align 4,,15
.globl BotGetAirGoal
	.type	BotGetAirGoal, @function
BotGetAirGoal:
.LFB72:
	.cfi_startproc
	movq	%rbx, -48(%rsp)
	movq	%rbp, -40(%rsp)
	movq	%rsi, %rbx
	.cfi_offset 6, -48
	.cfi_offset 3, -56
	movq	%r12, -32(%rsp)
	movq	%r13, -24(%rsp)
	leaq	4916(%rdi), %r13
	.cfi_offset 13, -32
	.cfi_offset 12, -40
	movq	%r14, -16(%rsp)
	movq	%r15, -8(%rsp)
	subq	$232, %rsp
	.cfi_def_cfa_offset 240
	movl	$0xc1700000, 48(%rsp)
	movl	$0xc1700000, 52(%rsp)
	leaq	32(%rsp), %r14
	.cfi_offset 15, -16
	.cfi_offset 14, -24
	movl	$0xc0000000, 56(%rsp)
	movl	$0x41700000, 32(%rsp)
	leaq	48(%rsp), %r15
	movl	$0x41700000, 36(%rsp)
	movl	$0x40000000, 40(%rsp)
	leaq	80(%rsp), %r12
	movq	%fs:40, %rax
	movq	%rax, 168(%rsp)
	xorl	%eax, %eax
	movl	4916(%rdi), %eax
	movss	.LC4(%rip), %xmm0
	movl	12(%rdi), %r9d
	movq	%rdi, %rbp
	movq	%r14, %rcx
	movq	%r15, %rdx
	movq	%r13, %rsi
	movl	%eax, 64(%rsp)
	movl	4920(%rdi), %eax
	movl	%eax, 68(%rsp)
	leaq	64(%rsp), %rax
	addss	4924(%rdi), %xmm0
	movq	%r12, %rdi
	movl	$65537, (%rsp)
	movq	%rax, %r8
	movq	%rax, 24(%rsp)
	movss	%xmm0, 72(%rsp)
	call	BotAI_Trace
	movl	92(%rsp), %edx
	movq	%r13, %r8
	movq	%r14, %rcx
	movq	24(%rsp), %rax
	movl	12(%rbp), %r9d
	movq	%r12, %rdi
	movl	$56, (%rsp)
	movl	%edx, 64(%rsp)
	movl	96(%rsp), %edx
	movq	%rax, %rsi
	movl	%edx, 68(%rsp)
	movl	100(%rsp), %edx
	movl	%edx, 72(%rsp)
	movq	%r15, %rdx
	call	BotAI_Trace
	movss	88(%rsp), %xmm0
	ucomiss	.LC5(%rip), %xmm0
	ja	.L23
.L17:
	xorl	%eax, %eax
.L19:
	movq	168(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L24
	movq	184(%rsp), %rbx
	movq	192(%rsp), %rbp
	movq	200(%rsp), %r12
	movq	208(%rsp), %r13
	movq	216(%rsp), %r14
	movq	224(%rsp), %r15
	addq	$232, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L23:
	leaq	12(%r12), %rdi
	call	BotPointAreaNum
	testl	%eax, %eax
	je	.L17
	movss	100(%rsp), %xmm0
	movl	92(%rsp), %edx
	subss	.LC3(%rip), %xmm0
	movl	%eax, 12(%rbx)
	movl	$1, %eax
	movl	$0xc1700000, 16(%rbx)
	movl	$0xc1700000, 20(%rbx)
	movl	%edx, (%rbx)
	movl	96(%rsp), %edx
	movl	$0xbf800000, 24(%rbx)
	movl	$0x41700000, 28(%rbx)
	movl	$0x41700000, 32(%rbx)
	movl	$0x3f800000, 36(%rbx)
	movl	%edx, 4(%rbx)
	movl	$128, 48(%rbx)
	movss	%xmm0, 8(%rbx)
	movl	$0, 44(%rbx)
	movl	$0, 52(%rbx)
	movl	$0, 40(%rbx)
	jmp	.L19
.L24:
	call	__stack_chk_fail
	.cfi_endproc
.LFE72:
	.size	BotGetAirGoal, .-BotGetAirGoal
	.p2align 4,,15
.globl BotClearPath
	.type	BotClearPath, @function
BotClearPath:
.LFB88:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r13
	.cfi_def_cfa_offset 32
	pushq	%r12
	.cfi_def_cfa_offset 40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	movq	%rsi, %rbp
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	pushq	%rbx
	.cfi_def_cfa_offset 56
	movq	%rdi, %rbx
	.cfi_offset 3, -56
	subq	$376, %rsp
	.cfi_def_cfa_offset 432
	movl	6264(%rdi), %edi
	movq	%fs:40, %rax
	movq	%rax, 360(%rsp)
	xorl	%eax, %eax
	testl	%edi, %edi
	je	.L49
	movl	20(%rsi), %edx
	testb	$17, %dl
	je	.L50
.L27:
	testb	$1, %dh
	jne	.L32
.L54:
	movss	floattime(%rip), %xmm0
	movss	6216(%rbx), %xmm1
.L33:
	ucomiss	%xmm0, %xmm1
	jbe	.L44
	andl	$17, %edx
	jne	.L44
	movl	6524(%rbx), %r12d
	testl	%r12d, %r12d
	jle	.L44
	movss	.LC11(%rip), %xmm0
	leaq	32(%rsp), %r14
	movss	%xmm0, 28(%rsp)
	movq	%rbx, %r13
	movl	$-1, %r15d
	xorl	%r12d, %r12d
	.p2align 4,,10
	.p2align 3
.L38:
	movl	6268(%r13), %edi
	movq	%r14, %rsi
	call	BotAI_GetEntityState
	movss	56(%rsp), %xmm0
	movss	60(%rsp), %xmm2
	subss	4916(%rbx), %xmm0
	subss	4920(%rbx), %xmm2
	movss	64(%rsp), %xmm1
	subss	4924(%rbx), %xmm1
	movss	%xmm0, 240(%rsp)
	mulss	%xmm0, %xmm0
	movss	%xmm2, 244(%rsp)
	mulss	%xmm2, %xmm2
	movss	%xmm1, 248(%rsp)
	mulss	%xmm1, %xmm1
	addss	%xmm2, %xmm0
	addss	%xmm1, %xmm0
	movss	28(%rsp), %xmm1
	sqrtss	%xmm0, %xmm0
	ucomiss	%xmm0, %xmm1
	jbe	.L36
	movss	%xmm0, 28(%rsp)
	movl	%r12d, %r15d
.L36:
	addl	$1, %r12d
	addq	$4, %r13
	cmpl	%r12d, 6524(%rbx)
	jg	.L38
	cmpl	$-1, %r15d
	je	.L44
	movslq	%r15d,%r15
	movq	%r14, %rsi
	leaq	40(%rbp), %r12
	movl	6268(%rbx,%r15,4), %edi
	call	BotAI_GetEntityState
	movss	.LC3(%rip), %xmm0
	leaq	240(%rsp), %rdi
	addss	64(%rsp), %xmm0
	movss	56(%rsp), %xmm2
	movss	60(%rsp), %xmm1
	movq	%r12, %rsi
	movss	%xmm2, 256(%rsp)
	subss	4944(%rbx), %xmm2
	movss	%xmm1, 260(%rsp)
	subss	4948(%rbx), %xmm1
	movss	%xmm0, 264(%rsp)
	subss	4952(%rbx), %xmm0
	movss	%xmm2, 240(%rsp)
	movss	%xmm1, 244(%rsp)
	movss	%xmm0, 248(%rsp)
	call	vectoangles
	movl	5004(%rbx), %r11d
	testl	%r11d, %r11d
	jle	.L39
	movl	5044(%rbx), %r10d
	testl	%r10d, %r10d
	jle	.L39
	movl	$8, 24(%rbp)
.L40:
	orl	$17, 20(%rbp)
	movl	160(%rbx), %eax
	cmpl	24(%rbp), %eax
	je	.L51
	.p2align 4,,10
	.p2align 3
.L44:
	movq	360(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L52
	addq	$376, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.L50:
	leaq	32(%rsp), %r14
	leaq	40(%rbp), %r12
	movq	%r14, %rsi
	call	BotAI_GetEntityState
	movss	.LC8(%rip), %xmm0
	leaq	240(%rsp), %rdi
	addss	64(%rsp), %xmm0
	movss	56(%rsp), %xmm2
	movss	60(%rsp), %xmm1
	movq	%r12, %rsi
	movss	%xmm2, 256(%rsp)
	subss	4944(%rbx), %xmm2
	movss	%xmm1, 260(%rsp)
	subss	4948(%rbx), %xmm1
	movss	%xmm0, 264(%rsp)
	subss	4952(%rbx), %xmm0
	movss	%xmm2, 240(%rsp)
	movss	%xmm1, 244(%rsp)
	movss	%xmm0, 248(%rsp)
	call	vectoangles
	movq	%rbx, %rdi
	call	BotSelectActivateWeapon
	cmpl	$-1, %eax
	movl	%eax, 24(%rbp)
	je	.L53
	testl	%eax, %eax
	je	.L49
	movl	20(%rbp), %edx
	orl	$17, %edx
	movl	%edx, 20(%rbp)
	cmpl	160(%rbx), %eax
	jne	.L27
	leaq	6572(%rbx), %rdi
	movq	%r12, %rsi
	movss	.LC9(%rip), %xmm0
	call	InFieldOfVision
	testl	%eax, %eax
	je	.L49
	movl	12(%rbx), %r9d
	leaq	4944(%rbx), %rsi
	leaq	272(%rsp), %rdi
	leaq	256(%rsp), %r8
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movl	$100663297, (%rsp)
	call	BotAI_Trace
	movss	280(%rsp), %xmm0
	ucomiss	.LC7(%rip), %xmm0
	jae	.L31
	movl	352(%rsp), %eax
	cmpl	32(%rsp), %eax
	jne	.L49
.L31:
	movl	8(%rbx), %edi
	call	trap_EA_Attack
	.p2align 4,,10
	.p2align 3
.L49:
	movl	20(%rbp), %edx
	testb	$1, %dh
	je	.L54
.L32:
	movss	floattime(%rip), %xmm0
	movss	.LC10(%rip), %xmm1
	addss	%xmm0, %xmm1
	movss	%xmm1, 6216(%rbx)
	jmp	.L33
.L39:
	movl	4992(%rbx), %r9d
	testl	%r9d, %r9d
	jle	.L41
	movl	5052(%rbx), %r8d
	testl	%r8d, %r8d
	jle	.L41
	movl	$5, 24(%rbp)
	jmp	.L40
.L41:
	movl	5012(%rbx), %edi
	testl	%edi, %edi
	jle	.L42
	movl	5060(%rbx), %esi
	testl	%esi, %esi
	jle	.L42
	movl	$9, 24(%rbp)
	jmp	.L40
.L51:
	leaq	6572(%rbx), %rdi
	movq	%r12, %rsi
	movss	.LC9(%rip), %xmm0
	call	InFieldOfVision
	testl	%eax, %eax
	je	.L44
	movl	12(%rbx), %r9d
	leaq	4944(%rbx), %rsi
	leaq	272(%rsp), %rdi
	leaq	256(%rsp), %r8
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movl	$100663297, (%rsp)
	call	BotAI_Trace
	movss	280(%rsp), %xmm0
	ucomiss	.LC7(%rip), %xmm0
	jae	.L43
	movl	352(%rsp), %eax
	cmpl	32(%rsp), %eax
	jne	.L44
.L43:
	movl	8(%rbx), %edi
	call	trap_EA_Attack
	jmp	.L44
.L42:
	movl	$0, 24(%rbp)
	.p2align 4,,4
	jmp	.L44
.L53:
	movl	$0, 24(%rbp)
	movl	20(%rbp), %edx
	.p2align 4,,5
	jmp	.L27
.L52:
	.p2align 4,,5
	call	__stack_chk_fail
	.cfi_endproc
.LFE88:
	.size	BotClearPath, .-BotClearPath
	.p2align 4,,15
.globl BotReachedGoal
	.type	BotReachedGoal, @function
BotReachedGoal:
.LFB75:
	.cfi_startproc
	movq	%rbx, -24(%rsp)
	movq	%rbp, -16(%rsp)
	movq	%rsi, %rbx
	.cfi_offset 6, -24
	.cfi_offset 3, -32
	movq	%r12, -8(%rsp)
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	48(%rsi), %eax
	movq	%rdi, %rbp
	testb	$1, %al
	jne	.L71
	.cfi_offset 12, -16
	testb	%al, %al
	jns	.L65
	leaq	4916(%rdi), %rdi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	jne	.L58
	movss	floattime(%rip), %xmm0
	xorl	%eax, %eax
	subss	.LC7(%rip), %xmm0
	movss	6184(%rbp), %xmm1
	ucomiss	%xmm0, %xmm1
	seta	%al
	jmp	.L59
	.p2align 4,,10
	.p2align 3
.L65:
	leaq	4916(%rdi), %rdi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	setne	%al
	movzbl	%al, %eax
.L59:
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
	addq	$24, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L71:
	leaq	4916(%rdi), %r12
	movq	%r12, %rdi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L57
	testb	$4, 48(%rbx)
	je	.L72
.L58:
	movl	$1, %eax
	.p2align 4,,5
	jmp	.L59
	.p2align 4,,10
	.p2align 3
.L72:
	movl	44(%rbx), %esi
	movl	6536(%rbp), %edi
	movss	.LC6(%rip), %xmm0
	call	trap_BotSetAvoidGoalTime
	movl	$1, %eax
	jmp	.L59
	.p2align 4,,10
	.p2align 3
.L57:
	movl	12(%rbp), %edi
	leaq	6572(%rbp), %rdx
	leaq	4944(%rbp), %rsi
	movq	%rbx, %rcx
	call	trap_BotItemGoalInVisButNotVisible
	testl	%eax, %eax
	jne	.L58
	movl	4956(%rbp), %eax
	cmpl	12(%rbx), %eax
	je	.L73
.L60:
	xorl	%eax, %eax
	jmp	.L59
	.p2align 4,,10
	.p2align 3
.L73:
	movss	(%rbx), %xmm1
	movss	16(%rbx), %xmm2
	addss	%xmm1, %xmm2
	movss	4916(%rbp), %xmm0
	ucomiss	%xmm2, %xmm0
	jbe	.L60
	addss	28(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	jbe	.L60
	movss	4(%rbx), %xmm1
	movss	20(%rbx), %xmm2
	addss	%xmm1, %xmm2
	movss	4920(%rbp), %xmm0
	ucomiss	%xmm2, %xmm0
	jbe	.L60
	addss	32(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	jbe	.L60
	movq	%r12, %rdi
	call	trap_AAS_Swimming
	testl	%eax, %eax
	sete	%al
	movzbl	%al, %eax
	jmp	.L59
	.cfi_endproc
.LFE75:
	.size	BotReachedGoal, .-BotReachedGoal
	.p2align 4,,15
.globl BotGetItemLongTermGoal
	.type	BotGetItemLongTermGoal, @function
BotGetItemLongTermGoal:
.LFB76:
	.cfi_startproc
	movq	%rbx, -24(%rsp)
	movq	%rbp, -16(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -24
	.cfi_offset 3, -32
	movq	%r12, -8(%rsp)
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movl	6536(%rdi), %edi
	movl	%esi, %r12d
	.cfi_offset 12, -16
	movq	%rdx, %rsi
	movq	%rdx, %rbp
	call	trap_BotGetTopGoal
	testl	%eax, %eax
	jne	.L75
.L83:
	xorl	%eax, %eax
	movl	%eax, 6076(%rbx)
	movl	%eax, 12(%rsp)
	movss	12(%rsp), %xmm1
.L76:
	movss	floattime(%rip), %xmm0
	ucomiss	%xmm1, %xmm0
	ja	.L78
	movl	$1, %eax
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L78:
	movl	6536(%rbx), %edi
	call	trap_BotPopGoal
	movl	6536(%rbx), %edi
	leaq	4960(%rbx), %rdx
	leaq	4916(%rbx), %rsi
	movl	%r12d, %ecx
	call	trap_BotChooseLTGItem
	testl	%eax, %eax
	je	.L80
	movss	.LC9(%rip), %xmm0
	addss	floattime(%rip), %xmm0
	movss	%xmm0, 6076(%rbx)
.L81:
	movl	6536(%rbx), %edi
	movq	%rbp, %rsi
	movq	16(%rsp), %rbx
	movq	24(%rsp), %rbp
	movq	32(%rsp), %r12
	addq	$40, %rsp
	jmp	trap_BotGetTopGoal
	.p2align 4,,10
	.p2align 3
.L75:
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	BotReachedGoal
	testl	%eax, %eax
	jne	.L77
	movss	6076(%rbx), %xmm1
	jmp	.L76
	.p2align 4,,10
	.p2align 3
.L80:
	movl	6536(%rbx), %edi
	call	trap_BotResetAvoidGoals
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	jmp	.L81
	.p2align 4,,10
	.p2align 3
.L77:
	movq	%rbx, %rdi
	call	BotChooseWeapon
	.p2align 4,,6
	jmp	.L83
	.cfi_endproc
.LFE76:
	.size	BotGetItemLongTermGoal, .-BotGetItemLongTermGoal
	.p2align 4,,15
.globl BotGoForAir
	.type	BotGoForAir, @function
BotGoForAir:
.LFB73:
	.cfi_startproc
	movq	%rbx, -48(%rsp)
	movq	%r12, -32(%rsp)
	xorl	%eax, %eax
	movq	%r13, -24(%rsp)
	movq	%rbp, -40(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -48
	.cfi_offset 13, -32
	.cfi_offset 12, -40
	.cfi_offset 3, -56
	movq	%r14, -16(%rsp)
	movq	%r15, -8(%rsp)
	subq	$136, %rsp
	.cfi_def_cfa_offset 144
	movss	%xmm0, 12(%rsp)
	movl	%esi, %r13d
	movq	%rdx, %r12
	movss	floattime(%rip), %xmm0
	subss	.LC12(%rip), %xmm0
	ucomiss	6184(%rdi), %xmm0
	ja	.L92
	.cfi_offset 15, -16
	.cfi_offset 14, -24
.L86:
	movq	88(%rsp), %rbx
	movq	96(%rsp), %rbp
	movq	104(%rsp), %r12
	movq	112(%rsp), %r13
	movq	120(%rsp), %r14
	movq	128(%rsp), %r15
	addq	$136, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L92:
	leaq	16(%rsp), %rbp
	leaq	4960(%rbx), %r14
	leaq	4916(%rbx), %r15
	movq	%rbp, %rsi
	call	BotGetAirGoal
	testl	%eax, %eax
	je	.L88
	jmp	.L95
	.p2align 4,,10
	.p2align 3
.L90:
	movl	6536(%rbx), %edi
	movq	%rbp, %rsi
	call	trap_BotGetTopGoal
	movq	%rbp, %rdi
	call	trap_AAS_PointContents
	testb	$56, %al
	je	.L94
	movl	6536(%rbx), %edi
	call	trap_BotPopGoal
.L88:
	movl	6536(%rbx), %edi
	movss	12(%rsp), %xmm0
	movq	%r12, %r8
	movl	%r13d, %ecx
	movq	%r14, %rdx
	movq	%r15, %rsi
	call	trap_BotChooseNBGItem
	testl	%eax, %eax
	jne	.L90
	movl	6536(%rbx), %edi
	call	trap_BotResetAvoidGoals
	xorl	%eax, %eax
	jmp	.L86
	.p2align 4,,10
	.p2align 3
.L94:
	movl	$1, %eax
	jmp	.L86
.L95:
	movl	6536(%rbx), %edi
	movq	%rbp, %rsi
	call	trap_BotPushGoal
	movl	$1, %eax
	jmp	.L86
	.cfi_endproc
.LFE73:
	.size	BotGoForAir, .-BotGoForAir
	.p2align 4,,15
.globl BotNearbyGoal
	.type	BotNearbyGoal, @function
BotNearbyGoal:
.LFB74:
	.cfi_startproc
	movq	%rbx, -32(%rsp)
	movq	%r12, -16(%rsp)
	movl	%esi, %ebx
	.cfi_offset 12, -24
	.cfi_offset 3, -40
	movq	%r13, -8(%rsp)
	movq	%rbp, -24(%rsp)
	subq	$56, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdi, %r12
	movq	%rdx, %r13
	.cfi_offset 6, -32
	.cfi_offset 13, -16
	movss	%xmm0, 12(%rsp)
	call	BotGoForAir
	testl	%eax, %eax
	je	.L97
	movl	$1, %eax
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	addq	$56, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L97:
	movq	%r12, %rdi
	leaq	4916(%r12), %rbp
	call	BotCTFCarryingFlag
	testl	%eax, %eax
	jne	.L102
.L99:
	movl	6536(%r12), %edi
	leaq	4960(%r12), %rdx
	movq	%r13, %r8
	movss	12(%rsp), %xmm0
	movl	%ebx, %ecx
	movq	%rbp, %rsi
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	addq	$56, %rsp
	jmp	trap_BotChooseNBGItem
	.p2align 4,,10
	.p2align 3
.L102:
	movl	6644(%r12), %edx
	movl	4956(%r12), %edi
	movl	$18616254, %ecx
	movq	%rbp, %rsi
	call	trap_AAS_AreaTravelTimeToGoalArea
	cmpl	$299, %eax
	jg	.L99
	movss	.LC13(%rip), %xmm0
	movss	%xmm0, 12(%rsp)
	jmp	.L99
	.cfi_endproc
.LFE74:
	.size	BotNearbyGoal, .-BotNearbyGoal
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC14:
	.string	"%s at %2.1f entered %s: %s from %s\n"
	.text
	.p2align 4,,15
.globl BotRecordNodeSwitch
	.type	BotRecordNodeSwitch, @function
BotRecordNodeSwitch:
.LFB71:
	.cfi_startproc
	movq	%rbx, -32(%rsp)
	movq	%rbp, -24(%rsp)
	movq	%rsi, %rbp
	.cfi_offset 6, -32
	.cfi_offset 3, -40
	movq	%r12, -16(%rsp)
	movq	%r13, -8(%rsp)
	subq	$104, %rsp
	.cfi_def_cfa_offset 112
	leaq	16(%rsp), %rbx
	movl	8(%rdi), %edi
	movq	%rdx, %r12
	.cfi_offset 13, -16
	.cfi_offset 12, -24
	movl	$36, %edx
	movq	%rcx, %r13
	movq	%rbx, %rsi
	movq	%fs:40, %rax
	movq	%rax, 56(%rsp)
	xorl	%eax, %eax
	call	ClientName
	movslq	numnodeswitches(%rip),%rax
	movss	floattime(%rip), %xmm0
	movq	%r12, %r9
	movq	%rbp, %r8
	movq	%rbx, %rcx
	movl	$.LC14, %edx
	movl	$144, %esi
	cvtps2pd	%xmm0, %xmm0
	movq	%r13, (%rsp)
	leaq	(%rax,%rax,8), %rdi
	movl	$1, %eax
	salq	$4, %rdi
	addq	$nodeswitch, %rdi
	call	Com_sprintf
	addl	$1, numnodeswitches(%rip)
	movq	56(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L106
	movq	72(%rsp), %rbx
	movq	80(%rsp), %rbp
	movq	88(%rsp), %r12
	movq	96(%rsp), %r13
	addq	$104, %rsp
	ret
.L106:
	call	__stack_chk_fail
	.cfi_endproc
.LFE71:
	.size	BotRecordNodeSwitch, .-BotRecordNodeSwitch
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC15:
	.string	""
.LC16:
	.string	"battle NBG"
	.text
	.p2align 4,,15
.globl AIEnter_Battle_NBG
	.type	AIEnter_Battle_NBG, @function
AIEnter_Battle_NBG:
.LFB102:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	$.LC15, %edx
	movl	$.LC16, %esi
	call	BotRecordNodeSwitch
	movq	$AINode_Battle_NBG, 4904(%rbx)
	popq	%rbx
	ret
	.cfi_endproc
.LFE102:
	.size	AIEnter_Battle_NBG, .-AIEnter_Battle_NBG
	.section	.rodata.str1.1
.LC17:
	.string	"battle retreat"
	.text
	.p2align 4,,15
.globl AIEnter_Battle_Retreat
	.type	AIEnter_Battle_Retreat, @function
AIEnter_Battle_Retreat:
.LFB100:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	$.LC15, %edx
	movl	$.LC17, %esi
	call	BotRecordNodeSwitch
	movq	$AINode_Battle_Retreat, 4904(%rbx)
	popq	%rbx
	ret
	.cfi_endproc
.LFE100:
	.size	AIEnter_Battle_Retreat, .-AIEnter_Battle_Retreat
	.section	.rodata.str1.1
.LC18:
	.string	"battle chase"
	.text
	.p2align 4,,15
.globl AIEnter_Battle_Chase
	.type	AIEnter_Battle_Chase, @function
AIEnter_Battle_Chase:
.LFB98:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movl	$.LC15, %edx
	movl	$.LC18, %esi
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	call	BotRecordNodeSwitch
	movl	floattime(%rip), %eax
	movq	$AINode_Battle_Chase, 4904(%rbx)
	movl	%eax, 6092(%rbx)
	popq	%rbx
	ret
	.cfi_endproc
.LFE98:
	.size	AIEnter_Battle_Chase, .-AIEnter_Battle_Chase
	.section	.rodata.str1.1
.LC19:
	.string	"battle fight"
	.text
	.p2align 4,,15
.globl AIEnter_Battle_SuicidalFight
	.type	AIEnter_Battle_SuicidalFight, @function
AIEnter_Battle_SuicidalFight:
.LFB96:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	$.LC15, %edx
	movl	$.LC19, %esi
	call	BotRecordNodeSwitch
	movl	6532(%rbx), %edi
	call	trap_BotResetLastAvoidReach
	movq	$AINode_Battle_Fight, 4904(%rbx)
	orl	$64, 5988(%rbx)
	popq	%rbx
	ret
	.cfi_endproc
.LFE96:
	.size	AIEnter_Battle_SuicidalFight, .-AIEnter_Battle_SuicidalFight
	.p2align 4,,15
.globl AIEnter_Battle_Fight
	.type	AIEnter_Battle_Fight, @function
AIEnter_Battle_Fight:
.LFB95:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	$.LC15, %edx
	movl	$.LC19, %esi
	call	BotRecordNodeSwitch
	movl	6532(%rbx), %edi
	call	trap_BotResetLastAvoidReach
	movq	$AINode_Battle_Fight, 4904(%rbx)
	popq	%rbx
	ret
	.cfi_endproc
.LFE95:
	.size	AIEnter_Battle_Fight, .-AIEnter_Battle_Fight
	.section	.rodata.str1.1
.LC20:
	.string	"seek LTG"
.LC21:
	.string	"no goal"
	.text
	.p2align 4,,15
.globl AIEnter_Seek_LTG
	.type	AIEnter_Seek_LTG, @function
AIEnter_Seek_LTG:
.LFB93:
	.cfi_startproc
	movq	%rbx, -24(%rsp)
	movq	%rbp, -16(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -24
	.cfi_offset 3, -32
	movq	%r12, -8(%rsp)
	subq	$248, %rsp
	.cfi_def_cfa_offset 256
	movl	6536(%rdi), %edi
	movq	%rsi, %rbp
	movq	%rsp, %rsi
	movq	%fs:40, %rax
	movq	%rax, 216(%rsp)
	xorl	%eax, %eax
	.cfi_offset 12, -16
	call	trap_BotGetTopGoal
	testl	%eax, %eax
	jne	.L122
	movq	%rbp, %rcx
	movl	$.LC21, %edx
	movl	$.LC20, %esi
	movq	%rbx, %rdi
	call	BotRecordNodeSwitch
.L119:
	movq	216(%rsp), %rax
	xorq	%fs:40, %rax
	movq	$AINode_Seek_LTG, 4904(%rbx)
	jne	.L123
	movq	224(%rsp), %rbx
	movq	232(%rsp), %rbp
	movq	240(%rsp), %r12
	addq	$248, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L122:
	leaq	64(%rsp), %r12
	movl	44(%rsp), %edi
	movl	$144, %edx
	movq	%r12, %rsi
	call	trap_BotGoalName
	movq	%rbp, %rcx
	movq	%r12, %rdx
	movl	$.LC20, %esi
	movq	%rbx, %rdi
	call	BotRecordNodeSwitch
	jmp	.L119
.L123:
	call	__stack_chk_fail
	.cfi_endproc
.LFE93:
	.size	AIEnter_Seek_LTG, .-AIEnter_Seek_LTG
	.section	.rodata.str1.1
.LC22:
	.string	"respawn: respawned"
	.text
	.p2align 4,,15
.globl AINode_Respawn
	.type	AINode_Respawn, @function
AINode_Respawn:
.LFB86:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movl	5992(%rdi), %eax
	testl	%eax, %eax
	jne	.L142
	movss	floattime(%rip), %xmm0
	ucomiss	6084(%rdi), %xmm0
	xorps	%xmm1, %xmm1
	ja	.L143
.L127:
	movss	6088(%rbx), %xmm0
	ucomiss	%xmm1, %xmm0
	jp	.L141
	je	.L129
.L141:
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	floattime(%rip), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC23(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	ja	.L144
.L129:
	movl	$1, %eax
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L144:
	movl	8(%rbx), %edi
	call	trap_EA_Talk
	movl	$1, %eax
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L142:
	call	BotIsDead
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L126
	movl	$.LC22, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorps	%xmm1, %xmm1
	jmp	.L127
	.p2align 4,,10
	.p2align 3
.L143:
	movl	$1, 5992(%rdi)
	movl	8(%rdi), %edi
	call	trap_EA_Respawn
	xorps	%xmm1, %xmm1
	ucomiss	6088(%rbx), %xmm1
	jp	.L140
	je	.L129
.L140:
	movl	6060(%rbx), %edx
	movl	6540(%rbx), %edi
	xorl	%esi, %esi
	movss	%xmm1, (%rsp)
	call	trap_BotEnterChat
	movl	$-1, 6548(%rbx)
	movss	(%rsp), %xmm1
	jmp	.L127
	.p2align 4,,10
	.p2align 3
.L126:
	movl	8(%rbx), %edi
	call	trap_EA_Respawn
	xorps	%xmm1, %xmm1
	jmp	.L127
	.cfi_endproc
.LFE86:
	.size	AINode_Respawn, .-AINode_Respawn
	.section	.rodata.str1.1
.LC25:
	.string	"stand: found enemy"
.LC26:
	.string	"stand: time out"
	.text
	.p2align 4,,15
.globl AINode_Stand
	.type	AINode_Stand, @function
AINode_Stand:
.LFB84:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movl	6052(%rdi), %eax
	cmpl	5076(%rdi), %eax
	jg	.L146
.L156:
	movss	floattime(%rip), %xmm2
.L147:
	ucomiss	6120(%rbx), %xmm2
	jbe	.L149
	movl	$-1, %esi
	movq	%rbx, %rdi
	call	BotFindEnemy
	testl	%eax, %eax
	jne	.L157
	movss	.LC7(%rip), %xmm0
	addss	floattime(%rip), %xmm0
	movss	%xmm0, 6120(%rbx)
.L149:
	movl	8(%rbx), %edi
	call	trap_EA_Talk
	movss	floattime(%rip), %xmm0
	movl	$1, %eax
	ucomiss	6104(%rbx), %xmm0
	jbe	.L152
	movl	6060(%rbx), %edx
	movl	6540(%rbx), %edi
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC26, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
.L152:
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L146:
	call	BotChat_HitTalking
	testl	%eax, %eax
	.p2align 4,,2
	je	.L156
	movq	%rbx, %rdi
	call	BotChatTime
	addss	floattime(%rip), %xmm0
	movsd	.LC24(%rip), %xmm1
	movq	%rbx, %rdi
	movsd	%xmm1, (%rsp)
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	addsd	%xmm1, %xmm0
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm0
	movss	%xmm0, 6120(%rbx)
	call	BotChatTime
	movss	floattime(%rip), %xmm2
	addss	%xmm2, %xmm0
	movsd	(%rsp), %xmm1
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	addsd	%xmm1, %xmm0
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm0
	movss	%xmm0, 6104(%rbx)
	jmp	.L147
	.p2align 4,,10
	.p2align 3
.L157:
	movq	%rbx, %rdi
	movl	$.LC25, %esi
	call	AIEnter_Battle_Fight
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbx
	ret
	.cfi_endproc
.LFE84:
	.size	AINode_Stand, .-AINode_Stand
	.section	.rodata.str1.1
.LC27:
	.string	"seek NBG"
	.text
	.p2align 4,,15
.globl AIEnter_Seek_NBG
	.type	AIEnter_Seek_NBG, @function
AIEnter_Seek_NBG:
.LFB91:
	.cfi_startproc
	movq	%rbx, -24(%rsp)
	movq	%rbp, -16(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -24
	.cfi_offset 3, -32
	movq	%r12, -8(%rsp)
	subq	$248, %rsp
	.cfi_def_cfa_offset 256
	movl	6536(%rdi), %edi
	movq	%rsi, %rbp
	movq	%rsp, %rsi
	movq	%fs:40, %rax
	movq	%rax, 216(%rsp)
	xorl	%eax, %eax
	.cfi_offset 12, -16
	call	trap_BotGetTopGoal
	testl	%eax, %eax
	jne	.L163
	movq	%rbp, %rcx
	movl	$.LC21, %edx
	movl	$.LC27, %esi
	movq	%rbx, %rdi
	call	BotRecordNodeSwitch
.L160:
	movq	216(%rsp), %rax
	xorq	%fs:40, %rax
	movq	$AINode_Seek_NBG, 4904(%rbx)
	jne	.L164
	movq	224(%rsp), %rbx
	movq	232(%rsp), %rbp
	movq	240(%rsp), %r12
	addq	$248, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L163:
	leaq	64(%rsp), %r12
	movl	44(%rsp), %edi
	movl	$144, %edx
	movq	%r12, %rsi
	call	trap_BotGoalName
	movq	%rbp, %rcx
	movq	%r12, %rdx
	movl	$.LC27, %esi
	movq	%rbx, %rdi
	call	BotRecordNodeSwitch
	jmp	.L160
.L164:
	call	__stack_chk_fail
	.cfi_endproc
.LFE91:
	.size	AIEnter_Seek_NBG, .-AIEnter_Seek_NBG
	.section	.rodata.str1.1
.LC28:
	.string	"activate entity"
	.text
	.p2align 4,,15
.globl AIEnter_Seek_ActivateEntity
	.type	AIEnter_Seek_ActivateEntity, @function
AIEnter_Seek_ActivateEntity:
.LFB89:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	$.LC15, %edx
	movl	$.LC28, %esi
	call	BotRecordNodeSwitch
	movq	$AINode_Seek_ActivateEntity, 4904(%rbx)
	popq	%rbx
	ret
	.cfi_endproc
.LFE89:
	.size	AIEnter_Seek_ActivateEntity, .-AIEnter_Seek_ActivateEntity
	.section	.rodata.str1.1
.LC29:
	.string	"respawn"
	.text
	.p2align 4,,15
.globl AIEnter_Respawn
	.type	AIEnter_Respawn, @function
AIEnter_Respawn:
.LFB85:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	$.LC15, %edx
	movl	$.LC29, %esi
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	call	BotRecordNodeSwitch
	movl	6532(%rbx), %edi
	call	trap_BotResetMoveState
	movl	6536(%rbx), %edi
	call	trap_BotResetGoalState
	movl	6536(%rbx), %edi
	call	trap_BotResetAvoidGoals
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movq	%rbx, %rdi
	call	BotChat_Death
	testl	%eax, %eax
	jne	.L171
	movss	.LC7(%rip), %xmm1
	addss	floattime(%rip), %xmm1
	movss	%xmm1, (%rsp)
	call	rand
	andl	$32767, %eax
	movss	(%rsp), %xmm1
	cvtsi2ss	%eax, %xmm0
	movl	$0x00000000, 6088(%rbx)
	divss	.LC30(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, 6084(%rbx)
.L169:
	movl	$0, 5992(%rbx)
	movq	$AINode_Respawn, 4904(%rbx)
	addq	$16, %rsp
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L171:
	movq	%rbx, %rdi
	call	BotChatTime
	movss	floattime(%rip), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm1, 6088(%rbx)
	movss	%xmm0, 6084(%rbx)
	jmp	.L169
	.cfi_endproc
.LFE85:
	.size	AIEnter_Respawn, .-AIEnter_Respawn
	.section	.rodata.str1.1
.LC31:
	.string	"stand"
	.text
	.p2align 4,,15
.globl AIEnter_Stand
	.type	AIEnter_Stand, @function
AIEnter_Stand:
.LFB83:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movl	$.LC15, %edx
	movl	$.LC31, %esi
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	call	BotRecordNodeSwitch
	movss	.LC7(%rip), %xmm0
	movq	$AINode_Stand, 4904(%rbx)
	addss	floattime(%rip), %xmm0
	movss	%xmm0, 6120(%rbx)
	popq	%rbx
	ret
	.cfi_endproc
.LFE83:
	.size	AIEnter_Stand, .-AIEnter_Stand
	.section	.rodata.str1.1
.LC32:
	.string	"observer: left observer"
	.text
	.p2align 4,,15
.globl AINode_Observer
	.type	AINode_Observer, @function
AINode_Observer:
.LFB82:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	call	BotIsObserver
	testl	%eax, %eax
	jne	.L175
	movl	$.LC32, %esi
	movq	%rbx, %rdi
	call	AIEnter_Stand
.L175:
	movl	$1, %eax
	popq	%rbx
	ret
	.cfi_endproc
.LFE82:
	.size	AINode_Observer, .-AINode_Observer
	.section	.rodata.str1.1
.LC33:
	.string	"intermission: chat"
	.text
	.p2align 4,,15
.globl AINode_Intermission
	.type	AINode_Intermission, @function
AINode_Intermission:
.LFB80:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	call	BotIntermission
	testl	%eax, %eax
	je	.L182
	movl	$1, %eax
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L182:
	movq	%rbx, %rdi
	.p2align 4,,5
	call	BotChat_StartLevel
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L183
	movss	.LC3(%rip), %xmm0
	addss	floattime(%rip), %xmm0
	movss	%xmm0, 6104(%rbx)
.L180:
	movq	%rbx, %rdi
	movl	$.LC33, %esi
	call	AIEnter_Stand
	movl	$1, %eax
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L183:
	movq	%rbx, %rdi
	call	BotChatTime
	addss	floattime(%rip), %xmm0
	movss	%xmm0, 6104(%rbx)
	jmp	.L180
	.cfi_endproc
.LFE80:
	.size	AINode_Intermission, .-AINode_Intermission
	.section	.rodata.str1.1
.LC34:
	.string	"observer"
	.text
	.p2align 4,,15
.globl AIEnter_Observer
	.type	AIEnter_Observer, @function
AIEnter_Observer:
.LFB81:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	$.LC15, %edx
	movl	$.LC34, %esi
	call	BotRecordNodeSwitch
	movq	%rbx, %rdi
	call	BotResetState
	movq	$AINode_Observer, 4904(%rbx)
	popq	%rbx
	ret
	.cfi_endproc
.LFE81:
	.size	AIEnter_Observer, .-AIEnter_Observer
	.section	.rodata.str1.1
.LC35:
	.string	"intermission"
	.text
	.p2align 4,,15
.globl AIEnter_Intermission
	.type	AIEnter_Intermission, @function
AIEnter_Intermission:
.LFB79:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rsi, %rcx
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	$.LC15, %edx
	movl	$.LC35, %esi
	call	BotRecordNodeSwitch
	movq	%rbx, %rdi
	call	BotResetState
	movq	%rbx, %rdi
	call	BotChat_EndLevel
	testl	%eax, %eax
	je	.L187
	movl	6060(%rbx), %edx
	movl	6540(%rbx), %edi
	xorl	%esi, %esi
	call	trap_BotEnterChat
.L187:
	movq	$AINode_Intermission, 4904(%rbx)
	popq	%rbx
	ret
	.cfi_endproc
.LFE79:
	.size	AIEnter_Intermission, .-AIEnter_Intermission
	.section	.rodata.str1.1
.LC36:
	.string	"battle nbg: observer"
.LC37:
	.string	"battle nbg: intermission"
.LC38:
	.string	"battle nbg: bot dead"
.LC39:
	.string	"battle nbg: no enemy"
.LC40:
	.string	"battle nbg: enemy dead"
.LC43:
	.string	"battle nbg: time out"
	.text
	.p2align 4,,15
.globl AINode_Battle_NBG
	.type	AINode_Battle_NBG, @function
AINode_Battle_NBG:
.LFB103:
	.cfi_startproc
	movq	%rbx, -24(%rsp)
	movq	%rbp, -16(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -24
	.cfi_offset 3, -32
	movq	%r12, -8(%rsp)
	subq	$328, %rsp
	.cfi_def_cfa_offset 336
	.cfi_offset 12, -16
	call	BotIsObserver
	testl	%eax, %eax
	jne	.L225
	movq	%rbx, %rdi
	call	BotIntermission
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L226
	movq	%rbx, %rdi
	call	BotIsDead
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L227
	movl	6548(%rbx), %edi
	testl	%edi, %edi
	js	.L228
	movq	%rsp, %rsi
	call	BotEntityInfo
	movq	%rsp, %rdi
	call	EntityIsDead
	testl	%eax, %eax
	jne	.L229
	movl	$18616254, 5984(%rbx)
	movl	bot_grapple+12(%rip), %ecx
	testl	%ecx, %ecx
	je	.L196
	movl	$18632638, 5984(%rbx)
.L196:
	movq	%rbx, %rdi
	call	BotInLavaOrSlime
	testl	%eax, %eax
	je	.L197
	orl	$6291456, 5984(%rbx)
.L197:
	movq	%rbx, %rdi
	call	BotCanAndWantsToRocketJump
	testl	%eax, %eax
	je	.L198
	orl	$4096, 5984(%rbx)
.L198:
	movq	%rbx, %rdi
	call	BotMapScripts
	movl	6548(%rbx), %ecx
	movl	12(%rbx), %edi
	leaq	6572(%rbx), %rdx
	leaq	4944(%rbx), %rsi
	movss	.LC41(%rip), %xmm0
	call	BotEntityVisible
	xorps	%xmm1, %xmm1
	ucomiss	%xmm1, %xmm0
	jp	.L224
	je	.L199
.L224:
	movl	floattime(%rip), %eax
	movss	32(%rsp), %xmm0
	movss	%xmm0, 296(%rsp)
	movl	%eax, 6096(%rbx)
	movl	24(%rsp), %eax
	movl	%eax, 288(%rsp)
	movl	28(%rsp), %eax
	movl	%eax, 292(%rsp)
	movl	6548(%rbx), %eax
	cmpl	$63, %eax
	jg	.L230
.L201:
	leaq	288(%rsp), %rdi
	call	BotPointAreaNum
	testl	%eax, %eax
	movl	%eax, %ebp
	jne	.L231
.L199:
	leaq	144(%rsp), %rbp
	movl	6536(%rbx), %edi
	movq	%rbp, %rsi
	call	trap_BotGetTopGoal
	testl	%eax, %eax
	jne	.L203
.L205:
	xorps	%xmm1, %xmm1
	movl	$0x00000000, 6080(%rbx)
.L204:
	movss	floattime(%rip), %xmm0
	ucomiss	%xmm1, %xmm0
	jbe	.L221
	movl	6536(%rbx), %edi
	call	trap_BotPopGoal
	movl	6536(%rbx), %edi
	movq	%rbp, %rsi
	call	trap_BotGetTopGoal
	testl	%eax, %eax
	je	.L208
	movl	$.LC43, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Retreat
	xorl	%eax, %eax
	jmp	.L191
	.p2align 4,,10
	.p2align 3
.L226:
	movl	$.LC37, %esi
	movq	%rbx, %rdi
	call	AIEnter_Intermission
	xorl	%eax, %eax
.L191:
	movq	304(%rsp), %rbx
	movq	312(%rsp), %rbp
	movq	320(%rsp), %r12
	addq	$328, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L225:
	movl	$.LC36, %esi
	movq	%rbx, %rdi
	call	AIEnter_Observer
	xorl	%eax, %eax
	jmp	.L191
	.p2align 4,,10
	.p2align 3
.L221:
	movq	%rbx, %rdi
	leaq	208(%rsp), %r12
	call	BotSetupForMovement
	movl	5984(%rbx), %ecx
	movl	6532(%rbx), %esi
	movq	%rbp, %rdx
	movq	%r12, %rdi
	call	trap_BotMoveToGoal
	movl	208(%rsp), %edx
	testl	%edx, %edx
	jne	.L232
.L209:
	xorl	%edx, %edx
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	BotAIBlocked
	movl	6548(%rbx), %esi
	movq	%rbx, %rdi
	call	BotUpdateBattleInventory
	movq	%rbx, %rdi
	call	BotChooseWeapon
	movl	228(%rsp), %eax
	testb	$3, %al
	jne	.L233
	testb	$8, %al
	jne	.L211
	testb	$32, 5988(%rbx)
	je	.L234
	.p2align 4,,10
	.p2align 3
.L211:
	testb	$16, %al
	.p2align 4,,2
	je	.L216
	movl	232(%rsp), %eax
	movl	%eax, 6568(%rbx)
.L216:
	movq	%rbx, %rdi
	call	BotCheckAttack
	movl	$1, %eax
	jmp	.L191
	.p2align 4,,10
	.p2align 3
.L227:
	movl	$.LC38, %esi
	movq	%rbx, %rdi
	call	AIEnter_Respawn
	xorl	%eax, %eax
	jmp	.L191
	.p2align 4,,10
	.p2align 3
.L228:
	movl	$.LC39, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_NBG
	xorl	%eax, %eax
	jmp	.L191
	.p2align 4,,10
	.p2align 3
.L203:
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	BotReachedGoal
	testl	%eax, %eax
	jne	.L205
	movss	6080(%rbx), %xmm1
	jmp	.L204
	.p2align 4,,10
	.p2align 3
.L229:
	movl	$.LC40, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_NBG
	xorl	%eax, %eax
	jmp	.L191
	.p2align 4,,10
	.p2align 3
.L230:
	cmpl	redobelisk+40(%rip), %eax
	je	.L202
	cmpl	blueobelisk+40(%rip), %eax
	jne	.L201
.L202:
	addss	.LC42(%rip), %xmm0
	movss	%xmm0, 296(%rsp)
	jmp	.L201
	.p2align 4,,10
	.p2align 3
.L233:
	movl	248(%rsp), %edx
	movl	%edx, 6584(%rbx)
	movl	252(%rsp), %edx
	movl	%edx, 6588(%rbx)
	movl	256(%rsp), %edx
	movl	%edx, 6592(%rbx)
	jmp	.L211
	.p2align 4,,10
	.p2align 3
.L208:
	movl	$.LC43, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Fight
	xorl	%eax, %eax
	jmp	.L191
	.p2align 4,,10
	.p2align 3
.L232:
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movl	$0x00000000, 6080(%rbx)
	jmp	.L209
	.p2align 4,,10
	.p2align 3
.L231:
	movl	%eax, %edi
	call	trap_AAS_AreaReachability
	testl	%eax, %eax
	je	.L199
	movl	288(%rsp), %eax
	movl	%ebp, 6552(%rbx)
	movl	%eax, 6556(%rbx)
	movl	292(%rsp), %eax
	movl	%eax, 6560(%rbx)
	movl	296(%rsp), %eax
	movl	%eax, 6564(%rbx)
	jmp	.L199
.L234:
	movl	6528(%rbx), %edi
	xorps	%xmm0, %xmm0
	movss	.LC7(%rip), %xmm1
	movl	$2, %esi
	call	trap_Characteristic_BFloat
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	ucomisd	.LC44(%rip), %xmm0
	jbe	.L222
	movq	%rbx, %rdi
	call	BotAimAtEnemy
	movl	228(%rsp), %eax
	jmp	.L211
.L222:
	movl	5984(%rbx), %edx
	movl	6532(%rbx), %edi
	leaq	288(%rsp), %rcx
	movss	.LC11(%rip), %xmm0
	movq	%rbp, %rsi
	call	trap_BotMovementViewTarget
	testl	%eax, %eax
	je	.L214
	movss	288(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	272(%rsp), %rdi
	movss	%xmm0, 272(%rsp)
	movss	292(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 276(%rsp)
	movss	296(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 280(%rsp)
	call	vectoangles
.L215:
	movss	.LC45(%rip), %xmm0
	movl	228(%rsp), %eax
	mulss	6592(%rbx), %xmm0
	movss	%xmm0, 6592(%rbx)
	jmp	.L211
.L214:
	leaq	6584(%rbx), %rsi
	leaq	28(%r12), %rdi
	call	vectoangles
	jmp	.L215
	.cfi_endproc
.LFE103:
	.size	AINode_Battle_NBG, .-AINode_Battle_NBG
	.section	.rodata.str1.1
.LC46:
	.string	"battle chase: observer"
.LC47:
	.string	"battle chase: intermission"
.LC48:
	.string	"battle chase: bot dead"
.LC49:
	.string	"battle chase: no enemy"
.LC50:
	.string	"battle chase: better enemy"
.LC51:
	.string	"battle chase: no enemy area"
.LC54:
	.string	"battle chase: time out"
.LC58:
	.string	"battle chase: nbg"
	.section	.rodata.str1.8
	.align 8
.LC59:
	.string	"battle chase: wants to retreat"
	.text
	.p2align 4,,15
.globl AINode_Battle_Chase
	.type	AINode_Battle_Chase, @function
AINode_Battle_Chase:
.LFB99:
	.cfi_startproc
	movq	%rbx, -24(%rsp)
	movq	%rbp, -16(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -24
	.cfi_offset 3, -32
	movq	%r12, -8(%rsp)
	subq	$184, %rsp
	.cfi_def_cfa_offset 192
	.cfi_offset 12, -16
	call	BotIsObserver
	testl	%eax, %eax
	jne	.L277
	movq	%rbx, %rdi
	call	BotIntermission
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L278
	movq	%rbx, %rdi
	call	BotIsDead
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L279
	movl	6548(%rbx), %ecx
	testl	%ecx, %ecx
	js	.L280
	movl	12(%rbx), %edi
	leaq	6572(%rbx), %rdx
	leaq	4944(%rbx), %rsi
	movss	.LC41(%rip), %xmm0
	call	BotEntityVisible
	xorps	%xmm1, %xmm1
	ucomiss	%xmm1, %xmm0
	jp	.L275
	jne	.L275
	movl	$-1, %esi
	movq	%rbx, %rdi
	call	BotFindEnemy
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L281
	movl	6552(%rbx), %ebp
	testl	%ebp, %ebp
	je	.L282
	movl	$18616254, 5984(%rbx)
	movl	bot_grapple+12(%rip), %edi
	testl	%edi, %edi
	je	.L245
	movl	$18632638, 5984(%rbx)
.L245:
	movq	%rbx, %rdi
	call	BotInLavaOrSlime
	testl	%eax, %eax
	je	.L246
	orl	$6291456, 5984(%rbx)
.L246:
	movq	%rbx, %rdi
	call	BotCanAndWantsToRocketJump
	testl	%eax, %eax
	je	.L247
	orl	$4096, 5984(%rbx)
.L247:
	movq	%rbx, %rdi
	call	BotMapScripts
	movl	6548(%rbx), %eax
	leaq	4916(%rbx), %rdi
	movq	%rsp, %rsi
	movl	$0xc1000000, 16(%rsp)
	movl	$0xc1000000, 20(%rsp)
	movl	$0xc1000000, 24(%rsp)
	movl	$0x41000000, 28(%rsp)
	movl	%eax, 40(%rsp)
	movl	6552(%rbx), %eax
	movl	$0x41000000, 32(%rsp)
	movl	$0x41000000, 36(%rsp)
	movl	%eax, 12(%rsp)
	movl	6556(%rbx), %eax
	movl	%eax, (%rsp)
	movl	6560(%rbx), %eax
	movl	%eax, 4(%rsp)
	movl	6564(%rbx), %eax
	movl	%eax, 8(%rsp)
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	jne	.L283
	xorps	%xmm0, %xmm0
	movss	6092(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	jp	.L276
	je	.L249
.L276:
	movss	floattime(%rip), %xmm0
	movaps	%xmm0, %xmm2
	subss	.LC53(%rip), %xmm2
	ucomiss	%xmm1, %xmm2
	ja	.L249
	ucomiss	6100(%rbx), %xmm0
	jbe	.L253
	addss	.LC7(%rip), %xmm0
	movl	5984(%rbx), %esi
	movq	%rsp, %rdx
	movq	%rbx, %rdi
	movss	%xmm0, 6100(%rbx)
	movss	.LC55(%rip), %xmm0
	call	BotNearbyGoal
	testl	%eax, %eax
	jne	.L284
.L253:
	movl	6548(%rbx), %esi
	movq	%rbx, %rdi
	leaq	64(%rsp), %r12
	call	BotUpdateBattleInventory
	movq	%rbx, %rdi
	call	BotSetupForMovement
	movl	6532(%rbx), %esi
	movl	5984(%rbx), %ecx
	movq	%rsp, %rdx
	movq	%r12, %rdi
	call	trap_BotMoveToGoal
	movl	64(%rsp), %esi
	testl	%esi, %esi
	jne	.L285
.L255:
	xorl	%edx, %edx
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	BotAIBlocked
	movl	84(%rsp), %eax
	testb	$11, %al
	je	.L256
	movl	104(%rsp), %edx
	movl	%edx, 6584(%rbx)
	movl	108(%rsp), %edx
	movl	%edx, 6588(%rbx)
	movl	112(%rsp), %edx
	movl	%edx, 6592(%rbx)
.L257:
	testb	$16, %al
	je	.L262
	movl	88(%rsp), %eax
	movl	%eax, 6568(%rbx)
.L262:
	movl	4956(%rbx), %eax
	cmpl	6552(%rbx), %eax
	je	.L286
.L263:
	movq	%rbx, %rdi
	call	BotWantsToRetreat
	movl	%eax, %edx
	movl	$1, %eax
	testl	%edx, %edx
	je	.L237
	movl	$.LC59, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Retreat
	movl	$1, %eax
	jmp	.L237
	.p2align 4,,10
	.p2align 3
.L278:
	movl	$.LC47, %esi
	movq	%rbx, %rdi
	call	AIEnter_Intermission
	xorl	%eax, %eax
.L237:
	movq	160(%rsp), %rbx
	movq	168(%rsp), %rbp
	movq	176(%rsp), %r12
	addq	$184, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L277:
	movl	$.LC46, %esi
	movq	%rbx, %rdi
	call	AIEnter_Observer
	xorl	%eax, %eax
	jmp	.L237
	.p2align 4,,10
	.p2align 3
.L275:
	movl	$.LC18, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Fight
	xorl	%eax, %eax
	jmp	.L237
	.p2align 4,,10
	.p2align 3
.L279:
	movl	$.LC48, %esi
	movq	%rbx, %rdi
	call	AIEnter_Respawn
	xorl	%eax, %eax
	jmp	.L237
	.p2align 4,,10
	.p2align 3
.L280:
	movl	$.LC49, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L237
	.p2align 4,,10
	.p2align 3
.L283:
	movl	$0x00000000, 6092(%rbx)
.L249:
	movl	$.LC54, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L237
	.p2align 4,,10
	.p2align 3
.L281:
	movl	$.LC50, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Fight
	xorl	%eax, %eax
	jmp	.L237
	.p2align 4,,10
	.p2align 3
.L282:
	movl	$.LC51, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L237
	.p2align 4,,10
	.p2align 3
.L256:
	testb	$32, 5988(%rbx)
	jne	.L257
	movss	floattime(%rip), %xmm0
	subss	.LC3(%rip), %xmm0
	movss	6092(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	jbe	.L272
	movq	%rbx, %rdi
	call	BotAimAtEnemy
.L260:
	movss	.LC45(%rip), %xmm0
	movl	84(%rsp), %eax
	mulss	6592(%rbx), %xmm0
	movss	%xmm0, 6592(%rbx)
	jmp	.L257
	.p2align 4,,10
	.p2align 3
.L285:
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movl	$0x00000000, 6076(%rbx)
	jmp	.L255
	.p2align 4,,10
	.p2align 3
.L286:
	movl	$0x00000000, 6092(%rbx)
	jmp	.L263
.L272:
	movl	5984(%rbx), %edx
	movl	6532(%rbx), %edi
	leaq	144(%rsp), %rcx
	movss	.LC11(%rip), %xmm0
	movq	%rsp, %rsi
	call	trap_BotMovementViewTarget
	testl	%eax, %eax
	je	.L261
	movss	144(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	128(%rsp), %rdi
	movss	%xmm0, 128(%rsp)
	movss	148(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 132(%rsp)
	movss	152(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 136(%rsp)
	call	vectoangles
	jmp	.L260
.L284:
	movss	floattime(%rip), %xmm0
	movl	6532(%rbx), %edi
	cvtps2pd	%xmm0, %xmm0
	addsd	.LC56(%rip), %xmm0
	addsd	.LC57(%rip), %xmm0
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm0
	movss	%xmm0, 6080(%rbx)
	call	trap_BotResetLastAvoidReach
	movl	$.LC58, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_NBG
	xorl	%eax, %eax
	jmp	.L237
.L261:
	leaq	6584(%rbx), %rsi
	leaq	28(%r12), %rdi
	call	vectoangles
	jmp	.L260
	.cfi_endproc
.LFE99:
	.size	AINode_Battle_Chase, .-AINode_Battle_Chase
	.section	.rodata.str1.1
.LC60:
	.string	"battle fight: observer"
.LC61:
	.string	"battle fight: intermission"
.LC62:
	.string	"battle fight: bot dead"
.LC63:
	.string	"battle fight: no enemy"
.LC64:
	.string	"battle fight: enemy dead"
.LC66:
	.string	"battle fight: invisible"
	.section	.rodata.str1.8
	.align 8
.LC67:
	.string	"battle fight: chat health decreased"
	.align 8
.LC68:
	.string	"battle fight: chat hit someone"
	.align 8
.LC69:
	.string	"battle fight: enemy out of sight"
	.align 8
.LC70:
	.string	"battle fight: wants to retreat"
	.text
	.p2align 4,,15
.globl AINode_Battle_Fight
	.type	AINode_Battle_Fight, @function
AINode_Battle_Fight:
.LFB97:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	movq	%rdi, %rbx
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	subq	$296, %rsp
	.cfi_def_cfa_offset 320
	call	BotIsObserver
	testl	%eax, %eax
	jne	.L320
	movq	%rbx, %rdi
	call	BotIntermission
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L321
	movq	%rbx, %rdi
	call	BotIsDead
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L322
	movl	6548(%rbx), %esi
	movq	%rbx, %rdi
	call	BotFindEnemy
	movl	6548(%rbx), %edi
	testl	%edi, %edi
	js	.L323
	leaq	64(%rsp), %rbp
	movq	%rbp, %rsi
	call	BotEntityInfo
	xorps	%xmm1, %xmm1
	movss	6144(%rbx), %xmm0
	ucomiss	%xmm1, %xmm0
	jp	.L319
	je	.L317
.L319:
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	floattime(%rip), %xmm0
	cvtps2pd	%xmm0, %xmm0
	subsd	.LC57(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jbe	.L295
	movl	6020(%rbx), %r9d
	movl	$0x00000000, 6144(%rbx)
	testl	%r9d, %r9d
	jne	.L324
.L297:
	movl	6000(%rbx), %eax
	cmpl	6548(%rbx), %eax
	je	.L325
.L298:
	movl	$0x00000000, 6076(%rbx)
	movl	$.LC64, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L321:
	movl	$.LC61, %esi
	movq	%rbx, %rdi
	call	AIEnter_Intermission
	xorl	%eax, %eax
.L289:
	addq	$296, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L320:
	movq	%rbx, %rdi
	movl	$.LC60, %esi
	call	AIEnter_Observer
	addq	$296, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L322:
	movq	%rbx, %rdi
	movl	$.LC62, %esi
	call	AIEnter_Respawn
	addq	$296, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	ret
	.p2align 4,,10
	.p2align 3
.L317:
	movq	%rbp, %rdi
	call	EntityIsDead
	testl	%eax, %eax
	je	.L295
	movl	floattime(%rip), %eax
	movl	%eax, 6144(%rbx)
.L295:
	movq	%rbp, %rdi
	call	EntityIsInvisible
	testl	%eax, %eax
	jne	.L326
.L299:
	movl	88(%rsp), %eax
	movss	96(%rsp), %xmm0
	movss	%xmm0, 280(%rsp)
	movl	%eax, 272(%rsp)
	movl	92(%rsp), %eax
	movl	%eax, 276(%rsp)
	movl	6548(%rbx), %eax
	cmpl	$63, %eax
	jle	.L301
	cmpl	redobelisk+40(%rip), %eax
	je	.L302
	cmpl	blueobelisk+40(%rip), %eax
	jne	.L301
.L302:
	addss	.LC42(%rip), %xmm0
	movss	%xmm0, 280(%rsp)
.L301:
	leaq	272(%rsp), %rdi
	call	BotPointAreaNum
	testl	%eax, %eax
	movl	%eax, %ebp
	jne	.L327
.L303:
	movl	6548(%rbx), %esi
	movq	%rbx, %rdi
	call	BotUpdateBattleInventory
	movl	6052(%rbx), %eax
	cmpl	5076(%rbx), %eax
	jg	.L328
.L304:
	movl	268(%rbx), %eax
	cmpl	6056(%rbx), %eax
	jle	.L305
	movq	%rbx, %rdi
	call	BotChat_HitNoKill
	testl	%eax, %eax
	jne	.L329
.L305:
	movl	6548(%rbx), %ecx
	movl	12(%rbx), %edi
	leaq	6572(%rbx), %rdx
	leaq	4944(%rbx), %rsi
	movss	.LC41(%rip), %xmm0
	call	BotEntityVisible
	xorps	%xmm1, %xmm1
	ucomiss	%xmm1, %xmm0
	je	.L330
.L306:
	movq	%rbx, %rdi
	call	BotBattleUseItems
	movl	$18616254, 5984(%rbx)
	movl	bot_grapple+12(%rip), %r8d
	testl	%r8d, %r8d
	je	.L308
	movl	$18632638, 5984(%rbx)
.L308:
	movq	%rbx, %rdi
	call	BotInLavaOrSlime
	testl	%eax, %eax
	je	.L309
	orl	$6291456, 5984(%rbx)
.L309:
	movq	%rbx, %rdi
	call	BotCanAndWantsToRocketJump
	testl	%eax, %eax
	je	.L310
	orl	$4096, 5984(%rbx)
.L310:
	movq	%rbx, %rdi
	call	BotChooseWeapon
	movl	5984(%rbx), %edx
	movq	%rsp, %rdi
	movq	%rbx, %rsi
	call	BotAttackMove
	movq	8(%rsp), %rdx
	movq	(%rsp), %rax
	movq	%rdx, 216(%rsp)
	movq	16(%rsp), %rdx
	testl	%eax, %eax
	movq	%rax, 208(%rsp)
	movq	%rdx, 224(%rsp)
	movq	24(%rsp), %rdx
	movq	%rdx, 232(%rsp)
	movq	32(%rsp), %rdx
	movq	%rdx, 240(%rsp)
	movq	40(%rsp), %rdx
	movq	%rdx, 248(%rsp)
	movl	48(%rsp), %edx
	movl	%edx, 256(%rsp)
	jne	.L331
.L311:
	leaq	208(%rsp), %rsi
	xorl	%edx, %edx
	movq	%rbx, %rdi
	call	BotAIBlocked
	movq	%rbx, %rdi
	call	BotAimAtEnemy
	movq	%rbx, %rdi
	call	BotCheckAttack
	testb	$64, 5988(%rbx)
	je	.L332
.L312:
	movl	$1, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L330:
	.p2align 4,,4
	jp	.L306
	movq	%rbx, %rdi
	.p2align 4,,7
	.p2align 3
	call	BotWantsToChase
	testl	%eax, %eax
	.p2align 4,,4
	je	.L307
	movl	$.LC69, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Chase
	xorl	%eax, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L323:
	movl	$.LC63, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L325:
	movq	%rbx, %rdi
	call	BotChat_Kill
	testl	%eax, %eax
	.p2align 4,,2
	je	.L298
	movq	%rbx, %rdi
	call	BotChatTime
	addss	floattime(%rip), %xmm0
	movl	$.LC64, %esi
	movq	%rbx, %rdi
	movss	%xmm0, 6104(%rbx)
	call	AIEnter_Stand
	xorl	%eax, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L324:
	movq	%rbx, %rdi
	call	BotChat_EnemySuicide
	.p2align 4,,4
	jmp	.L297
	.p2align 4,,10
	.p2align 3
.L329:
	movq	%rbx, %rdi
	call	BotChatTime
	addss	floattime(%rip), %xmm0
	movl	$.LC68, %esi
	movq	%rbx, %rdi
	movss	%xmm0, 6104(%rbx)
	call	AIEnter_Stand
	xorl	%eax, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L326:
	movq	%rbp, %rdi
	call	EntityIsShooting
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L299
	call	rand
	andl	$32767, %eax
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	ucomisd	.LC65(%rip), %xmm0
	jae	.L299
	jp	.L299
	movl	$.LC66, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L327:
	movl	%eax, %edi
	call	trap_AAS_AreaReachability
	testl	%eax, %eax
	.p2align 4,,2
	je	.L303
	movl	272(%rsp), %eax
	movl	%ebp, 6552(%rbx)
	movl	%eax, 6556(%rbx)
	movl	276(%rsp), %eax
	movl	%eax, 6560(%rbx)
	movl	280(%rsp), %eax
	movl	%eax, 6564(%rbx)
	jmp	.L303
	.p2align 4,,10
	.p2align 3
.L328:
	movq	%rbx, %rdi
	call	BotChat_HitNoDeath
	testl	%eax, %eax
	je	.L304
	movq	%rbx, %rdi
	call	BotChatTime
	addss	floattime(%rip), %xmm0
	movl	$.LC67, %esi
	movq	%rbx, %rdi
	movss	%xmm0, 6104(%rbx)
	call	AIEnter_Stand
	xorl	%eax, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L307:
	movl	$.LC69, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L332:
	movq	%rbx, %rdi
	call	BotWantsToRetreat
	testl	%eax, %eax
	.p2align 4,,2
	je	.L312
	movl	$.LC70, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Retreat
	movl	$1, %eax
	jmp	.L289
	.p2align 4,,10
	.p2align 3
.L331:
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movl	$0x00000000, 6076(%rbx)
	jmp	.L311
	.cfi_endproc
.LFE97:
	.size	AINode_Battle_Fight, .-AINode_Battle_Fight
	.section	.rodata.str1.1
.LC71:
	.string	"seek nbg: observer"
.LC72:
	.string	"seek nbg: intermision"
.LC73:
	.string	"seek nbg: bot dead"
.LC75:
	.string	"seek nbg: time out"
.LC77:
	.string	"seek nbg: found enemy"
	.text
	.p2align 4,,15
.globl AINode_Seek_NBG
	.type	AINode_Seek_NBG, @function
AINode_Seek_NBG:
.LFB92:
	.cfi_startproc
	movq	%rbx, -24(%rsp)
	movq	%rbp, -16(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -24
	.cfi_offset 3, -32
	movq	%r12, -8(%rsp)
	subq	$200, %rsp
	.cfi_def_cfa_offset 208
	.cfi_offset 12, -16
	call	BotIsObserver
	testl	%eax, %eax
	jne	.L363
	movq	%rbx, %rdi
	call	BotIntermission
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L364
	movq	%rbx, %rdi
	call	BotIsDead
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L365
	movl	$18616254, 5984(%rbx)
	movl	bot_grapple+12(%rip), %r11d
	testl	%r11d, %r11d
	je	.L338
	movl	$18632638, 5984(%rbx)
.L338:
	movq	%rbx, %rdi
	call	BotInLavaOrSlime
	testl	%eax, %eax
	je	.L339
	orl	$6291456, 5984(%rbx)
.L339:
	movq	%rbx, %rdi
	call	BotCanAndWantsToRocketJump
	testl	%eax, %eax
	jne	.L366
.L340:
	movq	%rbx, %rdi
	leaq	16(%rsp), %rbp
	call	BotMapScripts
	movl	6536(%rbx), %edi
	movl	$-1, 6548(%rbx)
	movq	%rbp, %rsi
	call	trap_BotGetTopGoal
	testl	%eax, %eax
	jne	.L341
.L361:
	xorl	%eax, %eax
	movl	%eax, 12(%rsp)
	movl	%eax, 6080(%rbx)
	movss	12(%rsp), %xmm1
.L342:
	movss	floattime(%rip), %xmm0
	ucomiss	%xmm1, %xmm0
	ja	.L367
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	BotAIPredictObstacles
	movl	%eax, %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	jne	.L335
	movq	%rbx, %rdi
	leaq	80(%rsp), %r12
	call	BotSetupForMovement
	movl	5984(%rbx), %ecx
	movl	6532(%rbx), %esi
	movq	%rbp, %rdx
	movq	%r12, %rdi
	call	trap_BotMoveToGoal
	movl	80(%rsp), %r10d
	testl	%r10d, %r10d
	jne	.L368
.L347:
	movl	$1, %edx
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	BotAIBlocked
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	BotClearPath
	movl	100(%rsp), %eax
	testb	$11, %al
	jne	.L369
	testb	$4, %al
	jne	.L370
	testb	$32, 5988(%rbx)
	je	.L371
.L349:
	testb	$16, %al
	.p2align 4,,2
	je	.L355
	movl	104(%rsp), %eax
	movl	%eax, 6568(%rbx)
.L355:
	movl	$-1, %esi
	movq	%rbx, %rdi
	call	BotFindEnemy
	movl	%eax, %edx
	movl	$1, %eax
	testl	%edx, %edx
	je	.L335
	movq	%rbx, %rdi
	call	BotWantsToRetreat
	testl	%eax, %eax
	jne	.L372
	movl	6532(%rbx), %edi
	call	trap_BotResetLastAvoidReach
	movl	6536(%rbx), %edi
	call	trap_BotEmptyGoalStack
	movl	$.LC77, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Fight
	movl	$1, %eax
	jmp	.L335
	.p2align 4,,10
	.p2align 3
.L364:
	movl	$.LC72, %esi
	movq	%rbx, %rdi
	call	AIEnter_Intermission
	xorl	%eax, %eax
.L335:
	movq	176(%rsp), %rbx
	movq	184(%rsp), %rbp
	movq	192(%rsp), %r12
	addq	$200, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L363:
	movl	$.LC71, %esi
	movq	%rbx, %rdi
	call	AIEnter_Observer
	xorl	%eax, %eax
	jmp	.L335
	.p2align 4,,10
	.p2align 3
.L366:
	orl	$4096, 5984(%rbx)
	jmp	.L340
	.p2align 4,,10
	.p2align 3
.L367:
	movl	6536(%rbx), %edi
	call	trap_BotPopGoal
	movss	floattime(%rip), %xmm0
	movl	$.LC75, %esi
	movq	%rbx, %rdi
	cvtps2pd	%xmm0, %xmm0
	addsd	.LC74(%rip), %xmm0
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm0
	movss	%xmm0, 6100(%rbx)
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L335
	.p2align 4,,10
	.p2align 3
.L341:
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	BotReachedGoal
	testl	%eax, %eax
	jne	.L343
	movss	6080(%rbx), %xmm1
	jmp	.L342
	.p2align 4,,10
	.p2align 3
.L365:
	movl	$.LC73, %esi
	movq	%rbx, %rdi
	call	AIEnter_Respawn
	xorl	%eax, %eax
	jmp	.L335
	.p2align 4,,10
	.p2align 3
.L369:
	movl	120(%rsp), %edx
	movl	%edx, 6584(%rbx)
	movl	124(%rsp), %edx
	movl	%edx, 6588(%rbx)
	movl	128(%rsp), %edx
	movl	%edx, 6592(%rbx)
	jmp	.L349
	.p2align 4,,10
	.p2align 3
.L368:
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movl	$0x00000000, 6080(%rbx)
	jmp	.L347
	.p2align 4,,10
	.p2align 3
.L343:
	movq	%rbx, %rdi
	call	BotChooseWeapon
	jmp	.L361
	.p2align 4,,10
	.p2align 3
.L372:
	movl	$.LC77, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_NBG
	movl	$1, %eax
	jmp	.L335
	.p2align 4,,10
	.p2align 3
.L370:
	call	rand
	andl	$32767, %eax
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	4912(%rbx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	mulsd	.LC76(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	ja	.L351
.L362:
	movl	100(%rsp), %eax
	jmp	.L349
.L371:
	movl	6536(%rbx), %edi
	movq	%rbp, %rsi
	call	trap_BotGetSecondGoal
	testl	%eax, %eax
	je	.L373
.L352:
	movl	5984(%rbx), %edx
	movl	6532(%rbx), %edi
	leaq	160(%rsp), %rcx
	movss	.LC11(%rip), %xmm0
	movq	%rbp, %rsi
	call	trap_BotMovementViewTarget
	testl	%eax, %eax
	je	.L353
	movss	160(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	144(%rsp), %rdi
	movss	%xmm0, 144(%rsp)
	movss	164(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 148(%rsp)
	movss	168(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 152(%rsp)
	call	vectoangles
.L354:
	movss	.LC45(%rip), %xmm0
	mulss	6592(%rbx), %xmm0
	movss	%xmm0, 6592(%rbx)
	jmp	.L362
.L353:
	leaq	6584(%rbx), %rsi
	leaq	28(%r12), %rdi
	call	vectoangles
	jmp	.L354
.L351:
	leaq	160(%rsp), %rsi
	movq	%rbx, %rdi
	call	BotRoamGoal
	movss	160(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	144(%rsp), %rdi
	movss	%xmm0, 144(%rsp)
	movss	164(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 148(%rsp)
	movss	168(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 152(%rsp)
	call	vectoangles
	movss	.LC45(%rip), %xmm0
	mulss	6592(%rbx), %xmm0
	movl	100(%rsp), %eax
	movss	%xmm0, 6592(%rbx)
	jmp	.L349
.L373:
	movl	6536(%rbx), %edi
	movq	%rbp, %rsi
	call	trap_BotGetTopGoal
	jmp	.L352
	.cfi_endproc
.LFE92:
	.size	AINode_Seek_NBG, .-AINode_Seek_NBG
	.section	.rodata.str1.8
	.align 8
.LC78:
	.string	"%s at %1.1f switched more than %d AI nodes\n"
	.section	.rodata.str1.1
.LC79:
	.string	"%s"
	.text
	.p2align 4,,15
.globl BotDumpNodeSwitches
	.type	BotDumpNodeSwitches, @function
BotDumpNodeSwitches:
.LFB70:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movl	$36, %edx
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$56, %rsp
	.cfi_def_cfa_offset 80
	movl	8(%rdi), %edi
	movq	%rsp, %rsi
	movq	%fs:40, %rax
	movq	%rax, 40(%rsp)
	xorl	%eax, %eax
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	call	ClientName
	movss	floattime(%rip), %xmm0
	movl	$1, %eax
	movl	$50, %ecx
	movq	%rsp, %rdx
	movl	$.LC78, %esi
	movl	$1, %edi
	cvtps2pd	%xmm0, %xmm0
	call	BotAI_Print
	movl	numnodeswitches(%rip), %eax
	testl	%eax, %eax
	jle	.L375
	movl	$nodeswitch, %ebp
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L376:
	movq	%rbp, %rdx
	xorl	%eax, %eax
	movl	$.LC79, %esi
	movl	$1, %edi
	addl	$1, %ebx
	addq	$144, %rbp
	call	BotAI_Print
	cmpl	%ebx, numnodeswitches(%rip)
	jg	.L376
.L375:
	xorl	%eax, %eax
	movl	$.LC15, %esi
	movl	$4, %edi
	call	BotAI_Print
	movq	40(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L380
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	ret
.L380:
	.p2align 4,,6
	call	__stack_chk_fail
	.cfi_endproc
.LFE70:
	.size	BotDumpNodeSwitches, .-BotDumpNodeSwitches
	.section	.rodata.str1.1
.LC80:
	.string	"help_start"
.LC81:
	.string	"yes"
.LC83:
	.string	"accompany_start"
.LC84:
	.string	"accompany_stop"
.LC88:
	.string	"accompany_arrive"
.LC89:
	.string	"BotLongTermGoal: go for air"
.LC91:
	.string	"accompany_cannotfind"
.LC92:
	.string	"dd_start_pointa"
.LC97:
	.string	"dd_start_pointb"
.LC98:
	.string	"defend_start"
.LC99:
	.string	"ondefense"
.LC100:
	.string	"defend_stop"
.LC101:
	.string	"kill_start"
.LC102:
	.string	"kill_done"
.LC103:
	.string	"getitem_start"
.LC104:
	.string	"getitem_notthere"
.LC105:
	.string	"getitem_gotit"
.LC106:
	.string	"camp_start"
.LC107:
	.string	"camp_stop"
.LC109:
	.string	"camp_arrive"
.LC110:
	.string	"inposition"
.LC111:
	.string	" to "
.LC112:
	.string	"patrol_start"
.LC113:
	.string	"patrol_stop"
.LC114:
	.string	"captureflag_start"
.LC115:
	.string	"ongetflag"
.LC116:
	.string	"returnflag_start"
.LC117:
	.string	"onreturnflag"
.LC118:
	.string	"attackenemybase_start"
.LC119:
	.string	"onoffense"
.LC120:
	.string	"harvest_start"
	.text
	.p2align 4,,15
.globl BotGetLongTermGoal
	.type	BotGetLongTermGoal, @function
BotGetLongTermGoal:
.LFB77:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	movl	%esi, %r13d
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	movq	%rcx, %rbp
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	pushq	%rbx
	.cfi_def_cfa_offset 40
	movq	%rdi, %rbx
	.cfi_offset 3, -40
	subq	$712, %rsp
	.cfi_def_cfa_offset 752
	movq	%fs:40, %rax
	movq	%rax, 696(%rsp)
	xorl	%eax, %eax
	testl	%edx, %edx
	movl	6608(%rdi), %eax
	sete	%r12b
	je	.L786
.L396:
	cmpl	$3, %eax
	je	.L787
.L432:
	cmpl	$16, %eax
	je	.L788
	cmpl	$17, %eax
	.p2align 4,,5
	je	.L789
	cmpl	$3, %eax
	.p2align 4,,5
	je	.L790
	cmpl	$11, %eax
	.p2align 4,,5
	je	.L791
	cmpl	$10, %eax
	.p2align 4,,5
	je	.L792
.L464:
	leal	-7(%rax), %edx
	cmpl	$1, %edx
	.p2align 4,,3
	ja	.L471
	testb	%r12b, %r12b
	.p2align 4,,3
	je	.L471
	xorps	%xmm2, %xmm2
	movss	6748(%rbx), %xmm0
	ucomiss	%xmm2, %xmm0
	jp	.L747
	je	.L472
.L747:
	movss	floattime(%rip), %xmm1
	ucomiss	%xmm0, %xmm1
	jbe	.L472
	cmpl	$8, %eax
	.p2align 4,,2
	je	.L793
.L475:
	movl	$0x00000000, 6748(%rbx)
.L472:
	movq	6632(%rbx), %rdx
	leaq	6632(%rbx), %rax
	movq	%rdx, (%rbp)
	movq	8(%rax), %rdx
	movq	%rdx, 8(%rbp)
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rbp)
	movq	24(%rax), %rdx
	movq	%rdx, 24(%rbp)
	movq	32(%rax), %rdx
	movq	%rdx, 32(%rbp)
	movq	40(%rax), %rdx
	movq	%rdx, 40(%rbp)
	movq	48(%rax), %rax
	movq	%rax, 48(%rbp)
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	jbe	.L476
	cmpl	$8, 6608(%rbx)
	je	.L794
.L478:
	movl	$0, 6608(%rbx)
.L476:
	movss	(%rbp), %xmm0
	subss	4916(%rbx), %xmm0
	movss	%xmm0, 368(%rsp)
	mulss	%xmm0, %xmm0
	movss	4(%rbp), %xmm3
	subss	4920(%rbx), %xmm3
	movss	%xmm3, 372(%rsp)
	mulss	%xmm3, %xmm3
	movss	8(%rbp), %xmm1
	subss	4924(%rbx), %xmm1
	addss	%xmm3, %xmm0
	movss	%xmm1, 376(%rsp)
	mulss	%xmm1, %xmm1
	addss	%xmm1, %xmm0
	ucomiss	.LC108(%rip), %xmm0
	jae	.L430
	jp	.L430
	ucomiss	6180(%rbx), %xmm2
	jne	.L480
	.p2align 4,,6
	jp	.L480
	cmpl	$8, 6608(%rbx)
	je	.L795
.L481:
	movl	floattime(%rip), %eax
	movl	%eax, 6180(%rbx)
.L480:
	call	rand
	andl	$32767, %eax
	movss	.LC30(%rip), %xmm2
	cvtsi2ss	%eax, %xmm0
	divss	%xmm2, %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	4912(%rbx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	mulsd	.LC76(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	ja	.L796
.L482:
	movss	floattime(%rip), %xmm0
	movaps	%xmm0, %xmm4
	movss	.LC10(%rip), %xmm3
	movss	6128(%rbx), %xmm1
	subss	%xmm3, %xmm4
	ucomiss	%xmm1, %xmm4
	ja	.L797
.L484:
	ucomiss	%xmm0, %xmm1
	ja	.L798
.L487:
	leaq	4916(%rbx), %rdi
	call	trap_AAS_Swimming
	testl	%eax, %eax
	je	.L489
	movss	floattime(%rip), %xmm0
	subss	.LC7(%rip), %xmm0
	movss	%xmm0, 6128(%rbx)
.L489:
	movl	12(%rbx), %esi
	leaq	4944(%rbx), %rdi
	call	trap_PointContents
	testb	$56, %al
	je	.L490
	cmpl	$8, 6608(%rbx)
	je	.L799
.L491:
	movl	$0, 6608(%rbx)
	jmp	.L490
	.p2align 4,,10
	.p2align 3
.L786:
	cmpl	$1, %eax
	je	.L800
	cmpl	$2, %eax
	.p2align 4,,2
	jne	.L396
	xorps	%xmm2, %xmm2
	movss	6748(%rdi), %xmm1
	ucomiss	%xmm2, %xmm1
	jne	.L397
	jp	.L397
.L757:
	movss	floattime(%rip), %xmm0
.L398:
	ucomiss	6752(%rbx), %xmm0
	ja	.L801
.L400:
	movl	6612(%rbx), %edi
	leaq	208(%rsp), %r12
	movss	%xmm2, (%rsp)
	movq	%r12, %rsi
	call	BotEntityInfo
	movl	6612(%rbx), %ecx
	movl	12(%rbx), %edi
	leaq	6572(%rbx), %rdx
	leaq	4944(%rbx), %rsi
	movss	.LC41(%rip), %xmm0
	call	BotEntityVisible
	movss	(%rsp), %xmm2
	ucomiss	%xmm2, %xmm0
	jp	.L741
	je	.L402
.L741:
	movss	232(%rsp), %xmm0
	movl	floattime(%rip), %eax
	movss	236(%rsp), %xmm3
	subss	4916(%rbx), %xmm0
	subss	4920(%rbx), %xmm3
	movss	240(%rsp), %xmm1
	subss	4924(%rbx), %xmm1
	movss	7020(%rbx), %xmm4
	mulss	%xmm4, %xmm4
	movl	%eax, 6756(%rbx)
	movss	%xmm0, 368(%rsp)
	mulss	%xmm0, %xmm0
	movss	%xmm3, 372(%rsp)
	mulss	%xmm3, %xmm3
	movss	%xmm1, 376(%rsp)
	mulss	%xmm1, %xmm1
	addss	%xmm3, %xmm0
	addss	%xmm1, %xmm0
	ucomiss	%xmm0, %xmm4
	ja	.L802
.L402:
	movl	208(%rsp), %edx
	testl	%edx, %edx
	jne	.L803
.L429:
	movq	6632(%rbx), %rdx
	leaq	6632(%rbx), %rax
	movq	%rdx, (%rbp)
	movq	8(%rax), %rdx
	movq	%rdx, 8(%rbp)
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rbp)
	movq	24(%rax), %rdx
	movq	%rdx, 24(%rbp)
	movq	32(%rax), %rdx
	movq	%rdx, 32(%rbp)
	movq	40(%rax), %rdx
	movq	%rdx, 40(%rbp)
	movq	48(%rax), %rax
	movq	%rax, 48(%rbp)
	movss	floattime(%rip), %xmm0
	subss	.LC90(%rip), %xmm0
	ucomiss	6756(%rbx), %xmm0
	ja	.L804
.L430:
	movl	$1, %eax
	jmp	.L394
	.p2align 4,,10
	.p2align 3
.L800:
	xorps	%xmm2, %xmm2
	movss	6748(%rdi), %xmm1
	ucomiss	%xmm2, %xmm1
	jne	.L383
	jp	.L383
.L756:
	movss	floattime(%rip), %xmm0
.L384:
	ucomiss	6752(%rbx), %xmm0
	jbe	.L386
	movl	$0, 6608(%rbx)
.L386:
	subss	.LC53(%rip), %xmm0
	ucomiss	6756(%rbx), %xmm0
	jbe	.L388
	movl	$0, 6608(%rbx)
.L388:
	movl	6612(%rbx), %edi
	leaq	208(%rsp), %r12
	movss	%xmm2, (%rsp)
	movq	%r12, %rsi
	call	BotEntityInfo
	movl	6612(%rbx), %ecx
	movl	12(%rbx), %edi
	leaq	6572(%rbx), %rdx
	leaq	4944(%rbx), %rsi
	movss	.LC41(%rip), %xmm0
	call	BotEntityVisible
	movss	(%rsp), %xmm2
	ucomiss	%xmm2, %xmm0
	jp	.L740
	je	.L711
.L740:
	movss	232(%rsp), %xmm0
	movss	236(%rsp), %xmm2
	subss	4916(%rbx), %xmm0
	subss	4920(%rbx), %xmm2
	movss	240(%rsp), %xmm1
	subss	4924(%rbx), %xmm1
	movss	%xmm0, 368(%rsp)
	mulss	%xmm0, %xmm0
	movss	%xmm2, 372(%rsp)
	mulss	%xmm2, %xmm2
	movss	%xmm1, 376(%rsp)
	mulss	%xmm1, %xmm1
	addss	%xmm2, %xmm0
	addss	%xmm1, %xmm0
	ucomiss	.LC82(%rip), %xmm0
	jb	.L805
.L392:
	movl	208(%rsp), %ecx
	testl	%ecx, %ecx
	jne	.L806
.L395:
	movq	6632(%rbx), %rdx
	leaq	6632(%rbx), %rax
.L762:
	movq	%rdx, (%rbp)
	movq	8(%rax), %rdx
	movq	%rdx, 8(%rbp)
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rbp)
	movq	24(%rax), %rdx
	movq	%rdx, 24(%rbp)
	movq	32(%rax), %rdx
	movq	%rdx, 32(%rbp)
	movq	40(%rax), %rdx
	movq	%rdx, 40(%rbp)
	movq	48(%rax), %rax
	movq	%rax, 48(%rbp)
	movl	$1, %eax
.L394:
	movq	696(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L807
	addq	$712, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
	.p2align 4,,10
	.p2align 3
.L383:
	movss	floattime(%rip), %xmm0
	ucomiss	%xmm1, %xmm0
	jbe	.L384
	movl	6612(%rbx), %edi
	leaq	656(%rsp), %rsi
	movl	$36, %edx
	movss	%xmm2, (%rsp)
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movl	$.LC80, %esi
	xorl	%eax, %eax
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	6616(%rbx), %esi
	movl	$.LC81, %edx
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movl	8(%rbx), %edi
	movl	$1048576, %esi
	call	trap_EA_Action
	movss	(%rsp), %xmm2
	movss	%xmm2, 6748(%rbx)
	jmp	.L756
	.p2align 4,,10
	.p2align 3
.L711:
	movl	208(%rsp), %ecx
	movl	floattime(%rip), %eax
	testl	%ecx, %ecx
	movl	%eax, 6756(%rbx)
	je	.L395
.L806:
	leaq	24(%r12), %rdi
	call	BotPointAreaNum
	testl	%eax, %eax
	movl	%eax, %r12d
	je	.L395
	movl	%eax, %edi
	call	trap_AAS_AreaReachability
	testl	%eax, %eax
	je	.L395
	movl	6612(%rbx), %eax
	movl	%r12d, 6644(%rbx)
	movl	$0xc1000000, 6648(%rbx)
	movl	$0xc1000000, 6652(%rbx)
	movl	$0xc1000000, 6656(%rbx)
	movl	$0x41000000, 6660(%rbx)
	movl	%eax, 6672(%rbx)
	movl	232(%rsp), %eax
	movl	$0x41000000, 6664(%rbx)
	movl	$0x41000000, 6668(%rbx)
	movl	%eax, 6632(%rbx)
	movl	236(%rsp), %eax
	movl	%eax, 6636(%rbx)
	movl	240(%rsp), %eax
	movl	%eax, 6640(%rbx)
	jmp	.L395
	.p2align 4,,10
	.p2align 3
.L788:
	movss	floattime(%rip), %xmm0
	ucomiss	6152(%rbx), %xmm0
	jbe	.L464
	xorps	%xmm2, %xmm2
	movss	6748(%rbx), %xmm1
	ucomiss	%xmm2, %xmm1
	jp	.L742
	je	.L437
.L742:
	ucomiss	%xmm1, %xmm0
	ja	.L808
.L437:
	movq	ctf_redflag(%rip), %rax
	movq	%rax, (%rbp)
	movq	ctf_redflag+8(%rip), %rax
	movq	%rax, 8(%rbp)
	movq	ctf_redflag+16(%rip), %rax
	movq	%rax, 16(%rbp)
	movq	ctf_redflag+24(%rip), %rax
	movq	%rax, 24(%rbp)
	movq	ctf_redflag+32(%rip), %rax
	movq	%rax, 32(%rbp)
	movq	ctf_redflag+40(%rip), %rax
	movq	%rax, 40(%rbp)
	movq	ctf_redflag+48(%rip), %rax
	movq	%rax, 48(%rbp)
	jmp	.L453
	.p2align 4,,10
	.p2align 3
.L805:
	jp	.L392
.L490:
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	xorl	%eax, %eax
	jmp	.L394
	.p2align 4,,10
	.p2align 3
.L789:
	movss	floattime(%rip), %xmm0
	ucomiss	6152(%rbx), %xmm0
	jbe	.L464
	xorps	%xmm2, %xmm2
	movss	6748(%rbx), %xmm1
	ucomiss	%xmm2, %xmm1
	jp	.L743
	je	.L443
.L743:
	ucomiss	%xmm1, %xmm0
	ja	.L809
.L443:
	movq	ctf_blueflag(%rip), %rax
	movq	%rax, (%rbp)
	movq	ctf_blueflag+8(%rip), %rax
	movq	%rax, 8(%rbp)
	movq	ctf_blueflag+16(%rip), %rax
	movq	%rax, 16(%rbp)
	movq	ctf_blueflag+24(%rip), %rax
	movq	%rax, 24(%rbp)
	movq	ctf_blueflag+32(%rip), %rax
	movq	%rax, 32(%rbp)
	movq	ctf_blueflag+40(%rip), %rax
	movq	%rax, 40(%rbp)
	movq	ctf_blueflag+48(%rip), %rax
	movq	%rax, 48(%rbp)
	jmp	.L453
	.p2align 4,,10
	.p2align 3
.L790:
	testb	%r12b, %r12b
	je	.L464
	movss	floattime(%rip), %xmm0
	ucomiss	6152(%rbx), %xmm0
	jbe	.L464
	xorps	%xmm2, %xmm2
	movss	6748(%rbx), %xmm1
	ucomiss	%xmm2, %xmm1
	jp	.L744
	je	.L450
.L744:
	ucomiss	%xmm1, %xmm0
	ja	.L810
.L450:
	movq	6632(%rbx), %rdx
	leaq	6632(%rbx), %rax
	movq	%rdx, (%rbp)
	movq	8(%rax), %rdx
	movq	%rdx, 8(%rbp)
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rbp)
	movq	24(%rax), %rdx
	movq	%rdx, 24(%rbp)
	movq	32(%rax), %rdx
	movq	%rdx, 32(%rbp)
	movq	40(%rax), %rdx
	movq	%rdx, 40(%rbp)
	movq	48(%rax), %rax
	movq	%rax, 48(%rbp)
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	ja	.L811
.L453:
	movss	(%rbp), %xmm0
	subss	4916(%rbx), %xmm0
	movss	%xmm0, 368(%rsp)
	mulss	%xmm0, %xmm0
	movss	4(%rbp), %xmm2
	subss	4920(%rbx), %xmm2
	movss	%xmm2, 372(%rsp)
	mulss	%xmm2, %xmm2
	movss	8(%rbp), %xmm1
	subss	4924(%rbx), %xmm1
	addss	%xmm2, %xmm0
	movss	%xmm1, 376(%rsp)
	mulss	%xmm1, %xmm1
	addss	%xmm1, %xmm0
	ucomiss	.LC93(%rip), %xmm0
	jae	.L430
	jp	.L430
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movss	.LC94(%rip), %xmm1
	movss	floattime(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm1, 32(%rsp)
	movss	%xmm0, 60(%rsp)
	call	rand
	andl	$32767, %eax
	movss	32(%rsp), %xmm1
	cvtsi2ss	%eax, %xmm0
	movq	%rbx, %rdi
	divss	.LC30(%rip), %xmm0
	mulss	%xmm1, %xmm0
	addss	60(%rsp), %xmm0
	movss	%xmm0, 6152(%rbx)
	call	BotHasPersistantPowerupAndWeapon
	testl	%eax, %eax
	je	.L456
	movl	$0x42c80000, 6156(%rbx)
	movl	$1, %eax
	jmp	.L394
	.p2align 4,,10
	.p2align 3
.L787:
	movl	6644(%rbx), %edx
	movl	4956(%rbx), %edi
	leaq	4916(%rbx), %rsi
	movl	$18616254, %ecx
	call	trap_AAS_AreaTravelTimeToGoalArea
	cvtsi2ss	%eax, %xmm0
	ucomiss	6156(%rbx), %xmm0
	jbe	.L759
	movl	$0x00000000, 6152(%rbx)
.L759:
	movl	6608(%rbx), %eax
	jmp	.L432
	.p2align 4,,10
	.p2align 3
.L792:
	testb	%r12b, %r12b
	je	.L464
	xorps	%xmm1, %xmm1
	movss	6748(%rbx), %xmm0
	ucomiss	%xmm1, %xmm0
	jp	.L746
	je	.L465
.L746:
	movss	floattime(%rip), %xmm2
	ucomiss	%xmm0, %xmm2
	ja	.L812
.L465:
	movq	6632(%rbx), %rdx
	leaq	6632(%rbx), %rax
	movq	%rdx, (%rbp)
	movq	8(%rax), %rdx
	movq	%rdx, 8(%rbp)
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rbp)
	movq	24(%rax), %rdx
	movq	%rdx, 24(%rbp)
	movq	32(%rax), %rdx
	movq	%rdx, 32(%rbp)
	movq	40(%rax), %rdx
	movq	%rdx, 40(%rbp)
	movq	48(%rax), %rax
	movq	%rax, 48(%rbp)
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	jbe	.L468
	movl	$0, 6608(%rbx)
.L468:
	movl	12(%rbx), %edi
	leaq	6572(%rbx), %rdx
	leaq	4944(%rbx), %rsi
	movq	%rbp, %rcx
	call	trap_BotItemGoalInVisButNotVisible
	testl	%eax, %eax
	jne	.L813
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	BotReachedGoal
	testl	%eax, %eax
	je	.L430
	leaq	400(%rsp), %rbp
	movl	6676(%rbx), %edi
	movl	$256, %edx
	movq	%rbp, %rsi
	call	trap_BotGoalName
	xorl	%ecx, %ecx
	movq	%rbp, %rdx
	movl	$.LC105, %esi
.L763:
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
.L764:
	movl	$0, 6608(%rbx)
	movl	$1, %eax
	jmp	.L394
	.p2align 4,,10
	.p2align 3
.L397:
	movss	floattime(%rip), %xmm0
	ucomiss	%xmm1, %xmm0
	jbe	.L398
	movl	6612(%rbx), %edi
	leaq	656(%rsp), %rsi
	movl	$36, %edx
	movss	%xmm2, (%rsp)
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movl	$.LC83, %esi
	xorl	%eax, %eax
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	6616(%rbx), %esi
	movl	$.LC81, %edx
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movl	8(%rbx), %edi
	movl	$1048576, %esi
	call	trap_EA_Action
	movss	(%rsp), %xmm2
	movss	%xmm2, 6748(%rbx)
	jmp	.L757
	.p2align 4,,10
	.p2align 3
.L801:
	movl	6612(%rbx), %edi
	leaq	656(%rsp), %rsi
	movl	$36, %edx
	movss	%xmm2, (%rsp)
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movl	$.LC84, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	BotAI_BotInitialChat
	movl	6612(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	$0, 6608(%rbx)
	movss	(%rsp), %xmm2
	jmp	.L400
	.p2align 4,,10
	.p2align 3
.L804:
	movl	6612(%rbx), %edi
	leaq	656(%rsp), %rsi
	movl	$36, %edx
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movl	$.LC91, %esi
	xorl	%eax, %eax
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6612(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	floattime(%rip), %eax
	movl	$0, 6608(%rbx)
	movl	%eax, 6756(%rbx)
	movl	$1, %eax
	jmp	.L394
	.p2align 4,,10
	.p2align 3
.L803:
	leaq	24(%r12), %rdi
	call	BotPointAreaNum
	testl	%eax, %eax
	movl	%eax, %r12d
	je	.L429
	movl	%eax, %edi
	call	trap_AAS_AreaReachability
	testl	%eax, %eax
	je	.L429
	movl	6612(%rbx), %eax
	movl	%r12d, 6644(%rbx)
	movl	$0xc1000000, 6648(%rbx)
	movl	$0xc1000000, 6652(%rbx)
	movl	$0xc1000000, 6656(%rbx)
	movl	$0x41000000, 6660(%rbx)
	movl	%eax, 6672(%rbx)
	movl	232(%rsp), %eax
	movl	$0x41000000, 6664(%rbx)
	movl	$0x41000000, 6668(%rbx)
	movl	%eax, 6632(%rbx)
	movl	236(%rsp), %eax
	movl	%eax, 6636(%rbx)
	movl	240(%rsp), %eax
	movl	%eax, 6640(%rbx)
	jmp	.L429
	.p2align 4,,10
	.p2align 3
.L791:
	testb	%r12b, %r12b
	je	.L464
	xorps	%xmm1, %xmm1
	movss	6748(%rbx), %xmm0
	ucomiss	%xmm1, %xmm0
	jp	.L745
	je	.L458
.L745:
	movss	floattime(%rip), %xmm2
	ucomiss	%xmm0, %xmm2
	ja	.L814
.L458:
	movl	6000(%rbx), %edi
	cmpl	6672(%rbx), %edi
	je	.L815
.L461:
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	jbe	.L523
.L775:
	movl	$0, 6608(%rbx)
.L523:
	movq	%rbp, %rdx
	movl	%r13d, %esi
	movq	%rbx, %rdi
	call	BotGetItemLongTermGoal
	jmp	.L394
	.p2align 4,,10
	.p2align 3
.L802:
	movl	12(%rbx), %edi
	leaq	64(%rsp), %rsi
	call	BotEntityInfo
	movss	96(%rsp), %xmm1
	movss	156(%rsp), %xmm3
	movss	240(%rsp), %xmm0
	addss	%xmm1, %xmm3
	movss	288(%rsp), %xmm4
	addss	%xmm0, %xmm4
	movss	(%rsp), %xmm2
	ucomiss	%xmm4, %xmm3
	jbe	.L405
	movss	232(%rsp), %xmm7
	movss	280(%rsp), %xmm5
	addss	%xmm7, %xmm5
	movss	88(%rsp), %xmm8
	movss	148(%rsp), %xmm9
	movss	.LC85(%rip), %xmm6
	addss	%xmm8, %xmm9
	subss	%xmm6, %xmm5
	ucomiss	%xmm5, %xmm9
	jbe	.L405
	addss	292(%rsp), %xmm7
	addss	136(%rsp), %xmm8
	addss	%xmm6, %xmm7
	ucomiss	%xmm8, %xmm7
	jbe	.L405
	movss	236(%rsp), %xmm7
	movss	284(%rsp), %xmm5
	addss	%xmm7, %xmm5
	movss	92(%rsp), %xmm8
	movss	152(%rsp), %xmm9
	addss	%xmm8, %xmm9
	subss	%xmm6, %xmm5
	ucomiss	%xmm5, %xmm9
	jbe	.L405
	addss	296(%rsp), %xmm7
	addss	140(%rsp), %xmm8
	addss	%xmm6, %xmm7
	ucomiss	%xmm8, %xmm7
	jbe	.L405
	subss	%xmm6, %xmm4
	ucomiss	%xmm4, %xmm3
	jbe	.L405
	addss	300(%rsp), %xmm0
	addss	144(%rsp), %xmm1
	addss	%xmm6, %xmm0
	ucomiss	%xmm1, %xmm0
	ja	.L816
	.p2align 4,,10
	.p2align 3
.L405:
	movss	floattime(%rip), %xmm0
	movss	.LC10(%rip), %xmm3
	subss	%xmm3, %xmm0
	ucomiss	6128(%rbx), %xmm0
	ja	.L817
.L414:
	leaq	4916(%rbx), %rdi
	movss	%xmm2, (%rsp)
	call	trap_AAS_Swimming
	testl	%eax, %eax
	movss	(%rsp), %xmm2
	je	.L417
	movss	floattime(%rip), %xmm0
	subss	.LC7(%rip), %xmm0
	movss	%xmm0, 6128(%rbx)
.L417:
	movss	floattime(%rip), %xmm1
	movaps	%xmm1, %xmm4
	movss	.LC3(%rip), %xmm3
	movss	6180(%rbx), %xmm0
	subss	%xmm3, %xmm4
	ucomiss	%xmm0, %xmm4
	jbe	.L418
	ucomiss	%xmm2, %xmm0
	je	.L818
.L420:
	movss	6128(%rbx), %xmm0
	ucomiss	%xmm1, %xmm0
	jbe	.L712
	movl	8(%rbx), %edi
	movss	%xmm3, 16(%rsp)
	call	trap_EA_Crouch
	movss	6180(%rbx), %xmm0
	movss	floattime(%rip), %xmm1
	movss	16(%rsp), %xmm3
	.p2align 4,,10
	.p2align 3
.L418:
	subss	%xmm3, %xmm1
	ucomiss	%xmm1, %xmm0
	jbe	.L713
	movss	232(%rsp), %xmm0
	subss	4916(%rbx), %xmm0
	movss	%xmm0, 368(%rsp)
	movss	236(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 372(%rsp)
	movss	240(%rsp), %xmm0
.L758:
	subss	4924(%rbx), %xmm0
	leaq	6584(%rbx), %rsi
	leaq	368(%rsp), %rdi
	movss	%xmm0, 376(%rsp)
	call	vectoangles
	movss	.LC45(%rip), %xmm0
	mulss	6592(%rbx), %xmm0
	movss	%xmm0, 6592(%rbx)
.L426:
	movl	5984(%rbx), %esi
	leaq	6632(%rbx), %rdx
	movq	%rbx, %rdi
	movss	.LC87(%rip), %xmm0
	call	BotGoForAir
	testl	%eax, %eax
	je	.L490
	movl	6532(%rbx), %edi
	call	trap_BotResetLastAvoidReach
	movss	.LC8(%rip), %xmm0
	movl	$.LC89, %esi
	addss	floattime(%rip), %xmm0
	movq	%rbx, %rdi
	movss	%xmm0, 6080(%rbx)
	call	AIEnter_Seek_NBG
	xorl	%eax, %eax
	jmp	.L394
	.p2align 4,,10
	.p2align 3
.L456:
	movl	$0x43af0000, 6156(%rbx)
	movl	$1, %eax
	jmp	.L394
.L713:
	call	rand
	andl	$32767, %eax
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	4912(%rbx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	mulsd	.LC76(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jbe	.L426
	leaq	384(%rsp), %rsi
	movq	%rbx, %rdi
	call	BotRoamGoal
	movss	384(%rsp), %xmm0
	subss	4916(%rbx), %xmm0
	movss	%xmm0, 368(%rsp)
	movss	388(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 372(%rsp)
	movss	392(%rsp), %xmm0
	jmp	.L758
	.p2align 4,,10
	.p2align 3
.L471:
	cmpl	$9, %eax
	je	.L819
.L492:
	movl	gametype(%rip), %edx
	cmpl	$9, %edx
	je	.L673
	cmpl	$4, %edx
	je	.L673
	cmpl	$5, %edx
	je	.L820
	cmpl	$6, %edx
	.p2align 4,,5
	je	.L821
	cmpl	$7, %edx
	.p2align 4,,5
	jne	.L523
	cmpl	$5, %eax
	.p2align 4,,5
	je	.L822
	cmpl	$13, %eax
	.p2align 4,,5
	je	.L823
	cmpl	$12, %eax
	.p2align 4,,5
	jne	.L523
	movss	floattime(%rip), %xmm1
	ucomiss	6168(%rbx), %xmm1
	jbe	.L523
	movss	6748(%rbx), %xmm0
	ucomiss	.LC5(%rip), %xmm0
	jp	.L755
	je	.L613
.L755:
	ucomiss	%xmm0, %xmm1
	jbe	.L613
	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$.LC120, %esi
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC119, %edx
	orl	$-1, %esi
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movl	$0x00000000, 6748(%rbx)
.L613:
	movl	$neutralobelisk, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	jbe	.L616
	movl	$0, 6608(%rbx)
.L616:
	leaq	4916(%rbx), %rdi
	movq	%rbp, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L430
	movss	.LC85(%rip), %xmm1
	addss	floattime(%rip), %xmm1
	movss	%xmm1, 32(%rsp)
	call	rand
	andl	$32767, %eax
	movss	32(%rsp), %xmm1
	cvtsi2ss	%eax, %xmm0
	movl	$1, %eax
	divss	.LC30(%rip), %xmm0
	mulss	.LC94(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, 6168(%rbx)
	jmp	.L394
	.p2align 4,,10
	.p2align 3
.L813:
	leaq	400(%rsp), %rbp
	movl	6676(%rbx), %edi
	movl	$256, %edx
	movq	%rbp, %rsi
	call	trap_BotGoalName
	xorl	%ecx, %ecx
	movq	%rbp, %rdx
	movl	$.LC104, %esi
	jmp	.L763
.L811:
	leaq	400(%rsp), %r12
	movl	6676(%rbx), %edi
	movl	$256, %edx
	movq	%r12, %rsi
	call	trap_BotGoalName
	xorl	%ecx, %ecx
	movq	%r12, %rdx
	movl	$.LC100, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$0, 6608(%rbx)
	jmp	.L453
.L817:
	movl	6528(%rbx), %edi
	xorps	%xmm0, %xmm0
	movss	.LC7(%rip), %xmm1
	movl	$36, %esi
	movss	%xmm2, (%rsp)
	movss	%xmm3, 16(%rsp)
	call	trap_Characteristic_BFloat
	movss	%xmm0, 32(%rsp)
	call	rand
	andl	$32767, %eax
	movss	32(%rsp), %xmm0
	cvtsi2ss	%eax, %xmm1
	movss	4912(%rbx), %xmm4
	mulss	%xmm0, %xmm4
	movss	(%rsp), %xmm2
	movss	16(%rsp), %xmm3
	divss	.LC30(%rip), %xmm1
	ucomiss	%xmm1, %xmm4
	jbe	.L414
	addss	floattime(%rip), %xmm3
	mulss	.LC2(%rip), %xmm0
	addss	%xmm0, %xmm3
	movss	%xmm3, 6128(%rbx)
	jmp	.L414
.L814:
	leaq	400(%rsp), %r12
	movl	6672(%rbx), %edi
	movl	$256, %edx
	movss	%xmm1, 32(%rsp)
	movq	%r12, %rsi
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%r12, %rdx
	movl	$.LC101, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movss	32(%rsp), %xmm1
	movss	%xmm1, 6748(%rbx)
	jmp	.L458
.L815:
	leaq	400(%rsp), %r12
	movl	$256, %edx
	movq	%r12, %rsi
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%r12, %rdx
	movl	$.LC102, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	$-1, 6000(%rbx)
	movl	$0, 6608(%rbx)
	jmp	.L461
.L810:
	leaq	400(%rsp), %r12
	movl	6676(%rbx), %edi
	movl	$256, %edx
	movss	%xmm2, (%rsp)
	movq	%r12, %rsi
	call	trap_BotGoalName
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movq	%r12, %rdx
	movl	$.LC98, %esi
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC99, %edx
	movl	$-1, %esi
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movss	(%rsp), %xmm2
	movss	%xmm2, 6748(%rbx)
	jmp	.L450
.L712:
	movss	%xmm3, 16(%rsp)
	call	rand
	andl	$32767, %eax
	movss	16(%rsp), %xmm3
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	4912(%rbx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	mulsd	.LC74(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	ja	.L423
	movss	6180(%rbx), %xmm0
	movss	floattime(%rip), %xmm1
	jmp	.L418
.L818:
	jp	.L420
	movl	8(%rbx), %edi
	movss	%xmm3, 16(%rsp)
	call	trap_EA_Gesture
	movl	6612(%rbx), %edi
	leaq	656(%rsp), %rsi
	movl	$36, %edx
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movl	$.LC88, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	BotAI_BotInitialChat
	movl	6612(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movss	floattime(%rip), %xmm0
	movss	%xmm0, 6180(%rbx)
	movaps	%xmm0, %xmm1
	movss	16(%rsp), %xmm3
	jmp	.L418
.L808:
	leaq	400(%rsp), %r12
	movl	6676(%rbx), %edi
	movl	$256, %edx
	movss	%xmm2, (%rsp)
	movq	%r12, %rsi
	call	trap_BotGoalName
	xorl	%ecx, %ecx
	movq	%r12, %rdx
	movl	$.LC92, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movss	(%rsp), %xmm2
	movss	%xmm2, 6748(%rbx)
	jmp	.L437
.L812:
	leaq	400(%rsp), %r12
	movl	6676(%rbx), %edi
	movl	$256, %edx
	movss	%xmm1, 32(%rsp)
	movq	%r12, %rsi
	call	trap_BotGoalName
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movq	%r12, %rdx
	movl	$.LC103, %esi
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	6616(%rbx), %esi
	movl	$.LC81, %edx
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movl	8(%rbx), %edi
	movl	$1048576, %esi
	call	trap_EA_Action
	movss	32(%rsp), %xmm1
	movss	%xmm1, 6748(%rbx)
	jmp	.L465
.L819:
	testb	%r12b, %r12b
	je	.L492
	movss	6748(%rbx), %xmm0
	ucomiss	.LC5(%rip), %xmm0
	jp	.L748
	je	.L493
.L748:
	movss	floattime(%rip), %xmm1
	ucomiss	%xmm0, %xmm1
	ja	.L824
.L493:
	movq	9136(%rbx), %rax
	testq	%rax, %rax
	je	.L825
	leaq	36(%rax), %rsi
	leaq	4916(%rbx), %rdi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L499
	testb	$4, 9144(%rbx)
	je	.L500
	movq	9136(%rbx), %rax
	movq	104(%rax), %rdx
	testq	%rdx, %rdx
	je	.L501
.L760:
	movq	%rdx, 9136(%rbx)
.L499:
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	ja	.L826
.L503:
	movq	9136(%rbx), %rdx
	testq	%rdx, %rdx
	je	.L723
	leaq	36(%rdx), %rax
	movq	36(%rdx), %rdx
	jmp	.L762
.L809:
	leaq	400(%rsp), %r12
	movl	6676(%rbx), %edi
	movl	$256, %edx
	movss	%xmm2, (%rsp)
	movq	%r12, %rsi
	call	trap_BotGoalName
	xorl	%ecx, %ecx
	movq	%r12, %rdx
	movl	$.LC97, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movss	(%rsp), %xmm2
	movss	%xmm2, 6748(%rbx)
	jmp	.L443
.L673:
	cmpl	$4, %eax
	je	.L827
	cmpl	$5, %eax
	je	.L828
	cmpl	$6, %eax
	.p2align 4,,5
	jne	.L523
	xorps	%xmm0, %xmm0
	movss	6748(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	jp	.L750
	je	.L534
.L750:
	movss	floattime(%rip), %xmm2
	ucomiss	%xmm1, %xmm2
	ja	.L829
.L534:
	movq	%rbx, %rdi
	call	BotTeam
	cmpl	$1, %eax
	je	.L538
	cmpl	$2, %eax
	.p2align 4,,2
	je	.L830
.L723:
	movl	$0, 6608(%rbx)
	xorl	%eax, %eax
	jmp	.L394
.L798:
	movl	8(%rbx), %edi
	call	trap_EA_Crouch
	jmp	.L487
.L797:
	movl	6528(%rbx), %edi
	xorps	%xmm0, %xmm0
	movss	.LC7(%rip), %xmm1
	movl	$36, %esi
	movss	%xmm2, (%rsp)
	movss	%xmm3, 16(%rsp)
	call	trap_Characteristic_BFloat
	movss	%xmm0, 32(%rsp)
	call	rand
	andl	$32767, %eax
	movss	(%rsp), %xmm2
	cvtsi2ss	%eax, %xmm1
	movss	32(%rsp), %xmm0
	movss	16(%rsp), %xmm3
	divss	%xmm2, %xmm1
	movss	4912(%rbx), %xmm2
	mulss	%xmm0, %xmm2
	ucomiss	%xmm1, %xmm2
	ja	.L486
	movss	6128(%rbx), %xmm1
	movss	floattime(%rip), %xmm0
	jmp	.L484
.L796:
	leaq	384(%rsp), %rsi
	movq	%rbx, %rdi
	movss	%xmm2, (%rsp)
	call	BotRoamGoal
	movss	384(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	368(%rsp), %rdi
	movss	%xmm0, 368(%rsp)
	movss	388(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 372(%rsp)
	movss	392(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 376(%rsp)
	call	vectoangles
	movss	.LC45(%rip), %xmm0
	mulss	6592(%rbx), %xmm0
	movss	(%rsp), %xmm2
	movss	%xmm0, 6592(%rbx)
	jmp	.L482
.L826:
	xorl	%edx, %edx
	movl	$.LC113, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	$0, 6608(%rbx)
	jmp	.L503
.L423:
	movl	8(%rbx), %edi
	movss	%xmm3, 16(%rsp)
	call	trap_EA_Gesture
	movss	6180(%rbx), %xmm0
	movss	floattime(%rip), %xmm1
	movss	16(%rsp), %xmm3
	jmp	.L418
.L825:
	movl	$0, 6608(%rbx)
	jmp	.L394
.L500:
	movq	9136(%rbx), %rax
	movq	96(%rax), %rdx
	testq	%rdx, %rdx
	jne	.L760
	movq	104(%rax), %rax
	orl	$4, 9144(%rbx)
	movq	%rax, 9136(%rbx)
	jmp	.L499
.L486:
	mulss	.LC2(%rip), %xmm0
	movaps	%xmm3, %xmm1
	movss	floattime(%rip), %xmm2
	addss	%xmm2, %xmm1
	addss	%xmm0, %xmm1
	movaps	%xmm2, %xmm0
	movss	%xmm1, 6128(%rbx)
	jmp	.L484
.L824:
	movq	9128(%rbx), %r13
	leaq	400(%rsp), %r12
	movb	$0, 400(%rsp)
	testq	%r13, %r13
	je	.L496
.L709:
	leaq	4(%r13), %rsi
	movl	$256, %edx
	movq	%r12, %rdi
	call	__strcat_chk
	cmpq	$0, 96(%r13)
	je	.L496
	movl	$256, %edx
	movl	$.LC111, %esi
	movq	%r12, %rdi
	call	__strcat_chk
	movq	96(%r13), %r13
	testq	%r13, %r13
	jne	.L709
.L496:
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	movq	%r12, %rdx
	movl	$.LC112, %esi
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	6616(%rbx), %esi
	movl	$.LC81, %edx
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movl	8(%rbx), %edi
	movl	$1048576, %esi
	call	trap_EA_Action
	movl	$0x00000000, 6748(%rbx)
	jmp	.L493
.L794:
	xorl	%edx, %edx
	movl	$.LC107, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	movss	%xmm2, (%rsp)
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	$2, %edx
	movl	6540(%rbx), %edi
	call	trap_BotEnterChat
	movss	(%rsp), %xmm2
	jmp	.L478
.L820:
	cmpl	$4, %eax
	je	.L831
	cmpl	$5, %eax
	.p2align 4,,5
	je	.L832
	cmpl	$13, %eax
	.p2align 4,,5
	je	.L833
	cmpl	$6, %eax
	.p2align 4,,5
	jne	.L523
	xorps	%xmm1, %xmm1
	movss	6748(%rbx), %xmm2
	ucomiss	%xmm1, %xmm2
	jne	.L571
	jp	.L571
.L761:
	movss	floattime(%rip), %xmm0
.L572:
	ucomiss	6752(%rbx), %xmm0
	ja	.L775
	jmp	.L523
	.p2align 4,,10
	.p2align 3
.L827:
	xorps	%xmm0, %xmm0
	movss	6748(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	jp	.L749
	.p2align 4,,2
	je	.L509
.L749:
	movss	floattime(%rip), %xmm2
	ucomiss	%xmm1, %xmm2
	jbe	.L509
	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$.LC114, %esi
	movq	%rbx, %rdi
	movss	%xmm0, 32(%rsp)
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC115, %edx
	orl	$-1, %esi
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movss	32(%rsp), %xmm0
	movss	%xmm0, 6748(%rbx)
.L509:
	movq	%rbx, %rdi
	call	BotTeam
	cmpl	$1, %eax
	je	.L513
	cmpl	$2, %eax
	jne	.L723
	movl	$ctf_redflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
.L515:
	leaq	4916(%rbx), %rdi
	movq	%rbp, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	jne	.L834
.L588:
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	jbe	.L600
.L773:
	movl	$0, 6608(%rbx)
.L600:
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	BotAlternateRoute
	movl	$1, %eax
	jmp	.L394
.L828:
	movss	floattime(%rip), %xmm0
	ucomiss	6160(%rbx), %xmm0
	jbe	.L523
	movq	%rbx, %rdi
	call	BotTeam
	cmpl	$1, %eax
	je	.L526
	cmpl	$2, %eax
	.p2align 4,,2
	jne	.L723
	movl	$ctf_blueflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
.L528:
	movq	%rbx, %rdi
	call	BotCTFCarryingFlag
	testl	%eax, %eax
	jne	.L529
	movl	$0, 6608(%rbx)
.L529:
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	jbe	.L530
	movl	$0, 6608(%rbx)
.L530:
	leaq	4916(%rbx), %rdi
	movq	%rbp, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L600
	movq	%rbx, %rdi
	call	BotCTFCarryingFlag
	testl	%eax, %eax
	.p2align 4,,2
	je	.L773
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movss	.LC10(%rip), %xmm1
	addss	floattime(%rip), %xmm1
	movss	%xmm1, 32(%rsp)
	call	rand
	andl	$32767, %eax
	movss	32(%rsp), %xmm1
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	mulss	.LC53(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, 6160(%rbx)
	jmp	.L600
.L799:
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$.LC107, %esi
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	cmpl	$8, 6768(%rbx)
	jne	.L491
	movl	$0, 6768(%rbx)
	jmp	.L491
.L821:
	cmpl	$13, %eax
	jne	.L523
	movss	floattime(%rip), %xmm2
	ucomiss	6164(%rbx), %xmm2
	jbe	.L523
	xorps	%xmm0, %xmm0
	movss	6748(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	jp	.L753
	je	.L578
.L753:
	ucomiss	%xmm1, %xmm2
	ja	.L835
.L578:
	movq	%rbx, %rdi
	.p2align 4,,7
	.p2align 3
	call	BotTeam
	cmpl	$1, %eax
	.p2align 4,,2
	je	.L582
	cmpl	$2, %eax
	.p2align 4,,2
	jne	.L723
	movl	$redobelisk, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
.L584:
	movq	%rbx, %rdi
	call	BotFeelingBad
	ucomiss	.LC13(%rip), %xmm0
	ja	.L523
	leaq	4916(%rbx), %rdi
	movq	%rbp, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L587
	movss	.LC94(%rip), %xmm1
	addss	floattime(%rip), %xmm1
	movss	%xmm1, 32(%rsp)
	call	rand
	andl	$32767, %eax
	movss	32(%rsp), %xmm1
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	mulss	.LC10(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, 6164(%rbx)
.L587:
	movss	4916(%rbx), %xmm0
	subss	(%rbp), %xmm0
	movss	4920(%rbx), %xmm2
	movss	4924(%rbx), %xmm1
	movss	%xmm0, 368(%rsp)
	mulss	%xmm0, %xmm0
	subss	4(%rbp), %xmm2
	movss	%xmm2, 372(%rsp)
	mulss	%xmm2, %xmm2
	subss	8(%rbp), %xmm1
	addss	%xmm2, %xmm0
	movss	%xmm1, 376(%rsp)
	mulss	%xmm1, %xmm1
	addss	%xmm1, %xmm0
	ucomiss	.LC108(%rip), %xmm0
	jae	.L588
	jp	.L588
	movss	.LC94(%rip), %xmm1
	addss	floattime(%rip), %xmm1
	movss	%xmm1, 32(%rsp)
	call	rand
	andl	$32767, %eax
	movss	32(%rsp), %xmm1
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	mulss	.LC10(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, 6164(%rbx)
	jmp	.L588
.L807:
	call	__stack_chk_fail
.L795:
	movl	6612(%rbx), %edi
	leaq	656(%rsp), %rsi
	movl	$36, %edx
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movq	%rbx, %rdi
	xorl	%eax, %eax
	movl	$.LC109, %esi
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	6616(%rbx), %esi
	movl	$.LC110, %edx
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	jmp	.L481
.L793:
	movl	6612(%rbx), %edi
	leaq	656(%rsp), %rsi
	movl	$36, %edx
	movss	%xmm2, (%rsp)
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movl	$.LC106, %esi
	xorl	%eax, %eax
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6616(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	6616(%rbx), %esi
	movl	$.LC81, %edx
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movl	8(%rbx), %edi
	movl	$1048576, %esi
	call	trap_EA_Action
	movss	(%rsp), %xmm2
	jmp	.L475
.L513:
	movl	$ctf_blueflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	jmp	.L515
.L501:
	movq	96(%rax), %rax
	andl	$-5, 9144(%rbx)
	movq	%rax, 9136(%rbx)
	jmp	.L499
.L831:
	xorps	%xmm0, %xmm0
	movss	6748(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	jp	.L751
	je	.L546
.L751:
	movss	floattime(%rip), %xmm2
	ucomiss	%xmm1, %xmm2
	ja	.L836
.L546:
	movl	$ctf_neutralflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	leaq	4916(%rbx), %rdi
	movq	%rbp, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L549
	movl	$0, 6608(%rbx)
.L549:
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	jbe	.L430
	jmp	.L764
.L830:
	movl	$ctf_redflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
.L540:
	leaq	4916(%rbx), %rdi
	movq	%rbp, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L588
.L784:
	movl	$0, 6608(%rbx)
	jmp	.L588
.L538:
	movl	$ctf_blueflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	jmp	.L540
.L834:
	movq	%rbx, %rdi
	call	BotTeam
	cmpl	$1, %eax
	je	.L518
	cmpl	$2, %eax
	.p2align 4,,2
	jne	.L784
	movl	$1, 6960(%rbx)
	.p2align 4,,2
	jmp	.L784
.L823:
	movss	floattime(%rip), %xmm1
	ucomiss	6164(%rbx), %xmm1
	jbe	.L523
	movss	6748(%rbx), %xmm0
	ucomiss	.LC5(%rip), %xmm0
	jp	.L754
	je	.L603
.L754:
	ucomiss	%xmm0, %xmm1
	jbe	.L603
	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$.LC118, %esi
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC119, %edx
	orl	$-1, %esi
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movl	$0x00000000, 6748(%rbx)
.L603:
	movq	%rbx, %rdi
	call	BotTeam
	cmpl	$1, %eax
	je	.L607
	cmpl	$2, %eax
	jne	.L723
	movl	$redobelisk, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
.L609:
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	jbe	.L610
	movl	$0, 6608(%rbx)
.L610:
	leaq	4916(%rbx), %rdi
	movq	%rbp, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L430
	movss	.LC3(%rip), %xmm1
	addss	floattime(%rip), %xmm1
	movss	%xmm1, 32(%rsp)
	call	rand
	andl	$32767, %eax
	movss	32(%rsp), %xmm1
	cvtsi2ss	%eax, %xmm0
	movl	$1, %eax
	divss	.LC30(%rip), %xmm0
	mulss	.LC10(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, 6164(%rbx)
	jmp	.L394
.L833:
	movss	floattime(%rip), %xmm2
	ucomiss	6164(%rbx), %xmm2
	jbe	.L523
	xorps	%xmm0, %xmm0
	movss	6748(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	jp	.L752
	je	.L562
.L752:
	ucomiss	%xmm1, %xmm2
	ja	.L837
.L562:
	movq	%rbx, %rdi
	.p2align 4,,7
	.p2align 3
	call	BotTeam
	cmpl	$1, %eax
	.p2align 4,,2
	je	.L566
	cmpl	$2, %eax
	.p2align 4,,2
	jne	.L723
	movl	$ctf_redflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	jmp	.L609
.L816:
	leaq	368(%rsp), %rbp
	leaq	36(%r12), %rdi
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movq	%rbp, %rsi
	call	AngleVectors
	movq	%rbp, %rdi
	movl	$0x00000000, 376(%rsp)
	leaq	352(%rsp), %rbp
	call	VectorNormalize
	movss	4916(%rbx), %xmm0
	movq	%rbp, %rdi
	subss	232(%rsp), %xmm0
	movss	%xmm0, 352(%rsp)
	movss	4920(%rbx), %xmm0
	subss	236(%rsp), %xmm0
	movss	%xmm0, 356(%rsp)
	movss	4924(%rbx), %xmm0
	subss	240(%rsp), %xmm0
	movss	%xmm0, 360(%rsp)
	call	VectorNormalize
	movss	368(%rsp), %xmm0
	movss	372(%rsp), %xmm1
	mulss	352(%rsp), %xmm0
	mulss	356(%rsp), %xmm1
	movss	(%rsp), %xmm2
	addss	%xmm1, %xmm0
	movss	376(%rsp), %xmm1
	mulss	360(%rsp), %xmm1
	addss	%xmm1, %xmm0
	cvtss2sd	%xmm0, %xmm0
	ucomisd	.LC86(%rip), %xmm0
	jbe	.L405
	movq	%rbx, %rdi
	call	BotSetupForMovement
	movl	6532(%rbx), %edi
	movl	$1, %edx
	movq	%rbp, %rsi
	movss	.LC87(%rip), %xmm0
	call	trap_BotMoveInDirection
	movss	(%rsp), %xmm2
	jmp	.L405
.L518:
	movl	$1, 6964(%rbx)
	jmp	.L784
.L571:
	movss	floattime(%rip), %xmm0
	ucomiss	%xmm2, %xmm0
	jbe	.L572
	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$.LC116, %esi
	movq	%rbx, %rdi
	movss	%xmm1, 32(%rsp)
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC117, %edx
	orl	$-1, %esi
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movss	32(%rsp), %xmm1
	movss	%xmm1, 6748(%rbx)
	jmp	.L761
.L832:
	movq	%rbx, %rdi
	call	BotTeam
	cmpl	$1, %eax
	je	.L553
	cmpl	$2, %eax
	.p2align 4,,2
	jne	.L723
	movl	$ctf_redflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
.L555:
	movq	%rbx, %rdi
	call	Bot1FCTFCarryingFlag
	testl	%eax, %eax
	jne	.L556
	movl	$0, 6608(%rbx)
.L556:
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	jbe	.L557
	movl	$0, 6608(%rbx)
.L557:
	leaq	4916(%rbx), %rdi
	movq	%rbp, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	jne	.L773
	jmp	.L600
.L526:
	movl	$ctf_redflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	jmp	.L528
.L829:
	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$.LC116, %esi
	movq	%rbx, %rdi
	movss	%xmm0, 32(%rsp)
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC117, %edx
	orl	$-1, %esi
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movss	32(%rsp), %xmm0
	movss	%xmm0, 6748(%rbx)
	jmp	.L534
.L822:
	movq	%rbx, %rdi
	call	BotTeam
	cmpl	$1, %eax
	je	.L594
	cmpl	$2, %eax
	.p2align 4,,2
	je	.L838
.L766:
	movq	%rbx, %rdi
	.p2align 4,,5
	call	BotGoHarvest
	xorl	%eax, %eax
	.p2align 4,,2
	jmp	.L394
.L835:
	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$.LC118, %esi
	movq	%rbx, %rdi
	movss	%xmm0, 32(%rsp)
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC119, %edx
	orl	$-1, %esi
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movss	32(%rsp), %xmm0
	movss	%xmm0, 6748(%rbx)
	jmp	.L578
.L838:
	movl	$redobelisk, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
.L596:
	movq	%rbx, %rdi
	call	BotHarvesterCarryingCubes
	testl	%eax, %eax
	je	.L766
	movss	floattime(%rip), %xmm0
	ucomiss	6752(%rbx), %xmm0
	ja	.L766
	leaq	4916(%rbx), %rdi
	movq	%rbp, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L600
	jmp	.L766
.L594:
	movl	$blueobelisk, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	jmp	.L596
.L582:
	movl	$blueobelisk, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	jmp	.L584
.L566:
	movl	$ctf_blueflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	jmp	.L609
.L553:
	movl	$ctf_blueflag, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	jmp	.L555
.L836:
	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$.LC114, %esi
	movq	%rbx, %rdi
	movss	%xmm0, 32(%rsp)
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC115, %edx
	orl	$-1, %esi
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movss	32(%rsp), %xmm0
	movss	%xmm0, 6748(%rbx)
	jmp	.L546
.L837:
	xorl	%eax, %eax
	xorl	%edx, %edx
	movl	$.LC118, %esi
	movq	%rbx, %rdi
	movss	%xmm0, 32(%rsp)
	call	BotAI_BotInitialChat
	movl	6540(%rbx), %edi
	movl	$1, %edx
	xorl	%esi, %esi
	call	trap_BotEnterChat
	movl	$.LC119, %edx
	orl	$-1, %esi
	movq	%rbx, %rdi
	call	BotVoiceChatOnly
	movss	32(%rsp), %xmm0
	movss	%xmm0, 6748(%rbx)
	jmp	.L562
.L607:
	movl	$blueobelisk, %esi
	movl	$14, %ecx
	movq	%rbp, %rdi
	rep movsl
	jmp	.L609
	.cfi_endproc
.LFE77:
	.size	BotGetLongTermGoal, .-BotGetLongTermGoal
	.section	.rodata.str1.1
.LC121:
	.string	"lead_stop"
.LC123:
	.string	"followme"
	.text
	.p2align 4,,15
.globl BotLongTermGoal
	.type	BotLongTermGoal, @function
BotLongTermGoal:
.LFB78:
	.cfi_startproc
	movq	%rbx, -40(%rsp)
	movq	%rbp, -32(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -40
	.cfi_offset 3, -48
	movq	%r12, -24(%rsp)
	movq	%r13, -16(%rsp)
	movl	%edx, %r12d
	.cfi_offset 13, -24
	.cfi_offset 12, -32
	movq	%r14, -8(%rsp)
	subq	$488, %rsp
	.cfi_def_cfa_offset 496
	movl	%esi, %r13d
	movq	%fs:40, %rax
	movq	%rax, 440(%rsp)
	xorl	%eax, %eax
	testl	%edx, %edx
	movq	%rcx, %rbp
	movss	6892(%rdi), %xmm0
	je	.L883
	.cfi_offset 14, -16
.L840:
	movq	%rbp, %rcx
	movl	%r12d, %edx
	movl	%r13d, %esi
	movq	%rbx, %rdi
	call	BotGetLongTermGoal
.L844:
	movq	440(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L884
	movq	448(%rsp), %rbx
	movq	456(%rsp), %rbp
	movq	464(%rsp), %r12
	movq	472(%rsp), %r13
	movq	480(%rsp), %r14
	addq	$488, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L883:
	xorps	%xmm1, %xmm1
	ucomiss	%xmm1, %xmm0
	jbe	.L840
	movss	floattime(%rip), %xmm1
	ucomiss	%xmm0, %xmm1
	ja	.L885
	xorps	%xmm2, %xmm2
	movss	6900(%rdi), %xmm0
	ucomiss	%xmm0, %xmm2
	jbe	.L845
	movss	.LC122(%rip), %xmm2
	xorps	%xmm2, %xmm0
	ucomiss	%xmm0, %xmm1
	ja	.L886
.L845:
	leaq	16(%rsp), %r14
	movl	6832(%rbx), %edi
	movq	%r14, %rsi
	call	BotEntityInfo
	movl	16(%rsp), %esi
	testl	%esi, %esi
	jne	.L877
.L882:
	movl	6832(%rbx), %ecx
.L848:
	movl	12(%rbx), %edi
	leaq	6572(%rbx), %rdx
	leaq	4944(%rbx), %rsi
	movss	.LC41(%rip), %xmm0
	call	BotEntityVisible
	xorps	%xmm1, %xmm1
	ucomiss	%xmm1, %xmm0
	jne	.L849
	jp	.L849
	movss	floattime(%rip), %xmm1
	movss	6896(%rbx), %xmm2
.L850:
	movaps	%xmm1, %xmm0
	subss	.LC7(%rip), %xmm0
	ucomiss	%xmm2, %xmm0
	ja	.L851
	movss	6904(%rbx), %xmm4
.L852:
	movss	4916(%rbx), %xmm7
	ucomiss	%xmm1, %xmm4
	movss	4920(%rbx), %xmm6
	movaps	%xmm7, %xmm0
	movaps	%xmm6, %xmm3
	subss	6836(%rbx), %xmm0
	movss	4924(%rbx), %xmm5
	subss	6840(%rbx), %xmm3
	movaps	%xmm5, %xmm2
	subss	6844(%rbx), %xmm2
	movss	%xmm0, 160(%rsp)
	mulss	%xmm0, %xmm0
	movss	%xmm3, 164(%rsp)
	mulss	%xmm3, %xmm3
	movss	%xmm2, 168(%rsp)
	mulss	%xmm2, %xmm2
	addss	%xmm3, %xmm0
	addss	%xmm2, %xmm0
	jbe	.L880
	subss	.LC9(%rip), %xmm1
	ucomiss	6900(%rbx), %xmm1
	ja	.L887
.L855:
	ucomiss	.LC82(%rip), %xmm0
	jae	.L857
	jp	.L857
	movl	$0x00000000, 6904(%rbx)
.L857:
	movq	6836(%rbx), %rdx
	leaq	6836(%rbx), %rax
	movq	%rdx, (%rbp)
	movq	8(%rax), %rdx
	movq	%rdx, 8(%rbp)
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rbp)
	movq	24(%rax), %rdx
	movq	%rdx, 24(%rbp)
	movq	32(%rax), %rdx
	movq	%rdx, 32(%rbp)
	movq	40(%rax), %rdx
	movq	%rdx, 40(%rbp)
	movq	48(%rax), %rax
	movq	%rax, 48(%rbp)
	movl	$1, %eax
	jmp	.L844
	.p2align 4,,10
	.p2align 3
.L849:
	movss	floattime(%rip), %xmm1
	movss	%xmm1, 6896(%rbx)
	movaps	%xmm1, %xmm2
	jmp	.L850
	.p2align 4,,10
	.p2align 3
.L880:
	ucomiss	.LC124(%rip), %xmm0
	jbe	.L840
	subss	.LC9(%rip), %xmm1
	ucomiss	6900(%rbx), %xmm1
	jbe	.L860
	movl	6832(%rbx), %edi
	leaq	176(%rsp), %rsi
	movl	$256, %edx
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movl	$.LC123, %esi
	xorl	%eax, %eax
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6612(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	floattime(%rip), %eax
	movss	4916(%rbx), %xmm7
	movss	4920(%rbx), %xmm6
	movss	4924(%rbx), %xmm5
	movl	%eax, 6900(%rbx)
.L860:
	movss	40(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	%xmm7, %xmm0
	leaq	160(%rsp), %rdi
	movss	%xmm0, 160(%rsp)
	movss	44(%rsp), %xmm0
	subss	%xmm6, %xmm0
	movss	%xmm0, 164(%rsp)
	movss	48(%rsp), %xmm0
	subss	%xmm5, %xmm0
	movss	%xmm0, 168(%rsp)
	call	vectoangles
	movss	.LC45(%rip), %xmm0
	xorl	%eax, %eax
	mulss	6592(%rbx), %xmm0
	movss	%xmm0, 6592(%rbx)
	jmp	.L844
	.p2align 4,,10
	.p2align 3
.L851:
	movss	.LC3(%rip), %xmm4
	addss	%xmm1, %xmm4
	movss	%xmm4, 6904(%rbx)
	jmp	.L852
	.p2align 4,,10
	.p2align 3
.L877:
	leaq	24(%r14), %rdi
	call	BotPointAreaNum
	testl	%eax, %eax
	movl	%eax, %r14d
	je	.L882
	movl	%eax, %edi
	call	trap_AAS_AreaReachability
	testl	%eax, %eax
	je	.L882
	movl	40(%rsp), %eax
	movl	6832(%rbx), %ecx
	movl	%r14d, 6848(%rbx)
	movl	$0xc1000000, 6852(%rbx)
	movl	$0xc1000000, 6856(%rbx)
	movl	$0xc1000000, 6860(%rbx)
	movl	%eax, 6836(%rbx)
	movl	44(%rsp), %eax
	movl	%ecx, 6876(%rbx)
	movl	$0x41000000, 6864(%rbx)
	movl	$0x41000000, 6868(%rbx)
	movl	$0x41000000, 6872(%rbx)
	movl	%eax, 6840(%rbx)
	movl	48(%rsp), %eax
	movl	%eax, 6844(%rbx)
	jmp	.L848
	.p2align 4,,10
	.p2align 3
.L886:
	movl	6832(%rdi), %edi
	leaq	176(%rsp), %rsi
	movl	$256, %edx
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movl	$.LC123, %esi
	xorl	%eax, %eax
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6612(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	floattime(%rip), %eax
	movl	%eax, 6900(%rbx)
	jmp	.L845
	.p2align 4,,10
	.p2align 3
.L887:
	movl	6832(%rbx), %edi
	leaq	176(%rsp), %rsi
	movl	$256, %edx
	movss	%xmm0, (%rsp)
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movl	$.LC123, %esi
	xorl	%eax, %eax
	movq	%rbx, %rdi
	call	BotAI_BotInitialChat
	movl	6612(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	floattime(%rip), %eax
	movss	(%rsp), %xmm0
	movl	%eax, 6900(%rbx)
	jmp	.L855
	.p2align 4,,10
	.p2align 3
.L885:
	movl	6832(%rdi), %edi
	leaq	176(%rsp), %rsi
	movl	$256, %edx
	call	EasyClientName
	xorl	%ecx, %ecx
	movq	%rax, %rdx
	movq	%rbx, %rdi
	xorl	%eax, %eax
	movl	$.LC121, %esi
	call	BotAI_BotInitialChat
	movl	6612(%rbx), %esi
	movl	6540(%rbx), %edi
	movl	$2, %edx
	call	trap_BotEnterChat
	movl	$0x00000000, 6892(%rbx)
	movq	%rbp, %rcx
	xorl	%edx, %edx
	movl	%r13d, %esi
	movq	%rbx, %rdi
	call	BotGetLongTermGoal
	jmp	.L844
.L884:
	call	__stack_chk_fail
	.cfi_endproc
.LFE78:
	.size	BotLongTermGoal, .-BotLongTermGoal
	.section	.rodata.str1.1
.LC125:
	.string	"battle retreat: observer"
.LC126:
	.string	"battle retreat: intermission"
.LC127:
	.string	"battle retreat: bot dead"
.LC128:
	.string	"battle retreat: no enemy"
.LC129:
	.string	"battle retreat: enemy dead"
	.section	.rodata.str1.8
	.align 8
.LC130:
	.string	"battle retreat: wants to chase"
	.section	.rodata.str1.1
.LC131:
	.string	"battle retreat: lost enemy"
.LC132:
	.string	"battle retreat: another enemy"
.LC133:
	.string	"battle retreat: no way out"
.LC135:
	.string	"battle retreat: nbg"
	.text
	.p2align 4,,15
.globl AINode_Battle_Retreat
	.type	AINode_Battle_Retreat, @function
AINode_Battle_Retreat:
.LFB101:
	.cfi_startproc
	movq	%rbx, -24(%rsp)
	movq	%rbp, -16(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -24
	.cfi_offset 3, -32
	movq	%r12, -8(%rsp)
	subq	$344, %rsp
	.cfi_def_cfa_offset 352
	.cfi_offset 12, -16
	call	BotIsObserver
	testl	%eax, %eax
	jne	.L934
	movq	%rbx, %rdi
	call	BotIntermission
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L935
	movq	%rbx, %rdi
	call	BotIsDead
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L936
	movl	6548(%rbx), %edi
	testl	%edi, %edi
	js	.L937
	leaq	16(%rsp), %rbp
	movq	%rbp, %rsi
	call	BotEntityInfo
	movq	%rbp, %rdi
	call	EntityIsDead
	testl	%eax, %eax
	jne	.L938
	movl	6548(%rbx), %esi
	movq	%rbx, %rdi
	call	BotFindEnemy
	movl	$18616254, 5984(%rbx)
	movl	bot_grapple+12(%rip), %ebp
	testl	%ebp, %ebp
	je	.L895
	movl	$18632638, 5984(%rbx)
.L895:
	movq	%rbx, %rdi
	call	BotInLavaOrSlime
	testl	%eax, %eax
	je	.L896
	orl	$6291456, 5984(%rbx)
.L896:
	movq	%rbx, %rdi
	call	BotMapScripts
	movl	6548(%rbx), %esi
	movq	%rbx, %rdi
	call	BotUpdateBattleInventory
	movq	%rbx, %rdi
	call	BotWantsToChase
	testl	%eax, %eax
	jne	.L939
	movl	6548(%rbx), %ecx
	movl	12(%rbx), %edi
	leaq	6572(%rbx), %rdx
	leaq	4944(%rbx), %rsi
	movss	.LC41(%rip), %xmm0
	call	BotEntityVisible
	ucomiss	.LC5(%rip), %xmm0
	jp	.L932
	je	.L898
.L932:
	movl	floattime(%rip), %eax
	movss	48(%rsp), %xmm0
	movss	%xmm0, 312(%rsp)
	movl	%eax, 6096(%rbx)
	movl	40(%rsp), %eax
	movl	%eax, 304(%rsp)
	movl	44(%rsp), %eax
	movl	%eax, 308(%rsp)
	movl	6548(%rbx), %eax
	cmpl	$63, %eax
	jle	.L900
	cmpl	redobelisk+40(%rip), %eax
	je	.L901
	cmpl	blueobelisk+40(%rip), %eax
	je	.L901
.L900:
	leaq	304(%rsp), %rdi
	call	BotPointAreaNum
	testl	%eax, %eax
	movl	%eax, %ebp
	je	.L898
	movl	%eax, %edi
	call	trap_AAS_AreaReachability
	testl	%eax, %eax
	je	.L898
	movl	304(%rsp), %eax
	movl	%ebp, 6552(%rbx)
	movl	%eax, 6556(%rbx)
	movl	308(%rsp), %eax
	movl	%eax, 6560(%rbx)
	movl	312(%rsp), %eax
	movl	%eax, 6564(%rbx)
	.p2align 4,,10
	.p2align 3
.L898:
	movss	floattime(%rip), %xmm1
	movaps	%xmm1, %xmm2
	movss	6096(%rbx), %xmm0
	subss	.LC85(%rip), %xmm2
	ucomiss	%xmm0, %xmm2
	ja	.L940
	ucomiss	%xmm0, %xmm1
	jbe	.L904
	movl	$-1, %esi
	movq	%rbx, %rdi
	call	BotFindEnemy
	testl	%eax, %eax
	je	.L904
	movl	$.LC132, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Fight
	xorl	%eax, %eax
	jmp	.L890
	.p2align 4,,10
	.p2align 3
.L935:
	movl	$.LC126, %esi
	movq	%rbx, %rdi
	call	AIEnter_Intermission
	xorl	%eax, %eax
.L890:
	movq	320(%rsp), %rbx
	movq	328(%rsp), %rbp
	movq	336(%rsp), %r12
	addq	$344, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L934:
	movl	$.LC125, %esi
	movq	%rbx, %rdi
	call	AIEnter_Observer
	xorl	%eax, %eax
	jmp	.L890
	.p2align 4,,10
	.p2align 3
.L938:
	movl	$.LC129, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L890
	.p2align 4,,10
	.p2align 3
.L904:
	movl	$1, %esi
	movq	%rbx, %rdi
	leaq	160(%rsp), %rbp
	call	BotTeamGoals
	movq	%rbx, %rdi
	call	BotBattleUseItems
	movl	5984(%rbx), %esi
	movq	%rbp, %rcx
	movl	$1, %edx
	movq	%rbx, %rdi
	call	BotLongTermGoal
	testl	%eax, %eax
	je	.L941
	movss	floattime(%rip), %xmm0
	ucomiss	6100(%rbx), %xmm0
	jbe	.L907
	addss	.LC7(%rip), %xmm0
	movl	gametype(%rip), %eax
	cmpl	$9, %eax
	movss	%xmm0, 6100(%rbx)
	je	.L927
	cmpl	$4, %eax
	je	.L927
	cmpl	$5, %eax
	je	.L942
	cmpl	$7, %eax
	.p2align 4,,5
	je	.L943
.L911:
	movss	.LC55(%rip), %xmm0
	movss	%xmm0, 12(%rsp)
.L912:
	movl	5984(%rbx), %esi
	movss	12(%rsp), %xmm0
	movq	%rbp, %rdx
	movq	%rbx, %rdi
	call	BotNearbyGoal
	testl	%eax, %eax
	jne	.L944
.L907:
	movq	%rbx, %rdi
	leaq	224(%rsp), %r12
	call	BotSetupForMovement
	movl	5984(%rbx), %ecx
	movl	6532(%rbx), %esi
	movq	%r12, %rdi
	movq	%rbp, %rdx
	call	trap_BotMoveToGoal
	movl	224(%rsp), %edi
	testl	%edi, %edi
	jne	.L945
.L914:
	xorl	%edx, %edx
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	BotAIBlocked
	movq	%rbx, %rdi
	call	BotChooseWeapon
	movl	244(%rsp), %eax
	testb	$3, %al
	je	.L915
	movl	264(%rsp), %edx
	movl	%edx, 6584(%rbx)
	movl	268(%rsp), %edx
	movl	%edx, 6588(%rbx)
	movl	272(%rsp), %edx
	movl	%edx, 6592(%rbx)
.L916:
	testb	$16, %al
	je	.L921
	movl	248(%rsp), %eax
	movl	%eax, 6568(%rbx)
.L921:
	movq	%rbx, %rdi
	call	BotCheckAttack
	movl	$1, %eax
	jmp	.L890
	.p2align 4,,10
	.p2align 3
.L936:
	movl	$.LC127, %esi
	movq	%rbx, %rdi
	call	AIEnter_Respawn
	xorl	%eax, %eax
	jmp	.L890
	.p2align 4,,10
	.p2align 3
.L939:
	movl	6536(%rbx), %edi
	call	trap_BotEmptyGoalStack
	movl	$.LC130, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Chase
	xorl	%eax, %eax
	jmp	.L890
	.p2align 4,,10
	.p2align 3
.L937:
	movl	$.LC128, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L890
	.p2align 4,,10
	.p2align 3
.L915:
	testb	$8, %al
	jne	.L916
	testb	$32, 5988(%rbx)
	.p2align 4,,2
	jne	.L916
	movl	6528(%rbx), %edi
	xorps	%xmm0, %xmm0
	movss	.LC7(%rip), %xmm1
	movl	$2, %esi
	call	trap_Characteristic_BFloat
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	ucomisd	.LC44(%rip), %xmm0
	jbe	.L930
	movq	%rbx, %rdi
	call	BotAimAtEnemy
	movl	244(%rsp), %eax
	jmp	.L916
	.p2align 4,,10
	.p2align 3
.L940:
	movl	$.LC131, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_LTG
	xorl	%eax, %eax
	jmp	.L890
	.p2align 4,,10
	.p2align 3
.L941:
	movl	$.LC133, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_SuicidalFight
	xorl	%eax, %eax
	jmp	.L890
	.p2align 4,,10
	.p2align 3
.L945:
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movl	$0x00000000, 6076(%rbx)
	jmp	.L914
	.p2align 4,,10
	.p2align 3
.L901:
	addss	.LC42(%rip), %xmm0
	movss	%xmm0, 312(%rsp)
	jmp	.L900
	.p2align 4,,10
	.p2align 3
.L927:
	movq	%rbx, %rdi
	call	BotCTFCarryingFlag
	testl	%eax, %eax
	je	.L911
.L933:
	movss	.LC13(%rip), %xmm0
	movss	%xmm0, 12(%rsp)
	jmp	.L912
.L944:
	movl	6532(%rbx), %edi
	call	trap_BotResetLastAvoidReach
	movss	12(%rsp), %xmm0
	movl	$.LC135, %esi
	divss	.LC95(%rip), %xmm0
	movq	%rbx, %rdi
	addss	floattime(%rip), %xmm0
	addss	.LC7(%rip), %xmm0
	movss	%xmm0, 6080(%rbx)
	call	AIEnter_Battle_NBG
	xorl	%eax, %eax
	jmp	.L890
.L942:
	movq	%rbx, %rdi
	call	Bot1FCTFCarryingFlag
	testl	%eax, %eax
	.p2align 4,,2
	je	.L911
	.p2align 4,,4
	jmp	.L933
.L943:
	movq	%rbx, %rdi
	.p2align 4,,7
	.p2align 3
	call	BotHarvesterCarryingCubes
	testl	%eax, %eax
	.p2align 4,,4
	je	.L911
	movss	.LC134(%rip), %xmm0
	movss	%xmm0, 12(%rsp)
	jmp	.L912
.L930:
	movl	5984(%rbx), %edx
	movl	6532(%rbx), %edi
	leaq	304(%rsp), %rcx
	movss	.LC11(%rip), %xmm0
	movq	%rbp, %rsi
	call	trap_BotMovementViewTarget
	testl	%eax, %eax
	je	.L919
	movss	304(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	288(%rsp), %rdi
	movss	%xmm0, 288(%rsp)
	movss	308(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 292(%rsp)
	movss	312(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 296(%rsp)
	call	vectoangles
.L920:
	movss	.LC45(%rip), %xmm0
	movl	244(%rsp), %eax
	mulss	6592(%rbx), %xmm0
	movss	%xmm0, 6592(%rbx)
	jmp	.L916
.L919:
	leaq	6584(%rbx), %rsi
	leaq	28(%r12), %rdi
	call	vectoangles
	jmp	.L920
	.cfi_endproc
.LFE101:
	.size	AINode_Battle_Retreat, .-AINode_Battle_Retreat
	.section	.rodata.str1.1
.LC136:
	.string	"seek ltg: observer"
.LC137:
	.string	"seek ltg: intermission"
.LC138:
	.string	"seek ltg: bot dead"
.LC139:
	.string	"seek ltg: random chat"
.LC140:
	.string	"seek ltg: found enemy"
.LC142:
	.string	"ltg seek: nbg"
	.text
	.p2align 4,,15
.globl AINode_Seek_LTG
	.type	AINode_Seek_LTG, @function
AINode_Seek_LTG:
.LFB94:
	.cfi_startproc
	movq	%rbx, -32(%rsp)
	movq	%rbp, -24(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -32
	.cfi_offset 3, -40
	movq	%r12, -16(%rsp)
	movq	%r13, -8(%rsp)
	subq	$200, %rsp
	.cfi_def_cfa_offset 208
	.cfi_offset 13, -16
	.cfi_offset 12, -24
	call	BotIsObserver
	testl	%eax, %eax
	jne	.L991
	movq	%rbx, %rdi
	call	BotIntermission
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L992
	movq	%rbx, %rdi
	call	BotIsDead
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L993
	movq	%rbx, %rdi
	call	BotChat_Random
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L994
	movl	$18616254, 5984(%rbx)
	movl	bot_grapple+12(%rip), %r9d
	testl	%r9d, %r9d
	je	.L952
	movl	$18632638, 5984(%rbx)
.L952:
	movq	%rbx, %rdi
	call	BotInLavaOrSlime
	testl	%eax, %eax
	je	.L953
	orl	$6291456, 5984(%rbx)
.L953:
	movq	%rbx, %rdi
	call	BotCanAndWantsToRocketJump
	testl	%eax, %eax
	je	.L954
	orl	$4096, 5984(%rbx)
.L954:
	movq	%rbx, %rdi
	call	BotMapScripts
	movss	floattime(%rip), %xmm0
	movl	$-1, 6548(%rbx)
	subss	.LC3(%rip), %xmm0
	movss	6176(%rbx), %xmm1
	ucomiss	%xmm0, %xmm1
	ja	.L995
.L955:
	movl	$-1, %esi
	movq	%rbx, %rdi
	call	BotFindEnemy
	testl	%eax, %eax
	je	.L958
	movq	%rbx, %rdi
	call	BotWantsToRetreat
	testl	%eax, %eax
	.p2align 4,,2
	je	.L959
	movl	$.LC140, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Retreat
	xorl	%eax, %eax
	jmp	.L948
	.p2align 4,,10
	.p2align 3
.L992:
	movl	$.LC137, %esi
	movq	%rbx, %rdi
	call	AIEnter_Intermission
	xorl	%eax, %eax
.L948:
	movq	168(%rsp), %rbx
	movq	176(%rsp), %rbp
	movq	184(%rsp), %r12
	movq	192(%rsp), %r13
	addq	$200, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L991:
	movl	$.LC136, %esi
	movq	%rbx, %rdi
	call	AIEnter_Observer
	xorl	%eax, %eax
	jmp	.L948
	.p2align 4,,10
	.p2align 3
.L993:
	movl	$.LC138, %esi
	movq	%rbx, %rdi
	call	AIEnter_Respawn
	xorl	%eax, %eax
	jmp	.L948
	.p2align 4,,10
	.p2align 3
.L958:
	xorl	%esi, %esi
	movq	%rbx, %rdi
	call	BotTeamGoals
	movl	5984(%rbx), %esi
	xorl	%edx, %edx
	movq	%rsp, %rcx
	movq	%rbx, %rdi
	call	BotLongTermGoal
	testl	%eax, %eax
	je	.L960
	movss	floattime(%rip), %xmm0
	ucomiss	6100(%rbx), %xmm0
	jbe	.L961
	addss	.LC45(%rip), %xmm0
	movq	%rbx, %rdi
	movl	$150, %r12d
	movss	%xmm0, 6100(%rbx)
	call	BotWantsToCamp
	cmpl	$3, 6608(%rbx)
	movl	$400, %eax
	cmove	%eax, %r12d
	movl	gametype(%rip), %eax
	cmpl	$9, %eax
	je	.L985
	cmpl	$4, %eax
	je	.L985
	cmpl	$5, %eax
	je	.L996
	cmpl	$7, %eax
	.p2align 4,,5
	je	.L997
.L968:
	cvtsi2ss	%r12d, %xmm0
	movl	5984(%rbx), %esi
	movq	%rsp, %rdx
	movq	%rbx, %rdi
	call	BotNearbyGoal
	testl	%eax, %eax
	je	.L961
	movl	6532(%rbx), %edi
	call	trap_BotResetLastAvoidReach
	cvtsi2sd	%r12d, %xmm1
	movl	$.LC142, %esi
	movss	.LC85(%rip), %xmm0
	movq	%rbx, %rdi
	addss	floattime(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	mulsd	.LC141(%rip), %xmm1
	cvtps2pd	%xmm0, %xmm0
	addsd	%xmm1, %xmm0
	unpcklpd	%xmm0, %xmm0
	cvtpd2ps	%xmm0, %xmm0
	movss	%xmm0, 6080(%rbx)
	call	AIEnter_Seek_NBG
	xorl	%eax, %eax
	jmp	.L948
	.p2align 4,,10
	.p2align 3
.L995:
	call	rand
	andl	$32767, %eax
	movss	4912(%rbx), %xmm1
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	ucomiss	%xmm0, %xmm1
	jbe	.L955
	movl	8(%rbx), %edi
	call	trap_EA_Gesture
	jmp	.L955
	.p2align 4,,10
	.p2align 3
.L994:
	movq	%rbx, %rdi
	call	BotChatTime
	addss	floattime(%rip), %xmm0
	movl	$.LC139, %esi
	movq	%rbx, %rdi
	movss	%xmm0, 6104(%rbx)
	call	AIEnter_Stand
	xorl	%eax, %eax
	jmp	.L948
	.p2align 4,,10
	.p2align 3
.L959:
	movl	6532(%rbx), %edi
	call	trap_BotResetLastAvoidReach
	movl	6536(%rbx), %edi
	call	trap_BotEmptyGoalStack
	movl	$.LC140, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Fight
	xorl	%eax, %eax
	jmp	.L948
	.p2align 4,,10
	.p2align 3
.L961:
	movq	%rsp, %rsi
	movq	%rbx, %rdi
	call	BotAIPredictObstacles
	movl	%eax, %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	jne	.L948
	movq	%rbx, %rdi
	leaq	64(%rsp), %r12
	call	BotSetupForMovement
	movl	5984(%rbx), %ecx
	movl	6532(%rbx), %esi
	movq	%rsp, %rdx
	movq	%r12, %rdi
	call	trap_BotMoveToGoal
	movl	64(%rsp), %r8d
	testl	%r8d, %r8d
	jne	.L998
.L971:
	movl	$1, %edx
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	BotAIBlocked
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	BotClearPath
	movl	84(%rsp), %eax
	testb	$11, %al
	je	.L972
	movl	104(%rsp), %edx
	movl	%edx, 6584(%rbx)
	movl	108(%rsp), %edx
	movl	%edx, 6588(%rbx)
	movl	112(%rsp), %edx
	movl	%edx, 6592(%rbx)
.L973:
	testb	$16, %al
	je	.L960
	movl	88(%rsp), %eax
	movl	%eax, 6568(%rbx)
	movl	$1, %eax
	jmp	.L948
	.p2align 4,,10
	.p2align 3
.L960:
	movl	$1, %eax
	jmp	.L948
.L972:
	testb	$4, %al
	jne	.L999
	testb	$32, 5988(%rbx)
	.p2align 4,,2
	jne	.L973
	leaq	144(%rsp), %r13
	movl	5984(%rbx), %edx
	movl	6532(%rbx), %edi
	movss	.LC11(%rip), %xmm0
	movq	%rsp, %rsi
	movq	%r13, %rcx
	call	trap_BotMovementViewTarget
	testl	%eax, %eax
	jne	.L1000
	movss	92(%rsp), %xmm0
	movss	96(%rsp), %xmm2
	mulss	%xmm0, %xmm0
	mulss	%xmm2, %xmm2
	movss	100(%rsp), %xmm1
	mulss	%xmm1, %xmm1
	addss	%xmm2, %xmm0
	addss	%xmm1, %xmm0
	ucomiss	.LC5(%rip), %xmm0
	jp	.L989
	je	.L987
.L989:
	leaq	6584(%rbx), %rsi
	leaq	28(%r12), %rdi
	call	vectoangles
	movss	6592(%rbx), %xmm1
	movss	.LC45(%rip), %xmm0
.L977:
	mulss	%xmm1, %xmm0
	movss	%xmm0, 6592(%rbx)
.L990:
	movl	84(%rsp), %eax
	jmp	.L973
	.p2align 4,,10
	.p2align 3
.L985:
	movq	%rbx, %rdi
	call	BotCTFCarryingFlag
	testl	%eax, %eax
	je	.L968
.L967:
	movl	$50, %r12d
	.p2align 4,,2
	jmp	.L968
.L998:
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movl	$0x00000000, 6076(%rbx)
	jmp	.L971
.L999:
	call	rand
	andl	$32767, %eax
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	4912(%rbx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	mulsd	.LC76(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jbe	.L990
	leaq	144(%rsp), %rsi
	movq	%rbx, %rdi
	call	BotRoamGoal
	movss	144(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	128(%rsp), %rdi
	movss	%xmm0, 128(%rsp)
	movss	148(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 132(%rsp)
	movss	152(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 136(%rsp)
	call	vectoangles
	movss	.LC45(%rip), %xmm0
	mulss	6592(%rbx), %xmm0
	movl	84(%rsp), %eax
	movss	%xmm0, 6592(%rbx)
	jmp	.L973
.L996:
	movq	%rbx, %rdi
	call	Bot1FCTFCarryingFlag
	testl	%eax, %eax
	jne	.L967
	.p2align 4,,4
	jmp	.L968
.L997:
	movq	%rbx, %rdi
	.p2align 4,,7
	.p2align 3
	call	BotHarvesterCarryingCubes
	testl	%eax, %eax
	movl	$80, %eax
	cmovne	%eax, %r12d
	jmp	.L968
.L987:
	call	rand
	andl	$32767, %eax
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	cvtss2sd	%xmm0, %xmm1
	cvtss2sd	4912(%rbx), %xmm0
	mulsd	.LC76(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	ja	.L980
	movss	6592(%rbx), %xmm1
	movss	.LC45(%rip), %xmm0
	jmp	.L977
.L1000:
	movss	144(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	128(%rsp), %rdi
	movss	%xmm0, 128(%rsp)
	movss	148(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 132(%rsp)
	movss	152(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 136(%rsp)
	call	vectoangles
	movss	6592(%rbx), %xmm1
	movss	.LC45(%rip), %xmm0
	jmp	.L977
.L980:
	movq	%r13, %rsi
	movq	%rbx, %rdi
	call	BotRoamGoal
	movss	144(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	128(%rsp), %rdi
	movss	%xmm0, 128(%rsp)
	movss	148(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 132(%rsp)
	movss	152(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 136(%rsp)
	call	vectoangles
	movss	6592(%rbx), %xmm1
	movss	.LC45(%rip), %xmm0
	mulss	%xmm0, %xmm1
	movss	%xmm1, 6592(%rbx)
	jmp	.L977
	.cfi_endproc
.LFE94:
	.size	AINode_Seek_LTG, .-AINode_Seek_LTG
	.section	.rodata.str1.1
.LC143:
	.string	"active entity: observer"
.LC144:
	.string	"activate entity: intermission"
.LC145:
	.string	"activate entity: bot dead"
.LC146:
	.string	"activate entity: no goal"
.LC147:
	.string	"activate entity: time out"
.LC148:
	.string	"activate entity: activated"
.LC149:
	.string	"activate entity: found enemy"
	.text
	.p2align 4,,15
.globl AINode_Seek_ActivateEntity
	.type	AINode_Seek_ActivateEntity, @function
AINode_Seek_ActivateEntity:
.LFB90:
	.cfi_startproc
	movq	%rbx, -32(%rsp)
	movq	%rbp, -24(%rsp)
	movq	%rdi, %rbx
	.cfi_offset 6, -32
	.cfi_offset 3, -40
	movq	%r12, -16(%rsp)
	movq	%r13, -8(%rsp)
	subq	$408, %rsp
	.cfi_def_cfa_offset 416
	movq	%fs:40, %rax
	movq	%rax, 360(%rsp)
	xorl	%eax, %eax
	.cfi_offset 13, -16
	.cfi_offset 12, -24
	call	BotIsObserver
	testl	%eax, %eax
	jne	.L1053
	movq	%rbx, %rdi
	call	BotIntermission
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L1054
	movq	%rbx, %rdi
	call	BotIsDead
	testl	%eax, %eax
	.p2align 4,,2
	jne	.L1055
	movl	$18616254, 5984(%rbx)
	movl	bot_grapple+12(%rip), %eax
	testl	%eax, %eax
	je	.L1006
	movl	$18632638, 5984(%rbx)
.L1006:
	movq	%rbx, %rdi
	call	BotInLavaOrSlime
	testl	%eax, %eax
	jne	.L1056
.L1007:
	movq	%rbx, %rdi
	call	BotMapScripts
	movq	7128(%rbx), %rbp
	movl	$-1, 6548(%rbx)
	testq	%rbp, %rbp
	je	.L1057
	movl	72(%rbp), %r13d
	leaq	4(%rbp), %r12
	testl	%r13d, %r13d
	jne	.L1058
.L1009:
	leaq	4916(%rbx), %rdi
	movq	%r12, %rsi
	call	trap_BotTouchingGoal
	testl	%eax, %eax
	je	.L1059
	movq	7128(%rbx), %rax
	movl	$0x00000000, 60(%rax)
	jmp	.L1013
	.p2align 4,,10
	.p2align 3
.L1054:
	movq	%rbx, %rdi
	call	BotClearActivateGoalStack
	movl	$.LC144, %esi
	movq	%rbx, %rdi
	call	AIEnter_Intermission
	xorl	%eax, %eax
.L1003:
	movq	360(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L1060
	movq	376(%rsp), %rbx
	movq	384(%rsp), %rbp
	movq	392(%rsp), %r12
	movq	400(%rsp), %r13
	addq	$408, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L1053:
	movq	%rbx, %rdi
	call	BotClearActivateGoalStack
	movl	$.LC143, %esi
	movq	%rbx, %rdi
	call	AIEnter_Observer
	xorl	%eax, %eax
	jmp	.L1003
	.p2align 4,,10
	.p2align 3
.L1056:
	orl	$6291456, 5984(%rbx)
	jmp	.L1007
	.p2align 4,,10
	.p2align 3
.L1058:
	movl	12(%rbx), %r9d
	leaq	4944(%rbx), %rsi
	leaq	272(%rsp), %rdi
	leaq	80(%rbp), %r8
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	movl	$100663297, (%rsp)
	call	BotAI_Trace
	movss	280(%rsp), %xmm0
	ucomiss	.LC7(%rip), %xmm0
	jae	.L1010
	movl	352(%rsp), %eax
	cmpl	44(%rbp), %eax
	je	.L1010
	movq	7128(%rbx), %rax
	cmpl	$0, 72(%rax)
	je	.L1009
	.p2align 4,,10
	.p2align 3
.L1013:
	movss	floattime(%rip), %xmm0
	ucomiss	60(%rax), %xmm0
	jbe	.L1047
	movq	%rbx, %rdi
	call	BotPopFromActivateGoalStack
	movq	7128(%rbx), %rax
	testq	%rax, %rax
	je	.L1024
.L1052:
	movss	.LC53(%rip), %xmm0
	addss	floattime(%rip), %xmm0
	movss	%xmm0, 60(%rax)
	xorl	%eax, %eax
	jmp	.L1003
	.p2align 4,,10
	.p2align 3
.L1010:
	movq	7128(%rbx), %rax
	movl	160(%rbx), %edx
	cmpl	76(%rax), %edx
	je	.L1061
.L1012:
	movl	44(%rbp), %edi
	leaq	16(%rsp), %rsi
	call	BotEntityInfo
	movq	7128(%rbx), %rax
	movss	92(%rax), %xmm0
	ucomiss	40(%rsp), %xmm0
	jne	.L1014
	jp	.L1014
	leaq	92(%rax), %rdx
	movss	4(%rdx), %xmm0
	ucomiss	44(%rsp), %xmm0
	jne	.L1014
	.p2align 4,,4
	jp	.L1014
	movss	8(%rdx), %xmm0
	ucomiss	48(%rsp), %xmm0
	jp	.L1014
	.p2align 4,,6
	je	.L1015
	.p2align 4,,10
	.p2align 3
.L1014:
	movl	$0x00000000, 60(%rax)
.L1015:
	movss	floattime(%rip), %xmm0
	ucomiss	60(%rax), %xmm0
	jbe	.L1046
	movq	%rbx, %rdi
	call	BotPopFromActivateGoalStack
	movq	7128(%rbx), %rax
	testq	%rax, %rax
	jne	.L1052
	movl	$.LC147, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_NBG
	xorl	%eax, %eax
	jmp	.L1003
	.p2align 4,,10
	.p2align 3
.L1055:
	movq	%rbx, %rdi
	call	BotClearActivateGoalStack
	movl	$.LC145, %esi
	movq	%rbx, %rdi
	call	AIEnter_Respawn
	xorl	%eax, %eax
	jmp	.L1003
	.p2align 4,,10
	.p2align 3
.L1059:
	movq	7128(%rbx), %rax
	jmp	.L1013
.L1061:
	movss	80(%rax), %xmm0
	leaq	224(%rsp), %r13
	subss	4944(%rbx), %xmm0
	leaq	240(%rsp), %rdi
	movq	%r13, %rsi
	movss	%xmm0, 240(%rsp)
	movss	84(%rax), %xmm0
	subss	4948(%rbx), %xmm0
	movss	%xmm0, 244(%rsp)
	movss	88(%rax), %xmm0
	subss	4952(%rbx), %xmm0
	movss	%xmm0, 248(%rsp)
	call	vectoangles
	leaq	6572(%rbx), %rdi
	movq	%r13, %rsi
	movss	.LC9(%rip), %xmm0
	call	InFieldOfVision
	testl	%eax, %eax
	je	.L1012
	movl	8(%rbx), %edi
	call	trap_EA_Attack
	.p2align 4,,4
	jmp	.L1012
	.p2align 4,,10
	.p2align 3
.L1047:
	movq	%r12, %rsi
	movq	%rbx, %rdi
	call	BotAIPredictObstacles
	movl	%eax, %edx
	xorl	%eax, %eax
	testl	%edx, %edx
	jne	.L1003
	movq	%rbx, %rdi
	leaq	160(%rsp), %rbp
	call	BotSetupForMovement
	movl	5984(%rbx), %ecx
	movl	6532(%rbx), %esi
	movq	%r12, %rdx
	movq	%rbp, %rdi
	call	trap_BotMoveToGoal
	movl	160(%rsp), %r11d
	testl	%r11d, %r11d
	jne	.L1062
.L1026:
	movl	$1, %edx
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	BotAIBlocked
.L1020:
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	BotClearPath
	movq	7128(%rbx), %rax
	movl	72(%rax), %r10d
	testl	%r10d, %r10d
	jne	.L1027
	movl	180(%rsp), %r13d
.L1028:
	testb	$11, %r13b
	je	.L1031
	movl	200(%rsp), %eax
	movl	%eax, 6584(%rbx)
	movl	204(%rsp), %eax
	movl	%eax, 6588(%rbx)
	movl	208(%rsp), %eax
	movl	%eax, 6592(%rbx)
.L1032:
	andl	$16, %r13d
	je	.L1037
	movl	184(%rsp), %eax
	movl	%eax, 6568(%rbx)
.L1037:
	movl	$-1, %esi
	movq	%rbx, %rdi
	call	BotFindEnemy
	movl	%eax, %edx
	movl	$1, %eax
	testl	%edx, %edx
	je	.L1003
	movq	%rbx, %rdi
	call	BotWantsToRetreat
	testl	%eax, %eax
	je	.L1039
	movl	$.LC149, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_NBG
.L1040:
	movq	%rbx, %rdi
	call	BotClearActivateGoalStack
	movl	$1, %eax
	jmp	.L1003
	.p2align 4,,10
	.p2align 3
.L1046:
	leaq	160(%rsp), %rbp
	movl	$6, %ecx
	xorl	%eax, %eax
	movq	%rbp, %rdi
	rep stosq
	movl	$0, (%rdi)
	jmp	.L1020
	.p2align 4,,10
	.p2align 3
.L1031:
	testb	$4, %r13b
	jne	.L1063
	testb	$32, 5988(%rbx)
	jne	.L1032
	movl	5984(%rbx), %edx
	movl	6532(%rbx), %edi
	leaq	256(%rsp), %rcx
	movss	.LC11(%rip), %xmm0
	movq	%r12, %rsi
	call	trap_BotMovementViewTarget
	testl	%eax, %eax
	je	.L1035
	movss	256(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	240(%rsp), %rdi
	movss	%xmm0, 240(%rsp)
	movss	260(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 244(%rsp)
	movss	264(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 248(%rsp)
	call	vectoangles
.L1036:
	movss	.LC45(%rip), %xmm0
	mulss	6592(%rbx), %xmm0
	movss	%xmm0, 6592(%rbx)
.L1051:
	movl	180(%rsp), %r13d
	jmp	.L1032
	.p2align 4,,10
	.p2align 3
.L1027:
	movl	180(%rsp), %r13d
	testb	$1, %r13b
	je	.L1064
.L1029:
	testb	$16, %r13b
	jne	.L1028
	orl	$16, %r13d
	movq	%rbx, %rdi
	movl	%r13d, 180(%rsp)
	call	BotSelectActivateWeapon
	movq	7128(%rbx), %rdx
	cmpl	$-1, %eax
	movl	%eax, 76(%rdx)
	je	.L1065
.L1030:
	movl	%eax, 184(%rsp)
	jmp	.L1028
.L1039:
	movl	6532(%rbx), %edi
	call	trap_BotResetLastAvoidReach
	movl	6536(%rbx), %edi
	call	trap_BotEmptyGoalStack
	movl	$.LC149, %esi
	movq	%rbx, %rdi
	call	AIEnter_Battle_Fight
	jmp	.L1040
.L1057:
	movq	%rbx, %rdi
	call	BotClearActivateGoalStack
	movl	$.LC146, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_NBG
	xorl	%eax, %eax
	jmp	.L1003
.L1063:
	call	rand
	andl	$32767, %eax
	cvtsi2ss	%eax, %xmm0
	divss	.LC30(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	4912(%rbx), %xmm0
	cvtps2pd	%xmm0, %xmm0
	mulsd	.LC76(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jbe	.L1051
	leaq	256(%rsp), %rsi
	movq	%rbx, %rdi
	call	BotRoamGoal
	movss	256(%rsp), %xmm0
	leaq	6584(%rbx), %rsi
	subss	4916(%rbx), %xmm0
	leaq	240(%rsp), %rdi
	movss	%xmm0, 240(%rsp)
	movss	260(%rsp), %xmm0
	subss	4920(%rbx), %xmm0
	movss	%xmm0, 244(%rsp)
	movss	264(%rsp), %xmm0
	subss	4924(%rbx), %xmm0
	movss	%xmm0, 248(%rsp)
	call	vectoangles
	movss	.LC45(%rip), %xmm0
	mulss	6592(%rbx), %xmm0
	movl	180(%rsp), %r13d
	movss	%xmm0, 6592(%rbx)
	jmp	.L1032
.L1064:
	movss	80(%rax), %xmm0
	leaq	40(%rbp), %rsi
	subss	4944(%rbx), %xmm0
	leaq	240(%rsp), %rdi
	movss	%xmm0, 240(%rsp)
	movss	84(%rax), %xmm0
	subss	4948(%rbx), %xmm0
	movss	%xmm0, 244(%rsp)
	movss	88(%rax), %xmm0
	subss	4952(%rbx), %xmm0
	movss	%xmm0, 248(%rsp)
	call	vectoangles
	movl	180(%rsp), %r13d
	orl	$1, %r13d
	movl	%r13d, 180(%rsp)
	jmp	.L1029
.L1062:
	movl	6532(%rbx), %edi
	call	trap_BotResetAvoidReach
	movq	7128(%rbx), %rax
	movl	$0x00000000, 60(%rax)
	jmp	.L1026
.L1024:
	movl	$.LC148, %esi
	movq	%rbx, %rdi
	call	AIEnter_Seek_NBG
	xorl	%eax, %eax
	jmp	.L1003
.L1035:
	leaq	6584(%rbx), %rsi
	leaq	28(%rbp), %rdi
	call	vectoangles
	jmp	.L1036
.L1065:
	movl	$0, 76(%rdx)
	xorl	%eax, %eax
	jmp	.L1030
.L1060:
	.p2align 4,,7
	.p2align 3
	call	__stack_chk_fail
	.cfi_endproc
.LFE90:
	.size	AINode_Seek_ActivateEntity, .-AINode_Seek_ActivateEntity
	.comm	numnodeswitches,4,4
	.comm	nodeswitch,7344,32
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC2:
	.long	1097859072
	.align 4
.LC3:
	.long	1073741824
	.align 4
.LC4:
	.long	1148846080
	.align 4
.LC5:
	.long	0
	.align 4
.LC6:
	.long	3212836864
	.align 4
.LC7:
	.long	1065353216
	.align 4
.LC8:
	.long	1090519040
	.align 4
.LC9:
	.long	1101004800
	.align 4
.LC10:
	.long	1084227584
	.align 4
.LC11:
	.long	1133903872
	.align 4
.LC12:
	.long	1086324736
	.align 4
.LC13:
	.long	1112014848
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC23:
	.long	0
	.long	1071644672
	.align 8
.LC24:
	.long	2576980378
	.long	1069128089
	.section	.rodata.cst4
	.align 4
.LC30:
	.long	1191181824
	.align 4
.LC41:
	.long	1135869952
	.align 4
.LC42:
	.long	1098907648
	.section	.rodata.cst8
	.align 8
.LC44:
	.long	858993459
	.long	1070805811
	.section	.rodata.cst4
	.align 4
.LC45:
	.long	1056964608
	.align 4
.LC53:
	.long	1092616192
	.align 4
.LC55:
	.long	1125515264
	.section	.rodata.cst8
	.align 8
.LC56:
	.long	0
	.long	1076756480
	.align 8
.LC57:
	.long	0
	.long	1072693248
	.align 8
.LC65:
	.long	2576980378
	.long	1070176665
	.align 8
.LC74:
	.long	2576980378
	.long	1068079513
	.align 8
.LC76:
	.long	2576980378
	.long	1072273817
	.section	.rodata.cst4
	.align 4
.LC82:
	.long	1176256512
	.align 4
.LC85:
	.long	1082130432
	.section	.rodata.cst8
	.align 8
.LC86:
	.long	1717986918
	.long	1072064102
	.section	.rodata.cst4
	.align 4
.LC87:
	.long	1137180672
	.align 4
.LC90:
	.long	1114636288
	.align 4
.LC93:
	.long	1167663104
	.align 4
.LC94:
	.long	1077936128
	.align 4
.LC95:
	.long	1120403456
	.align 4
.LC108:
	.long	1163984896
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC122:
	.long	2147483648
	.long	0
	.long	0
	.long	0
	.section	.rodata.cst4
	.align 4
.LC124:
	.long	1215570944
	.align 4
.LC134:
	.long	1117782016
	.section	.rodata.cst8
	.align 8
.LC141:
	.long	1202590843
	.long	1065646817
	.ident	"GCC: (Ubuntu 4.4.1-4ubuntu8) 4.4.1"
	.section	.note.GNU-stack,"",@progbits
