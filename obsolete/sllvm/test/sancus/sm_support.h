#ifndef SM_SUPPORT_H
#define SM_SUPPORT_H

#include "config.h"

#include <stddef.h>

#if __GNUC__ >= 5 || __clang_major__ >= 5

#define __PS(name) sllvm_pm_##name##_text_start
#define __PE(name) sllvm_pm_##name##_text_end
#define __SS(name) sllvm_pm_##name##_data_start
#define __SE(name) sllvm_pm_##name##_data_end

#define SM_GET_WRAP_NONCE(sm) ({      \
    extern unsigned sllvm_nonce_##sm; \
    sllvm_nonce_##sm;                 \
})
#define SM_GET_WRAP_TAG(sm) ({                   \
    extern char sllvm_tag_##sm[SANCUS_TAG_SIZE]; \
    sllvm_tag_##sm;                              \
})

#define DECLARE_SM(name, vendor_id)  \
    extern char __PS(name);          \
    extern char __PE(name);          \
    extern char __SS(name);          \
    extern char __SE(name);          \
    extern struct SancusModule name;
#define DECLARE_MMIO_SM(name, secret_start, secret_end, vendor) \
  DECLARE_SM(name, vendor)

#define SM_DATA(name) static
#define SM_FUNC(name) static
#define SM_ENTRY(name) __attribute__((eentry))
#define SM_MMIO_ENTRY(name) __attribute__((eentry))

#else

#define __PS(name) __sm_##name##_public_start
#define __PE(name) __sm_##name##_public_end
#define __SS(name) __sm_##name##_secret_start
#define __SE(name) __sm_##name##_secret_end

/**
 * This macro can be used to declare a SancusModule structure.
 *
 * Every Sancus module should be declared once (and only once) in the global
 * scope using this macro.
 *
 * @param name      Should be the same as the argument passed to the SM_ENTRY(),
 *                  SM_FUNC() and SM_DATA() macros but without quotation marks.
 * @param vendor_id The ID of the module's vendor.
 */
#define DECLARE_SM(name, vendor_id)                                 \
    extern char __PS(name), __PE(name), __SS(name), __SE(name);     \
    struct SancusModule name = {0, vendor_id, #name,                \
                                &__PS(name), &__PE(name),           \
                                &__SS(name), &__SE(name)}

// Helper macro to ensure arguments are expanded
#define DECLARE_MMIO_SM_AUX(name, secret_start, secret_end, vendor) \
    asm("__sm_mmio_" #name "_secret_start =" #secret_start "\n");   \
    asm("__sm_mmio_" #name "_secret_end =" #secret_end "\n");       \
    DECLARE_SM(name, vendor)

/**
 * NOTE: secret_start is inclusive; secret_end is exclusive
 */
#define DECLARE_EXCLUSIVE_MMIO_SM(name, secret_start, secret_end,   \
                                  caller_id, vendor)                \
    DECLARE_MMIO_SM_AUX(name, secret_start, secret_end, vendor);    \
    asm("__sm_mmio_" #name "_caller_id =" #caller_id "\n")

#define DECLARE_MMIO_SM(name, secret_start, secret_end, vendor)     \
    DECLARE_MMIO_SM_AUX(name, secret_start, secret_end, vendor)


void __unprotected_entry(void);
extern char __unprotected_sp;

#define __ANNOTATE(x) __attribute__((annotate(x)))
#define __STR(x) #x

/**
 * Annotation for module entry points.
 *
 * Use as follows:
 * @code
 * void SM_ENTRY("mod_name") entry_name(void) {...}
 * @endcode
 */
#define SM_ENTRY(name) __ANNOTATE("sm_entry:" __STR(name)) \
                       __attribute__((noinline, used))

#define SM_MMIO_ENTRY(name) __ANNOTATE("sm_mmio_entry:" __STR(name)) \
                       __attribute__((noinline, used, naked))

/**
 * Annotation for internal module function (i.e., not entry points).
 *
 * @see SM_ENTRY()
 */
#define SM_FUNC(name)  __ANNOTATE("sm:" __STR(name))

/**
 * Annotation for data the should be part of the secret section.
 *
 * Note that the secret section is always zero-initialized. This means that data
 * cannot be initialized if it is to be placed in the secret section.
 *
 * Use as follows:
 * @code
 * int SM_DATA("mod_name") data_name;
 * @endcode
 */
#define SM_DATA(name)  SM_FUNC(name)

/**
 * Macro to create an ISR inside an SM. Inside the ISR, you can use the variable
 * num_name to get the IRQ number that cause the execution of the ISR.
 *
 * Use as follows:
 * @code
 * SM_ISR(mod_name, num_name) {...}
 * @endcode
 */
#define SM_ISR(name, num_name) void SM_FUNC(name) __attribute__((used)) \
                               __sm_##name##_isr_func(unsigned num_name)

/**
 * Macro to indicate that you want to handle a certain IRQ (specified by
 * irq_num) inside the SM with name sm_name. Note that using this macro without
 * having an SM_ISR() function will just ignore the IRQ.
 */
#define SM_HANDLE_IRQ(sm_name, irq_num)                                 \
    asm(".global __sm_" #sm_name "_handles_irq_" #irq_num "\n"          \
                "__sm_" #sm_name "_handles_irq_" #irq_num " = 1\n")

/**
 * Macro to get the address of the entry point of SM @p sm.
 */
#define SM_GET_ENTRY(sm) ({         \
    extern char __sm_##sm##_entry;  \
    (void*)&__sm_##sm##_entry;      \
})

/**
 * Macro to get the index of entry point @p entry of SM @p sm.
 *
 * Both arguments should be names without quotes.
 */
#define SM_GET_ENTRY_IDX(sm, entry) ({              \
    extern char __sm_##sm##_entry_##entry##_idx;    \
    (entry_idx)&__sm_##sm##_entry_##entry##_idx;    \
})

