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

# Nemdef unit tests (have high code coverage)
NEMDEF =
NEMDEF += triangle
NEMDEF += diamond
#NEMDEF += fork
#NEMDEF += multifork
NEMDEF += call
NEMDEF += indirect
NEMDEF += loop
NEMDEF += ifthenloop
NEMDEF += ifthenloopif
NEMDEF += ifthenlooploop
NEMDEF += ifthenlooplooptail

# Compiler runtime functions (e.g. Routines for integer arithmetic)
#   libgcc (GCC), compiler-rt (LLVM), ...
#NEMDEF += mulhi3

# Crytpo primitives
#NEMDEF +=

# Sancus applications
NEMDEF += bsl
#NEMDEF += keypad   => Support "secret return types" (or use dummy param)

#--------------------------------------------------------------------------

NEMDEF_ALL   = $(NEMDEF:=.nemdef-all)
NEMDEF_SIM   = $(NEMDEF:=.nemdef-sim)
NEMDEF_CLEAN = $(NEMDEF:=.nemdef-clean)

nemdef-all  : $(NEMDEF_ALL)
nemdef-sim  : $(NEMDEF_SIM)
nemdef-clean: $(NEMDEF_CLEAN)

$(NEMDEF_ALL): %.nemdef-all:
	$(MAKE) -C $* -f Makefile.vulnerable all
	$(MAKE) -C $* -f Makefile.nemdef     all

$(NEMDEF_SIM): %.nemdef-sim:
	$(MAKE) -C $* -f Makefile.vulnerable sim
	$(MAKE) -C $* -f Makefile.nemdef     sim

$(NEMDEF_CLEAN): %.nemdef-clean:
	$(MAKE) -C $* -f Makefile.vulnerable clean
	$(MAKE) -C $* -f Makefile.nemdef     clean
