	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB0:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB0:
	.align 1
	.align 4
	.globl __ZNKSt5ctypeIcE8do_widenEc
	.weak_definition __ZNKSt5ctypeIcE8do_widenEc
__ZNKSt5ctypeIcE8do_widenEc:
LFB1067:
	movl	%esi, %eax
	ret
LFE1067:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE0:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE0:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB1:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB1:
	.align 1
	.align 4
	.globl __ZN3pcf11point_cloudINS_12point_xyzrgbEED1Ev
	.weak_definition __ZN3pcf11point_cloudINS_12point_xyzrgbEED1Ev
__ZN3pcf11point_cloudINS_12point_xyzrgbEED1Ev:
LFB9837:
	ret
LFE9837:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE1:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE1:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB2:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB2:
	.align 1
	.align 4
	.globl __ZN3pcf11point_cloudINS_12point_xyzrgbEED0Ev
	.weak_definition __ZN3pcf11point_cloudINS_12point_xyzrgbEED0Ev
__ZN3pcf11point_cloudINS_12point_xyzrgbEED0Ev:
LFB9838:
	jmp	__ZdlPv
LFE9838:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE2:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE2:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB3:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB3:
	.align 1
	.align 4
	.globl __ZN3pcf17point_cloud_allocINS_12point_xyzrgbEED0Ev
	.weak_definition __ZN3pcf17point_cloud_allocINS_12point_xyzrgbEED0Ev
__ZN3pcf17point_cloud_allocINS_12point_xyzrgbEED0Ev:
LFB9703:
	movq	__ZTVN3pcf17point_cloud_allocINS_12point_xyzrgbEEE@GOTPCREL(%rip), %rax
	pushq	%rbx
LCFI0:
	movq	%rdi, %rbx
	addq	$16, %rax
	movq	%rax, (%rdi)
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	L5
	call	_free
L5:
	movq	%rbx, %rdi
	popq	%rbx
LCFI1:
	jmp	__ZdlPv
LFE9703:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE3:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE3:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB4:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB4:
	.align 1
	.align 4
	.globl __ZN3pcf17point_cloud_allocINS_12point_xyzrgbEED1Ev
	.weak_definition __ZN3pcf17point_cloud_allocINS_12point_xyzrgbEED1Ev
__ZN3pcf17point_cloud_allocINS_12point_xyzrgbEED1Ev:
LFB9702:
	movq	__ZTVN3pcf17point_cloud_allocINS_12point_xyzrgbEEE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, (%rdi)
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	L11
	jmp	_free
	.align 4
L11:
	ret
LFE9702:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE4:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE4:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB5:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB5:
	.align 1
	.align 4
	.globl __ZN3pcf16ply_reader_errorD0Ev
	.weak_definition __ZN3pcf16ply_reader_errorD0Ev
__ZN3pcf16ply_reader_errorD0Ev:
LFB9712:
	movq	__ZTVN3pcf16ply_reader_errorE@GOTPCREL(%rip), %rax
	pushq	%rbx
LCFI2:
	movq	%rdi, %rbx
	addq	$16, %rax
	movq	%rax, (%rdi)
	call	__ZNSt13runtime_errorD2Ev
	movq	%rbx, %rdi
	popq	%rbx
LCFI3:
	jmp	__ZdlPv
LFE9712:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE5:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE5:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB6:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB6:
	.align 1
	.align 4
	.globl __ZN3pcf16ply_reader_errorD1Ev
	.weak_definition __ZN3pcf16ply_reader_errorD1Ev
__ZN3pcf16ply_reader_errorD1Ev:
LFB9711:
	movq	__ZTVN3pcf16ply_reader_errorE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, (%rdi)
	jmp	__ZNSt13runtime_errorD2Ev
LFE9711:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE6:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE6:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB7:
	.text
LHOTB7:
	.align 4,0x90
__ZN3pcf11point_cloudINS_12point_xyzrgbEE20apply_transformationIN5Eigen9TransformIfLi3ELi18ELi0EEEEEvRKT_._omp_fn.0:
LFB11134:
	pushq	%r13
LCFI4:
	pushq	%r12
LCFI5:
	pushq	%rbp
LCFI6:
	pushq	%rbx
LCFI7:
	subq	$8, %rsp
LCFI8:
	movq	(%rdi), %rax
	movq	8(%rax), %r12
	movq	16(%rax), %r13
	cmpq	%r13, %r12
	jae	L16
	movq	%rdi, %rbx
	call	_omp_get_num_threads
	movslq	%eax, %rbp
	call	_omp_get_thread_num
	movslq	%eax, %rcx
	leaq	15(%r13), %rax
	subq	%r12, %rax
	leaq	15(%rax), %rsi
	testq	%rax, %rax
	cmovs	%rsi, %rax
	sarq	$4, %rax
	cqto
	idivq	%rbp
	cmpq	%rdx, %rcx
	jge	L21
	addq	$1, %rax
	xorl	%edx, %edx
L21:
	imulq	%rax, %rcx
	addq	%rcx, %rdx
	addq	%rdx, %rax
	cmpq	%rax, %rdx
	jge	L16
	salq	$4, %rax
	salq	$4, %rdx
	leaq	(%r12,%rax), %rcx
	movq	8(%rbx), %rax
	addq	%r12, %rdx
	.align 4,0x90
L20:
	movss	(%rdx), %xmm4
	addq	$16, %rdx
	movss	-12(%rdx), %xmm0
	movss	(%rax), %xmm2
	movss	12(%rax), %xmm1
	mulss	%xmm4, %xmm2
	movss	4(%rax), %xmm5
	mulss	%xmm0, %xmm1
	movss	-8(%rdx), %xmm3
	mulss	%xmm4, %xmm5
	mulss	8(%rax), %xmm4
	addss	%xmm1, %xmm2
	movss	16(%rax), %xmm1
	mulss	%xmm0, %xmm1
	mulss	20(%rax), %xmm0
	addss	%xmm5, %xmm1
	movss	28(%rax), %xmm5
	addss	%xmm4, %xmm0
	movss	32(%rax), %xmm4
	mulss	%xmm3, %xmm5
	mulss	%xmm3, %xmm4
	mulss	24(%rax), %xmm3
	addss	%xmm5, %xmm1
	addss	%xmm4, %xmm0
	addss	40(%rax), %xmm1
	addss	44(%rax), %xmm0
	addss	%xmm3, %xmm2
	addss	36(%rax), %xmm2
	movss	%xmm1, -12(%rdx)
	movss	%xmm0, -8(%rdx)
	movss	%xmm2, -16(%rdx)
	cmpq	%rdx, %rcx
	ja	L20
