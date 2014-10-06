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
	.globl __ZN3pcf11point_cloudINS_9point_xyzEED1Ev
	.weak_definition __ZN3pcf11point_cloudINS_9point_xyzEED1Ev
__ZN3pcf11point_cloudINS_9point_xyzEED1Ev:
LFB9857:
	ret
LFE9857:
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
	.globl __ZN3pcf17point_cloud_allocINS_9point_xyzEED1Ev
	.weak_definition __ZN3pcf17point_cloud_allocINS_9point_xyzEED1Ev
__ZN3pcf17point_cloud_allocINS_9point_xyzEED1Ev:
LFB9708:
	movq	__ZTVN3pcf17point_cloud_allocINS_9point_xyzEEE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, (%rdi)
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	L3
	jmp	__ZdaPv
	.align 4
L3:
	ret
LFE9708:
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
	.globl __ZN3pcf11point_cloudINS_9point_xyzEED0Ev
	.weak_definition __ZN3pcf11point_cloudINS_9point_xyzEED0Ev
__ZN3pcf11point_cloudINS_9point_xyzEED0Ev:
LFB9858:
	jmp	__ZdlPv
LFE9858:
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
	.globl __ZN3pcf17point_cloud_allocINS_9point_xyzEED0Ev
	.weak_definition __ZN3pcf17point_cloud_allocINS_9point_xyzEED0Ev
__ZN3pcf17point_cloud_allocINS_9point_xyzEED0Ev:
LFB9709:
	movq	__ZTVN3pcf17point_cloud_allocINS_9point_xyzEEE@GOTPCREL(%rip), %rax
	pushq	%rbx
LCFI0:
	movq	%rdi, %rbx
	addq	$16, %rax
	movq	%rax, (%rdi)
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	L8
	call	__ZdaPv
L8:
	movq	%rbx, %rdi
	popq	%rbx
LCFI1:
	jmp	__ZdlPv
LFE9709:
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
LFB9718:
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
LFE9718:
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
LFB9717:
	movq	__ZTVN3pcf16ply_reader_errorE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, (%rdi)
	jmp	__ZNSt13runtime_errorD2Ev
LFE9717:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE6:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE6:
	.section __TEXT,__text_cold,regular,pure_instructions
	.align 1
LCOLDB7:
LHOTB7:
	.align 1
__ZNSs4_Rep10_M_disposeERKSaIcE.part.7:
LFB11633:
	orl	$-1, %eax
	lock; xaddl	%eax, 16(%rdi)
	testl	%eax, %eax
	jg	L16
	jmp	__ZNSs4_Rep10_M_destroyERKSaIcE
L16:
	ret
LFE11633:
LCOLDE7:
LHOTE7:
	.cstring
	.align 3
LC9:
	.ascii "Point cloud data not properly aligned.\0"
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB10:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB10:
	.align 1
	.align 4
	.globl __ZN3pcf17point_cloud_allocINS_9point_xyzEEC1Em
	.weak_definition __ZN3pcf17point_cloud_allocINS_9point_xyzEEC1Em
__ZN3pcf17point_cloud_allocINS_9point_xyzEEC1Em:
LFB9705:
	movabsq	$571957152676052992, %rax
	pushq	%r12
LCFI4:
	movq	%rsi, %r12
	pushq	%rbp
LCFI5:
	pushq	%rbx
LCFI6:
	subq	$32, %rsp
LCFI7:
	cmpq	%rax, %rsi
	ja	L19
	movq	%rsi, %rbx
	movq	%rdi, %rbp
	salq	$4, %rbx
	movq	%rbx, %rdi
LEHB0:
	call	__Znam
	testq	%r12, %r12
	je	L23
	movaps	LC8(%rip), %xmm0
	xorl	%edx, %edx
	.align 4
L20:
	movq	$0, (%rax,%rdx)
	movhps	%xmm0, 8(%rax,%rdx)
	addq	$16, %rdx
	cmpq	%rdx, %rbx
	jne	L20
L23:
	movq	__ZTVN3pcf11point_cloudINS_9point_xyzEEE@GOTPCREL(%rip), %rdx
	addq	%rax, %rbx
	movq	%rax, 8(%rbp)
	movq	%rbx, 16(%rbp)
	addq	$16, %rdx
	testb	$15, %al
	movq	%rdx, 0(%rbp)
	jne	L35
	movq	__ZTVN3pcf17point_cloud_allocINS_9point_xyzEEE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, 0(%rbp)
	addq	$32, %rsp
LCFI8:
	popq	%rbx
LCFI9:
	popq	%rbp
LCFI10:
	popq	%r12
LCFI11:
	ret
L19:
LCFI12:
	call	___cxa_throw_bad_array_new_length
