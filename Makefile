CMAKE_FLAGS_SLLVM =

-include Makefile.local

MAKEFILE_DIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

BUILD_TYPE ?= Release # One of (Debug, Release)
JOBS       ?= 1
SANCUS_KEY ?= deadbeefcafebabec0defeeddefec8ed

BUILDDIR   ?= $(MAKEFILE_DIR)build
INSTALLDIR ?= $(MAKEFILE_DIR)install

CMAKE_GENERATOR ?= Unix Makefiles

#############################################################################

SANCUS_SECURITY = $(shell echo $$(($$(echo -n $(SANCUS_KEY) | wc -m) * 4)))

DEPS =
DEPS += curl
DEPS += cmake
DEPS += g++
DEPS += texinfo
# Python 2 (required by vcdcat)
#DEPS += python
#DEPS += python-pip
# Python 3
DEPS += python3
DEPS += python3-pip
DEPS += python3-numpy
DEPS += gcc-msp430
DEPS += expect
DEPS += tcl
DEPS += iverilog
DEPS += gtkwave
DEPS += wget
DEPS += unzip

PIP3S =
PIP3S += vcdvcd # for vcdcat
PIP3S += pyelftools
PIP3S += matplotlib
PIP3S += mplcursors # for adding annotations to matplotlib grahps

WGET  = wget
GIT   = git
TAR   = tar
UNZIP = unzip
SH    = bash
MKDIR = mkdir -p
CMAKE = cmake
APT   = apt
PIP2  = pip2
PIP3  = pip3
DPKG  = dpkg

LIT = $(BUILDDIR_SLLVM)/bin/llvm-lit

#############################################################################

DISTRINET_DEB_URL = https://distrinet.cs.kuleuven.be/software/sancus/downloads
CLANG_SANCUS_DEB  = clang-sancus_4.0.1-2_amd64.deb

LLVM_REPO          = https://github.com/llvm/llvm-project
LLVM_FORK          = https://github.com/hanswinderix/llvm-project
LEGACY_SANCUS_REPO = https://github.com/sancus-pma/sancus-main
LEGACY_SANCUS_FORK = https://github.com/hanswinderix/sancus-main
VULCAN_REPO        = https://github.com/sancus-pma/vulcan
VULCAN_FORK        = https://github.com/hanswinderix/vulcan

# See http://www.ti.com/tool/MSP430-GCC-OPENSOURCE
TI_MSPGCC_DIR         = 9_3_0_1
TI_MSPGCC_URL	        = https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/$(TI_MSPGCC_DIR)/export
TI_MSPGCC_VER         = 9.3.0.31
TI_MSPGCC_SUPPORT_VER = 1.211
TI_MSPGCC_NAME        = msp430-gcc-$(TI_MSPGCC_VER)-source-patches
TI_MSPGCC_TBZ         = $(TI_MSPGCC_NAME).tar.bz2
TI_MSPGCC_SUPPORT     = msp430-gcc-support-files
TI_MSPGCC_SUPPORT_ZIP = $(TI_MSPGCC_SUPPORT)-$(TI_MSPGCC_SUPPORT_VER).zip

SRCDIR_SANCUS          = $(MAKEFILE_DIR)sancus
SRCDIR_VULCAN          = $(MAKEFILE_DIR)vulcan
SRCDIR_LEGACY_SANCUS   = $(MAKEFILE_DIR)sancus-main
SRCDIR_SANCUS_CORE     = $(SRCDIR_LEGACY_SANCUS)/sancus-core
SRCDIR_SANCUS_COMPILER = $(SRCDIR_LEGACY_SANCUS)/sancus-compiler
SRCDIR_SANCUS_SUPPORT  = $(SRCDIR_LEGACY_SANCUS)/sancus-support
SRCDIR_SANCUS_EXAMPLES = $(SRCDIR_LEGACY_SANCUS)/sancus-examples
SRCDIR_SLLVM           = $(MAKEFILE_DIR)sllvm
SRCDIR_MSPGCC          = $(MAKEFILE_DIR)$(TI_MSPGCC_NAME)
SRCDIR_GCC             = $(SRCDIR_MSPGCC)/gcc
SRCDIR_BINUTILS        = $(SRCDIR_MSPGCC)/binutils
SRCDIR_NEWLIB          = $(SRCDIR_MSPGCC)/newlib
SRCDIR_TEST            = $(MAKEFILE_DIR)test
SRCDIR_TEST_SANCUS     = $(SRCDIR_TEST)/sancus