/**
 * Macro to get a pointer to the tag used by @p caller to verify @p callee.
 *
 * Both arguments should be names without quotes.
 */
#define SM_GET_TAG(caller, callee) ({           \
    extern char __sm_##caller##_mac_##callee;   \
    (void*)&__sm_##caller##_mac_##callee;       \
})

/**
 * Macro to get a pointer to the ID of @p callee that is used by @p caller after
 * the initial verification.
 *
 * Both arguments should be names without quotes.
 */
#define SM_GET_VERIFY_ID(caller, callee) ({     \
    extern char __sm_##caller##_id_##callee;    \
    (sm_id*)&__sm_##caller##_id_##callee;       \
})

/**
 * Macro to get the nonce used for wrapping sm.
 */
#define SM_GET_WRAP_NONCE(sm) ({            \
    extern unsigned __sm_##sm##_wrap_nonce; \
    __sm_##sm##_wrap_nonce;                 \
})

/**
 * Macro to get the tag produced by wrapping sm.
 */
#define SM_GET_WRAP_TAG(sm) ({          \
    extern char __sm_##sm##_wrap_tag;   \
    (void*)&__sm_##sm##_wrap_tag;       \
})

#endif

#define __OUTSIDE_SM( p, sm )                                                  \
    ( ((void*) p < (void*) &__PS(sm)) || ((void*) p >= (void*) &__PE(sm)) ) && \
    ( ((void*) p < (void*) &__SS(sm)) || ((void*) p >= (void*) &__SE(sm)) )

/*
 * Returns true iff whole buffer [p,p+len-1] is outside of the sm SancusModule
 */
#define sancus_is_outside_sm( sm, p, len) \
    ( __OUTSIDE_SM(p, sm) && __OUTSIDE_SM((p+len-1), sm) )

/**
 * Interrupt vector for the Sancus violation ISR.
 *
 * Use as follows:
 * @code
 * void __attribute__((interrupt(SM_VECTOR))) the_isr(void) {...}
 * @endcode
 */
#if __GNUC__ >= 5 || __clang_major__ >= 5
#define SM_VECTOR 14
#else
#define SM_VECTOR 26
#endif

/**
 * Return value of sancus_get_caller_id() for unprotected code.
 */
#define SM_ID_UNPROTECTED 0

/**
 * Return value of sancus_get_caller_id() for an IRQ.
 */
#define SM_ID_IRQ 0xffff

/**
 * The number of bits security offered by the crypto functions
 */
#define SANCUS_SECURITY CONFIG_SECURITY

/**
 * The size of the tags used and produces by the crypto functions.
 */
#define SANCUS_TAG_SIZE (SANCUS_SECURITY / 8)

/**
 * The size of the keys used by the crypto functions.
 */
#define SANCUS_KEY_SIZE (SANCUS_SECURITY / 8)

/**
 * Type used to represent the internal ID of an SM assigned by Sancus.
 *
 * Note that 0 is an invalid ID so it is often used to indicate an error
 * condition.
 */
typedef unsigned sm_id;

/**
 * Type used to represent vendor IDs.
 */
typedef unsigned vendor_id;

/**
 * Type used to represent the index of an entry point.
 */
typedef unsigned entry_idx;

/**
 * Structure containing all the relevant information of a Sancus module.
 *
 * Note that #id will only be correct after a call to sancus_enable().
 */
struct SancusModule
{
    sm_id id;               ///< Sancus ID.
    vendor_id vendor_id;    ///< ID of the vendor of this module.
    const char* name;       ///< Name of this module.
    void* public_start;     ///< Start address of the public section
    void* public_end;       ///< End address of the public section
    void* secret_start;     ///< Start address of the secret section
    void* secret_end;       ///< End address of the secret section
};

