##############################################################"
# Unit tests
##############################################################"

TESTS =
TESTS += multiply
TESTS += hello

TEST_ALL   = $(TESTS:=.all)
TEST_SIM   = $(TESTS:=.sim)
TEST_CLEAN = $(TESTS:=.clean)

all  : $(TEST_ALL)
sim  : $(TEST_SIM)
clean: $(TEST_CLEAN)

$(TEST_ALL): %.all:
	$(MAKE) -C $* all

$(TEST_SIM): %.sim:
	$(MAKE) -C $* sim

$(TEST_CLEAN): %.clean:
	$(MAKE) -C $* clean

##############################################################"
# Nemesis Defender benchmarks
##############################################################"

# 1) Nemdef unit tests (have high code coverage)
NEMDEF =
NEMDEF += triangle
NEMDEF += diamond
NEMDEF += fork
NEMDEF += multifork
NEMDEF += call
NEMDEF += indirect
NEMDEF += ifcompound
NEMDEF += loop
NEMDEF += ifthenloop
NEMDEF += ifthenloopif
NEMDEF += ifthenlooploop
NEMDEF += ifthenlooplooptail

# 2) Compiler runtime functions (e.g. Routines for integer arithmetic)
#   o libgcc      (GCC) : gcc-8.2.0/libgcc/config/msp430/mpy.c
#                        used by MSP430 GCC
#                        (MSPGCC has a similar timing channel [Cuyt])
#   o compiler-rt (LLVM): has no support for MSP430
NEMDEF += mulhi3

# 3) Primitives from cryptographic libraries
# [Agat, 2000]
#NEMDEF +=
# [Molnar, 2005] (section 5.1)
#NEMDEF +=
# [Coppens, 2009] (section 4)
# "The following is an example in pseudocode based on Applied Cryptography by
#   Bruce Schneier." [Wikipedia]
NEMDEF += modexp
# [Mantel, 2015] (sections 2 and 4)
#NEMDEF +=
# [Almeida, 2016]
#NEMDEF +=
# [Dewald, 2017] (section 6.2) => source code is in paper
#NEMDEF +=
# [Wu, 2018] (section 7)
#NEMDEF += threeway   # Useless since LLVM generates CT code for mu() :-/
# [Simon, 2018] (section 4.1.2)
# OpenSSL, mbedTLS
#NEMDEF +=
# [Cauligi, 2019] (section 5.1)
#NEMDEF +=

# 4) Sancus applications (inspired on applications from Nemesis paper)
NEMDEF += bsl
NEMDEF += keypad   # TODO: Support "secret return types" (or use dummy param)

#--------------------------------------------------------------------------

NEMDEF_ALL   = $(NEMDEF:=.nemdef-all)
NEMDEF_SIM   = $(NEMDEF:=.nemdef-sim)
NEMDEF_PP    = $(NEMDEF:=.nemdef-pp)
NEMDEF_CLEAN = $(NEMDEF:=.nemdef-clean)
NEMDEF_FORCE = $(NEMDEF:=.nemdef-force)

nemdef-all  : $(NEMDEF_ALL)
nemdef-sim  : $(NEMDEF_SIM)
nemdef-pp   : $(NEMDEF_PP)
nemdef-clean: $(NEMDEF_CLEAN)
nemdef-force: $(NEMDEF_FORCE)

$(NEMDEF_ALL): %.nemdef-all:
	$(MAKE) -C $* -f Makefile.vulnerable all
	$(MAKE) -C $* -f Makefile.nemdef     all

$(NEMDEF_SIM): %.nemdef-sim:
	$(MAKE) -C $* -f Makefile.vulnerable sim
	$(MAKE) -C $* -f Makefile.nemdef     sim

$(NEMDEF_PP): %.nemdef-pp:
	$(MAKE) -C $* -f Makefile.vulnerable pp
	$(MAKE) -C $* -f Makefile.nemdef     pp

$(NEMDEF_CLEAN): %.nemdef-clean:
	$(MAKE) -C $* -f Makefile.vulnerable clean
	$(MAKE) -C $* -f Makefile.nemdef     clean

$(NEMDEF_FORCE): %.nemdef-force:
	$(MAKE) $*.nemdef-clean
	$(MAKE) $*.nemdef-all
