#ifndef SLLVM_SANCUS_H
#define SLLVM_SANCUS_H

namespace sllvm { namespace sancus {

  constexpr const char *local_stack = "sllvm_lstack";
  constexpr const char *local_r1 = "sllvm_lr1";
  constexpr const char *local_r4 = "sllvm_lr4";
  constexpr const char *local_r5 = "sllvm_lr5";
  constexpr const char *local_r8 = "sllvm_lr8";
  constexpr const char *local_r9 = "sllvm_lr9";
  constexpr const char *local_r10 = "sllvm_lr10";
  constexpr const char *local_r11 = "sllvm_lr11";
  constexpr const char *global_r1 = "sllvm_r1";
  constexpr const char *global_pc = "sllvm_pc"; // TODO: Remove
  constexpr const char *fname_dispatch = "sllvm_dispatch";
  constexpr const unsigned R6_PRet = 0xffff; // Return from call to pm
  constexpr const unsigned R6_URet = 0xfffe; // Return from call to non-pm

  constexpr const char* asm_eenter = R"(
	.p2align	1
	.type	sllvm_eenter,@function
sllvm_eenter:
	ret
  )";

  constexpr const char* asm_eexit = R"(
	.p2align	1
	.type	sllvm_eexit,@function
sllvm_eexit:
	; Clear the status bits
	and	#0x7ef8, r2
	; This might be a return-to-enclave
	mov	#0xffff, r6
	ret
  )";

  constexpr const char* asm_excall = R"(
	.p2align	1
	.type	sllvm_excall,@function
sllvm_excall:
	; Save callee save Registers
	mov	r4, &sllvm_lr4
	mov	r5, &sllvm_lr5
	mov	r8, &sllvm_lr8
	mov	r9, &sllvm_lr9
	mov	r10, &sllvm_lr10
	mov	r11, &sllvm_lr11
	; Clear callee save Registers
	clr	r4
	clr	r5
	clr	r8
	clr	r9
	clr	r10
	clr	r11
	; Clear the status bits
	and	#0x7ef8, r2
	ret
  )";

  constexpr const char* asm_eresume = R"(
	.p2align	1
	.type	sllvm_eresume,@function
sllvm_eresume:
	; Restore callee save Registers
	mov	&sllvm_lr4, r4,
	mov	&sllvm_lr5, r5,
	mov	&sllvm_lr8, r8,
	mov	&sllvm_lr9, r9,
	mov	&sllvm_lr10, r10
	mov	&sllvm_lr11, r11
	ret
  )";

  // TODO: Paramterize (Module, tag, nonce, vendor_id)
  // TODO: Have symbol names generated (sllvm_data_*, sllvm_text_*)
  constexpr const char* asm_protect = R"(
	.globl	sllvm_protect
	.p2align	1
	.type	sllvm_protect,@function
sllvm_protect:
	mov	#0, r9    ; tag
	mov	#0, r10   ; nonce
	mov	#1234, r11   ; vendor id
	mov	#sllvm_text_section_start,	r12
	mov	#sllvm_text_section_end,	r13
	mov	#sllvm_data_section_start,	r14
	mov	#sllvm_data_section_end,	r15
	.word	0x1381
	mov	r15, r12
	ret
  )";

  constexpr const char* asm_attest = R"(
	.p2align	1
	.type	sllvm_attest,@function
sllvm_sllvm_attest:
	ret
  )";

} }

#endif