BUILDDIR_SLLVM    = $(BUILDDIR)/sllvm
BUILDDIR_GCC      = $(BUILDDIR)/gcc
BUILDDIR_BINUTILS = $(BUILDDIR)/binutils
BUILDDIR_GDB      = $(BUILDDIR)/gdb

BUILDDIR_SANCUS_CORE     = $(BUILDDIR)/sancus-core
BUILDDIR_SANCUS_COMPILER = $(BUILDDIR)/sancus-compiler
BUILDDIR_SANCUS_SUPPORT  = $(BUILDDIR)/sancus-support

CMAKE_FLAGS_SLLVM += -G "$(CMAKE_GENERATOR)"
ifeq ($(CMAKE_GENERATOR), Ninja)
CMAKE_FLAGS_SLLVM += -DLLVM_PARALLEL_COMPILE_JOBS=$(JOBS)
CMAKE_FLAGS_SLLVM += -DLLVM_PARALLEL_LINK_JOBS=2
endif
CMAKE_FLAGS_SLLVM += -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
CMAKE_FLAGS_SLLVM += -DCMAKE_INSTALL_PREFIX=$(INSTALLDIR)
#CMAKE_FLAGS_SLLVM += -DLLVM_TARGETS_TO_BUILD="MSP430;RISCV"
CMAKE_FLAGS_SLLVM += -DLLVM_TARGETS_TO_BUILD="MSP430"
CMAKE_FLAGS_SLLVM += -DLLVM_ENABLE_PROJECTS="clang"
CMAKE_FLAGS_SLLVM += -DLLVM_USE_LINKER=gold
CMAKE_FLAGS_SLLVM += -DLLVM_ENABLE_PLUGINS=ON
CMAKE_FLAGS_SLLVM += -DLLVM_USE_NEWPM=OFF
#CMAKE_FLAGS_SLLVM += -DLLVM_TARGETS_TO_BUILD="MSP430;X86"
#CMAKE_FLAGS_SLLVM += -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
#CMAKE_FLAGS_SLLVM += -DLLVM_ENABLE_ASSERTIONS=ON
#For DEBUG builds, use shared libs, unless you have a lot of memory
#CMAKE_FLAGS_SLLVM += -DBUILD_SHARED_LIBS=ON
#CMAKE_FLAGS_SLLVM += -DLLVM_BUILD_LLVM_DYLIB=ON
#CMAKE_FLAGS_SLLVM += -DLLVM_CCACHE_BUILD=ON

CMAKE_FLAGS_SANCUS_COMMON =
CMAKE_FLAGS_SANCUS_COMMON += -DCMAKE_INSTALL_PREFIX=$(INSTALLDIR)

CMAKE_FLAGS_SANCUS_CORE =
CMAKE_FLAGS_SANCUS_CORE += $(CMAKE_FLAGS_SANCUS_COMMON)
CMAKE_FLAGS_SANCUS_CORE += -DSECURITY=$(SANCUS_SECURITY)
CMAKE_FLAGS_SANCUS_CORE += -DMASTER_KEY=$(SANCUS_KEY)

CMAKE_FLAGS_SANCUS_COMPILER =
CMAKE_FLAGS_SANCUS_COMPILER += $(CMAKE_FLAGS_SANCUS_COMMON)
CMAKE_FLAGS_SANCUS_COMPILER += -DMSP430_GCC_PREFIX=msp430-elf
CMAKE_FLAGS_SANCUS_COMPILER += -DSLLVM_INSTALL_DIR=$(INSTALLDIR)
CMAKE_FLAGS_SANCUS_COMPILER += -DSECURITY=$(SANCUS_SECURITY)
#CMAKE_FLAGS_SANCUS_COMPILER += -DMASTER_KEY=$(SANCUS_KEY)