/**
 * Enables the protection of the given module.
 *
 * A minimal working example of enabling a Sancus module:
 * @code
 * void SM_ENTRY("mod") a_module_entry(void) {...}
 * DECLARE_SM(mod, 1234);
 * int main(void)
 * {
 *     sancus_enable(&mod);
 *     ...
 * }
 * @endcode
 *
 * @param sm Pointer to a SancusModule to enable. This is typically created by
 *           taking the address of the name defined by DECLARE_SM().
 *
 * @return   The ID of the enabled SM. 0 on error.
 */
sm_id sancus_enable(struct SancusModule* sm);

/**
 * Enables the protection of the given encrypted module.
 */
sm_id sancus_enable_wrapped(struct SancusModule* sm, unsigned nonce, void* tag);

#undef always_inline
#define always_inline static inline __attribute__((always_inline))

/**
 * Disable the protection of the calling module.
 */
always_inline void sancus_disable(void *continuation)
{
    // The unprotect instruction takes care of clearing all SM code/data
    // memory; we clear unused CPU registers here. Note that we do not mark
    // them as clobbered, for execution continues at the continuation argument.
    asm(
        "mov %0, r15\n\t"
        "clr r1\n\t"
        "and #0x7ef8, r2\n\t"
        "clr r4\n\t"
        "clr r5\n\t"
        "clr r6\n\t"
        "clr r7\n\t"
        "clr r8\n\t"
        "clr r9\n\t"
        "clr r10\n\t"
        "clr r11\n\t"
        "clr r12\n\t"
        "clr r13\n\t"
        "clr r14\n\t"
        ".word 0x1380\n\t"
        ::"m"(continuation):);
}

/**
 * Verify the module located at a specific address.
 *
 * @see sancus_verify()
 */
always_inline sm_id sancus_verify_address(const void* expected_tag,
                                          const void* address)
{
    sm_id ret;
    asm("mov %1, r14\n\t"
        "mov %2, r15\n\t"
        ".word 0x1382\n\t"
        "mov r15, %0"
        : "=m"(ret)
        : "r"(address), "r"(expected_tag)
        : "14", "15");
    return ret;
}

/**
 * Verify the correctness of a module.
 *
 * The correctness of a module is verified by calculating a MAC of the layout
 * of the module described by @p sm and comparing it with the MAC given by
 * @p expected_tag.
 *
 * Note that this function normally should not be called directly; the compiler
 * will insert verification code on inter-module calls.
 *
 * @return The ID of the verified SM on success, 0 otherwise.
 */
always_inline sm_id sancus_verify(const void* expected_tag,
                                  struct SancusModule* sm)
{
    return sancus_verify_address(expected_tag, sm->public_start);
}

/**
 * Verify the correctness of the calling module.
 *
 * Correctness is defined as with sancus_verify(). See sancus_get_caller_id()
 * for the definition of the calling module.
 *
 * @return True iff the verification succeeded.
 */
always_inline int sancus_verify_caller(const void* expected_tag)
{
    int ret;
    asm("mov %1, r15\n\t"
        ".word 0x1383\n\t"
        "mov r15, %0"
        : "=m"(ret)
        : "r"(expected_tag)
        : "15");
    return ret;
}

/**
 * Wrap a message using the Sancus authenticated encryption features.
 *
 * @p body_len bytes of data starting at @p body are wrapped using the key
 * @p key and @p ad_len of associated data starting at @p ad. The resulting
 * cipher text is written to @p cipher (make sure there is a buffer available
 * of at least @p body_len bytes) and the MAC to @p tag (the needed buffer size
 * depends of the amount of security bits the Sancus core has been synthesized
 * with).
 *
 * @param key       Optional pointer to input key buffer of SANCUS_KEY_SIZE
 *                  bytes; NULL to use the SM key of the executing module.
 * @param ad        Required pointer to start of associated data input buffer;
 *                  cannot be NULL.
 * @param ad_len    Length of associated data input buffer.
 * @param body      Optional pointer to input buffer holding the plain text;
                    NULL when only MAC over associated data is required.
 * @param body_len  Length of plain text data input buffer.
 * @param cipher    Optional pointer to output buffer of @p body_len bytes for
 *                  the cipher text; cannot be NULL when @p body is non-zero.
 * @param tag       Required pointer to output buffer of SANCUS_TAG_SIZE bytes
 *                  for MAC over both associated data and plain text (if any);
 *                  cannot be NULL;
 *
 * @return True iff the wrapping succeeded.
 */
