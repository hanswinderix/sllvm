BASEDIR=/home/hans/devel/thesis
SLLVM_HOME=${BASEDIR}/llvm/local
RUST_HOME=${BASEDIR}/rust/local
SGX_HOME=${BASEDIR}/linux.sgx

CLANG=${SLLVM_HOME}/bin/clang
#RUSTC=${RUST_HOME}/bin/rustc
RUSTC=rustc
OPT=${SLLVM_HOME}/bin/opt
LLC=${SLLVM_HOME}/bin/llc
MSP430GCC=msp430-elf-gcc
MSP430LD=${MSP430GCC}
MSP430OBJDUMP=msp430-elf-objdump
EDGER8R=${SGX_HOME}/build/linux/sgx_edger8r

SANCUSCLANG=clang
SANCUSLLC=llc

DIFF=meld

SSIM=sancus-sim

#MCU=msp430f1121
MCU=msp430f149

CLANGFLAGS=
MSP430LDFLAGS=
MSP430GCCFLAGS=
RUSTCFLAGS=
INCFLAGS=

-include Makefile.local

SIM=arith

SSIMFLAGS=
SSIMFLAGS+=--ram 16K
SSIMFLAGS+=--rom 41K

SancusModuleCreator=${SLLVM_HOME}/lib/SancusModuleCreator.so
SLLVMLIB=${SLLVM_HOME}/lib/SLLVM.so

# Include flags needed to compile the sancus-examples 
# TODO: Should be platform independent (don't hardcode /usr/local/share...)
# TODO: Only required when compiling the Sancus examples
# TODO: Not required for SGX
INCFLAGS+=-I .  # To be able to angle bracket include sancus/sm_support.h
#INCFLAGS+=-I /usr/msp430/include
INCFLAGS+=-I /usr/local/include
INCFLAGS+=-I /usr/local/msp430-elf/include
INCFLAGS+=-I /usr/local/share/sancus-support/include # for sm_io.h
#INCFLAGS+=-I /usr/lib/gcc/msp430/4.6.3/include
#INCFLAGS+=-I /usr/local/share/sancus-compiler/include

CLANGFLAGS+=${INCFLAGS}
CLANGFLAGS+=-Werror
CLANGFLAGS+=-O0
CLANGFLAGS+=-emit-llvm

MSP430CLANGFLAGS+=${CLANGFLAGS}
MSP430CLANGFLAGS+=-D__$(shell echo $(MCU) | tr a-z A-Z)__
MSP430CLANGFLAGS+=-target msp430-elf

MSP430GCCFLAGS+=-mmcu=${MCU}
MSP430GCCFLAGS+=${INCFLAGS}

X86_64CLANGFLAGS+=${CLANGFLAGS}
X86_64CLANGFLAGS+=-target x86_64

RUSTCFLAGS+=--crate-type lib
#RUSTCFLAGS+=--target msp430-none-elf
#RUSTCFLAGS+=--target msp430-none-elf-legacy
#RUSTCFLAGS+=--sysroot ~/.xargo/
RUSTCFLAGS+=--emit llvm-ir

MSP430LDFLAGS+=-mmcu=${MCU}
MSP430LDFLAGS+=-Tmsp430-elf-sllvm.x

# Linker flags needed to compile the sancus-examples 
# TODO: Only required when compiling the Sancus examples
# TODO: Should be platform independent (don't hardcode /usr/local/share...)
MSP430LDFLAGS+=-L/usr/local/share/sancus-support/lib/
MSP430LDFLAGS+=-L/usr/local/include
MSP430LDFLAGS+=-lsm-io
MSP430LDFLAGS+=-ldev
MSP430LDFLAGS+=-L/usr/local/share/sancus-compiler/lib/
MSP430LDFLAGS+=-lsancus-sm-support
#MSP430LDFLAGS+=-lsancus-host-support

LLCFLAGS=
LLCFLAGS+=-O3
#LLCFLAGS+=-view-dag-combine1-dags  # DAG before first optimization pass.
#LLCFLAGS+=-view-legalize-dags      # DAG before legalization
#LLCFLAGS+=-view-dag-combine2-dags  # DAG before second optimization pass
#LLCFLAGS+=-view-isel-dags          # DAG before select phase
#LLCFLAGS+=-view-sched-dags         # DAG before scheduling
#LLCFLAGS+=-view-sunit-dags         # display scheduler's dependency graph

SANCUSOPTFLAGS=
SANCUSOPTFLAGS+=--load ${SancusModuleCreator}
SANCUSOPTFLAGS+=--create-sm

SLLVMOPT1FLAGS=
SLLVMOPT1FLAGS+=--load ${SLLVMLIB}
SLLVMOPT1FLAGS+=--sllvm-transform

SANCUSFLAGS=
SANCUSFLAGS+=--load ${SLLVMLIB}
SANCUSFLAGS+=--sancus-transform