L16:
	addq	$8, %rsp
LCFI9:
	popq	%rbx
LCFI10:
	popq	%rbp
LCFI11:
	popq	%r12
LCFI12:
	popq	%r13
LCFI13:
	ret
LFE11134:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE7:
	.text
LHOTE7:
	.section __TEXT,__text_cold,regular,pure_instructions
	.align 1
LCOLDB8:
LHOTB8:
	.align 1
__ZNSs4_Rep10_M_disposeERKSaIcE.part.3:
LFB11137:
	orl	$-1, %eax
	lock; xaddl	%eax, 16(%rdi)
	testl	%eax, %eax
	jg	L24
	jmp	__ZNSs4_Rep10_M_destroyERKSaIcE
L24:
	ret
LFE11137:
LCOLDE8:
LHOTE8:
	.cstring
	.align 3
LC9:
	.ascii "Point cloud memory not properly aligned.\0"
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB10:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB10:
	.align 1
	.align 4
	.globl __ZN3pcf17point_cloud_allocINS_12point_xyzrgbEEC1Em
	.weak_definition __ZN3pcf17point_cloud_allocINS_12point_xyzrgbEEC1Em
__ZN3pcf17point_cloud_allocINS_12point_xyzrgbEEC1Em:
LFB9699:
	movabsq	$571957152676052992, %rax
	pushq	%rbp
LCFI14:
	pushq	%rbx
LCFI15:
	subq	$40, %rsp
LCFI16:
	cmpq	%rax, %rsi
	ja	L27
	salq	$4, %rsi
	movq	%rdi, %rbp
	movq	%rsi, %rdi
	movq	%rsi, %rbx
	call	_malloc
	testq	%rax, %rax
	jne	L28
	testq	%rbx, %rbx
	jne	L44
L28:
	movq	__ZTVN3pcf11point_cloudINS_12point_xyzrgbEEE@GOTPCREL(%rip), %rdx
	addq	%rax, %rbx
	movq	%rax, 8(%rbp)
	movq	%rbx, 16(%rbp)
	addq	$16, %rdx
	testb	$15, %al
	movq	%rdx, 0(%rbp)
	jne	L45
	movq	__ZTVN3pcf17point_cloud_allocINS_12point_xyzrgbEEE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, 0(%rbp)
	addq	$40, %rsp
LCFI17:
	popq	%rbx
LCFI18:
	popq	%rbp
LCFI19:
	ret
L44:
LCFI20:
	movl	$8, %edi
	call	___cxa_allocate_exception
	movq	__ZTVSt9bad_alloc@GOTPCREL(%rip), %rdx
	movq	__ZTISt9bad_alloc@GOTPCREL(%rip), %rsi
	movq	%rax, %rdi
	addq	$16, %rdx
	movq	%rdx, (%rax)
	movq	__ZNSt9bad_allocD1Ev@GOTPCREL(%rip), %rdx
LEHB0:
	call	___cxa_throw
L27:
	call	___cxa_throw_bad_array_new_length
LEHE0:
L45:
	leaq	16(%rsp), %rbx
	movl	$16, %edi
	call	___cxa_allocate_exception
	leaq	15(%rsp), %rdx
	movq	%rbx, %rdi
	leaq	LC9(%rip), %rsi
	movq	%rax, %rbp
LEHB1:
	call	__ZNSsC1EPKcRKSaIcE
LEHE1:
	movq	%rbx, %rsi
	movq	%rbp, %rdi
LEHB2:
	call	__ZNSt13runtime_errorC1ERKSs