CMAKE_FLAGS_SANCUS_SUPPORT =
CMAKE_FLAGS_SANCUS_SUPPORT += $(CMAKE_FLAGS_SANCUS_COMMON)
CMAKE_FLAGS_SANCUS_SUPPORT += -DMSP430_GCC_PREFIX=msp430-elf
CMAKE_FLAGS_SANCUS_SUPPORT += -DSLLVM_INSTALL_DIR=$(INSTALLDIR)
#CMAKE_FLAGS_SANCUS_SUPPORT += -DSECURITY=$(SANCUS_SECURITY)
#CMAKE_FLAGS_SANCUS_SUPPORT += -DMASTER_KEY=$(SANCUS_KEY)

CONFIGURE_FLAGS_COMMON =
CONFIGURE_FLAGS_COMMON += --target=msp430-elf
CONFIGURE_FLAGS_COMMON += --enable-languages=c,c++
CONFIGURE_FLAGS_COMMON += --disable-nls
CONFIGURE_FLAGS_COMMON += --prefix=$(INSTALLDIR)

CONFIGURE_FLAGS_BINUTILS =
CONFIGURE_FLAGS_BINUTILS += $(CONFIGURE_FLAGS_COMMON)
CONFIGURE_FLAGS_BINUTILS += --disable-sim
CONFIGURE_FLAGS_BINUTILS += --disable-gdb
CONFIGURE_FLAGS_BINUTILS += --disable-werror

CONFIGURE_FLAGS_GCC =
CONFIGURE_FLAGS_GCC += $(CONFIGURE_FLAGS_COMMON)
CONFIGURE_FLAGS_GCC += --enable-target-optspace
CONFIGURE_FLAGS_GCC += --enable-newlib-nano-formatted-io

CONFIGURE_FLAGS_GDB =
CONFIGURE_FLAGS_GDB += $(CONFIGURE_FLAGS_COMMON)
CONFIGURE_FLAGS_GDB += --disable-binutils
CONFIGURE_FLAGS_GDB += --disable-gas
CONFIGURE_FLAGS_GDB += --disable-ld
CONFIGURE_FLAGS_GDB += --disable-gprof
CONFIGURE_FLAGS_GDB += --disable-etc
CONFIGURE_FLAGS_GDB += --without-mpfr
CONFIGURE_FLAGS_GDB += --without-lzma
CONFIGURE_FLAGS_GDB += --with-python=no

MAKE_FLAGS_LEGACY_SANCUS =
MAKE_FLAGS_LEGACY_SANCUS += SANCUS_INSTALL_PREFIX=$(INSTALLDIR)
MAKE_FLAGS_LEGACY_SANCUS += SECURITY=$(SANCUS_SECURITY)
MAKE_FLAGS_LEGACY_SANCUS += MASTER_KEY=$(SANCUS_KEY)

#############################################################################

.PHONY: all
all:
	@echo BUILD_TYPE=$(BUILD_TYPE)
	@echo JOBS=$(JOBS)
	@echo CMAKE_GENERATOR=$(CMAKE_GENERATOR)

.PHONY: install-deps
install-deps:
	$(APT) install $(DEPS) -y
	$(PIP3) install $(PIP3S)
# TODO: Currently, the 'build-legacy-sancus-compiler' target needs
#       clang-sancus. This dependency should be removed as SLLVM *probably*
#       only needs the sm_support.h header from sancus-compiler header which
#       is required to compile the Sancus examples.
	$(RM) $(CLANG_SANCUS_DEB)
	$(WGET) $(DISTRINET_DEB_URL)/$(CLANG_SANCUS_DEB)
	$(DPKG) -i $(CLANG_SANCUS_DEB)


.PHONY: fetch
fetch: fetch-mspgcc
fetch: fetch-legacy-sancus
#fetch: fetch-vulcan
fetch: fetch-sllvm

.PHONY: configure
configure: configure-mspgcc
configure: configure-legacy-sancus
#configure: configure-vulcan
configure: configure-sllvm

.PHONY: install
install:
	$(MKDIR) $(INSTALLDIR)
	$(MAKE) install-mspgcc
	$(MAKE) install-sllvm
	$(MAKE) install-legacy-sancus
	$(MAKE) install-crypto

.PHONY: test
test:	test-sllvm
test:	test-nemdef
test:	test-sancus-examples
#test:	test-vulcan