always_inline int sancus_wrap_with_key(const void* key,
                                       const void* ad, size_t ad_len,
                                       const void* body, size_t body_len,
                                       void* cipher, void* tag)
{
    void* ad_end = (char*)ad + ad_len;
    void* body_end = (char*)body + body_len;
    int ret;

    // we use memory constraints for all operands because otherwise LLVM's
    // register allocator messes up and uses some of the clobbered registers
    asm("mov %1, r9\n\t"
        "mov %2, r10\n\t"
        "mov %3, r11\n\t"
        "mov %4, r12\n\t"
        "mov %5, r13\n\t"
        "mov %6, r14\n\t"
        "mov %7, r15\n\t"
        ".word 0x1384\n\t"
        "mov r15, %0"
        : "=m"(ret)
        : "m"(key),
          "m"(ad), "m"(ad_end),
          "m"(body), "m"(body_end),
          "m"(cipher), "m"(tag)
        : "9", "10", "11", "12", "13", "14", "15");

    return ret;
}

/**
 * Wrap a message using the Sancus authenticated encryption features.
 *
 * This is the same as sancus_wrap_with_key using the key of the caller module.
 */
always_inline int sancus_wrap(const void* ad, size_t ad_len,
                              const void* body, size_t body_len,
                              void* cipher, void* tag)
{
    return sancus_wrap_with_key(NULL, ad, ad_len, body, body_len, cipher, tag);
}

/**
 * Unwrap a message using the Sancus authenticated encryption features.
 *
 * See sancus_wrap_with_key() for an explanation of the parameters.
 */
always_inline int sancus_unwrap_with_key(const void* key,
                                         const void* ad, size_t ad_len,
                                         const void* cipher,
                                         size_t cipher_len,
                                         const void* tag, void* body)
{
    void* ad_end = (char*)ad + ad_len;
    void* cipher_end = (char*)cipher + cipher_len;
    int ret;

    // we use memory constraints for all operands because otherwise LLVM's
    // register allocator messes up and uses some of the clobbered registers
    asm("mov %1, r9\n\t"
        "mov %2, r10\n\t"
        "mov %3, r11\n\t"
        "mov %4, r12\n\t"
        "mov %5, r13\n\t"
        "mov %6, r14\n\t"
        "mov %7, r15\n\t"
        ".word 0x1385\n\t"
        "mov r15, %0"
        : "=m"(ret)
        : "m"(key),
          "m"(ad), "m"(ad_end),
          "m"(cipher), "m"(cipher_end),
          "m"(body), "m"(tag)
        : "9", "10", "11", "12", "13", "14", "15");

    return ret;
}

/**
 * Unwrap a message using the Sancus authenticated encryption features.
 *
 * This is the same as sancus_unwrap_with_key using the key of the caller
 * module.
 */
always_inline int sancus_unwrap(const void* ad, size_t ad_len,
                                const void* cipher, size_t cipher_len,
                                const void* tag, void* body)
{
    return sancus_unwrap_with_key(NULL, ad, ad_len, cipher, cipher_len,
                                  tag, body);
}

/**
 * The same as sancus_wrap() but only produces the MAC of the message.
 */
always_inline int sancus_tag(const void* body, size_t body_len, void* tag)
{
    return sancus_wrap(body, body_len, NULL, 0, NULL, tag);
}

/**
 * The same as sancus_wrap_with_key() but only produces the MAC of the message.
 */
always_inline int sancus_tag_with_key(const void* key,
                                      const void* body, size_t body_len,
                                      void* tag)
{
    return sancus_wrap_with_key(key, body, body_len, NULL, 0, NULL, tag);
}

/**
 * Get the Sancus ID of the module loaded at @p addr.
 */
always_inline sm_id sancus_get_id(void* addr)
{
    sm_id ret;
    asm("mov %1, r15\n\t"
        ".word 0x1386\n\t"
        "mov r15, %0"
        : "=m"(ret)
        : "m"(addr)
        : "15");
    return ret;
}

/**
 * Get the Sancus ID of the currently executing module.
 */
always_inline sm_id sancus_get_self_id(void)
{
    void* addr;
    asm("mov r0, %0" : "=m"(addr));
    return sancus_get_id(addr);
}

/**
 * Get the ID of the calling module.
 *
 * The calling module is defined as the previously executing module. That is,
 * the module that entered the currently executing module.
 */
always_inline sm_id sancus_get_caller_id(void)
{
    sm_id ret;
    asm(".word 0x1387\n\t"
        "mov r15, %0"
        : "=m"(ret)
        :
        : "15");
    return ret;
}

/**
 * Perform a call to a Sancus module.
 *
 * The call will be inlined at the call site which means that the control flow
 * will not pass through unprotected code. @p entry should point to the module's
 * entry point and @p index is the index of the entry function to be called. The
 * rest of the parameters (maximum 3 at the moment) are forwarded to the called
 * module.
 *
 * @note This function is implemented as a compiler intrinsic.
 */
unsigned sancus_call(void* entry, entry_idx index, ...);

#undef always_inline

#endif