LEHE0:
L35:
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
	je	L24
	leaq	14(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L24:
	movq	__ZNSt13runtime_errorD1Ev@GOTPCREL(%rip), %rdx
	movq	%rbp, %rdi
	movq	__ZTISt13runtime_error@GOTPCREL(%rip), %rsi
LEHB3:
	call	___cxa_throw
L30:
	movq	%rax, %rbx
	jmp	L25
L29:
	movq	%rax, %rbx
	jmp	L27
L25:
	movq	16(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L27
	leaq	14(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L27:
	movq	%rbp, %rdi
	call	___cxa_free_exception
	movq	%rbx, %rdi
	call	__Unwind_Resume
LEHE3:
LFE9705:
	.section __DATA,__gcc_except_tab
GCC_except_table0:
LLSDA9705:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x34
	.set L$set$0,LEHB0-LFB9705
	.long L$set$0
	.set L$set$1,LEHE0-LEHB0
	.long L$set$1
	.long	0
	.byte	0
	.set L$set$2,LEHB1-LFB9705
	.long L$set$2
	.set L$set$3,LEHE1-LEHB1
	.long L$set$3
	.set L$set$4,L29-LFB9705
	.long L$set$4
	.byte	0
	.set L$set$5,LEHB2-LFB9705
	.long L$set$5
	.set L$set$6,LEHE2-LEHB2
	.long L$set$6
	.set L$set$7,L30-LFB9705
	.long L$set$7
	.byte	0
	.set L$set$8,LEHB3-LFB9705
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
LCOLDB13:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB13:
	.align 1
	.align 4
	.globl __ZN5Eigen9TransformIfLi3ELi2ELi0EEC1ILi1ELi0EEERKNS0_IfLi3EXT_EXT0_EEE
	.weak_definition __ZN5Eigen9TransformIfLi3ELi2ELi0EEC1ILi1ELi0EEERKNS0_IfLi3EXT_EXT0_EEE
__ZN5Eigen9TransformIfLi3ELi2ELi0EEC1ILi1ELi0EEERKNS0_IfLi3EXT_EXT0_EEE:
LFB9750:
	movss	(%rsi), %xmm0
	movss	%xmm0, (%rdi)
	movss	4(%rsi), %xmm0
	movss	%xmm0, 4(%rdi)
	movss	8(%rsi), %xmm0
	movss	%xmm0, 8(%rdi)
	movss	16(%rsi), %xmm0
	movss	%xmm0, 16(%rdi)
	movss	20(%rsi), %xmm0
	movss	%xmm0, 20(%rdi)
	movss	24(%rsi), %xmm0
	movss	%xmm0, 24(%rdi)
	movss	32(%rsi), %xmm0
	movss	%xmm0, 32(%rdi)
	movss	36(%rsi), %xmm0
	movss	%xmm0, 36(%rdi)
	movss	40(%rsi), %xmm0
	movss	%xmm0, 40(%rdi)
	movss	48(%rsi), %xmm0
	movss	%xmm0, 48(%rdi)
	movss	52(%rsi), %xmm0
	movss	%xmm0, 52(%rdi)
	movss	56(%rsi), %xmm0
	movl	$0x3f800000, 60(%rdi)
	movss	%xmm0, 56(%rdi)
	pxor	%xmm0, %xmm0
	movss	%xmm0, 12(%rdi)
	movss	%xmm0, 28(%rdi)
	movss	%xmm0, 44(%rdi)
	ret
LFE9750:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE13:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE13:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB14:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB14:
	.align 1
	.align 4
	.globl __ZN5Eigen8IOFormatD1Ev
	.weak_definition __ZN5Eigen8IOFormatD1Ev
__ZN5Eigen8IOFormatD1Ev:
LFB9758:
	pushq	%rbx
LCFI13:
	movq	%rdi, %rbx
	subq	$16, %rsp
LCFI14:
	movq	48(%rdi), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L60
L39:
	movq	40(%rbx), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L61
L42:
	movq	32(%rbx), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L62
L45:
	movq	24(%rbx), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L63
L48:
	movq	16(%rbx), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L64
L51:
	movq	8(%rbx), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L65
L54:
	movq	(%rbx), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L66
L37:
	addq	$16, %rsp
LCFI15:
	popq	%rbx
LCFI16:
	ret
	.align 4
L60:
LCFI17:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L39
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L39
	.align 4
L61:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L42
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L42
	.align 4
L62:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L45
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L45
	.align 4
L63:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L48
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L48
	.align 4
L64:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L51
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L51
	.align 4
L65:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L54
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L54
	.align 4
L66:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L37
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L37
LFE9758:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE14:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE14:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB15:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB15:
	.align 1
	.align 4
	.globl __ZN3pcf10ply_reader11read_ascii_INS_9point_xyzEEEvPT_m
	.weak_definition __ZN3pcf10ply_reader11read_ascii_INS_9point_xyzEEEvPT_m
__ZN3pcf10ply_reader11read_ascii_INS_9point_xyzEEEvPT_m:
LFB9859:
	pushq	%r15
LCFI18:
	pushq	%r14
LCFI19:
	movq	%rsi, %r14
	pushq	%r13
LCFI20:
	pushq	%r12
LCFI21:
	pushq	%rbp
LCFI22:
	movq	%rdx, %rbp
	pushq	%rbx
LCFI23:
	subq	$72, %rsp
LCFI24:
	movq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rax
	movq	%rdi, 16(%rsp)
	addq	$24, %rax
	movq	%rax, 48(%rsp)
	movq	1072(%rdi), %rax
	testq	%rax, %rax
	jne	L110
	testq	%rdx, %rdx
	je	L73
	movq	$0, 8(%rsp)
L70:
	movq	%rbp, 24(%rsp)
	leaq	48(%rsp), %r15
	.align 4
L82:
	movq	16(%rsp), %rdi
	movq	%r15, %rsi
LEHB4:
	call	__ZN3pcf10ply_reader10read_line_ERSs
	movq	48(%rsp), %rbx
	movl	-8(%rbx), %edx
	testl	%edx, %edx
	js	L92
	movq	%r15, %rdi
	call	__ZNSs12_M_leak_hardEv
	movq	48(%rsp), %rbx
	movl	-8(%rbx), %eax
	testl	%eax, %eax
	js	L92
	movq	%r15, %rdi
	call	__ZNSs12_M_leak_hardEv
	movq	48(%rsp), %r12
L74:
	addq	-24(%r12), %r12
	movl	$1, %ebp
	movq	8(%rsp), %r13
	cmpq	%r12, %rbx
	jne	L102
	jmp	L81
	.align 4
L111:
	movq	__DefaultRuneLocale@GOTPCREL(%rip), %rax
	movslq	%edi, %rdi
	movl	60(%rax,%rdi,4), %eax
	shrl	$14, %eax
	andl	$1, %eax
L78:
	andl	$1, %eax
	cmpb	%al, %bpl
	je	L79
	testb	%bpl, %bpl
	je	L79
	movq	%rbx, 0(%r13)
	addq	$8, %r13
L79:
	addq	$1, %rbx
	cmpq	%rbx, %r12
	je	L81
	movl	%eax, %ebp
L102:
	movsbl	(%rbx), %edi
	testb	%dil, %dil
	jns	L111
	movl	$16384, %esi
	call	___maskrune
	testl	%eax, %eax
	setne	%al
	movzbl	%al, %eax
	jmp	L78
	.align 4
L81:
	movq	8(%rsp), %rdx
	movq	%r14, %rsi
	movq	16(%rsp), %rdi
	call	__ZNK3pcf10ply_reader17read_ascii_point_ERNS_9point_xyzEPPKc
LEHE4:
	addq	$16, %r14
	subq	$1, 24(%rsp)
	jne	L82
	cmpq	$0, 8(%rsp)
	je	L73
L72:
	movq	8(%rsp), %rdi
	call	__ZdlPv
L73:
	movq	48(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L112
L67:
	addq	$72, %rsp
LCFI25:
	popq	%rbx
LCFI26:
	popq	%rbp
LCFI27:
	popq	%r12
LCFI28:
	popq	%r13
LCFI29:
	popq	%r14
LCFI30:
	popq	%r15
LCFI31:
	ret
	.align 4
L92:
LCFI32:
	movq	%rbx, %r12
	jmp	L74
L110:
	movabsq	$2305843009213693951, %rdx
	cmpq	%rdx, %rax
	ja	L113
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
	jne	L70
	jmp	L72
	.align 4
L112:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L67
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	addq	$72, %rsp
LCFI33:
	popq	%rbx
LCFI34:
	popq	%rbp
LCFI35:
	popq	%r12
LCFI36:
	popq	%r13
LCFI37:
	popq	%r14
LCFI38:
	popq	%r15
LCFI39:
	ret
L113:
LCFI40:
	call	__ZSt17__throw_bad_allocv
LEHE5:
L95:
	movq	%rax, %rbx
	jmp	L89
L96:
	movq	%rax, %rbx
L87:
	movq	8(%rsp), %rax
	testq	%rax, %rax
	je	L89
	movq	%rax, %rdi
	call	__ZdlPv
L89:
	movq	48(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L90
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L90:
	movq	%rbx, %rdi
LEHB6:
	call	__Unwind_Resume
LEHE6:
LFE9859:
	.section __DATA,__gcc_except_tab
GCC_except_table1:
LLSDA9859:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x27
	.set L$set$10,LEHB4-LFB9859
	.long L$set$10
	.set L$set$11,LEHE4-LEHB4
	.long L$set$11
	.set L$set$12,L96-LFB9859
	.long L$set$12
	.byte	0
	.set L$set$13,LEHB5-LFB9859
	.long L$set$13
	.set L$set$14,LEHE5-LEHB5
	.long L$set$14
	.set L$set$15,L95-LFB9859
	.long L$set$15
	.byte	0
	.set L$set$16,LEHB6-LFB9859
	.long L$set$16
	.set L$set$17,LEHE6-LEHB6
	.long L$set$17
	.long	0
	.byte	0
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE15:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE15:
	.cstring
	.align 3
LC16:
	.ascii "Attempted to read beyond bounds.\0"
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB17:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB17:
	.align 1
	.align 4
	.globl __ZN3pcf10ply_reader4readINS_9point_xyzEEEvPT_m
	.weak_definition __ZN3pcf10ply_reader4readINS_9point_xyzEEEvPT_m
__ZN3pcf10ply_reader4readINS_9point_xyzEEEvPT_m:
LFB9711:
	pushq	%r15
LCFI41:
	movq	%rdx, %rax
	movq	%rdx, %r15
	pushq	%r14
LCFI42:
	pushq	%r13
LCFI43:
	pushq	%r12
LCFI44:
	movq	%rdi, %r12
	pushq	%rbp
LCFI45:
	pushq	%rbx
LCFI46:
	subq	$40, %rsp
LCFI47:
	addq	1232(%rdi), %rax
	cmpq	1048(%rdi), %rax
	ja	L133
	cmpl	$2, 1056(%rdi)
	movq	%rsi, %rbp
	je	L134
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
	je	L121
	.align 4
L129:
	movq	%rbx, %rdx
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	__ZNK3pcf10ply_reader18read_binary_point_ERNS_9point_xyzEPc
LEHE8:
	addq	1064(%r12), %rbx
	addq	$16, %rbp
	cmpq	%rbx, %r13
	jne	L129
L121:
	movq	%r14, %rdi
	call	__ZdaPv
L118:
	addq	%r15, 1232(%r12)
	addq	$40, %rsp
LCFI48:
	popq	%rbx
LCFI49:
	popq	%rbp
LCFI50:
	popq	%r12
LCFI51:
	popq	%r13
LCFI52:
	popq	%r14
LCFI53:
	popq	%r15
LCFI54:
	ret
	.align 4
L134:
LCFI55:
LEHB9:
	call	__ZN3pcf10ply_reader11read_ascii_INS_9point_xyzEEEvPT_m
LEHE9:
	jmp	L118
L133:
	leaq	16(%rsp), %rbx
	movl	$16, %edi
	call	___cxa_allocate_exception
	leaq	14(%rsp), %rdx
	movq	%rbx, %rdi
	leaq	LC16(%rip), %rsi
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
	je	L116
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L116:
	movq	__ZN3pcf16ply_reader_errorD1Ev@GOTPCREL(%rip), %rdx
	movq	%rbp, %rdi
	movq	__ZTIN3pcf16ply_reader_errorE@GOTPCREL(%rip), %rsi
LEHB12:
	call	___cxa_throw
L127:
	movq	%rax, %rbx
	jmp	L123
L126:
	movq	%rax, %rbx
	jmp	L125
L128:
	movq	%rax, %rbx
	jmp	L122
L123:
	movq	16(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L125
	leaq	15(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L125:
	movq	%rbp, %rdi
	call	___cxa_free_exception
	movq	%rbx, %rdi
	call	__Unwind_Resume
L122:
	movq	%r14, %rdi
	call	__ZdaPv
	movq	%rbx, %rdi
	call	__Unwind_Resume
LEHE12:
LFE9711:
	.section __DATA,__gcc_except_tab
GCC_except_table2:
LLSDA9711:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x4e
	.set L$set$18,LEHB7-LFB9711
	.long L$set$18
	.set L$set$19,LEHE7-LEHB7
	.long L$set$19
	.long	0
	.byte	0
	.set L$set$20,LEHB8-LFB9711
	.long L$set$20
	.set L$set$21,LEHE8-LEHB8
	.long L$set$21
	.set L$set$22,L128-LFB9711
	.long L$set$22
	.byte	0
	.set L$set$23,LEHB9-LFB9711
	.long L$set$23
	.set L$set$24,LEHE9-LEHB9
	.long L$set$24
	.long	0
	.byte	0
	.set L$set$25,LEHB10-LFB9711
	.long L$set$25
	.set L$set$26,LEHE10-LEHB10
	.long L$set$26
	.set L$set$27,L126-LFB9711
	.long L$set$27
	.byte	0
	.set L$set$28,LEHB11-LFB9711
	.long L$set$28
	.set L$set$29,LEHE11-LEHB11
	.long L$set$29
	.set L$set$30,L127-LFB9711
	.long L$set$30
	.byte	0
	.set L$set$31,LEHB12-LFB9711
	.long L$set$31
	.set L$set$32,LEHE12-LEHB12
	.long L$set$32
	.long	0
	.byte	0
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE17:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE17:
	.cstring
	.align 3
LC18:
	.ascii "basic_string::_S_construct null not valid\0"
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDB19:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB19:
	.align 4
	.globl __ZN5Eigen8internal12print_matrixINS_6MatrixIfLi4ELi4ELi0ELi4ELi4EEEEERSoS4_RKT_RKNS_8IOFormatE
	.weak_definition __ZN5Eigen8internal12print_matrixINS_6MatrixIfLi4ELi4ELi0ELi4ELi4EEEEERSoS4_RKT_RKNS_8IOFormatE
__ZN5Eigen8internal12print_matrixINS_6MatrixIfLi4ELi4ELi0ELi4ELi4EEEEERSoS4_RKT_RKNS_8IOFormatE:
LFB9986:
	pushq	%r15
LCFI56:
	movq	%rdx, %r15
	pushq	%r14
LCFI57:
	pushq	%r13
LCFI58:
	pushq	%r12
LCFI59:
	pushq	%rbp
LCFI60:
	movq	%rdi, %rbp
	pushq	%rbx
LCFI61:
	subq	$568, %rsp
LCFI62:
	movslq	56(%rdx), %rax
	movq	%rsi, 120(%rsp)
	cmpl	$-1, %eax
	je	L193
	cmpl	$-2, %eax
	je	L194
	testq	%rax, %rax
	movq	%rax, 112(%rsp)
	movq	$0, 128(%rsp)
	je	L136
L137:
	movq	0(%rbp), %rax
	movq	%rbp, %rbx
	addq	-24(%rax), %rbx
	movq	%rbx, %rax
	movq	8(%rbx), %rbx
	movq	%rbx, 128(%rsp)
	movq	112(%rsp), %rbx
	movq	%rbx, 8(%rax)
L136:
	testb	$1, 60(%r15)
	jne	L138
	movq	__ZTTSt18basic_stringstreamIcSt11char_traitsIcESaIcEE@GOTPCREL(%rip), %rax
	movq	$0, 16(%rsp)
	movq	16(%rax), %r12
	movq	24(%rax), %rax
	movq	%rax, 24(%rsp)
	movq	120(%rsp), %rax
	leaq	16(%rax), %rbx
	addq	$80, %rax
	movq	%rax, 96(%rsp)
	leaq	160(%rsp), %rax
	movq	%rax, 56(%rsp)
	leaq	176(%rsp), %rax
	movq	%rax, 64(%rsp)
	leaq	158(%rsp), %rax
	movq	%rax, 136(%rsp)
	leaq	159(%rsp), %rax
	movq	%rbx, 32(%rsp)
	leaq	192(%rsp), %rbx
	movq	%rax, 88(%rsp)
	movq	32(%rsp), %rax
L139:
	subq	$16, %rax
	movq	%rax, 8(%rsp)
L180:
	leaq	104(%rbx), %rdi
	call	__ZNSt8ios_baseC2Ev
	movq	24(%rsp), %rcx
	movq	%rbx, %rdi
	xorl	%esi, %esi
	movq	__ZTVSt9basic_iosIcSt11char_traitsIcEE@GOTPCREL(%rip), %rax
	movb	$0, 520(%rsp)
	movq	$0, 512(%rsp)
	movb	$0, 521(%rsp)
	movq	$0, 528(%rsp)
	addq	$16, %rax
	movq	$0, 536(%rsp)
	movq	%rax, 296(%rsp)
	movq	-24(%r12), %rax
	movq	$0, 544(%rsp)
	movq	$0, 552(%rsp)
	movq	%r12, 192(%rsp)
	movq	%rcx, 192(%rsp,%rax)
	movq	$0, 200(%rsp)
	addq	-24(%r12), %rdi
LEHB13:
	call	__ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E
LEHE13:
	movq	__ZTTSt18basic_stringstreamIcSt11char_traitsIcESaIcEE@GOTPCREL(%rip), %rax
	xorl	%esi, %esi
	movq	32(%rax), %r13
	movq	40(%rax), %rax
	movq	-24(%r13), %rdx
	movq	%r13, 208(%rsp)
	movq	%rax, 40(%rsp)
	leaq	16(%rbx,%rdx), %rdi
	movq	%rax, (%rdi)
LEHB14:
	call	__ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E
LEHE14:
	movq	__ZTTSt18basic_stringstreamIcSt11char_traitsIcESaIcEE@GOTPCREL(%rip), %rax
	leaq	80(%rbx), %rdi
	movq	8(%rax), %r14
	movq	48(%rax), %rax
	movq	%rax, %rcx
	movq	%rax, 48(%rsp)
	movq	-24(%r14), %rax
	movq	%rcx, 192(%rsp,%rax)
	movq	__ZTVSt18basic_stringstreamIcSt11char_traitsIcESaIcEE@GOTPCREL(%rip), %rax
	movq	$0, 224(%rsp)
	movq	$0, 232(%rsp)
	movq	$0, 240(%rsp)
	movq	$0, 248(%rsp)
	leaq	24(%rax), %rdx
	movq	%rdx, 192(%rsp)
	leaq	104(%rax), %rdx
	addq	$64, %rax
	movq	%rax, 208(%rsp)
	movq	__ZTVSt15basic_streambufIcSt11char_traitsIcEE@GOTPCREL(%rip), %rax
	movq	%rdx, 296(%rsp)
	movq	$0, 256(%rsp)
	movq	$0, 264(%rsp)
	addq	$16, %rax
	movq	%rax, 216(%rsp)
	call	__ZNSt6localeC1Ev
	movq	__ZTVSt15basic_stringbufIcSt11char_traitsIcESaIcEE@GOTPCREL(%rip), %rax
	leaq	24(%rbx), %rsi
	movl	$24, 280(%rsp)
	leaq	104(%rbx), %rdi
	addq	$16, %rax
	movq	%rax, 216(%rsp)
	movq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rax
	addq	$24, %rax
	movq	%rax, 288(%rsp)
LEHB15:
	call	__ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E
LEHE15:
	movq	0(%rbp), %rax
	leaq	104(%rbx), %rdi
	movq	%rbp, %rsi
	addq	-24(%rax), %rsi
LEHB16:
	call	__ZNSt9basic_iosIcSt11char_traitsIcEE7copyfmtERKS2_
	movq	8(%rsp), %rax
	leaq	16(%rbx), %rdi
	pxor	%xmm0, %xmm0
	cvtss2sd	(%rax), %xmm0
	call	__ZNSo9_M_insertIdEERSoT_
LEHE16:
	movq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rax
	leaq	24(%rax), %rdi
	movq	256(%rsp), %rax
	movq	%rdi, 160(%rsp)
	testq	%rax, %rax
	je	L147
	movq	240(%rsp), %rdx
	cmpq	%rdx, %rax
	jbe	L148
	movq	248(%rsp), %rcx
	cmpq	%rcx, %rax
	movq	%rcx, 72(%rsp)
	je	L196
	testq	%rcx, %rcx
	jne	L236
	leaq	LC18(%rip), %rdi
LEHB17:
	call	__ZSt19__throw_logic_errorPKc
L236:
	subq	72(%rsp), %rax
	xorl	%esi, %esi
	movq	88(%rsp), %rdx
	movq	%rax, %rdi
	movq	%rax, 80(%rsp)
	call	__ZNSs4_Rep9_S_createEmmRKSaIcE
LEHE17:
	cmpq	$1, 80(%rsp)
	leaq	24(%rax), %rcx
	movq	%rax, %r8
	jne	L152
	movq	72(%rsp), %rax
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %r8
	movzbl	(%rax), %eax
	movb	%al, 24(%r8)
	jne	L237
L149:
	movq	64(%rsp), %rsi
	movq	%rcx, 176(%rsp)
	movq	56(%rsp), %rdi
LEHB18:
	call	__ZNSs4swapERSs
LEHE18:
L229:
	movq	176(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L238
L157:
	movq	160(%rsp), %rax
	movq	16(%rsp), %rcx
	movq	-24(%rax), %rdx
	leaq	-24(%rax), %rdi
	cmpq	%rdx, %rcx
	cmovge	%rcx, %rdx
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	movq	%rdx, 16(%rsp)
	jne	L239
L175:
	movq	__ZTVSt18basic_stringstreamIcSt11char_traitsIcESaIcEE@GOTPCREL(%rip), %rax
	leaq	24(%rax), %rdx
	movq	%rdx, 192(%rsp)
	leaq	104(%rax), %rdx
	addq	$64, %rax
	movq	%rax, 208(%rsp)
	movq	__ZTVSt15basic_stringbufIcSt11char_traitsIcESaIcEE@GOTPCREL(%rip), %rax
	movq	%rdx, 296(%rsp)
	addq	$16, %rax
	movq	%rax, 216(%rsp)
	movq	288(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L240
L178:
	movq	__ZTVSt15basic_streambufIcSt11char_traitsIcEE@GOTPCREL(%rip), %rax
	leaq	80(%rbx), %rdi
	addq	$16, %rax
	movq	%rax, 216(%rsp)
	call	__ZNSt6localeD1Ev
	movq	-24(%r14), %rax
	leaq	104(%rbx), %rdi
	movq	48(%rsp), %rcx
	movq	%rcx, 192(%rsp,%rax)
	movq	-24(%r13), %rax
	movq	40(%rsp), %rcx
	movq	%r13, 208(%rsp)
	movq	%rcx, 208(%rsp,%rax)
	movq	-24(%r12), %rax
	movq	24(%rsp), %rcx
	movq	%r12, 192(%rsp)
	movq	%rcx, 192(%rsp,%rax)
	movq	__ZTVSt9basic_iosIcSt11char_traitsIcEE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, 296(%rsp)
	call	__ZNSt8ios_baseD2Ev
	addq	$4, 8(%rsp)
	movq	8(%rsp), %rax
	cmpq	%rax, 32(%rsp)
	jne	L180
	addq	$16, 32(%rsp)
	movq	32(%rsp), %rax
	cmpq	96(%rsp), %rax
	jne	L139
L181:
	movq	(%r15), %rsi
	movq	%rbp, %rdi
	xorl	%r13d, %r13d
	movq	-24(%rsi), %rdx
LEHB19:
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	120(%rsp), %r12
	movq	16(%rsp), %r14
L182:
	movq	16(%r15), %rsi
	movq	%rbp, %rdi
	movq	-24(%rsi), %rdx
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	testq	%r14, %r14
	je	L183
	movq	0(%rbp), %rax
	movq	%rbp, %rdi
	pxor	%xmm0, %xmm0
	xorl	%ebx, %ebx
	cvtss2sd	(%r12), %xmm0
	movq	-24(%rax), %rax
	movq	%r14, 16(%rbp,%rax)
	call	__ZNSo9_M_insertIdEERSoT_
L184:
	movq	48(%r15), %rsi
	movq	%rbp, %rdi
	movq	-24(%rsi), %rdx
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	0(%rbp), %rax
	pxor	%xmm0, %xmm0
	movq	%rbp, %rdi
	cvtss2sd	16(%r12,%rbx), %xmm0
	addq	$16, %rbx
	movq	-24(%rax), %rax
	movq	%r14, 16(%rbp,%rax)
	call	__ZNSo9_M_insertIdEERSoT_
	cmpq	$48, %rbx
	jne	L184
L186:
	movq	24(%r15), %rsi
	movq	%rbp, %rdi
	movq	-24(%rsi), %rdx
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	cmpq	$3, %r13
	je	L241
	movq	32(%r15), %rsi
	movq	%rbp, %rdi
	addq	$1, %r13
	addq	$4, %r12
	movq	-24(%rsi), %rdx
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	40(%r15), %rsi
	movq	%rbp, %rdi
	movq	-24(%rsi), %rdx
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
LEHE19:
	jmp	L182
	.align 4
L148:
	movq	248(%rsp), %rax
	cmpq	%rax, %rdx
	movq	%rax, 72(%rsp)
	je	L197
	testq	%rax, %rax
	jne	L161
	testq	%rdx, %rdx
	je	L161
	leaq	LC18(%rip), %rdi
LEHB20:
	call	__ZSt19__throw_logic_errorPKc
L161:
	subq	72(%rsp), %rdx
	xorl	%esi, %esi
	movq	%rdx, %rax
	movq	88(%rsp), %rdx
	movq	%rax, %rdi
	movq	%rax, 80(%rsp)
	call	__ZNSs4_Rep9_S_createEmmRKSaIcE
LEHE20:
	cmpq	$1, 80(%rsp)
	leaq	24(%rax), %rcx
	movq	%rax, %r8
	jne	L162
	movq	72(%rsp), %rax
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %r8
	movzbl	(%rax), %eax
	movb	%al, 24(%r8)
	jne	L242
L158:
	movq	64(%rsp), %rsi
	movq	%rcx, 176(%rsp)
	movq	56(%rsp), %rdi
LEHB21:
	call	__ZNSs4swapERSs
LEHE21:
	jmp	L229
	.align 4
L183:
	movq	%rbp, %rdi
	pxor	%xmm0, %xmm0
	xorl	%ebx, %ebx
	cvtss2sd	(%r12), %xmm0
LEHB22:
	call	__ZNSo9_M_insertIdEERSoT_
L185:
	movq	48(%r15), %rsi
	movq	%rbp, %rdi
	movq	-24(%rsi), %rdx
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	pxor	%xmm0, %xmm0
	movq	%rbp, %rdi
	cvtss2sd	16(%r12,%rbx), %xmm0
	call	__ZNSo9_M_insertIdEERSoT_
	addq	$16, %rbx
	cmpq	$48, %rbx
	jne	L185
	jmp	L186
	.align 4
L241:
	movq	8(%r15), %rsi
	movq	%rbp, %rdi
	movq	-24(%rsi), %rdx
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
LEHE22:
	cmpq	$0, 112(%rsp)
	je	L208
	movq	0(%rbp), %rax
	movq	128(%rsp), %rbx
	movq	-24(%rax), %rax
	movq	%rbx, 8(%rbp,%rax)
L208:
	addq	$568, %rsp
LCFI63:
	movq	%rbp, %rax
	popq	%rbx
LCFI64:
	popq	%rbp
LCFI65:
	popq	%r12
LCFI66:
	popq	%r13
LCFI67:
	popq	%r14
LCFI68:
	popq	%r15
LCFI69:
	ret
	.align 4
L152:
LCFI70:
	movq	80(%rsp), %rdx
	movq	%rcx, %rdi
	movq	%r8, 104(%rsp)
	movq	72(%rsp), %rsi
	call	_memcpy
	movq	104(%rsp), %r8
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %r8
	movq	%rax, %rcx
	je	L149
L237:
	movq	80(%rsp), %rax
	movl	$0, 16(%r8)
	movq	%rax, (%r8)
	movb	$0, 24(%r8,%rax)
	jmp	L149
	.align 4
L162:
	movq	80(%rsp), %rdx
	movq	%rcx, %rdi
	movq	%r8, 104(%rsp)
	movq	72(%rsp), %rsi
	call	_memcpy
	movq	104(%rsp), %r8
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %r8
	movq	%rax, %rcx
	je	L158
L242:
	movq	80(%rsp), %rax
	movl	$0, 16(%r8)
	movq	%rax, (%r8)
	movb	$0, 24(%r8,%rax)
	jmp	L158
	.align 4
L240:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L178
	movq	64(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L178
	.align 4
L239:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L175
	movq	64(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L175
	.align 4
L238:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L157
	movq	136(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L157
	.align 4
L196:
	movq	%rdi, %rcx
	jmp	L149
	.align 4
L197:
	movq	%rdi, %rcx
	jmp	L158
	.align 4
L147:
	movq	56(%rsp), %rdi
	leaq	96(%rbx), %rsi
LEHB23:
	call	__ZNSs6assignERKSs
LEHE23:
	jmp	L157
	.align 4
L138:
	movq	$0, 16(%rsp)
	jmp	L181
	.align 4
L193:
	movq	$0, 112(%rsp)
	movq	$0, 128(%rsp)
	jmp	L136
	.align 4
L194:
	movq	$7, 112(%rsp)
	jmp	L137
	.align 4
L203:
	movq	%rax, %rbp
	jmp	L169
L205:
L230:
	movq	%rax, %rbp
	jmp	L170
L204:
	jmp	L230
L170:
	movq	176(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L169
	leaq	158(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L169:
	movq	160(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L173
	leaq	176(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L173:
	movq	%rbx, %rdi
	call	__ZNSt18basic_stringstreamIcSt11char_traitsIcESaIcEED1Ev
	movq	%rbp, %rdi
LEHB24:
	call	__Unwind_Resume
L199:
	movq	%rax, %rbp
	jmp	L173
L201:
	movq	%rax, %rbp
	jmp	L143
L202:
	jmp	L141
L200:
	movq	%rax, %rbp
	jmp	L142
L143:
	movq	__ZTVSt15basic_stringbufIcSt11char_traitsIcESaIcEE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	movq	%rax, 216(%rsp)
	movq	288(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L243
L145:
	movq	__ZTVSt15basic_streambufIcSt11char_traitsIcEE@GOTPCREL(%rip), %rax
	leaq	80(%rbx), %rdi
	addq	$16, %rax
	movq	%rax, 216(%rsp)
	call	__ZNSt6localeD1Ev
	movq	__ZTTSt18basic_stringstreamIcSt11char_traitsIcESaIcEE@GOTPCREL(%rip), %rax
	movq	%rbx, %rdi
	leaq	8(%rax), %rsi
	call	__ZNSdD2Ev
L142:
	movq	__ZTVSt9basic_iosIcSt11char_traitsIcEE@GOTPCREL(%rip), %rax
	leaq	104(%rbx), %rdi
	addq	$16, %rax
	movq	%rax, 296(%rsp)
	call	__ZNSt8ios_baseD2Ev
	movq	%rbp, %rdi
	call	__Unwind_Resume
LEHE24:
L243:
	movl	$-1, %edx
	lock; xaddl	%edx, -8(%rax)
	testl	%edx, %edx
	jg	L145
	leaq	176(%rsp), %rsi
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	jmp	L145
L141:
	movq	-24(%r12), %rdx
	movq	%r12, 192(%rsp)
	movq	%rax, %rbp
	movq	24(%rsp), %rsi
	movq	%rsi, 192(%rsp,%rdx)
	jmp	L142
LFE9986:
	.section __DATA,__gcc_except_tab
GCC_except_table3:
LLSDA9986:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x9c,0x1
	.set L$set$33,LEHB13-LFB9986
	.long L$set$33
	.set L$set$34,LEHE13-LEHB13
	.long L$set$34
	.set L$set$35,L200-LFB9986
	.long L$set$35
	.byte	0
	.set L$set$36,LEHB14-LFB9986
	.long L$set$36
	.set L$set$37,LEHE14-LEHB14
	.long L$set$37
	.set L$set$38,L202-LFB9986
	.long L$set$38
	.byte	0
	.set L$set$39,LEHB15-LFB9986
	.long L$set$39
	.set L$set$40,LEHE15-LEHB15
	.long L$set$40
	.set L$set$41,L201-LFB9986
	.long L$set$41
	.byte	0
	.set L$set$42,LEHB16-LFB9986
	.long L$set$42
	.set L$set$43,LEHE16-LEHB16
	.long L$set$43
	.set L$set$44,L199-LFB9986
	.long L$set$44
	.byte	0
	.set L$set$45,LEHB17-LFB9986
	.long L$set$45
	.set L$set$46,LEHE17-LEHB17
	.long L$set$46
	.set L$set$47,L203-LFB9986
	.long L$set$47
	.byte	0
	.set L$set$48,LEHB18-LFB9986
	.long L$set$48
	.set L$set$49,LEHE18-LEHB18
	.long L$set$49
	.set L$set$50,L205-LFB9986
	.long L$set$50
	.byte	0
	.set L$set$51,LEHB19-LFB9986
	.long L$set$51
	.set L$set$52,LEHE19-LEHB19
	.long L$set$52
	.long	0
	.byte	0
	.set L$set$53,LEHB20-LFB9986
	.long L$set$53
	.set L$set$54,LEHE20-LEHB20
	.long L$set$54
	.set L$set$55,L203-LFB9986
	.long L$set$55
	.byte	0
	.set L$set$56,LEHB21-LFB9986
	.long L$set$56
	.set L$set$57,LEHE21-LEHB21
	.long L$set$57
	.set L$set$58,L204-LFB9986
	.long L$set$58
	.byte	0
	.set L$set$59,LEHB22-LFB9986
	.long L$set$59
	.set L$set$60,LEHE22-LEHB22
	.long L$set$60
	.long	0
	.byte	0
	.set L$set$61,LEHB23-LFB9986
	.long L$set$61
	.set L$set$62,LEHE23-LEHB23
	.long L$set$62
	.set L$set$63,L203-LFB9986
	.long L$set$63
	.byte	0
	.set L$set$64,LEHB24-LFB9986
	.long L$set$64
	.set L$set$65,LEHE24-LEHB24
	.long L$set$65
	.long	0
	.byte	0
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE19:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE19:
	.cstring
LC20:
	.ascii "\0"
LC21:
	.ascii "\12\0"
LC22:
	.ascii " \0"
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDB23:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB23:
	.align 4
	.globl __ZN5EigenlsINS_6MatrixIfLi4ELi4ELi0ELi4ELi4EEEEERSoS3_RKNS_9DenseBaseIT_EE
	.weak_definition __ZN5EigenlsINS_6MatrixIfLi4ELi4ELi0ELi4ELi4EEEEERSoS3_RKNS_9DenseBaseIT_EE
__ZN5EigenlsINS_6MatrixIfLi4ELi4ELi0ELi4ELi4EEEEERSoS3_RKNS_9DenseBaseIT_EE:
LFB9755:
	pushq	%r15
LCFI71:
	pushq	%r14
LCFI72:
	pushq	%r13
LCFI73:
	pushq	%r12
LCFI74:
	pushq	%rbp
LCFI75:
	pushq	%rbx
LCFI76:
	subq	$216, %rsp
LCFI77:
	leaq	128(%rsp), %rbp
	movq	%rdi, (%rsp)
	movq	%rsi, 8(%rsp)
	leaq	46(%rsp), %rdx
	movq	%rbp, %rdi
	leaq	LC20(%rip), %rsi
LEHB25:
	call	__ZNSsC1EPKcRKSaIcE
LEHE25:
	leaq	112(%rsp), %r12
	leaq	45(%rsp), %rdx
	movq	%r12, %rdi
	leaq	LC20(%rip), %rsi
LEHB26:
	call	__ZNSsC1EPKcRKSaIcE
LEHE26:
	leaq	96(%rsp), %r13
	leaq	44(%rsp), %rdx
	movq	%r13, %rdi
	leaq	LC20(%rip), %rsi
LEHB27:
	call	__ZNSsC1EPKcRKSaIcE
LEHE27:
	leaq	80(%rsp), %rbx
	leaq	43(%rsp), %rdx
	movq	%rbx, %rdi
	leaq	LC20(%rip), %rsi
LEHB28:
	call	__ZNSsC1EPKcRKSaIcE
LEHE28:
	leaq	64(%rsp), %r15
	leaq	42(%rsp), %rdx
	movq	%r15, %rdi
	leaq	LC21(%rip), %rsi
LEHB29:
	call	__ZNSsC1EPKcRKSaIcE
LEHE29:
	leaq	48(%rsp), %rax
	leaq	41(%rsp), %rdx
	movq	%rax, %rdi
	movq	%rax, 16(%rsp)
	leaq	LC22(%rip), %rsi
LEHB30:
	call	__ZNSsC1EPKcRKSaIcE
LEHE30:
	leaq	144(%rsp), %r14
	movq	%r12, %rsi
	movq	%r14, %rdi
LEHB31:
	call	__ZNSsC1ERKSs
LEHE31:
	leaq	8(%r14), %rdi
	movq	%rbp, %rsi
LEHB32:
	call	__ZNSsC1ERKSs
LEHE32:
	leaq	16(%r14), %rdi
	movq	%rbx, %rsi
LEHB33:
	call	__ZNSsC1ERKSs
LEHE33:
	leaq	24(%r14), %rdi
	movq	%r13, %rsi
LEHB34:
	call	__ZNSsC1ERKSs
LEHE34:
	leaq	32(%r14), %rdi
	movq	%r15, %rsi
LEHB35:
	call	__ZNSsC1ERKSs
LEHE35:
	leaq	47(%rsp), %rcx
	leaq	40(%r14), %rdi
	movq	%rcx, %rdx
	movq	%rcx, 24(%rsp)
	leaq	LC20(%rip), %rsi
LEHB36:
	call	__ZNSsC1EPKcRKSaIcE
LEHE36:
	movq	16(%rsp), %rsi
	leaq	48(%r14), %rdi
LEHB37:
	call	__ZNSsC1ERKSs
LEHE37:
	movq	152(%rsp), %rax
	movl	$-1, 200(%rsp)
	movl	$0, 204(%rsp)
	movl	-24(%rax), %edx
	subl	$1, %edx
	js	L245
	leaq	8(%r14), %r15
	movslq	%edx, %rbx
	movl	%edx, %edx
	leaq	-1(%rbx), %r12
	leaq	40(%r14), %r13
	subq	%rdx, %r12
	jmp	L246
	.align 4
L250:
	movq	184(%rsp), %rax
	movq	-24(%rax), %rdx
	leaq	1(%rdx), %rbp
	cmpq	-16(%rax), %rbp
	ja	L247
	movl	-8(%rax), %ecx
	testl	%ecx, %ecx
	jle	L317
L247:
	movq	%rbp, %rsi
	movq	%r13, %rdi
LEHB38:
	call	__ZNSs7reserveEm
	movq	184(%rsp), %rax
	movq	-24(%rax), %rdx
L317:
	movb	$32, (%rax,%rdx)
	movq	184(%rsp), %rax
	leaq	-24(%rax), %rdx
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdx
	jne	L339
L248:
	subq	$1, %rbx
	cmpq	%r12, %rbx
	je	L245
	movq	152(%rsp), %rax
L246:
	movl	-8(%rax), %edx
	testl	%edx, %edx
	js	L249
	movq	%r15, %rdi
	call	__ZNSs12_M_leak_hardEv
LEHE38:
	movq	152(%rsp), %rax
L249:
	cmpb	$10, (%rax,%rbx)
	jne	L250
L245:
	movq	8(%rsp), %rsi
	movq	%r14, %rdx
	movq	(%rsp), %rdi
LEHB39:
	call	__ZN5Eigen8internal12print_matrixINS_6MatrixIfLi4ELi4ELi0ELi4ELi4EEEEERSoS4_RKT_RKNS_8IOFormatE
LEHE39:
	movq	192(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L340
L267:
	movq	184(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L341
L270:
	movq	176(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L342
L273:
	movq	168(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L343
L276:
	movq	160(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L344
L279:
	movq	152(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L345
L282:
	movq	144(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L346
L285:
	movq	48(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L347
L288:
	movq	64(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L348
L291:
	movq	80(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L349
L294:
	movq	96(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L350
L297:
	movq	112(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L351
L300:
	movq	128(%rsp), %rdx
	leaq	-24(%rdx), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	jne	L352
L334:
	addq	$216, %rsp
LCFI78:
	popq	%rbx
LCFI79:
	popq	%rbp
LCFI80:
	popq	%r12
LCFI81:
	popq	%r13
LCFI82:
	popq	%r14
LCFI83:
	popq	%r15
LCFI84:
	ret
	.align 4
L339:
LCFI85:
	movl	$0, -8(%rax)
	movq	%rbp, -24(%rax)
	movb	$0, (%rax,%rbp)
	jmp	L248
	.align 4
L340:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L267
	movq	24(%rsp), %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L267
L341:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L270
	movq	24(%rsp), %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L270
L342:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L273
	movq	24(%rsp), %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L273
L343:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L276
	movq	24(%rsp), %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L276
L344:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L279
	movq	24(%rsp), %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L279
L345:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L282
	movq	24(%rsp), %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L282
L346:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L285
	movq	24(%rsp), %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L285
L347:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L288
	movq	%r14, %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L288
L348:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L291
	movq	%r14, %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L291
L349:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L294
	movq	%r14, %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L294
L350:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L297
	movq	%r14, %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L297
L351:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L300
	movq	%r14, %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L300
L352:
	movl	$-1, %ecx
	lock; xaddl	%ecx, -8(%rdx)
	testl	%ecx, %ecx
	jg	L334
	movq	%r14, %rsi
	movq	%rax, (%rsp)
	call	__ZNSs4_Rep10_M_destroyERKSaIcE
	movq	(%rsp), %rax
	jmp	L334
L321:
	movq	%rax, %rbx
	jmp	L313
L320:
	movq	%rax, %rbx
	jmp	L315
L251:
	movq	192(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L253
	movq	24(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L253:
	movq	184(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L255
	movq	24(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L255:
	movq	176(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L257
	movq	24(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L257:
	movq	168(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L259
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L259:
	movq	160(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L261
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L261:
	movq	152(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L263
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L263:
	movq	144(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L265
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L265:
	movq	48(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L307
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L307:
	movq	64(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L309
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L309:
	movq	80(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L311
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L311:
	movq	96(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L313
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L313:
	movq	112(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L315
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L315:
	movq	128(%rsp), %rax
	leaq	-24(%rax), %rdi
	cmpq	__ZNSs4_Rep20_S_empty_rep_storageE@GOTPCREL(%rip), %rdi
	je	L316
	leaq	47(%rsp), %rsi
	call	__ZNSs4_Rep10_M_disposeERKSaIcE.part.7
L316:
	movq	%rbx, %rdi
LEHB40:
	call	__Unwind_Resume
LEHE40:
L332:
	movq	%rax, %rbx
	jmp	L253
L331:
	movq	%rax, %rbx
	jmp	L255
L330:
	movq	%rax, %rbx
	jmp	L257
L329:
	movq	%rax, %rbx
	jmp	L259
L328:
	movq	%rax, %rbx
	jmp	L261
L327:
	movq	%rax, %rbx
	jmp	L263
L326:
	movq	%rax, %rbx
	jmp	L305
L333:
	movq	%rax, %rbx
	jmp	L251
L305:
	movq	%r14, %rdi
	call	__ZN5Eigen8IOFormatD1Ev
	jmp	L265
L323:
	movq	%rax, %rbx
	jmp	L309
L322:
	movq	%rax, %rbx
	jmp	L311
L325:
	movq	%rax, %rbx
	jmp	L265
L324:
	movq	%rax, %rbx
	jmp	L307
LFE9755:
	.section __DATA,__gcc_except_tab
GCC_except_table4:
LLSDA9755:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0xd0,0x1
	.set L$set$66,LEHB25-LFB9755
	.long L$set$66
	.set L$set$67,LEHE25-LEHB25
	.long L$set$67
	.long	0
	.byte	0
	.set L$set$68,LEHB26-LFB9755
	.long L$set$68
	.set L$set$69,LEHE26-LEHB26
	.long L$set$69
	.set L$set$70,L320-LFB9755
	.long L$set$70
	.byte	0
	.set L$set$71,LEHB27-LFB9755
	.long L$set$71
	.set L$set$72,LEHE27-LEHB27
	.long L$set$72
	.set L$set$73,L321-LFB9755
	.long L$set$73
	.byte	0
	.set L$set$74,LEHB28-LFB9755
	.long L$set$74
	.set L$set$75,LEHE28-LEHB28
	.long L$set$75
	.set L$set$76,L322-LFB9755
	.long L$set$76
	.byte	0
	.set L$set$77,LEHB29-LFB9755
	.long L$set$77
	.set L$set$78,LEHE29-LEHB29
	.long L$set$78
	.set L$set$79,L323-LFB9755
	.long L$set$79
	.byte	0
	.set L$set$80,LEHB30-LFB9755
	.long L$set$80
	.set L$set$81,LEHE30-LEHB30
	.long L$set$81
	.set L$set$82,L324-LFB9755
	.long L$set$82
	.byte	0
	.set L$set$83,LEHB31-LFB9755
	.long L$set$83
	.set L$set$84,LEHE31-LEHB31
	.long L$set$84
	.set L$set$85,L325-LFB9755
	.long L$set$85
	.byte	0
	.set L$set$86,LEHB32-LFB9755
	.long L$set$86
	.set L$set$87,LEHE32-LEHB32
	.long L$set$87
	.set L$set$88,L327-LFB9755
	.long L$set$88
	.byte	0
	.set L$set$89,LEHB33-LFB9755
	.long L$set$89
	.set L$set$90,LEHE33-LEHB33
	.long L$set$90
	.set L$set$91,L328-LFB9755
	.long L$set$91
	.byte	0
	.set L$set$92,LEHB34-LFB9755
	.long L$set$92
	.set L$set$93,LEHE34-LEHB34
	.long L$set$93
	.set L$set$94,L329-LFB9755
	.long L$set$94
	.byte	0
	.set L$set$95,LEHB35-LFB9755
	.long L$set$95
	.set L$set$96,LEHE35-LEHB35
	.long L$set$96
	.set L$set$97,L330-LFB9755
	.long L$set$97
	.byte	0
	.set L$set$98,LEHB36-LFB9755
	.long L$set$98
	.set L$set$99,LEHE36-LEHB36
	.long L$set$99
	.set L$set$100,L331-LFB9755
	.long L$set$100
	.byte	0
	.set L$set$101,LEHB37-LFB9755
	.long L$set$101
	.set L$set$102,LEHE37-LEHB37
	.long L$set$102
	.set L$set$103,L332-LFB9755
	.long L$set$103
	.byte	0
	.set L$set$104,LEHB38-LFB9755
	.long L$set$104
	.set L$set$105,LEHE38-LEHB38
	.long L$set$105
	.set L$set$106,L333-LFB9755
	.long L$set$106
	.byte	0
	.set L$set$107,LEHB39-LFB9755
	.long L$set$107
	.set L$set$108,LEHE39-LEHB39
	.long L$set$108
	.set L$set$109,L326-LFB9755
	.long L$set$109
	.byte	0
	.set L$set$110,LEHB40-LFB9755
	.long L$set$110
	.set L$set$111,LEHE40-LEHB40
	.long L$set$111
	.long	0
	.byte	0
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE23:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE23:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB24:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB24:
	.align 1
	.align 4
	.globl __ZN5Eigen9TransformIfLi3ELi1ELi0EEaSINS_9AngleAxisIfEEEERS1_RKNS_12RotationBaseIT_Li3EEE
	.weak_definition __ZN5Eigen9TransformIfLi3ELi1ELi0EEaSINS_9AngleAxisIfEEEERS1_RKNS_12RotationBaseIT_Li3EEE
__ZN5Eigen9TransformIfLi3ELi1ELi0EEaSINS_9AngleAxisIfEEEERS1_RKNS_12RotationBaseIT_Li3EEE:
LFB10118:
	pushq	%rbp
LCFI86:
	pxor	%xmm2, %xmm2
	movq	%rsi, %rbp
	pushq	%rbx
LCFI87:
	movq	%rdi, %rbx
	subq	$40, %rsp
LCFI88:
	movss	12(%rsi), %xmm0
	movss	%xmm2, 24(%rsp)
	movss	%xmm0, 28(%rsp)
	movq	24(%rsp), %xmm0
	movss	%xmm2, 12(%rsp)
	call	_cexpf
	movss	12(%rsp), %xmm2
	movq	%rbx, %rax
	movss	LC12(%rip), %xmm8
	movq	%xmm0, 16(%rsp)
	movss	16(%rsp), %xmm6
	movaps	%xmm8, %xmm0
	movss	0(%rbp), %xmm14
	subss	%xmm6, %xmm0
	movss	8(%rbp), %xmm5
	movaps	%xmm14, %xmm1
	movss	4(%rbp), %xmm12
	movss	%xmm2, 48(%rbx)
	movaps	%xmm5, %xmm9
	movss	20(%rsp), %xmm4
	movaps	%xmm5, %xmm7
	movss	%xmm2, 52(%rbx)
	mulss	%xmm0, %xmm1
	movaps	%xmm12, %xmm13
	movaps	%xmm12, %xmm3
	movss	%xmm2, 56(%rbx)
	movaps	%xmm4, %xmm10
	movaps	%xmm4, %xmm11
	movss	%xmm2, 12(%rbx)
	mulss	%xmm5, %xmm4
	movss	%xmm2, 28(%rbx)
	mulss	%xmm0, %xmm3
	movss	%xmm2, 44(%rbx)
	mulss	%xmm1, %xmm13
	movss	%xmm8, 60(%rbx)
	mulss	%xmm1, %xmm9
	mulss	%xmm14, %xmm1
	mulss	%xmm5, %xmm0
	mulss	%xmm12, %xmm11
	mulss	%xmm14, %xmm10
	addss	%xmm6, %xmm1
	mulss	%xmm3, %xmm7
	mulss	%xmm0, %xmm5
	mulss	%xmm12, %xmm3
	movss	%xmm1, (%rbx)
	movaps	%xmm4, %xmm1
	addss	%xmm13, %xmm1
	movaps	%xmm5, %xmm0
	subss	%xmm4, %xmm13
	addss	%xmm6, %xmm3
	movss	%xmm1, 4(%rbx)
	movaps	%xmm9, %xmm1
	addss	%xmm6, %xmm0
	subss	%xmm11, %xmm1
	movss	%xmm13, 16(%rbx)
	addss	%xmm11, %xmm9
	movss	%xmm3, 20(%rbx)
	movss	%xmm0, 40(%rbx)
	movss	%xmm1, 8(%rbx)
	movaps	%xmm10, %xmm1
	addss	%xmm7, %xmm1
	movss	%xmm9, 32(%rbx)
	subss	%xmm10, %xmm7
	movss	%xmm1, 24(%rbx)
	movss	%xmm7, 36(%rbx)
	addq	$40, %rsp
LCFI89:
	popq	%rbx
LCFI90:
	popq	%rbp
LCFI91:
	ret
LFE10118:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE24:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE24:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
	.align 1
LCOLDB25:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTB25:
	.align 1
	.align 4
	.globl __ZN5Eigen9TransformIfLi3ELi1ELi0EE9translateINS_6MatrixIfLi3ELi1ELi0ELi3ELi1EEEEERS1_RKNS_10MatrixBaseIT_EE
	.weak_definition __ZN5Eigen9TransformIfLi3ELi1ELi0EE9translateINS_6MatrixIfLi3ELi1ELi0ELi3ELi1EEEEERS1_RKNS_10MatrixBaseIT_EE
__ZN5Eigen9TransformIfLi3ELi1ELi0EE9translateINS_6MatrixIfLi3ELi1ELi0ELi3ELi1EEEEERS1_RKNS_10MatrixBaseIT_EE:
LFB10120:
	movss	(%rsi), %xmm4
	movq	%rdi, %rax
	movss	4(%rsi), %xmm0
	movss	(%rdi), %xmm2
	movss	16(%rdi), %xmm1
	mulss	%xmm4, %xmm2
	movss	4(%rdi), %xmm5
	mulss	%xmm0, %xmm1
	movss	8(%rsi), %xmm3
	mulss	%xmm4, %xmm5
	mulss	8(%rdi), %xmm4
	addss	%xmm1, %xmm2
	movss	20(%rdi), %xmm1
	mulss	%xmm0, %xmm1
	mulss	24(%rdi), %xmm0
	addss	%xmm5, %xmm1
	movss	36(%rdi), %xmm5
	addss	%xmm4, %xmm0
	movss	40(%rdi), %xmm4
	mulss	%xmm3, %xmm5
	mulss	%xmm3, %xmm4
	mulss	32(%rdi), %xmm3
	addss	%xmm5, %xmm1
	addss	%xmm4, %xmm0
	addss	%xmm3, %xmm2
	addss	52(%rdi), %xmm1
	addss	48(%rdi), %xmm2
	addss	56(%rdi), %xmm0
	movss	%xmm1, 52(%rdi)
	movss	%xmm2, 48(%rdi)
	movss	%xmm0, 56(%rdi)
	ret
LFE10120:
	.section __TEXT,__text_cold_coal,coalesced,pure_instructions
LCOLDE25:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
LHOTE25:
	.cstring
LC26:
	.ascii "Size: \0"
LC27:
	.ascii "Imported point cloud.\0"
LC30:
	.ascii ", \0"
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB31:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTB31:
	.align 4
	.globl _main
_main:
LFB9596:
	pushq	%r14
LCFI92:
	movl	$10, %edx
	pushq	%r13
LCFI93:
	pushq	%r12
LCFI94:
	pushq	%rbp
LCFI95:
	pushq	%rbx
LCFI96:
	subq	$1456, %rsp
LCFI97:
	movq	8(%rsi), %rsi
	leaq	208(%rsp), %r13
	movq	%r13, %rdi
LEHB41:
	call	__ZN3pcf10ply_readerC1EPKcc
LEHE41:
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	leaq	LC26(%rip), %rsi
	movq	1256(%rsp), %rbx
LEHB42:
	call	__ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	__ZNSo9_M_insertImEERSoT_
	movq	%rax, %rdi
	call	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
	leaq	48(%rsp), %rdi
	movq	%rbx, %rsi
	call	__ZN3pcf17point_cloud_allocINS_9point_xyzEEC1Em
LEHE42:
	movq	56(%rsp), %rsi
	movq	%rbx, %rdx
	movq	%r13, %rdi
LEHB43:
	call	__ZN3pcf10ply_reader4readINS_9point_xyzEEEvPT_m
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	leaq	LC27(%rip), %rsi
	call	__ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%rax, %rdi
	call	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
	movaps	LC29(%rip), %xmm0
	leaq	144(%rsp), %r12
	movl	$0x3dcccccd, 16(%rsp)
	leaq	32(%rsp), %rbp
	movq	%r12, %rdi
	movl	$0x3dcccccd, 20(%rsp)
	movaps	%xmm0, 32(%rsp)
	leaq	80(%rsp), %r14
	movq	%rbp, %rsi
	movl	$0x3dcccccd, 24(%rsp)
	leaq	16(%rsp), %rbx
	call	__ZN5Eigen9TransformIfLi3ELi1ELi0EEaSINS_9AngleAxisIfEEEERS1_RKNS_12RotationBaseIT_Li3EEE
	movaps	144(%rsp), %xmm0
	movq	%rbx, %rsi
	movq	%r14, %rdi
	movaps	%xmm0, 80(%rsp)
	movaps	160(%rsp), %xmm0
	movaps	%xmm0, 96(%rsp)
	movaps	176(%rsp), %xmm0
	movaps	%xmm0, 112(%rsp)
	movaps	192(%rsp), %xmm0
	movaps	%xmm0, 128(%rsp)
	call	__ZN5Eigen9TransformIfLi3ELi1ELi0EE9translateINS_6MatrixIfLi3ELi1ELi0ELi3ELi1EEEEERS1_RKNS_10MatrixBaseIT_EE
	movq	%r14, %rsi
	movq	%r12, %rdi
	call	__ZN5Eigen9TransformIfLi3ELi2ELi0EEC1ILi1ELi0EEERKNS0_IfLi3EXT_EXT0_EEE
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	movq	%r12, %rsi
	call	__ZN5EigenlsINS_6MatrixIfLi4ELi4ELi0ELi4ELi4EEEEERSoS3_RKNS_9DenseBaseIT_EE
	movq	%rax, %rdi
	call	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
	movq	56(%rsp), %rax
	movq	__ZSt4cout@GOTPCREL(%rip), %r12
	.align 4
L364:
	movss	24(%rax), %xmm6
	movl	$1, %edx
	movq	%rbp, %rsi
	movq	%r12, %rdi
	movss	20(%rax), %xmm7
	movss	%xmm6, 4(%rsp)
	movss	16(%rax), %xmm6
	movss	%xmm7, 8(%rsp)
	movss	%xmm6, 12(%rsp)
	movb	$40, 32(%rsp)
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	%rax, %rdi
	pxor	%xmm0, %xmm0
	cvtss2sd	12(%rsp), %xmm0
	call	__ZNSo9_M_insertIdEERSoT_
	movl	$2, %edx
	movq	%rax, %rdi
	movq	%rax, %r14
	leaq	LC30(%rip), %rsi
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	%r14, %rdi
	pxor	%xmm0, %xmm0
	cvtss2sd	8(%rsp), %xmm0
	call	__ZNSo9_M_insertIdEERSoT_
	movl	$2, %edx
	movq	%rax, %rdi
	movq	%rax, %r14
	leaq	LC30(%rip), %rsi
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	%r14, %rdi
	pxor	%xmm0, %xmm0
	cvtss2sd	4(%rsp), %xmm0
	call	__ZNSo9_M_insertIdEERSoT_
	movl	$1, %edx
	movq	%rbx, %rsi
	movq	%rax, %rdi
	movb	$41, 16(%rsp)
	call	__ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	__ZSt4cout@GOTPCREL(%rip), %rax
	movq	(%rax), %rdx
	movq	-24(%rdx), %rdx
	movq	240(%rdx,%rax), %r14
	testq	%r14, %r14
	je	L377
	cmpb	$0, 56(%r14)
	je	L358
	movsbl	67(%r14), %esi
L359:
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	call	__ZNSo3putEc
	movq	%rax, %rdi
	call	__ZNSo5flushEv
# 50 "src/point_cloud.h" 1
	# TRANSFORMATION
# 0 "" 2
	movq	56(%rsp), %rcx
	movq	64(%rsp), %rdx
	movq	%rcx, %rax
	cmpq	%rdx, %rcx
	jae	L361
	.align 4
L373:
	movss	(%rax), %xmm1
	addq	$16, %rax
	movss	-12(%rax), %xmm0
	movss	144(%rsp), %xmm3
	movss	160(%rsp), %xmm2
	movss	176(%rsp), %xmm4
	mulss	%xmm1, %xmm3
	movss	192(%rsp), %xmm5
	mulss	%xmm0, %xmm2
	addss	%xmm3, %xmm2
	movss	-8(%rax), %xmm3
	mulss	%xmm3, %xmm4
	addss	%xmm4, %xmm2
	movss	-4(%rax), %xmm4
	mulss	%xmm4, %xmm5
	movss	%xmm4, 92(%rsp)
	addss	%xmm5, %xmm2
	movss	148(%rsp), %xmm5
	mulss	%xmm1, %xmm5
	mulss	152(%rsp), %xmm1
	movss	%xmm2, 80(%rsp)
	movss	164(%rsp), %xmm2
	mulss	%xmm0, %xmm2
	mulss	168(%rsp), %xmm0
	addss	%xmm5, %xmm2
	movss	180(%rsp), %xmm5
	addss	%xmm0, %xmm1
	mulss	%xmm3, %xmm5
	mulss	184(%rsp), %xmm3
	addss	%xmm5, %xmm2
	movss	196(%rsp), %xmm5
	movaps	%xmm3, %xmm0
	addss	%xmm1, %xmm0
	mulss	%xmm4, %xmm5
	movss	200(%rsp), %xmm1
	mulss	%xmm4, %xmm1
	addss	%xmm5, %xmm2
	addss	%xmm1, %xmm0
	movss	%xmm2, 84(%rsp)
	movss	%xmm0, 88(%rsp)
	movaps	80(%rsp), %xmm0
	movaps	%xmm0, -16(%rax)
	cmpq	%rdx, %rax
	jb	L373
L361:
# 53 "src/point_cloud.h" 1
	# /TRANSFORMATION
# 0 "" 2
	movq	%rcx, %rax
	jmp	L364
	.align 4
L358:
	movq	%r14, %rdi
	call	__ZNKSt5ctypeIcE13_M_widen_initEv
	movq	(%r14), %rax
	movl	$10, %esi
	movq	48(%rax), %rax
	cmpq	__ZNKSt5ctypeIcE8do_widenEc@GOTPCREL(%rip), %rax
	je	L359
	movq	%r14, %rdi
	call	*%rax
	movsbl	%al, %esi
	jmp	L359
L377:
	call	__ZSt16__throw_bad_castv
LEHE43:
L370:
	movq	%rax, %rbx
	movq	56(%rsp), %rdi
	movq	__ZTVN3pcf17point_cloud_allocINS_9point_xyzEEE@GOTPCREL(%rip), %rax
	addq	$16, %rax
	testq	%rdi, %rdi
	movq	%rax, 48(%rsp)
	je	L367
	call	__ZdaPv
L367:
	leaq	8(%r13), %rdi
	call	__ZNSt14basic_ifstreamIcSt11char_traitsIcEED1Ev
	movq	%rbx, %rdi
LEHB44:
	call	__Unwind_Resume
LEHE44:
L369:
	movq	%rax, %rbx
	jmp	L367
LFE9596:
	.section __DATA,__gcc_except_tab
GCC_except_table5:
LLSDA9596:
	.byte	0xff
	.byte	0xff
	.byte	0x3
	.byte	0x34
	.set L$set$112,LEHB41-LFB9596
	.long L$set$112
	.set L$set$113,LEHE41-LEHB41
	.long L$set$113
	.long	0
	.byte	0
	.set L$set$114,LEHB42-LFB9596
	.long L$set$114
	.set L$set$115,LEHE42-LEHB42
	.long L$set$115
	.set L$set$116,L369-LFB9596
	.long L$set$116
	.byte	0
	.set L$set$117,LEHB43-LFB9596
	.long L$set$117
	.set L$set$118,LEHE43-LEHB43
	.long L$set$118
	.set L$set$119,L370-LFB9596
	.long L$set$119
	.byte	0
	.set L$set$120,LEHB44-LFB9596
	.long L$set$120
	.set L$set$121,LEHE44-LEHB44
	.long L$set$121
	.long	0
	.byte	0
	.section __TEXT,__text_startup,regular,pure_instructions
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE31:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTE31:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDB32:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTB32:
	.align 4
__GLOBAL__sub_I_main.cc:
LFB11625:
	leaq	__ZStL8__ioinit(%rip), %rdi
	subq	$8, %rsp
LCFI98:
	call	__ZNSt8ios_base4InitC1Ev
	movq	__ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	leaq	___dso_handle(%rip), %rdx
	addq	$8, %rsp
LCFI99:
	leaq	__ZStL8__ioinit(%rip), %rsi
	jmp	___cxa_atexit
LFE11625:
	.section __TEXT,__text_cold,regular,pure_instructions
LCOLDE32:
	.section __TEXT,__text_startup,regular,pure_instructions
LHOTE32:
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
	.globl __ZTIN3pcf11point_cloudINS_9point_xyzEEE
	.weak_definition __ZTIN3pcf11point_cloudINS_9point_xyzEEE
	.section __DATA,__const_coal,coalesced
	.align 4
__ZTIN3pcf11point_cloudINS_9point_xyzEEE:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN3pcf11point_cloudINS_9point_xyzEEE
	.globl __ZTSN3pcf11point_cloudINS_9point_xyzEEE
	.weak_definition __ZTSN3pcf11point_cloudINS_9point_xyzEEE
	.section __TEXT,__const_coal,coalesced
	.align 5
__ZTSN3pcf11point_cloudINS_9point_xyzEEE:
	.ascii "N3pcf11point_cloudINS_9point_xyzEEE\0"
	.globl __ZTIN3pcf17point_cloud_allocINS_9point_xyzEEE
	.weak_definition __ZTIN3pcf17point_cloud_allocINS_9point_xyzEEE
	.section __DATA,__const_coal,coalesced
	.align 4
__ZTIN3pcf17point_cloud_allocINS_9point_xyzEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSN3pcf17point_cloud_allocINS_9point_xyzEEE
	.quad	__ZTIN3pcf11point_cloudINS_9point_xyzEEE
	.globl __ZTSN3pcf17point_cloud_allocINS_9point_xyzEEE
	.weak_definition __ZTSN3pcf17point_cloud_allocINS_9point_xyzEEE
	.section __TEXT,__const_coal,coalesced
	.align 5
__ZTSN3pcf17point_cloud_allocINS_9point_xyzEEE:
	.ascii "N3pcf17point_cloud_allocINS_9point_xyzEEE\0"
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
	.globl __ZTVN3pcf11point_cloudINS_9point_xyzEEE
	.weak_definition __ZTVN3pcf11point_cloudINS_9point_xyzEEE
	.align 5
__ZTVN3pcf11point_cloudINS_9point_xyzEEE:
	.quad	0
	.quad	__ZTIN3pcf11point_cloudINS_9point_xyzEEE
	.quad	__ZN3pcf11point_cloudINS_9point_xyzEED1Ev
	.quad	__ZN3pcf11point_cloudINS_9point_xyzEED0Ev
	.globl __ZTVN3pcf17point_cloud_allocINS_9point_xyzEEE
	.weak_definition __ZTVN3pcf17point_cloud_allocINS_9point_xyzEEE
	.align 5
__ZTVN3pcf17point_cloud_allocINS_9point_xyzEEE:
	.quad	0
	.quad	__ZTIN3pcf17point_cloud_allocINS_9point_xyzEEE
	.quad	__ZN3pcf17point_cloud_allocINS_9point_xyzEED1Ev
	.quad	__ZN3pcf17point_cloud_allocINS_9point_xyzEED0Ev
	.static_data
__ZStL8__ioinit:
	.space	1
	.literal16
	.align 4
LC8:
	.long	0
	.long	0
	.long	0
	.long	1065353216
	.literal4
	.align 2
LC12:
	.long	1065353216
	.literal16
	.align 4
LC29:
	.long	1065353216
	.long	0
	.long	0
	.long	1023454717
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$122,LECIE1-LSCIE1
	.long L$set$122
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
	.set L$set$123,LEFDE1-LASFDE1
	.long L$set$123
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB1067-.
	.set L$set$124,LFE1067-LFB1067
	.quad L$set$124
	.byte	0x8
	.quad	0
	.align 3
LEFDE1:
LSFDE3:
	.set L$set$125,LEFDE3-LASFDE3
	.long L$set$125
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB9857-.
	.set L$set$126,LFE9857-LFB9857
	.quad L$set$126
	.byte	0x8
	.quad	0
	.align 3
LEFDE3:
LSFDE5:
	.set L$set$127,LEFDE5-LASFDE5
	.long L$set$127
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB9708-.
	.set L$set$128,LFE9708-LFB9708
	.quad L$set$128
	.byte	0x8
	.quad	0
	.align 3
LEFDE5:
LSFDE7:
	.set L$set$129,LEFDE7-LASFDE7
	.long L$set$129
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB9858-.
	.set L$set$130,LFE9858-LFB9858
	.quad L$set$130
	.byte	0x8
	.quad	0
	.align 3
LEFDE7:
LSFDE9:
	.set L$set$131,LEFDE9-LASFDE9
	.long L$set$131
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB9709-.
	.set L$set$132,LFE9709-LFB9709
	.quad L$set$132
	.byte	0x8
	.quad	0
	.byte	0x4
	.set L$set$133,LCFI0-LFB9709
	.long L$set$133
	.byte	0xe
	.byte	0x10
	.byte	0x83
	.byte	0x2
	.byte	0x4
	.set L$set$134,LCFI1-LCFI0
	.long L$set$134
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE9:
LSFDE11:
	.set L$set$135,LEFDE11-LASFDE11
	.long L$set$135
LASFDE11:
	.long	LASFDE11-EH_frame1
	.quad	LFB9718-.
	.set L$set$136,LFE9718-LFB9718
	.quad L$set$136
	.byte	0x8
	.quad	0
	.byte	0x4
	.set L$set$137,LCFI2-LFB9718
	.long L$set$137
	.byte	0xe
	.byte	0x10
	.byte	0x83
	.byte	0x2
	.byte	0x4
	.set L$set$138,LCFI3-LCFI2
	.long L$set$138
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE11:
LSFDE13:
	.set L$set$139,LEFDE13-LASFDE13
	.long L$set$139
LASFDE13:
	.long	LASFDE13-EH_frame1
	.quad	LFB9717-.
	.set L$set$140,LFE9717-LFB9717
	.quad L$set$140
	.byte	0x8
	.quad	0
	.align 3
LEFDE13:
LSFDE15:
	.set L$set$141,LEFDE15-LASFDE15
	.long L$set$141
LASFDE15:
	.long	LASFDE15-EH_frame1
	.quad	LFB11633-.
	.set L$set$142,LFE11633-LFB11633
	.quad L$set$142
	.byte	0x8
	.quad	0
	.align 3
LEFDE15:
LSFDE17:
	.set L$set$143,LEFDE17-LASFDE17
	.long L$set$143
LASFDE17:
	.long	LASFDE17-EH_frame1
	.quad	LFB9705-.
	.set L$set$144,LFE9705-LFB9705
	.quad L$set$144
	.byte	0x8
	.quad	LLSDA9705-.
	.byte	0x4
	.set L$set$145,LCFI4-LFB9705
	.long L$set$145
	.byte	0xe
	.byte	0x10
	.byte	0x8c
	.byte	0x2
	.byte	0x4
	.set L$set$146,LCFI5-LCFI4
	.long L$set$146
	.byte	0xe
	.byte	0x18
	.byte	0x86
	.byte	0x3
	.byte	0x4
	.set L$set$147,LCFI6-LCFI5
	.long L$set$147
	.byte	0xe
	.byte	0x20
	.byte	0x83
	.byte	0x4
	.byte	0x4
	.set L$set$148,LCFI7-LCFI6
	.long L$set$148
	.byte	0xe
	.byte	0x40
	.byte	0x4
	.set L$set$149,LCFI8-LCFI7
	.long L$set$149
	.byte	0xa
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$150,LCFI9-LCFI8
	.long L$set$150
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$151,LCFI10-LCFI9
	.long L$set$151
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$152,LCFI11-LCFI10
	.long L$set$152
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$153,LCFI12-LCFI11
	.long L$set$153
	.byte	0xb
	.align 3
LEFDE17:
LSFDE19:
	.set L$set$154,LEFDE19-LASFDE19
	.long L$set$154
LASFDE19:
	.long	LASFDE19-EH_frame1
	.quad	LFB9750-.
	.set L$set$155,LFE9750-LFB9750
	.quad L$set$155
	.byte	0x8
	.quad	0
	.align 3
LEFDE19:
LSFDE21:
	.set L$set$156,LEFDE21-LASFDE21
	.long L$set$156
LASFDE21:
	.long	LASFDE21-EH_frame1
	.quad	LFB9758-.
	.set L$set$157,LFE9758-LFB9758
	.quad L$set$157
	.byte	0x8
	.quad	0
	.byte	0x4
	.set L$set$158,LCFI13-LFB9758
	.long L$set$158
	.byte	0xe
	.byte	0x10
	.byte	0x83
	.byte	0x2
	.byte	0x4
	.set L$set$159,LCFI14-LCFI13
	.long L$set$159
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$160,LCFI15-LCFI14
	.long L$set$160
	.byte	0xa
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$161,LCFI16-LCFI15
	.long L$set$161
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$162,LCFI17-LCFI16
	.long L$set$162
	.byte	0xb
	.align 3
LEFDE21:
LSFDE23:
	.set L$set$163,LEFDE23-LASFDE23
	.long L$set$163
LASFDE23:
	.long	LASFDE23-EH_frame1
	.quad	LFB9859-.
	.set L$set$164,LFE9859-LFB9859
	.quad L$set$164
	.byte	0x8
	.quad	LLSDA9859-.
	.byte	0x4
	.set L$set$165,LCFI18-LFB9859
	.long L$set$165
	.byte	0xe
	.byte	0x10
	.byte	0x8f
	.byte	0x2
	.byte	0x4
	.set L$set$166,LCFI19-LCFI18
	.long L$set$166
	.byte	0xe
	.byte	0x18
	.byte	0x8e
	.byte	0x3
	.byte	0x4
	.set L$set$167,LCFI20-LCFI19
	.long L$set$167
	.byte	0xe
	.byte	0x20
	.byte	0x8d
	.byte	0x4
	.byte	0x4
	.set L$set$168,LCFI21-LCFI20
	.long L$set$168
	.byte	0xe
	.byte	0x28
	.byte	0x8c
	.byte	0x5
	.byte	0x4
	.set L$set$169,LCFI22-LCFI21
	.long L$set$169
	.byte	0xe
	.byte	0x30
	.byte	0x86
	.byte	0x6
	.byte	0x4
	.set L$set$170,LCFI23-LCFI22
	.long L$set$170
	.byte	0xe
	.byte	0x38
	.byte	0x83
	.byte	0x7
	.byte	0x4
	.set L$set$171,LCFI24-LCFI23
	.long L$set$171
	.byte	0xe
	.byte	0x80,0x1
	.byte	0x4
	.set L$set$172,LCFI25-LCFI24
	.long L$set$172
	.byte	0xa
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$173,LCFI26-LCFI25
	.long L$set$173
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$174,LCFI27-LCFI26
	.long L$set$174
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$175,LCFI28-LCFI27
	.long L$set$175
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$176,LCFI29-LCFI28
	.long L$set$176
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$177,LCFI30-LCFI29
	.long L$set$177
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$178,LCFI31-LCFI30
	.long L$set$178
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$179,LCFI32-LCFI31
	.long L$set$179
	.byte	0xb
	.byte	0x4
	.set L$set$180,LCFI33-LCFI32
	.long L$set$180
	.byte	0xa
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$181,LCFI34-LCFI33
	.long L$set$181
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$182,LCFI35-LCFI34
	.long L$set$182
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$183,LCFI36-LCFI35
	.long L$set$183
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$184,LCFI37-LCFI36
	.long L$set$184
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$185,LCFI38-LCFI37
	.long L$set$185
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$186,LCFI39-LCFI38
	.long L$set$186
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$187,LCFI40-LCFI39
	.long L$set$187
	.byte	0xb
	.align 3
LEFDE23:
LSFDE25:
	.set L$set$188,LEFDE25-LASFDE25
	.long L$set$188
LASFDE25:
	.long	LASFDE25-EH_frame1
	.quad	LFB9711-.
	.set L$set$189,LFE9711-LFB9711
	.quad L$set$189
	.byte	0x8
	.quad	LLSDA9711-.
	.byte	0x4
	.set L$set$190,LCFI41-LFB9711
	.long L$set$190
	.byte	0xe
	.byte	0x10
	.byte	0x8f
	.byte	0x2
	.byte	0x4
	.set L$set$191,LCFI42-LCFI41
	.long L$set$191
	.byte	0xe
	.byte	0x18
	.byte	0x8e
	.byte	0x3
	.byte	0x4
	.set L$set$192,LCFI43-LCFI42
	.long L$set$192
	.byte	0xe
	.byte	0x20
	.byte	0x8d
	.byte	0x4
	.byte	0x4
	.set L$set$193,LCFI44-LCFI43
	.long L$set$193
	.byte	0xe
	.byte	0x28
	.byte	0x8c
	.byte	0x5
	.byte	0x4
	.set L$set$194,LCFI45-LCFI44
	.long L$set$194
	.byte	0xe
	.byte	0x30
	.byte	0x86
	.byte	0x6
	.byte	0x4
	.set L$set$195,LCFI46-LCFI45
	.long L$set$195
	.byte	0xe
	.byte	0x38
	.byte	0x83
	.byte	0x7
	.byte	0x4
	.set L$set$196,LCFI47-LCFI46
	.long L$set$196
	.byte	0xe
	.byte	0x60
	.byte	0x4
	.set L$set$197,LCFI48-LCFI47
	.long L$set$197
	.byte	0xa
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$198,LCFI49-LCFI48
	.long L$set$198
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$199,LCFI50-LCFI49
	.long L$set$199
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$200,LCFI51-LCFI50
	.long L$set$200
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$201,LCFI52-LCFI51
	.long L$set$201
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$202,LCFI53-LCFI52
	.long L$set$202
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$203,LCFI54-LCFI53
	.long L$set$203
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$204,LCFI55-LCFI54
	.long L$set$204
	.byte	0xb
	.align 3
LEFDE25:
LSFDE27:
	.set L$set$205,LEFDE27-LASFDE27
	.long L$set$205
LASFDE27:
	.long	LASFDE27-EH_frame1
	.quad	LFB9986-.
	.set L$set$206,LFE9986-LFB9986
	.quad L$set$206
	.byte	0x8
	.quad	LLSDA9986-.
	.byte	0x4
	.set L$set$207,LCFI56-LFB9986
	.long L$set$207
	.byte	0xe
	.byte	0x10
	.byte	0x8f
	.byte	0x2
	.byte	0x4
	.set L$set$208,LCFI57-LCFI56
	.long L$set$208
	.byte	0xe
	.byte	0x18
	.byte	0x8e
	.byte	0x3
	.byte	0x4
	.set L$set$209,LCFI58-LCFI57
	.long L$set$209
	.byte	0xe
	.byte	0x20
	.byte	0x8d
	.byte	0x4
	.byte	0x4
	.set L$set$210,LCFI59-LCFI58
	.long L$set$210
	.byte	0xe
	.byte	0x28
	.byte	0x8c
	.byte	0x5
	.byte	0x4
	.set L$set$211,LCFI60-LCFI59
	.long L$set$211
	.byte	0xe
	.byte	0x30
	.byte	0x86
	.byte	0x6
	.byte	0x4
	.set L$set$212,LCFI61-LCFI60
	.long L$set$212
	.byte	0xe
	.byte	0x38
	.byte	0x83
	.byte	0x7
	.byte	0x4
	.set L$set$213,LCFI62-LCFI61
	.long L$set$213
	.byte	0xe
	.byte	0xf0,0x4
	.byte	0x4
	.set L$set$214,LCFI63-LCFI62
	.long L$set$214
	.byte	0xa
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$215,LCFI64-LCFI63
	.long L$set$215
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$216,LCFI65-LCFI64
	.long L$set$216
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$217,LCFI66-LCFI65
	.long L$set$217
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$218,LCFI67-LCFI66
	.long L$set$218
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$219,LCFI68-LCFI67
	.long L$set$219
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$220,LCFI69-LCFI68
	.long L$set$220
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$221,LCFI70-LCFI69
	.long L$set$221
	.byte	0xb
	.align 3
LEFDE27:
LSFDE29:
	.set L$set$222,LEFDE29-LASFDE29
	.long L$set$222
LASFDE29:
	.long	LASFDE29-EH_frame1
	.quad	LFB9755-.
	.set L$set$223,LFE9755-LFB9755
	.quad L$set$223
	.byte	0x8
	.quad	LLSDA9755-.
	.byte	0x4
	.set L$set$224,LCFI71-LFB9755
	.long L$set$224
	.byte	0xe
	.byte	0x10
	.byte	0x8f
	.byte	0x2
	.byte	0x4
	.set L$set$225,LCFI72-LCFI71
	.long L$set$225
	.byte	0xe
	.byte	0x18
	.byte	0x8e
	.byte	0x3
	.byte	0x4
	.set L$set$226,LCFI73-LCFI72
	.long L$set$226
	.byte	0xe
	.byte	0x20
	.byte	0x8d
	.byte	0x4
	.byte	0x4
	.set L$set$227,LCFI74-LCFI73
	.long L$set$227
	.byte	0xe
	.byte	0x28
	.byte	0x8c
	.byte	0x5
	.byte	0x4
	.set L$set$228,LCFI75-LCFI74
	.long L$set$228
	.byte	0xe
	.byte	0x30
	.byte	0x86
	.byte	0x6
	.byte	0x4
	.set L$set$229,LCFI76-LCFI75
	.long L$set$229
	.byte	0xe
	.byte	0x38
	.byte	0x83
	.byte	0x7
	.byte	0x4
	.set L$set$230,LCFI77-LCFI76
	.long L$set$230
	.byte	0xe
	.byte	0x90,0x2
	.byte	0x4
	.set L$set$231,LCFI78-LCFI77
	.long L$set$231
	.byte	0xa
	.byte	0xe
	.byte	0x38
	.byte	0x4
	.set L$set$232,LCFI79-LCFI78
	.long L$set$232
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.set L$set$233,LCFI80-LCFI79
	.long L$set$233
	.byte	0xe
	.byte	0x28
	.byte	0x4
	.set L$set$234,LCFI81-LCFI80
	.long L$set$234
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.set L$set$235,LCFI82-LCFI81
	.long L$set$235
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$236,LCFI83-LCFI82
	.long L$set$236
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$237,LCFI84-LCFI83
	.long L$set$237
	.byte	0xe
	.byte	0x8
	.byte	0x4
	.set L$set$238,LCFI85-LCFI84
	.long L$set$238
	.byte	0xb
	.align 3
LEFDE29:
LSFDE31:
	.set L$set$239,LEFDE31-LASFDE31
	.long L$set$239
LASFDE31:
	.long	LASFDE31-EH_frame1
	.quad	LFB10118-.
	.set L$set$240,LFE10118-LFB10118
	.quad L$set$240
	.byte	0x8
	.quad	0
	.byte	0x4
	.set L$set$241,LCFI86-LFB10118
	.long L$set$241
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$242,LCFI87-LCFI86
	.long L$set$242
	.byte	0xe
	.byte	0x18
	.byte	0x83
	.byte	0x3
	.byte	0x4
	.set L$set$243,LCFI88-LCFI87
	.long L$set$243
	.byte	0xe
	.byte	0x40
	.byte	0x4
	.set L$set$244,LCFI89-LCFI88
	.long L$set$244
	.byte	0xe
	.byte	0x18
	.byte	0x4
	.set L$set$245,LCFI90-LCFI89
	.long L$set$245
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$246,LCFI91-LCFI90
	.long L$set$246
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE31:
LSFDE33:
	.set L$set$247,LEFDE33-LASFDE33
	.long L$set$247
LASFDE33:
	.long	LASFDE33-EH_frame1
	.quad	LFB10120-.
	.set L$set$248,LFE10120-LFB10120
	.quad L$set$248
	.byte	0x8
	.quad	0
	.align 3
LEFDE33:
LSFDE35:
	.set L$set$249,LEFDE35-LASFDE35
	.long L$set$249
LASFDE35:
	.long	LASFDE35-EH_frame1
	.quad	LFB9596-.
	.set L$set$250,LFE9596-LFB9596
	.quad L$set$250
	.byte	0x8
	.quad	LLSDA9596-.
	.byte	0x4
	.set L$set$251,LCFI92-LFB9596
	.long L$set$251
	.byte	0xe
	.byte	0x10
	.byte	0x8e
	.byte	0x2
	.byte	0x4
	.set L$set$252,LCFI93-LCFI92
	.long L$set$252
	.byte	0xe
	.byte	0x18
	.byte	0x8d
	.byte	0x3
	.byte	0x4
	.set L$set$253,LCFI94-LCFI93
	.long L$set$253
	.byte	0xe
	.byte	0x20
	.byte	0x8c
	.byte	0x4
	.byte	0x4
	.set L$set$254,LCFI95-LCFI94
	.long L$set$254
	.byte	0xe
	.byte	0x28
	.byte	0x86
	.byte	0x5
	.byte	0x4
	.set L$set$255,LCFI96-LCFI95
	.long L$set$255
	.byte	0xe
	.byte	0x30
	.byte	0x83
	.byte	0x6
	.byte	0x4
	.set L$set$256,LCFI97-LCFI96
	.long L$set$256
	.byte	0xe
	.byte	0xe0,0xb
	.align 3
LEFDE35:
LSFDE37:
	.set L$set$257,LEFDE37-LASFDE37
	.long L$set$257
LASFDE37:
	.long	LASFDE37-EH_frame1
	.quad	LFB11625-.
	.set L$set$258,LFE11625-LFB11625
	.quad L$set$258
	.byte	0x8
	.quad	0
	.byte	0x4
	.set L$set$259,LCFI98-LFB11625
	.long L$set$259
	.byte	0xe
	.byte	0x10
	.byte	0x4
	.set L$set$260,LCFI99-LCFI98
	.long L$set$260
	.byte	0xe
	.byte	0x8
	.align 3
LEFDE37:
	.mod_init_func
	.align 3
	.quad	__GLOBAL__sub_I_main.cc
	.constructor
	.destructor
	.align 1
	.subsections_via_symbols