LEHE2:
	movq	16(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L46
L30:
	movq	__ZNSt13runtime_errorD1Ev@GOTPCREL(%rip), %rdx
	movq	%rbp, %rdi
	movq	__ZTISt13runtime_error@GOTPCREL(%rip), %rsi
LEHB3:
	call	___cxa_throw
L34:
	movq	%rax, %rbx
	jmp	L33
L46:
	leaq	14(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.3
	jmp	L30
L35:
	movq	%rax, %rbx
L31:
	movq	16(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L33
	leaq	14(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.3
L33:
	movq	%rbp, %rdi
	call	___cxa_free_exception
	movq	%rbx, %rdi
	call	__Unwind_Resume
LEHE3:
LFE9699:
	.section __DATA,__gcc_except_tab
GCC_except_table0:
LLSDA9699:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x34
	.set L$set$0,LEHB0-LFB9699
	.long L$set$0
	.set L$set$1,LEHE0-LEHB0
	.long L$set$1
	.long	0
	.byte	0
	.set L$set$2,LEHB1-LFB9699
	.long L$set$2
	.set L$set$3,LEHE1-LEHB1
	.long L$set$3
	.set L$set$4,L34-LFB9699
	.long L$set$4
	.byte	0
	.set L$set$5,LEHB2-LFB9699
	.long L$set$5
	.set L$set$6,LEHE2-LEHB2
	.long L$set$6
	.set L$set$7,L35-LFB9699
	.long L$set$7
	.byte	0
	.set L$set$8,LEHB3-LFB9699
	.long L$set$8
	.set L$set$9,LEHE3-LEHB3
	.long L$set$9
	.long	0
	.byte	0
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE10:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE10:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB11:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB11:
	.align 1
	.align 4
	.globl __ZN3pcf10ply_reader11read_ascii_INS_12point_xyzrgbEEEvPT_m
	.weak_definition __ZN3pcf10ply_reader11read_ascii_INS_12point_xyzrgbEEEvPT_m
__ZN3pcf10ply_reader11read_ascii_INS_12point_xyzrgbEEEvPT_m:
LFB9839:
	pushq	%r15
LCFI21:
	pushq	%r14
LCFI22:
	movq	%rsi, %r14
	pushq	%r13
LCFI23:
	pushq	%r12
LCFI24:
	pushq	%rbp
LCFI25:
	movq	%rdx, %rbp
	pushq	%rbx
LCFI26:
	subq	$72, %rsp
LCFI27:
	movq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rax
	movq	%rdi, 16(%rsp)
	addq	$24, %rax
	movq	%rax, 48(%rsp)
	movq	1072(%rdi), %rax
	testq	%rax, %rax
	jne	L90
	testq	%rdx, %rdx
	je	L53
	movq	$0, 8(%rsp)
L50:
	movq	%rbp, 24(%rsp)
	leaq	48(%rsp), %r15
	.align 4
L62:
	movq	16(%rsp), %rdi
	movq	%r15, %rsi
LEHB4:
	call	__ZN3pcf10ply_reader10read_line_ERSs
	movq	48(%rsp), %rbx
	movl	-8(%rbx), %edx
	testl	%edx, %edx
	js	L72
	movq	%r15, %rdi
	call	__ZNSs12_M_leak_hardEv
	movq	48(%rsp), %rbx
	movl	-8(%rbx), %eax
	testl	%eax, %eax
	js	L72
	movq	%r15, %rdi
	call	__ZNSs12_M_leak_hardEv
	movq	48(%rsp), %r12
L54:
	addq	-24(%r12), %r12
	movl	$1, %ebp
	movq	8(%rsp), %r13
	cmpq	%r12, %rbx
	jne	L82
	jmp	L61
	.align 4
L91:
	movq	__DefaultRuneLocale@GOTPCREL(%rip), %rax
	movslq	%edi, %rdi
	movl	60(%rax,%rdi,4), %eax
	shrl	$14, %eax
	andl	$1, %eax
L58:
	andl	$1, %eax
	cmpb	%al, %bpl
	je	L59
	testb	%bpl, %bpl
	je	L59
	movq	%rbx, 0(%r13)
	addq	$8, %r13
L59:
	addq	$1, %rbx
	cmpq	%rbx, %r12
	je	L61
	movl	%eax, %ebp
L82:
	movsbl	(%rbx), %edi
	testb	%dil, %dil
	jns	L91
	movl	$16384, %esi
	call	___maskrune
	testl	%eax, %eax
	setne	%al
	movzbl	%al, %eax
	jmp	L58
	.align 4
L61:
	movq	8(%rsp), %rdx
	movq	%r14, %rsi
	movq	16(%rsp), %rdi
	call	__ZNK3pcf10ply_reader17read_ascii_point_ERNS_12point_xyzrgbEPPKc
LEHE4:
	addq	$16, %r14
	subq	$1, 24(%rsp)
	jne	L62
	cmpq	$0, 8(%rsp)
	je	L53
L52:
	movq	8(%rsp), %rdi
	call	__ZdlPv
L53:
	movq	48(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L92
L47:
	addq	$72, %rsp
LCFI28:
	popq	%rbx
LCFI29:
	popq	%rbp
LCFI30:
	popq	%r12
LCFI31:
	popq	%r13
LCFI32:
	popq	%r14
LCFI33:
	popq	%r15
LCFI34:
	ret
	.align 4
L72:
LCFI35:
	movq	%rbx, %r12
	jmp	L54
L90:
	movabsq	$2305843009213693951, %rdx
	cmpq	%rdx, %rax
	ja	L93
	leaq	0(,%rax,8), %rbx
	movq	%rbx, %rdi
LEHB5:
	call	__Znwm
	xorl	%esi, %esi
	movq	%rbx, %rdx
	movq	%rax, %rdi
	movq	%rax, 8(%rsp)
	call	_memset
	testq	%rbp, %rbp
	jne	L50
	jmp	L52
	.align 4
L92:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L47
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	addq	$72, %rsp
LCFI36:
	popq	%rbx
LCFI37:
	popq	%rbp
LCFI38:
	popq	%r12
LCFI39:
	popq	%r13
LCFI40:
	popq	%r14
LCFI41:
	popq	%r15
LCFI42:
	ret
L93:
LCFI43:
	call	__ZSt17__throw_bad_allocv
LEHE5:
L75:
	movq	%rax, %rbx
	jmp	L69
L76:
	movq	%rax, %rbx
L67:
	movq	8(%rsp), %rax
	testq	%rax, %rax
	je	L69
	movq	%rax, %rdi
	call	__ZdlPv
L69:
	movq	48(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L70
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.3
L70:
	movq	%rbx, %rdi
LEHB6:
	call	__Unwind_Resume
LEHE6:
LFE9839:
	.section __DATA,__gcc_except_tab
GCC_except_table1:
LLSDA9839:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x27
	.set L$set$10,LEHB4-LFB9839
	.long L$set$10
	.set L$set$11,LEHE4-LEHB4
	.long L$set$11
	.set L$set$12,L76-LFB9839
	.long L$set$12
	.byte	0
	.set L$set$13,LEHB5-LFB9839
	.long L$set$13
	.set L$set$14,LEHE5-LEHB5
	.long L$set$14
	.set L$set$15,L75-LFB9839
	.long L$set$15
	.byte	0
	.set L$set$16,LEHB6-LFB9839
	.long L$set$16
	.set L$set$17,LEHE6-LEHB6
	.long L$set$17
	.long	0
	.byte	0
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE11:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE11:
	.cstring
	.align 3
LC12:
	.ascii "Attempted to read beyond bounds.\0"
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB13:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB13:
	.align 1
	.align 4
	.globl __ZN3pcf10ply_reader4readINS_12point_xyzrgbEEEvPT_m
	.weak_definition __ZN3pcf10ply_reader4readINS_12point_xyzrgbEEEvPT_m
__ZN3pcf10ply_reader4readINS_12point_xyzrgbEEEvPT_m:
LFB9705:
	pushq	%r15
LCFI44:
	movq	%rdx, %rax
	movq	%rdx, %r15
	pushq	%r14
LCFI45:
	pushq	%r13
LCFI46:
	pushq	%r12
LCFI47:
	movq	%rdi, %r12
	pushq	%rbp
LCFI48:
	pushq	%rbx
LCFI49:
	subq	$40, %rsp
LCFI50:
	addq	1232(%rdi), %rax
	cmpq	1048(%rdi), %rax
	ja	L113
	cmpl	$2, 1056(%rdi)
	movq	%rsi, %rbp
	je	L114
	movq	%rdx, %r13
	imulq	1064(%rdi), %r13
	movq	%r13, %rdi
LEHB7:
	call	__Znam
LEHE7:
	leaq	8(%r12), %rdi
	movq	%r13, %rdx
	movq	%rax, %rsi
	movq	%rax, %r14
LEHB8:
	call	__ZNSi4readEPcl
	addq	%r14, %r13
	movq	%r14, %rbx
	cmpq	%r13, %r14
	je	L101
	.align 4
L109:
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	__ZNK3pcf10ply_reader18read_binary_point_ERNS_12point_xyzrgbEPc
LEHE8:
	addq	1064(%r12), %rbx
	addq	$16, %rbp
	cmpq	%rbx, %r13
	jne	L109
L101:
	movq	%r14, %rdi
	call	__ZdaPv
L98:
	addq	%r15, 1232(%r12)
	addq	$40, %rsp
LCFI51:
	popq	%rbx
LCFI52:
	popq	%rbp
LCFI53:
	popq	%r12
LCFI54:
	popq	%r13
LCFI55:
	popq	%r14
LCFI56:
	popq	%r15
LCFI57:
	ret
	.align 4
L114:
LCFI58:
LEHB9:
	call	__ZN3pcf10ply_reader11read_ascii_INS_12point_xyzrgbEEEvPT_m
LEHE9:
	jmp	L98
L113:
	leaq	16(%rsp), %rbx
	movl	$16, %edi
	call	___cxa_allocate_exception
	leaq	14(%rsp), %rdx
	movq	%rbx, %rdi
	leaq	LC12(%rip), %rsi
	movq	%rax, %rbp
LEHB10:
	call	__ZNSsC1EPKcRKSaIcE
LEHE10:
	movq	%rbx, %rsi
	movq	%rbp, %rdi
LEHB11:
	call	__ZNSt13runtime_errorC2ERKSs
LEHE11:
	movq	__ZTVN3pcf16ply_reader_errorE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, 0(%rbp)
	movq	16(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L96
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.3
L96:
	movq	__ZN3pcf16ply_reader_errorD1Ev@GOTPCREL(%rip), %rdx
	movq	%rbp, %rdi
	movq	__ZTIN3pcf16ply_reader_errorE@GOTPCREL(%rip), %rsi
LEHB12:
	call	___cxa_throw
L107:
	movq	%rax, %rbx
	jmp	L103
L106:
	movq	%rax, %rbx
	jmp	L105
L108:
	movq	%rax, %rbx
	jmp	L102
L103:
	movq	16(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L105
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.3
L105:
	movq	%rbp, %rdi
	call	___cxa_free_exception
	movq	%rbx, %rdi
	call	__Unwind_Resume
L102:
	movq	%r14, %rdi
	call	__ZdaPv
	movq	%rbx, %rdi
	call	__Unwind_Resume
LEHE12:
LFE9705:
	.section __DATA,__gcc_except_tab
GCC_except_table2:
LLSDA9705:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x4e
	.set L$set$18,LEHB7-LFB9705
	.long L$set$18
	.set L$set$19,LEHE7-LEHB7
	.long L$set$19
	.long	0
	.byte	0
	.set L$set$20,LEHB8-LFB9705
	.long L$set$20
	.set L$set$21,LEHE8-LEHB8
	.long L$set$21
	.set L$set$22,L108-LFB9705
	.long L$set$22
	.byte	0
	.set L$set$23,LEHB9-LFB9705
	.long L$set$23
	.set L$set$24,LEHE9-LEHB9
	.long L$set$24
	.long	0
	.byte	0
	.set L$set$25,LEHB10-LFB9705
	.long L$set$25
	.set L$set$26,LEHE10-LEHB10
	.long L$set$26
	.set L$set$27,L106-LFB9705
	.long L$set$27
	.byte	0
	.set L$set$28,LEHB11-LFB9705
	.long L$set$28
	.set L$set$29,LEHE11-LEHB11
	.long L$set$29
	.set L$set$30,L107-LFB9705
	.long L$set$30
	.byte	0
	.set L$set$31,LEHB12-LFB9705
	.long L$set$31
	.set L$set$32,LEHE12-LEHB12
	.long L$set$32
	.long	0
	.byte	0
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE13:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE13:
	.cstring
LC14:
	.ascii "Size: \0"
LC15:
	.ascii "Imported point cloud.\0"
LC20:
	.ascii "Transformation applied.\0"
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB21:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTB21:
	.align 4
	.globl _main
_main:
LFB9587:
	pushq	%r15
LCFI59:
	movl	$10, %edx
	pushq	%r14
LCFI60:
	pushq	%r13
LCFI61:
	pushq	%r12
LCFI62:
	pushq	%rbp
LCFI63:
	pushq	%rbx
LCFI64:
	subq	$1352, %rsp
LCFI65:
	movq	8(%rsi), %rsi
	leaq	96(%rsp), %r15
	movq	%r15, %rdi
LEHB13:
	call	__ZN3pcf10ply_readerC1EPKcc
LEHE13:
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$6, %edx
	leaq	LC14(%rip), %rsi
	movq	1144(%rsp), %rbx
LEHB14:
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	movq	%rbx, %rsi
	call	__ZNSo9_M_insertImEERSoT_
	movq	%rax, %rdi
	call	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
	leaq	16(%rsp), %rbp
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	__ZN3pcf17point_cloud_allocINS_12point_xyzrgbEEC1Em
LEHE14:
	movq	24(%rsp), %rsi
	movq	%rbx, %rdx
	movq	%r15, %rdi
LEHB15:
	call	__ZN3pcf10ply_reader4readINS_12point_xyzrgbEEEvPT_m
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$21, %edx
	leaq	LC15(%rip), %rsi
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	call	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
	leaq	48(%rsp), %r13
	movq	%rsp, %r14
	movq	__ZSt4cout@GOTPCREL(%rip), %r12
	jmp	L119
	.align 4
L131:
	movsbl	67(%rbx), %esi
L118:
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	call	__ZNSo3putEc
	movq	%rax, %rdi
	call	__ZNSo5flushEv
L119:
	movss	60(%rsp), %xmm14
	pxor	%xmm6, %xmm6
	xorl	%edx, %edx
	movq	%r14, %rsi
	movss	64(%rsp), %xmm13
	pxor	%xmm11, %xmm11
	xorl	%ecx, %ecx
	pxor	%xmm10, %xmm10
	movss	68(%rsp), %xmm0
	mulss	%xmm14, %xmm11
	pxor	%xmm9, %xmm9
	movq	%rbp, (%rsp)
	mulss	%xmm13, %xmm10
	movss	48(%rsp), %xmm5
	movq	%r13, 8(%rsp)
	mulss	%xmm0, %xmm9
	movss	52(%rsp), %xmm4
	movss	56(%rsp), %xmm12
	leaq	__ZN3pcf11point_cloudINS_12point_xyzrgbEE20apply_transformationIN5Eigen9TransformIfLi3ELi18ELi0EEEEEvRKT_._omp_fn.0(%rip), %rdi
	addss	%xmm5, %xmm11
	mulss	%xmm6, %xmm5
	movss	72(%rsp), %xmm3
	addss	%xmm4, %xmm10
	mulss	%xmm6, %xmm4
	movss	76(%rsp), %xmm2
	addss	%xmm12, %xmm9
	mulss	%xmm6, %xmm12
	movss	LC17(%rip), %xmm6
	movss	80(%rsp), %xmm1
	movss	LC17(%rip), %xmm8
	mulss	%xmm0, %xmm6
	mulss	LC18(%rip), %xmm0
	movss	LC17(%rip), %xmm7
	mulss	%xmm14, %xmm8
	mulss	LC18(%rip), %xmm14
	addss	%xmm12, %xmm6
	mulss	%xmm13, %xmm7
	mulss	LC18(%rip), %xmm13
	addss	%xmm12, %xmm0
	pxor	%xmm12, %xmm12
	mulss	%xmm3, %xmm12
	addss	%xmm5, %xmm8
	addss	%xmm14, %xmm5
	addss	%xmm4, %xmm7
	addss	%xmm13, %xmm4
	addss	%xmm12, %xmm11
	movss	%xmm11, 48(%rsp)
	pxor	%xmm11, %xmm11
	mulss	%xmm2, %xmm11
	addss	%xmm11, %xmm10
	movss	%xmm10, 52(%rsp)
	pxor	%xmm10, %xmm10
	mulss	%xmm1, %xmm10
	addss	%xmm10, %xmm9
	movss	%xmm9, 56(%rsp)
	movss	LC19(%rip), %xmm9
	mulss	%xmm3, %xmm9
	mulss	LC17(%rip), %xmm3
	addss	%xmm9, %xmm8
	addss	%xmm5, %xmm3
	movss	%xmm8, 60(%rsp)
	movss	LC19(%rip), %xmm8
	movss	%xmm3, 72(%rsp)
	mulss	%xmm2, %xmm8
	mulss	LC17(%rip), %xmm2
	addss	%xmm8, %xmm7
	addss	%xmm4, %xmm2
	movss	%xmm7, 64(%rsp)
	movss	LC19(%rip), %xmm7
	movss	%xmm2, 76(%rsp)
	mulss	%xmm1, %xmm7
	mulss	LC17(%rip), %xmm1
	addss	%xmm7, %xmm6
	addss	%xmm1, %xmm0
	movss	%xmm6, 68(%rsp)
	movss	%xmm0, 80(%rsp)
	call	_GOMP_parallel
	movl	$23, %edx
	movq	%r12, %rdi
	leaq	LC20(%rip), %rsi
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	__ZSt4cout@GOTPCREL(%rip), %rax
	movq	(%rax), %rdx
	movq	-24(%rdx), %rdx
	movq	240(%rdx,%rax), %rbx
	testq	%rbx, %rbx
	je	L130
	cmpb	$0, 56(%rbx)
	jne	L131
	movq	%rbx, %rdi
	call	__ZNKSt5ctypeIcE13_M_widen_initEv
	movq	(%rbx), %rax
	movl	$10, %esi
	movq	48(%rax), %rax
	cmpq	__ZNKSt5ctypeIcE8do_widenEc@GOTPCREL(%rip), %rax
	je	L118
	movq	%rbx, %rdi
	call	*%rax
	movsbl	%al, %esi
	jmp	L118
L130:
	call	__ZSt16__throw_bad_castv
LEHE15:
L125:
	movq	%rax, %rbx
	movq	24(%rsp), %rdi
	movq	__ZTVN3pcf17point_cloud_allocINS_12point_xyzrgbEEE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	testq	%rdi, %rdi
	movq	%rax, 16(%rsp)
	je	L122
	call	_free
L122:
	leaq	8(%r15), %rdi
	call	__ZNSt14basic_ifstreamIcSt11char_traitsIcEED1Ev
	movq	%rbx, %rdi
LEHB16:
	call	__Unwind_Resume
LEHE16:
L124:
	movq	%rax, %rbx
	jmp	L122
LFE9587:
	.section __DATA,__gcc_except_tab
GCC_except_table3:
LLSDA9587:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x34
	.set L$set$33,LEHB13-LFB9587
	.long L$set$33
	.set L$set$34,LEHE13-LEHB13
	.long L$set$34
	.long	0
	.byte	0
	.set L$set$35,LEHB14-LFB9587
	.long L$set$35
	.set L$set$36,LEHE14-LEHB14
	.long L$set$36
	.set L$set$37,L124-LFB9587
	.long L$set$37
	.byte	0
	.set L$set$38,LEHB15-LFB9587
	.long L$set$38
	.set L$set$39,LEHE15-LEHB15
	.long L$set$39
	.set L$set$40,L125-LFB9587
	.long L$set$40
	.byte	0
	.set L$set$41,LEHB16-LFB9587
	.long L$set$41
	.set L$set$42,LEHE16-LEHB16
	.long L$set$42
	.long	0
	.byte	0
	.section __TEXT,__text_startup,regular,pure_instructions
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE21:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTE21:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB22:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTB22:
	.align 4
__GLOBAL__sub_I_main.cc:
LFB11133:
	leaq	__ZStL8__ioinit(%rip), %rdi
	subq	$8, %rsp
LCFI66:
	call	__ZNSt8ios_base4InitC1Ev
	movq	__ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	leaq	___dso_handle(%rip), %rdx
	addq	$8, %rsp
LCFI67:
	leaq	__ZStL8__ioinit(%rip), %rsi
	jmp	___cxa_atexit
LFE11133:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE22:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTE22:
	.globl __ZTIN3pcf16ply_reader_errorE
	.weak_definition __ZTIN3pcf16ply_reader_errorE
	.section __DATA,__const_coal,coalesced
	.align 4
__ZTIN3pcf16ply_reader_errorE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSN3pcf16ply_reader_errorE
	.quad	__ZTISt13runtime_error
	.globl __ZTSN3pcf16ply_reader_errorE
	.weak_definition __ZTSN3pcf16ply_reader_errorE
	.section __TEXT,__const_coal,coalesced
	.align 4
__ZTSN3pcf16ply_reader_errorE:
	.ascii "N3pcf16ply_reader_errorE\0"
	.globl __ZTIN3pcf11point_cloudINS_12point_xyzrgbEEE
	.weak_definition __ZTIN3pcf11point_cloudINS_12point_xyzrgbEEE
	.section __DATA,__const_coal,coalesced
	.align 4
__ZTIN3pcf11point_cloudINS_12point_xyzrgbEEE:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN3pcf11point_cloudINS_12point_xyzrgbEEE
	.globl __ZTSN3pcf11point_cloudINS_12point_xyzrgbEEE
	.weak_definition __ZTSN3pcf11point_cloudINS_12point_xyzrgbEEE
	.section __TEXT,__const_coal,coalesced
	.align 5
__ZTSN3pcf11point_cloudINS_12point_xyzrgbEEE:
	.ascii "N3pcf11point_cloudINS_12point_xyzrgbEEE\0"
	.globl __ZTIN3pcf17point_cloud_allocINS_12point_xyzrgbEEE
	.weak_definition __ZTIN3pcf17point_cloud_allocINS_12point_xyzrgbEEE
	.section __DATA,__const_coal,coalesced
	.align 4
__ZTIN3pcf17point_cloud_allocINS_12point_xyzrgbEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSN3pcf17point_cloud_allocINS_12point_xyzrgbEEE
	.quad	__ZTIN3pcf11point_cloudINS_12point_xyzrgbEEE
	.globl __ZTSN3pcf17point_cloud_allocINS_12point_xyzrgbEEE
	.weak_definition __ZTSN3pcf17point_cloud_allocINS_12point_xyzrgbEEE
	.section __TEXT,__const_coal,coalesced
	.align 5
__ZTSN3pcf17point_cloud_allocINS_12point_xyzrgbEEE:
	.ascii "N3pcf17point_cloud_allocINS_12point_xyzrgbEEE\0"
	.globl __ZTVN3pcf16ply_reader_errorE
	.weak_definition __ZTVN3pcf16ply_reader_errorE
	.section __DATA,__const_coal,coalesced
	.align 5
__ZTVN3pcf16ply_reader_errorE:
	.quad	0
	.quad	__ZTIN3pcf16ply_reader_errorE
	.quad	__ZN3pcf16ply_reader_errorD1Ev
	.quad	__ZN3pcf16ply_reader_errorD0Ev
	.quad	__ZNKSt13runtime_error4whatEv
	.globl __ZTVN3pcf11point_cloudINS_12point_xyzrgbEEE
	.weak_definition __ZTVN3pcf11point_cloudINS_12point_xyzrgbEEE
	.align 5
__ZTVN3pcf11point_cloudINS_12point_xyzrgbEEE:
	.quad	0
	.quad	__ZTIN3pcf11point_cloudINS_12point_xyzrgbEEE
	.quad	__ZN3pcf11point_cloudINS_12point_xyzrgbEED1Ev
	.quad	__ZN3pcf11point_cloudINS_12point_xyzrgbEED0Ev
	.globl __ZTVN3pcf17point_cloud_allocINS_12point_xyzrgbEEE
	.weak_definition __ZTVN3pcf17point_cloud_allocINS_12point_xyzrgbEEE
	.align 5
__ZTVN3pcf17point_cloud_allocINS_12point_xyzrgbEEE:
	.quad	0
	.quad	__ZTIN3pcf17point_cloud_allocINS_12point_xyzrgbEEE
	.quad	__ZN3pcf17point_cloud_allocINS_12point_xyzrgbEED1Ev
	.quad	__ZN3pcf17point_cloud_allocINS_12point_xyzrgbEED0Ev
	.static_data
__ZStL8__ioinit:
	.space	1
	.literal4
	.align 2
LC17:
	.long	1064532081
	.align 2
LC18:
	.long	3198039930
	.align 2
LC19:
	.long	1050556282
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$43,LECIE1-LSCIE1
	.long L$set$43
LSCIE1:
	.long	0
	.byte	0x1
	.ascii "zPLR\0"
	.byte	0x1
	.byte	0x78
	.byte	0x10
	.byte	0x7
	.byte	0x9b
	.long	___gxx_personality_v0+4@GOTPCREL
	.byte	0x10
	.byte	0x10
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.byte	0x90
	.byte	0x1
	.align 3
LECIE1:
LSFDE1:
	.set L$set$44,LEFDE1-LASFDE1
	.long L$set$44
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB1067-.
	.set L$set$45,LFE1067-LFB1067
	.quad L$set$45
	.byte	0x8
	.quad	0
	.align 3
LEFDE1:
LSFDE3:
	.set L$set$46,LEFDE3-LASFDE3
	.long L$set$46
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB9837-.
	.set L$set$47,LFE9837-LFB9837
	.quad L$set$47
	.byte	0x8
	.quad	0
	.align 3
LEFDE3:
LSFDE5:
	.set L$set$48,LEFDE5-LASFDE5
	.long L$set$48
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB9838-.
	.set L$set$49,LFE9838-LFB9838
	.quad L$set$49
	.byte	0x8
	.quad	0
	.align 3
LEFDE5:
LSFDE7:
	.set L$set$50,LEFDE7-LASFDE7
	.long L$set$50
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB9703-.
	.set L$set$51,LFE9703-LFB9703
	.quad L$set$51
	.byte	0x8
	.quad	0
	.byte	0x4
	.set L$set$52,LCFI0-LFB9703
	.long L$set$52
	.byte	0xe
	.byte	0x10
	.byte	0x83
	.byte	0x2
	.byte	0x4
	.set L$set$53,LCFI1-LCFI0
	.long L$set$53
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE7:
LSFDE9:
	.set L$set$54,LEFDE9-LASFDE9
	.long L$set$54
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB9702-.
	.set L$set$55,LFE9702-LFB9702
	.quad L$set$55
	.byte	0x8
	.quad	0
	.align 3
LEFDE9:
LSFDE11:
	.set L$set$56,LEFDE11-LASFDE11
	.long L$set$56
LASFDE11:
	.long	LASFDE11-EH_frame1
	.quad	LFB9712-.
	.set L$set$57,LFE9712-LFB9712
	.quad L$set$57
	.byte	0x8
	.quad	0
	.byte	0x4
	.set L$set$58,LCFI2-LFB9712
	.long L$set$58
	.byte	0xe
	.byte	0x10
	.byte	0x83
	.byte	0x2
	.byte	0x4
	.set L$set$59,LCFI3-LCFI2
	.long L$set$59
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE11:
LSFDE13:
	.set L$set$60,LEFDE13-LASFDE13
	.long L$set$60
LASFDE13:
	.long	LASFDE13-EH_frame1
	.quad	LFB9711-.
	.set L$set$61,LFE9711-LFB9711
	.quad L$set$61
	.byte	0x8
	.quad	0
	.align 3
LEFDE13:
LSFDE15:
	.set L$set$62,LEFDE15-LASFDE15
	.long L$set$62
LASFDE15:
	.long	LASFDE15-EH_frame1
	.quad	LFB11134-.
	.set L$set$63,LFE11134-LFB11134
	.quad L$set$63
	.byte	0x8
	.quad	0
	.byte	0x4
	.set L$set$64,LCFI4-LFB11134
	.long L$set$64
	.byte	0xe
	.byte	0x10
	.byte	0x8d
	.byte	0x2
	.byte	0x4
	.set L$set$65,LCFI5-LCFI4
	.long L$set$65
	.byte	0xe
	.byte	0x18
	.byte	0x8c
	.byte	0x3
	.byte	0x4
	.set L$set$66,LCFI6-LCFI5
	.long L$set$66
	.byte	0xe
	.byte	0x20
	.byte	0x86
	.byte	0x4
	.byte	0x4
	.set L$set$67,LCFI7-LCFI6
	.long L$set$67
	.byte	0xe
	.byte	0x28
	.byte	0x83
	.byte	0x5
	.byte	0x4
	.set L$set$68,LCFI8-LCFI7
	.long L$set$68
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$69,LCFI9-LCFI8
	.long L$set$69
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$70,LCFI10-LCFI9
	.long L$set$70
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$71,LCFI11-LCFI10
	.long L$set$71
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$72,LCFI12-LCFI11
	.long L$set$72
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$73,LCFI13-LCFI12
	.long L$set$73
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE15:
LSFDE17:
	.set L$set$74,LEFDE17-LASFDE17
	.long L$set$74
LASFDE17:
	.long	LASFDE17-EH_frame1
	.quad	LFB11137-.
	.set L$set$75,LFE11137-LFB11137
	.quad L$set$75
	.byte	0x8
	.quad	0
	.align 3
LEFDE17:
LSFDE19:
	.set L$set$76,LEFDE19-LASFDE19
	.long L$set$76
LASFDE19:
	.long	LASFDE19-EH_frame1
	.quad	LFB9699-.
	.set L$set$77,LFE9699-LFB9699
	.quad L$set$77
	.byte	0x8
	.quad	LLSDA9699-.
	.byte	0x4
	.set L$set$78,LCFI14-LFB9699
	.long L$set$78
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$79,LCFI15-LCFI14
	.long L$set$79
	.byte	0xe
	.byte	0x18
	.byte	0x83
	.byte	0x3
	.byte	0x4
	.set L$set$80,LCFI16-LCFI15
	.long L$set$80
	.byte	0xe
	.byte	0x40
	.byte	0x4
	.set L$set$81,LCFI17-LCFI16
	.long L$set$81
	.byte	0xa
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$82,LCFI18-LCFI17
	.long L$set$82
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$83,LCFI19-LCFI18
	.long L$set$83
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$84,LCFI20-LCFI19
	.long L$set$84
	.byte	0xb
	.align 3
LEFDE19:
LSFDE21:
	.set L$set$85,LEFDE21-LASFDE21
	.long L$set$85
LASFDE21:
	.long	LASFDE21-EH_frame1
	.quad	LFB9839-.
	.set L$set$86,LFE9839-LFB9839
	.quad L$set$86
	.byte	0x8
	.quad	LLSDA9839-.
	.byte	0x4
	.set L$set$87,LCFI21-LFB9839
	.long L$set$87
	.byte	0xe
	.byte	0x10
	.byte	0x8f
	.byte	0x2
	.byte	0x4
	.set L$set$88,LCFI22-LCFI21
	.long L$set$88
	.byte	0xe
	.byte	0x18
	.byte	0x8e
	.byte	0x3
	.byte	0x4
	.set L$set$89,LCFI23-LCFI22
	.long L$set$89
	.byte	0xe
	.byte	0x20
	.byte	0x8d
	.byte	0x4
	.byte	0x4
	.set L$set$90,LCFI24-LCFI23
	.long L$set$90
	.byte	0xe
	.byte	0x28
	.byte	0x8c
	.byte	0x5
	.byte	0x4
	.set L$set$91,LCFI25-LCFI24
	.long L$set$91
	.byte	0xe
	.byte	0x30
	.byte	0x86
	.byte	0x6
	.byte	0x4
	.set L$set$92,LCFI26-LCFI25
	.long L$set$92
	.byte	0xe
	.byte	0x38
	.byte	0x83
	.byte	0x7
	.byte	0x4
	.set L$set$93,LCFI27-LCFI26
	.long L$set$93
	.byte	0xe
	.byte	0x80,0x1
	.byte	0x4
	.set L$set$94,LCFI28-LCFI27
	.long L$set$94
	.byte	0xa
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$95,LCFI29-LCFI28
	.long L$set$95
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$96,LCFI30-LCFI29
	.long L$set$96
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$97,LCFI31-LCFI30
	.long L$set$97
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$98,LCFI32-LCFI31
	.long L$set$98
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$99,LCFI33-LCFI32
	.long L$set$99
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$100,LCFI34-LCFI33
	.long L$set$100
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$101,LCFI35-LCFI34
	.long L$set$101
	.byte	0xb
	.byte	0x4
	.set L$set$102,LCFI36-LCFI35
	.long L$set$102
	.byte	0xa
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$103,LCFI37-LCFI36
	.long L$set$103
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$104,LCFI38-LCFI37
	.long L$set$104
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$105,LCFI39-LCFI38
	.long L$set$105
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$106,LCFI40-LCFI39
	.long L$set$106
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$107,LCFI41-LCFI40
	.long L$set$107
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$108,LCFI42-LCFI41
	.long L$set$108
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$109,LCFI43-LCFI42
	.long L$set$109
	.byte	0xb
	.align 3
LEFDE21:
LSFDE23:
	.set L$set$110,LEFDE23-LASFDE23
	.long L$set$110
LASFDE23:
	.long	LASFDE23-EH_frame1
	.quad	LFB9705-.
	.set L$set$111,LFE9705-LFB9705
	.quad L$set$111
	.byte	0x8
	.quad	LLSDA9705-.
	.byte	0x4
	.set L$set$112,LCFI44-LFB9705
	.long L$set$112
	.byte	0xe
	.byte	0x10
	.byte	0x8f
	.byte	0x2
	.byte	0x4
	.set L$set$113,LCFI45-LCFI44
	.long L$set$113
	.byte	0xe
	.byte	0x18
	.byte	0x8e
	.byte	0x3
	.byte	0x4
	.set L$set$114,LCFI46-LCFI45
	.long L$set$114
	.byte	0xe
	.byte	0x20
	.byte	0x8d
	.byte	0x4
	.byte	0x4
	.set L$set$115,LCFI47-LCFI46
	.long L$set$115
	.byte	0xe
	.byte	0x28
	.byte	0x8c
	.byte	0x5
	.byte	0x4
	.set L$set$116,LCFI48-LCFI47
	.long L$set$116
	.byte	0xe
	.byte	0x30
	.byte	0x86
	.byte	0x6
	.byte	0x4
	.set L$set$117,LCFI49-LCFI48
	.long L$set$117
	.byte	0xe
	.byte	0x38
	.byte	0x83
	.byte	0x7
	.byte	0x4
	.set L$set$118,LCFI50-LCFI49
	.long L$set$118
	.byte	0xe
	.byte	0x60
	.byte	0x4
	.set L$set$119,LCFI51-LCFI50
	.long L$set$119
	.byte	0xa
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$120,LCFI52-LCFI51
	.long L$set$120
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$121,LCFI53-LCFI52
	.long L$set$121
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$122,LCFI54-LCFI53
	.long L$set$122
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$123,LCFI55-LCFI54
	.long L$set$123
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$124,LCFI56-LCFI55
	.long L$set$124
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$125,LCFI57-LCFI56
	.long L$set$125
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$126,LCFI58-LCFI57
	.long L$set$126
	.byte	0xb
	.align 3
LEFDE23:
LSFDE25:
	.set L$set$127,LEFDE25-LASFDE25
	.long L$set$127
LASFDE25:
	.long	LASFDE25-EH_frame1
	.quad	LFB9587-.
	.set L$set$128,LFE9587-LFB9587
	.quad L$set$128
	.byte	0x8
	.quad	LLSDA9587-.
	.byte	0x4
	.set L$set$129,LCFI59-LFB9587
	.long L$set$129
	.byte	0xe
	.byte	0x10
	.byte	0x8f
	.byte	0x2
	.byte	0x4
	.set L$set$130,LCFI60-LCFI59
	.long L$set$130
	.byte	0xe
	.byte	0x18
	.byte	0x8e
	.byte	0x3
	.byte	0x4
	.set L$set$131,LCFI61-LCFI60
	.long L$set$131
	.byte	0xe
	.byte	0x20
	.byte	0x8d
	.byte	0x4
	.byte	0x4
	.set L$set$132,LCFI62-LCFI61
	.long L$set$132
	.byte	0xe
	.byte	0x28
	.byte	0x8c
	.byte	0x5
	.byte	0x4
	.set L$set$133,LCFI63-LCFI62
	.long L$set$133
	.byte	0xe
	.byte	0x30
	.byte	0x86
	.byte	0x6
	.byte	0x4
	.set L$set$134,LCFI64-LCFI63
	.long L$set$134
	.byte	0xe
	.byte	0x38
	.byte	0x83
	.byte	0x7
	.byte	0x4
	.set L$set$135,LCFI65-LCFI64
	.long L$set$135
	.byte	0xe
	.byte	0x80,0xb
	.align 3
LEFDE25:
LSFDE27:
	.set L$set$136,LEFDE27-LASFDE27
	.long L$set$136
LASFDE27:
	.long	LASFDE27-EH_frame1
	.quad	LFB11133-.
	.set L$set$137,LFE11133-LFB11133
	.quad L$set$137
	.byte	0x8
	.quad	0
	.byte	0x4
	.set L$set$138,LCFI66-LFB11133
	.long L$set$138
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$139,LCFI67-LCFI66
	.long L$set$139
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE27:
	.mod_init_func
	.align 3
	.quad	__GLOBAL__sub_I_main.cc
	.constructor
	.destructor
	.align 1
	.subsections_via_symbols
