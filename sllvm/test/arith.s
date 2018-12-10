.text
.file	"arith.s"

;----------------------------------------------------------------------------
; foo
;----------------------------------------------------------------------------

.section .sllvm.text.foo.__mspabi_mpyi_foo,"ax",@progbits
.global __mspabi_mpyi_foo
.p2align 1
.type	__mspabi_mpyi_foo,@function
__mspabi_mpyi_foo:
 	push	r2		;
 	dint			
 	nop			
 	mov	r12,	&0x0130	;
 	mov	r13,	&0x0138	;
 	mov	&0x013a,r12	;0x013a
 	reti			

.section .sllvm.text.foo.__mspabi_divu_foo,"ax",@progbits
.global __mspabi_divu_foo
.p2align 1
.type	__mspabi_divu_foo,@function
__mspabi_divu_foo:
  clr.b	r14		;
  call	#udivmodhi4_foo
  ret			

.section .sllvm.text.foo.udivmodhi4_foo,"ax",@progbits
.global udivmodhi4_foo
.p2align 1
.type	udivmodhi4_foo,@function
udivmodhi4_foo:
 	mov.b	#17,	r15	;#0x0011
 	mov.b	#1,	r11	;r3 As==01
.L1:
	cmp	r12,	r13	;
	jnc	$+18     	;abs 0x885e
	clr.b	r15		;
.L2:
 	cmp	#0,	r11	;r3 As==00
 	jnz	$+30     	;abs 0x8870
 	cmp	#0,	r14	;r3 As==00
 	jz	$+4      	;abs 0x885a
 	mov	r12,	r15	;
 	mov	r15,	r12	;
 	ret			
 	add	#-1,	r15	;r3 As==11
 	cmp	#0,	r15	;r3 As==00
 	jz	$-14     	;abs 0x8854
 	cmp	#0,	r13	;r3 As==00
 	jl	$-24     	;abs 0x884e
 	rla	r13		;
 	rla	r11		;
 	br	#.L1
 	cmp	r13,	r12	;
 	jnc	$+6      	;abs 0x8878
 	sub	r13,	r12	;
 	bis	r11,	r15	;
 	clrc			
 	rrc	r11		;
 	clrc			
 	rrc	r13		;
 	br	#.L2

.section .sllvm.text.foo.__mspabi_remi_foo,"ax",@progbits
.global __mspabi_remi_foo
.p2align 1
.type	__mspabi_remi_foo,@function
__mspabi_remi_foo:
 	push	r10		;
 	cmp	#0,	r12	;r3 As==00
 	jge	$+40     	;abs 0x88fc
 	clr.b	r14		;
 	sub	r12,	r14	;
 	mov	r14,	r12	;
 	mov.b	#1,	r10	;r3 As==01
.L3:
 	cmp	#0,	r13	;r3 As==00
 	jge	$+8      	;abs 0x88e8
 	clr.b	r14		;
 	sub	r13,	r14	;
 	mov	r14,	r13	;
 	mov.b	#1,	r14	;r3 As==01
 	call	#udivmodhi4_foo
 	cmp	#0,	r10	;r3 As==00
 	jz	$+8      	;abs 0x88f8
 	clr.b	r13		;
 	sub	r12,	r13	;
 	mov	r13,	r12	;
 	pop	r10		;
 	ret			
 	clr.b	r10		;
 	br	#.L3

;----------------------------------------------------------------------------
; bar
;----------------------------------------------------------------------------

.section .sllvm.text.bar.__mspabi_mpyi_bar,"ax",@progbits
.global __mspabi_mpyi_bar
.p2align 1
.type	__mspabi_mpyi_bar,@function
__mspabi_mpyi_bar:
 	push	r2		;
 	dint			
 	nop			
 	mov	r12,	&0x0130	;
 	mov	r13,	&0x0138	;
 	mov	&0x013a,r12	;0x013a
 	reti			

.section .sllvm.text.bar.__mspabi_divu_bar,"ax",@progbits
.global __mspabi_divu_bar
.p2align 1
.type	__mspabi_divu_bar,@function
__mspabi_divu_bar:
  clr.b	r14		;
  call	#udivmodhi4_bar
  ret			

.section .sllvm.text.bar.udivmodhi4_bar,"ax",@progbits
.global udivmodhi4_bar
.p2align 1
.type	udivmodhi4_bar,@function
udivmodhi4_bar:
 	mov.b	#17,	r15	;#0x0011
 	mov.b	#1,	r11	;r3 As==01
.L11:
	cmp	r12,	r13	;
	jnc	$+18     	;abs 0x885e
	clr.b	r15		;
.L12:
 	cmp	#0,	r11	;r3 As==00
 	jnz	$+30     	;abs 0x8870
 	cmp	#0,	r14	;r3 As==00
 	jz	$+4      	;abs 0x885a
 	mov	r12,	r15	;
 	mov	r15,	r12	;
 	ret			
 	add	#-1,	r15	;r3 As==11
 	cmp	#0,	r15	;r3 As==00
 	jz	$-14     	;abs 0x8854
 	cmp	#0,	r13	;r3 As==00
 	jl	$-24     	;abs 0x884e
 	rla	r13		;
 	rla	r11		;
 	br	#.L11
 	cmp	r13,	r12	;
 	jnc	$+6      	;abs 0x8878
 	sub	r13,	r12	;
 	bis	r11,	r15	;
 	clrc			
 	rrc	r11		;
 	clrc			
 	rrc	r13		;
 	br	#.L12

.section .sllvm.text.bar.__mspabi_remi_bar,"ax",@progbits
.global __mspabi_remi_bar
.p2align 1
.type	__mspabi_remi_bar,@function
__mspabi_remi_bar:
 	push	r10		;
 	cmp	#0,	r12	;r3 As==00
 	jge	$+40     	;abs 0x88fc
 	clr.b	r14		;
 	sub	r12,	r14	;
 	mov	r14,	r12	;
 	mov.b	#1,	r10	;r3 As==01
.L13:
 	cmp	#0,	r13	;r3 As==00
 	jge	$+8      	;abs 0x88e8
 	clr.b	r14		;
 	sub	r13,	r14	;
 	mov	r14,	r13	;
 	mov.b	#1,	r14	;r3 As==01
 	call	#udivmodhi4_bar
 	cmp	#0,	r10	;r3 As==00
 	jz	$+8      	;abs 0x88f8
 	clr.b	r13		;
 	sub	r12,	r13	;
 	mov	r13,	r12	;
 	pop	r10		;
 	ret			
 	clr.b	r10		;
 	br	#.L13