SGXFLAGS=
SGXFLAGS+=--load ${SLLVMLIB}
SGXFLAGS+=--sgx-transform

.SUFFIXES:

.PRECIOUS: %.ll
.PRECIOUS: %.s
.PRECIOUS: %.legacy.ll
.PRECIOUS: %.legacy.opt.ll
.PRECIOUS: %.legacy.opt.s
.PRECIOUS: %.sllvm.msp430.ll
.PRECIOUS: %.sllvm.msp430.opt1.ll
.PRECIOUS: %.sllvm.msp430.sancus.ll
.PRECIOUS: %.sllvm.msp430.sancus.s
.PRECIOUS: %.sllvm.x86_64.ll
.PRECIOUS: %.sllvm.x86_64.opt1.ll
.PRECIOUS: %.sllvm.x86_64.sgx.ll
.PRECIOUS: %.sllvm.x86_64.sgx.s

##############################################################################
# Sancus examples

#all: fileio.objdump
#all: foo.objdump # There are two tests with a foo module.objdump
all: arith.objdump

##############################################################################

#all: hello.objdump
#all: hello.legacy.opt.s
#all: hello.sllvm.msp430.sancus.s

#all: test.objdump
#all: test2.objdump
#all: enclave.legacy.opt.s

#all: a.out.objdump
#all: enclave1.sllvm.msp430.sancus.s
#all: enclave2.sllvm.msp430.sancus.s
#all: enclave3.sllvm.msp430.sancus.s
#all: main.sllvm.msp430.sancus.s

#all: enclave1.legacy.opt.s
#all: main.legacy.opt.s

#all: enclave1.sllvm.x86_64.sgx.ll
#all: enclave1.sllvm.x86_64.sgx.o

#all: hello2.objdump
#all: enclave4.sllvm.msp430.ll

#############################################################################
%.ll: %.c
	${CLANG} ${MSP430CLANGFLAGS} -S -c $< -o $@

%.s: %.ll
	${LLC} ${LLCFLAGS} $<

#############################################################################
%.legacy.ll: %.c
	${SANCUSCLANG} ${MSP430CLANGFLAGS} -DENABLE_SANCUS -S -c $< -o $@

%.legacy.opt.ll: %.legacy.ll
	${OPT} ${SANCUSOPTFLAGS} -S $< -o $@ > /dev/null

%.legacy.opt.s: %.legacy.opt.ll
	${SANCUSLLC} ${LLCFLAGS} $<

#############################################################################
%.sllvm.msp430.ll: %.c
	${CLANG} ${MSP430CLANGFLAGS} -DENABLE_SLLVM -S -c $< -o $@

%.sllvm.x86_64.ll: %.c
	${CLANG} ${X86_64CLANGFLAGS} -DENABLE_SLLVM -S -c $< -o $@

#############################################################################
%.sllvm.msp430.ll: %.rs
	${RUSTC} ${RUSTCFLAGS} $< -o $@

#############################################################################
%.sllvm.msp430.s: %.sllvm.msp430.ll
	${LLC} ${LLCFLAGS} $<

#############################################################################
%.sllvm.msp430.opt1.ll: %.sllvm.msp430.ll
	${OPT} ${SLLVMOPT1FLAGS} -S $< -o $@ > /dev/null

%.sllvm.x86_64.opt1.ll: %.sllvm.x86_64.ll
	${OPT} ${SLLVMOPT1FLAGS} -S $< -o $@ > /dev/null

#############################################################################
%.sllvm.msp430.opt1.s: %.sllvm.msp430.opt1.ll
	${LLC} ${LLCFLAGS} $<

#############################################################################
%.sllvm.msp430.sancus.ll: %.sllvm.msp430.opt1.ll
	${OPT} ${SANCUSFLAGS} -S $< -o $@ > /dev/null

%.sllvm.x86_64.sgx.ll: %.sllvm.x86_64.opt1.ll
	${OPT} ${SGXFLAGS} -S $< -o $@ > /dev/null

#############################################################################
%.sllvm.msp430.sancus.s: %.sllvm.msp430.sancus.ll
	${LLC} ${LLCFLAGS} $<

%.sllvm.x86_64.sgx.s: %.sllvm.x86_64.sgx.ll
	${LLC} ${LLCFLAGS} $<

#############################################################################
%.sllvm.msp430.sancus.o: %.sllvm.msp430.sancus.s
	$(MSP430GCC) ${MSP430GCCFLAGS} -c $<

%.sllvm.x86_64.sgx.o: %.sllvm.x86_64.sgx.s
	$(CLANG) -c $<

#############################################################################
sancus.o: sancus.c
	$(MSP430GCC) ${MSP430GCCFLAGS} -c $<

panic.o: panic.s
	$(MSP430GCC) ${MSP430GCCFLAGS} -c $<


#----------------------------------------------------------------------------

