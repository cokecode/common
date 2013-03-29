	.file	"a.cpp"
	.text
	.align 2
.globl _Z3fooRP1A
	.type	_Z3fooRP1A,@function
_Z3fooRP1A:
.LFB2:
	pushl	%ebp
.LCFI0:
	movl	%esp, %ebp
.LCFI1:
	pushl	%ebx
.LCFI2:
	subl	$4, %esp
.LCFI3:
	movl	8(%ebp), %ebx
	subl	$12, %esp
	pushl	$4
.LCFI4:
	call	_Znwj
	addl	$16, %esp
	movl	%eax, (%ebx)
	movl	-4(%ebp), %ebx
	leave
	ret
.LFE2:
.Lfe1:
	.size	_Z3fooRP1A,.Lfe1-_Z3fooRP1A
	.section	.rodata
.LC0:
	.string	"a = %p, b = %p\n"
	.text
	.align 2
.globl main
	.type	main,@function
main:
.LFB4:
	pushl	%ebp
.LCFI5:
	movl	%esp, %ebp
.LCFI6:
	subl	$8, %esp
.LCFI7:
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	movl	$0, -8(%ebp)
	subl	$12, %esp
	leal	-4(%ebp), %eax
	pushl	%eax
.LCFI8:
	call	_Z3fooRP1A
	addl	$16, %esp
	subl	$12, %esp
	leal	-8(%ebp), %eax
	pushl	%eax
	call	_Z3fooRP1A
	addl	$16, %esp
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	movl	$0, %eax
	leave
	ret
.LFE4:
.Lfe2:
	.size	main,.Lfe2-main
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zP"
	.uleb128 0x1
	.sleb128 -4
	.byte	0x8
	.uleb128 0x5
	.byte	0x0
	.long	__gxx_personality_v0
	.byte	0xc
	.uleb128 0x4
	.uleb128 0x4
	.byte	0x88
	.uleb128 0x1
	.align 4
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB2
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI3-.LCFI1
	.byte	0x83
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0x2e
	.uleb128 0x10
	.align 4
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB4
	.long	.LFE4-.LFB4
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB4
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI8-.LCFI6
	.byte	0x2e
	.uleb128 0x10
	.align 4
.LEFDE3:
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.2.3 20030502 (Red Hat Linux 3.2.3-20)"
