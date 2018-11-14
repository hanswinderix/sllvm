#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>

void exit_success(void);

/* ======== HELLO WORLD SM ======== */

DECLARE_SM(hello, 0x1234);
#define HELLO_SECRET_CST    0xbeef

int       SM_DATA(hello) hello_secret;
int const SM_DATA(hello) hello_const = HELLO_SECRET_CST;

void SM_FUNC(hello) hello_init(void)
{
    /* Confidential loading guarantees secrecy of constant in text section. */
    hello_secret = hello_const;
    ASSERT(hello_secret == HELLO_SECRET_CST);
}

void SM_ENTRY(hello) hello_greet(void)
{
    hello_init();
    pr_info2("Hi from SM with ID %d, called by %d\n",
        sancus_get_self_id(), sancus_get_caller_id());
}

void SM_ENTRY(hello) hello_disable(void)
{
    sancus_disable(exit_success);
}

/* ======== UNTRUSTED CONTEXT ======== */

volatile int *hello_const_pt = (volatile int *) &hello_const;

int main()
{
    msp430_io_init();
    ASSERT((*hello_const_pt) != HELLO_SECRET_CST);

    sancus_enable_wrapped(&hello, SM_GET_WRAP_NONCE(hello), SM_GET_WRAP_TAG(hello));
    pr_sm_info(&hello);

    hello_greet();
    hello_disable();

    // should never reach here
    ASSERT(0);
}

void exit_success(void)
{
    // TODO unprotect instruction should also clear caller ID
    //ASSERT(!sancus_get_caller_id());
    ASSERT(!sancus_get_id(hello_greet));
    ASSERT(!hello_secret);
    ASSERT(!(*hello_const_pt));

    pr_info("SM disabled; all done!");
    EXIT();
}
