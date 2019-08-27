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

NEMDEF =
NEMDEF += triangle
#NEMDEF += fork
#NEMDEF += multifork
NEMDEF += diamond
NEMDEF += indirect
NEMDEF += loop
NEMDEF += ifthenloop

NEMDEF += bsl
#NEMDEF += keypad Suport "secret return types"

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