a.out: sancus.o
a.out: main.sllvm.msp430.sancus.o
#a.out: enclave4.sllvm.msp430.sancus.o
a.out: enclave1.sllvm.msp430.sancus.o
a.out: enclave2.sllvm.msp430.sancus.o
a.out: panic.o
	ln -f msp430-sllvm-text.x msp430-elf-sllvm-text.x
	ln -f msp430-sllvm-data.x msp430-elf-sllvm-data.x
	${MSP430LD} ${MSP430LDFLAGS} $^

a.out.objdump: a.out
	${MSP430OBJDUMP} -D a.out > $@

#----------------------------------------------------------------------------

test: sancus.o
test: test.sllvm.msp430.sancus.o
	ln -f msp430-sllvm-text.x msp430-elf-sllvm-text.x
	ln -f msp430-sllvm-data.x msp430-elf-sllvm-data.x
	${MSP430LD} $^ ${MSP430LDFLAGS} -o $@

test.objdump: test
	${MSP430OBJDUMP} -D test > $@

#----------------------------------------------------------------------------

test2: sancus.o
test2: test2.sllvm.msp430.sancus.o
test2: enclave.sllvm.msp430.sancus.o
	ln -f msp430-sllvm-text.x msp430-elf-sllvm-text.x
	ln -f msp430-sllvm-data.x msp430-elf-sllvm-data.x
	${MSP430LD} $^ ${MSP430LDFLAGS} -o $@

test2.objdump: test2
	${MSP430OBJDUMP} -D test2 > $@

#----------------------------------------------------------------------------

foo: sancus.o
foo: foo.sllvm.msp430.sancus.o
	ln -f msp430-sllvm-text.x msp430-elf-sllvm-text.x
	ln -f msp430-sllvm-data.x msp430-elf-sllvm-data.x
	${MSP430LD} $^ ${MSP430LDFLAGS} -o $@

foo.objdump: foo
	${MSP430OBJDUMP} -D foo > $@

#----------------------------------------------------------------------------

fileio.sllvm.msp430.ll: CLANGFLAGS+=-DTEST_DATA=\""hello"\"

fileio: MSP430LDFLAGS+=-ldev-fileio

fileio: sancus.o
fileio: fileio.sllvm.msp430.sancus.o
	ln -f msp430-sllvm-text.x msp430-elf-sllvm-text.x
	ln -f msp430-sllvm-data.x msp430-elf-sllvm-data.x
	${MSP430LD} $^ ${MSP430LDFLAGS} -o $@

fileio.objdump: fileio
	${MSP430OBJDUMP} -D fileio > $@

#----------------------------------------------------------------------------

arith: sancus.o
arith: foo.sllvm.msp430.sancus.o
arith: bar.sllvm.msp430.sancus.o
arith: main.sllvm.msp430.sancus.o
	ln -f msp430-arith-text.x msp430-elf-sllvm-text.x
	ln -f msp430-arith-data.x msp430-elf-sllvm-data.x
	${MSP430LD} $^ ${MSP430LDFLAGS} -o $@

arith.objdump: arith
	${MSP430OBJDUMP} -D $< > $@

#----------------------------------------------------------------------------

hello: sancus.o
hello: hello.sllvm.msp430.sancus.o
	ln -f msp430-sllvm-text.x msp430-elf-sllvm-text.x
	ln -f msp430-sllvm-data.x msp430-elf-sllvm-data.x
	${MSP430LD} $^ ${MSP430LDFLAGS} -o $@

hello.objdump: hello
	${MSP430OBJDUMP} -D hello > $@

#----------------------------------------------------------------------------

hello2: sancus.o
hello2: hello.sllvm.msp430.sancus.o
hello2: enclave4.sllvm.msp430.sancus.o
hello2: panic.o
	ln -f msp430-sllvm-text.x msp430-elf-sllvm-text.x
	ln -f msp430-sllvm-data.x msp430-elf-sllvm-data.x
	${MSP430LD} ${MSP430LDFLAGS} $^ -lsm-io -o $@

hello2.objdump: hello2
	${MSP430OBJDUMP} -D hello2 > $@

.PHONY: enclave1-diff
enclave1-diff:
	${DIFF} enclave1.ll enclave1.sllvm.sancus.ll

.PHONY: force
force:
	${MAKE} clean
	${MAKE}

.PHONY: sim
sim: ${SIM}
	${SSIM} ${SSIMFLAGS} ${SIM}

.PHONY: clean
clean:
	${RM} *.ll
	${RM} *.bc
	${RM} *sllvm*.s
	${RM} *legacy*.s
	${RM} enclave1.s
	${RM} enclave2.s
	${RM} main.s
	${RM} *.o
	${RM} *.objdump
	${RM} a.out
	${RM} hello2
	${RM} test
	${RM} test2
	${RM} sancus_sim.fst
	${RM} sancus_sim.fst.hier
	${RM} sim-input.bin
	${RM} sim-output.bin