#############################################################################

.PHONY: fetch-mpsgcc
fetch-mspgcc:
	$(WGET) $(TI_MSPGCC_URL)/$(TI_MSPGCC_TBZ)
	$(TAR) -jxf $(TI_MSPGCC_TBZ)
	cd $(SRCDIR_MSPGCC) && $(SH) README-apply-patches.sh
	$(WGET) $(TI_MSPGCC_URL)/$(TI_MSPGCC_SUPPORT_ZIP)

.PHONY: fetch-legacy-sancus
fetch-legacy-sancus:
	$(GIT) clone $(LEGACY_SANCUS_FORK) $(SRCDIR_LEGACY_SANCUS)
	cd $(SRCDIR_LEGACY_SANCUS) && $(GIT) remote add upstream $(LEGACY_SANCUS_REPO)
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) sancus-core
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) sancus-compiler
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) sancus-support
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) sancus-examples

.PHONY: fetch-vulcan
fetch-vulcan:
	$(GIT) clone $(VULCAN_FORK) $(SRCDIR_VULCAN)
	cd $(SRCDIR_VULCAN) && $(GIT) remote add upstream $(VULCAN_REPO)

.PHONY: fetch-sllvm
fetch-sllvm:
	$(GIT) clone --depth 1 --no-single-branch --progress $(LLVM_FORK) $(SRCDIR_SLLVM)
	cd $(SRCDIR_SLLVM) && $(GIT) remote add upstream $(LLVM_REPO)

# Based on $(SRCDIR_MSPGCC)/README-build.sh
.PHONY: configure-mspgcc
configure-mspgcc:
	cd $(SRCDIR_GCC) && $(SH) ./contrib/download_prerequisites
	ln -fns $(SRCDIR_NEWLIB)/newlib $(SRCDIR_GCC)/newlib
	ln -fns $(SRCDIR_NEWLIB)/libgloss $(SRCDIR_GCC)/libgloss
	$(MKDIR) $(BUILDDIR_BINUTILS)
	$(MKDIR) $(BUILDDIR_GCC)
	$(MKDIR) $(BUILDDIR_GDB)
	cd $(BUILDDIR_BINUTILS) && \
		$(SRCDIR_BINUTILS)/configure $(CONFIGURE_FLAGS_BINUTILS)
	cd $(BUILDDIR_GCC) && $(SRCDIR_GCC)/configure $(CONFIGURE_FLAGS_GCC)

.PHONY: configure-legacy-sancus
configure-legacy-sancus:

.PHONY: configure-vulcan
configure-vulcan:

.PHONY: configure-sllvm
configure-sllvm:
	$(MKDIR) $(BUILDDIR_SLLVM)
	cd $(BUILDDIR_SLLVM) && $(CMAKE) $(CMAKE_FLAGS_SLLVM) $(SRCDIR_SLLVM)/llvm

.PHONY: configure-sllvm-for-eclipse
configure-sllvm-for-eclipse: CMAKE_GENERATOR=Eclipse CDT4 - Unix Makefiles
configure-sllvm-for-eclipse:
	$(MKDIR) $(BUILDDIR_SLLVM)-eclipse
	cd $(BUILDDIR_SLLVM)-eclipse && \
	  $(CMAKE) $(CMAKE_FLAGS_SLLVM) $(SRCDIR_SLLVM)/llvm

.PHONY: build-mspgcc-binutils
build-mspgcc-binutils:
	$(MAKE) -C $(BUILDDIR_BINUTILS) -j$(JOBS)

.PHONY: build-mspgcc-gcc
build-mspgcc-gcc:
	$(MKDIR) $(INSTALLDIR)
# Requires previously installed mspgcc binutils, hence "export PATH"
	export PATH=$(INSTALLDIR)/bin:$$PATH; $(MAKE) -C $(BUILDDIR_GCC) -j$(JOBS)

.PHONY: build-sancus-core
build-sancus-core:
	$(MKDIR) $(BUILDDIR_SANCUS_CORE)
	cd $(BUILDDIR_SANCUS_CORE) && \
		$(CMAKE) $(CMAKE_FLAGS_SANCUS_CORE) $(SRCDIR_SANCUS_CORE)

.PHONY: build-sancus-support
build-sancus-support:
	$(MKDIR) $(BUILDDIR_SANCUS_SUPPORT)
	cd $(BUILDDIR_SANCUS_SUPPORT) && \
		$(CMAKE) $(CMAKE_FLAGS_SANCUS_SUPPORT) $(SRCDIR_SANCUS_SUPPORT)

.PHONY: build-legacy-sancus-compiler
build-legacy-sancus-compiler:
	$(MKDIR) $(BUILDDIR_SANCUS_COMPILER)
# Requires previously installed mspgcc binutils, hence "export PATH"
	export PATH=$(INSTALLDIR)/bin:$$PATH; cd $(BUILDDIR_SANCUS_COMPILER) && \
		$(CMAKE) $(CMAKE_FLAGS_SANCUS_COMPILER) $(SRCDIR_SANCUS_COMPILER)

.PHONY: build-vulcan
build-vulcan:
	$(MAKE) SLLVM_INSTALL_DIR=$(INSTALLDIR) -C $(SRCDIR_VULCAN)

.PHONY: build-sllvm
build-sllvm:
ifneq ($(CMAKE_GENERATOR), Ninja)
	$(CMAKE) --build $(BUILDDIR_SLLVM) -- -j$(JOBS)
else
	$(CMAKE) --build $(BUILDDIR_SLLVM)
endif

.PHONY: install-mspgcc
install-mspgcc:
	$(MKDIR) $(INSTALLDIR)
	$(MAKE) install-mspgcc-binutils
	$(MAKE) install-mspgcc-gcc
	$(MAKE) install-mspgcc-support-files

.PHONY: install-mspgcc-binutils
install-mspgcc-binutils: build-mspgcc-binutils
	$(MAKE) -C $(BUILDDIR_BINUTILS) install

.PHONY: install-mspgcc-gcc
install-mspgcc-gcc: build-mspgcc-gcc
# Requires previously installed mspgcc binutils, hence "export PATH"
	export PATH=$(INSTALLDIR)/bin:$$PATH; $(MAKE) -C $(BUILDDIR_GCC) install

.PHONY: install-mspgcc-support-files
install-mspgcc-support-files:
	$(RM) -r $(TI_MSPGCC_SUPPORT)
	$(UNZIP) $(TI_MSPGCC_SUPPORT_ZIP)
	$(MKDIR) $(INSTALLDIR)/include
	cp -R $(TI_MSPGCC_SUPPORT)/include/* $(INSTALLDIR)/include
# Overwrite the msp430.h and devices.csv support files of
#  $(TI_MSPGCC_SUPPORT_ZIP) with versions that include support for the Sancus
#  security architecture.
	cp $(SRCDIR_SANCUS)/$(TI_MSPGCC_SUPPORT)/devices.csv             \
		$(SRCDIR_SANCUS)/$(TI_MSPGCC_SUPPORT)/msp430.h                 \
		$(SRCDIR_SANCUS)/$(TI_MSPGCC_SUPPORT)/msp430sancus.h           \
		$(SRCDIR_SANCUS)/$(TI_MSPGCC_SUPPORT)/msp430sancus.ld          \
		$(SRCDIR_SANCUS)/$(TI_MSPGCC_SUPPORT)/msp430sancus_symbols.ld  \
		$(SRCDIR_SANCUS)/$(TI_MSPGCC_SUPPORT)/memory.ld                \
		$(INSTALLDIR)/include
# Provide empty data and text ld files for compilation units without
# protected modules
	touch $(INSTALLDIR)/include/text.ld
	touch $(INSTALLDIR)/include/data.ld
	touch $(INSTALLDIR)/include/symbols.ld

.PHONY: install-legacy-sancus
install-legacy-sancus:
	$(MKDIR) $(INSTALLDIR)
	$(MAKE) install-sancus-core
	$(MAKE) install-legacy-sancus-compiler
	$(MAKE) install-sancus-support

.PHONY: install-sancus-core
install-sancus-core: build-sancus-core
	$(CMAKE) --build $(BUILDDIR_SANCUS_CORE) --target install

.PHONY: install-legacy-sancus-compiler
install-legacy-sancus-compiler: build-legacy-sancus-compiler
	$(CMAKE) --build $(BUILDDIR_SANCUS_COMPILER) --target install

.PHONY: install-sancus-support
install-sancus-support: build-sancus-support
	$(CMAKE) --build $(BUILDDIR_SANCUS_SUPPORT) --target install

# TODO: Install crypto script in other directory ?
.PHONY: install-crypto
install-crypto:
	cp $(SRCDIR_SANCUS)/crypto/*.py $(INSTALLDIR)/bin/
	chmod +x $(INSTALLDIR)/bin/crypto.py
	echo "libname = '$(INSTALLDIR)/share/sancus-compiler/libsancus-crypto.so'" \
	                                               >> $(INSTALLDIR)/bin/config.py

.PHONY: install-sllvm
install-sllvm: build-sllvm
	$(CMAKE) --build $(BUILDDIR_SLLVM) --target install

.PHONY: install-and-test-sancus-legacy
install-and-test-sancus-legacy:
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) install
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) test

.PHONY: test-sancus-examples
test-sancus-examples:
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS)   \
		SLLVM_INSTALL_DIR=$(INSTALLDIR)    \
		SANCUS_SECURITY=$(SANCUS_SECURITY) \
		examples-sim

.PHONY: test-sllvm
test-sllvm: clean-test-sllvm
	$(MAKE) test-nemdef
	$(MAKE) -C $(SRCDIR_TEST_SANCUS) SANCUS_KEY=$(SANCUS_KEY) sim
	$(MAKE) -C $(SRCDIR_TEST_SANCUS) nemdef-pp

.PHONY: test-nemdef
TEST_DIR = $(SRCDIR_SLLVM)/llvm/test/CodeGen/MIR/MSP430
test-nemdef:
	$(LIT) $(TEST_DIR)

.PHONY: test-nemdefa
test-nemdefa:
	$(LIT) -a $(TEST_DIR)

.PHONY: test-nemdefvv
test-nemdefvv:
	$(LIT) -vv $(TEST_DIR)

.PHONY: test-vulcan
test-vulcan: build-vulcan
	$(MAKE) SLLVM_INSTALL_DIR=$(INSTALLDIR) -C $(SRCDIR_VULCAN) sim

.PHONY: clean
clean: clean-vulcan
clean: clean-test-sllvm
	$(RM) -r $(BUILDDIR)
# TODO: LLVMConfig has to be removed to avoid a conflict with the legacy Sancus
#        installation when configuring the legacy Sancus compiler. This conflict
#        should be fixed.
	$(RM) $(INSTALLDIR)/lib/cmake/llvm/LLVMConfig.cmake

.PHONY: clean-test-sllvm
clean-test-sllvm:
	$(MAKE) -C $(SRCDIR_TEST_SANCUS) clean

.PHONY: clean-vulcan
clean-vulcan:
	$(MAKE) -C $(SRCDIR_VULCAN) clean

.PHONY: clean-fetch
clean-fetch: clean
	$(RM) $(TI_MSPGCC_TBZ)
	$(RM) -r $(SRCDIR_MSPGCC)
	$(RM) $(TI_MSPGCC_SUPPORT_ZIP)
	$(RM) -r $(TI_MSPGCC_SUPPORT)
	$(RM) -r $(SRCDIR_SLLVM)
	$(RM) -r $(SRCDIR_LEGACY_SANCUS)
	$(RM) -r $(SRCDIR_VULCAN)
	$(RM) $(CLANG_SANCUS_DEB)

.PHONY: clean-then-install
clean-then-install:
	$(MAKE) clean
	$(MAKE) configure
	$(MAKE) install

.PHONY: clean-fetch-install
clean-fetch-install:
	$(MAKE) clean-fetch
	$(MAKE) fetch
	$(MAKE) clean-then-install

.PHONY: uninstall
uninstall:
	$(RM) -r $(INSTALLDIR)

.PHONY: realclean
realclean:
	$(MAKE) clean
	$(MAKE) uninstall
	$(MAKE) -C $(SRCDIR_TEST_SANCUS) clean
	$(MAKE) -C $(SRCDIR_TEST_SANCUS) nemdef-clean

.PHONY: realclean-then-install
realclean-then-install:
	$(MAKE) realclean
	$(MAKE) clean-then-install

#############################################################################

.PHONY: status
status:
	$(GIT) status -sb
	-$(GIT) -C $(SRCDIR_LEGACY_SANCUS)   status -sb
	-$(GIT) -C $(SRCDIR_SANCUS_CORE)     status -sb
	-$(GIT) -C $(SRCDIR_SANCUS_COMPILER) status -sb
	-$(GIT) -C $(SRCDIR_SANCUS_SUPPORT)  status -sb
	-$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) status -sb
#	-$(GIT) -C $(SRCDIR_VULCAN)          status -sb
	-$(GIT) -C $(SRCDIR_SLLVM)           status -sb

.PHONY: pull
pull: pull-sancus
pull: pull-sllvm
#pull: pull-vulcan
	$(GIT) pull

.PHONY: pull-sancus
pull-sancus:
	$(GIT) -C $(SRCDIR_LEGACY_SANCUS)   pull
	$(GIT) -C $(SRCDIR_SANCUS_CORE)     pull
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) pull
	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT)  pull
	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) pull

.PHONY: pull-vulcan
pull-vulcan:
	$(GIT) -C $(SRCDIR_VULCAN) pull

.PHONY: pull-sllvm
pull-sllvm:
	$(GIT) -C $(SRCDIR_SLLVM) pull

.PHONY: push
push: push-sllvm
push: push-legacy-sancus
push: push-vulcan
	$(GIT) push

.PHONY: push-sllvm
push-sllvm:
	$(GIT) -C $(SRCDIR_SLLVM) push

.PHONY: push-legacy-sancus
push-legacy-sancus:
	$(GIT) -C $(SRCDIR_LEGACY_SANCUS)   push
#$(GIT) -C $(SRCDIR_SANCUS_CORE)     push
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) push
	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT)  push
	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) push

.PHONY: push-vulcan
push-vulcan:
	$(GIT) -C $(SRCDIR_VULCAN) push

.PHONY: diff
diff:
	$(GIT) difftool
	$(GIT) -C $(SRCDIR_SLLVM) difftool
	$(GIT) -C $(SRCDIR_LEGACY_SANCUS)   difftool
#$(GIT) -C $(SRCDIR_SANCUS_CORE)     difftool
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) difftool
	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT)  difftool
	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) difftool
	$(GIT) -C $(SRCDIR_VULCAN) difftool

.PHONY: diff-sllvm
diff-sllvm:
	$(GIT) -C $(SRCDIR_SLLVM) difftool

# EuroS&P 2021 submission
.PHONY: checkout-sllvm-0.9b
checkout-sllvm-0.9b:
	$(GIT) checkout sllvm-0.9b
	$(GIT) -C $(SRCDIR_SLLVM) checkout sllvm-0.9b

.PHONY: checkout-master
checkout-master:
	$(GIT) checkout master
	$(GIT) -C $(SRCDIR_SLLVM) checkout master

.PHONY: checkout-dma-attack
checkout-dma-attack:
	$(GIT) checkout dma-attack
	$(GIT) -C $(SRCDIR_SLLVM) checkout dma-attack

.PHONY: sync
sync: sync-llvm
sync: sync-legacy-sancus
sync: sync-vulcan

.PHONY: sync-llvm
sync-llvm:
	$(GIT) -C $(SRCDIR_SLLVM) fetch upstream
	$(GIT) -C $(SRCDIR_SLLVM) checkout master
	$(GIT) -C $(SRCDIR_SLLVM) merge upstream/main
	#(GIT) -C $(SRCDIR_SLLVM) rebase upstream/main

.PHONY: sync-legacy-sancus
sync-legacy-sancus:
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) fetch upstream
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) checkout master
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) merge upstream/master
	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT) fetch upstream
	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT) checkout master
#	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT) merge upstream/master
	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) fetch upstream
	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) checkout master
#	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) merge upstream/master

.PHONY: sync-vulcan
sync-vulcan:
	$(GIT) -C $(SRCDIR_VULCAN) fetch upstream
	$(GIT) -C $(SRCDIR_VULCAN) checkout master
#	$(GIT) -C $(SRCDIR_VULCAN) merge upstream/master
