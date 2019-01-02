-include Makefile.local

BUILD_TYPE ?= Release # One of (Debug, Release)
JOBS       ?= 1

SANCUS_SECURITY ?= 64
SANCUS_KEY      ?= deadbeefcafebabe

WGET  = wget
GIT   = git
TAR   = tar
UNZIP = unzip
SH    = bash
MKDIR = mkdir -p
CMAKE = cmake
APT   = apt

BUILDDIR   = $(PWD)/build
INSTALLDIR = $(PWD)/install

LLVM_REPO          = https://github.com/llvm-mirror/llvm.git
LLVM_FORK          = https://github.com/hanswinderix/llvm.git
CLANG_REPO         = https://github.com/llvm-mirror/clang.git
CLANG_FORK         = https://github.com/hanswinderix/clang.git
LEGACY_SANCUS_REPO = https://github.com/sancus-pma/sancus-main.git
LEGACY_SANCUS_FORK = https://github.com/hanswinderix/sancus-main.git

# See http://www.ti.com/tool/MSP430-GCC-OPENSOURCE
TI_MSPGCC_URL         = http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/exports
TI_MSPGCC_VER         = 7.3.2.154
TI_MSPGCC_SUPPORT_VER = 1.206
TI_MSPGCC_NAME        = msp430-gcc-$(TI_MSPGCC_VER)-source-patches
TI_MSPGCC_TBZ         = $(TI_MSPGCC_NAME).tar.bz2
TI_MSPGCC_SUPPORT     = msp430-gcc-support-files
TI_MSPGCC_SUPPORT_ZIP = $(TI_MSPGCC_SUPPORT)-$(TI_MSPGCC_SUPPORT_VER).zip

SRCDIR_SANCUS          = $(PWD)/sancus
SRCDIR_LEGACY_SANCUS   = $(PWD)/sancus-main
SRCDIR_SANCUS_CORE     = $(SRCDIR_LEGACY_SANCUS)/sancus-core
SRCDIR_SANCUS_COMPILER = $(SRCDIR_LEGACY_SANCUS)/sancus-compiler
SRCDIR_SANCUS_SUPPORT  = $(SRCDIR_LEGACY_SANCUS)/sancus-support
SRCDIR_SANCUS_EXAMPLES = $(SRCDIR_LEGACY_SANCUS)/sancus-examples
SRCDIR_SLLVM           = $(PWD)/sllvm
SRCDIR_CLANG           = $(SRCDIR_SLLVM)/tools/clang
SRCDIR_MSPGCC          = $(PWD)/$(TI_MSPGCC_NAME)
SRCDIR_GCC             = $(SRCDIR_MSPGCC)/gcc
SRCDIR_BINUTILS        = $(SRCDIR_MSPGCC)/binutils
SRCDIR_NEWLIB          = $(SRCDIR_MSPGCC)/newlib

BUILDDIR_SLLVM    = $(BUILDDIR)/sllvm
BUILDDIR_GCC      = $(BUILDDIR)/gcc
BUILDDIR_BINUTILS = $(BUILDDIR)/binutils
BUILDDIR_GDB      = $(BUILDDIR)/gdb

BUILDDIR_SANCUS_CORE     = $(BUILDDIR)/sancus-core
BUILDDIR_SANCUS_COMPILER = $(BUILDDIR)/sancus-compiler
BUILDDIR_SANCUS_SUPPORT  = $(BUILDDIR)/sancus-support

CMAKE_FLAGS_SLLVM =
CMAKE_FLAGS_SLLVM += -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
CMAKE_FLAGS_SLLVM += -DCMAKE_INSTALL_PREFIX=$(INSTALLDIR)
CMAKE_FLAGS_SLLVM += -DLLVM_USE_LINKER=gold
CMAKE_FLAGS_SLLVM += -DLLVM_ENABLE_ASSERTIONS=ON
CMAKE_FLAGS_SLLVM += -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_FLAGS_SLLVM += -DLLVM_TARGETS_TO_BUILD=MSP430
#CMAKE_FLAGS_SLLVM += -DLLVM_TARGETS_TO_BUILD="MSP430;X86"

CMAKE_FLAGS_SANCUS_COMMON =
CMAKE_FLAGS_SANCUS_COMMON += -DCMAKE_INSTALL_PREFIX=$(INSTALLDIR)

CMAKE_FLAGS_SANCUS_CORE =
CMAKE_FLAGS_SANCUS_CORE += $(CMAKE_FLAGS_SANCUS_COMMON)
CMAKE_FLAGS_SANCUS_CORE += -DSECURITY=$(SANCUS_SECURITY)
CMAKE_FLAGS_SANCUS_CORE += -DMASTER_KEY=$(SANCUS_KEY)

CMAKE_FLAGS_SANCUS_COMPILER =
CMAKE_FLAGS_SANCUS_COMPILER += $(CMAKE_FLAGS_SANCUS_COMMON)
CMAKE_FLAGS_SANCUS_COMPILER += -DMSP430_GCC_PREFIX=msp430-elf
CMAKE_FLAGS_SANCUS_COMPILER += -DSECURITY=$(SANCUS_SECURITY)
#CMAKE_FLAGS_SANCUS_COMPILER += -DMASTER_KEY=$(SANCUS_KEY)

CMAKE_FLAGS_SANCUS_SUPPORT =
CMAKE_FLAGS_SANCUS_SUPPORT += $(CMAKE_FLAGS_SANCUS_COMMON)
CMAKE_FLAGS_SANCUS_SUPPORT += -DMSP430_GCC_PREFIX=msp430-elf
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

.PHONY: all
all: 
	@echo BUILD_TYPE=$(BUILD_TYPE)
	@echo JOBS=$(JOBS)

.PHONY: install
install:
	$(MKDIR) $(INSTALLDIR)
	$(MAKE) install-mspgcc-binutils
	$(MAKE) install-mspgcc-gcc
	$(MAKE) install-mspgcc-support-files
	$(MAKE) install-sancus-core
	$(MAKE) install-legacy-sancus-compiler
	$(MAKE) install-sancus-support
	$(MAKE) install-sllvm

.PHONY: install-after-clean
install-after-clean:
	$(MAKE) clean
	$(MAKE) configure
	$(MAKE) install

.PHONY: install-after-clean-fetch
install-after-clean-fetch:
	$(MAKE) clean-fetch
	$(MAKE) fetch
	$(MAKE) install-after-clean

.PHONY: install-from-scratch
install-from-scratch:
	$(MAKE) clean-fetch
	$(MAKE) fetch
	$(MAKE) install-deps
	$(MAKE) install-after-clean

# TODO: Currently, the 'build-legacy-sancus-compiler' target needs 
#       clang-sancus. This dependency should be removed as SLLVM *probably* 
#       only needs the sm_support.h header from sancus-compiler header which
#       is required to compile the Sancus examples.
.PHONY: install-deps
install-deps:
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) clang-sancus # TODO: Remove (see above)

.PHONY: install-and-test-sancus-legacy
install-and-test-sancus-legacy:
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) install
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) test

.PHONY: test
test:	test-sancus

.PHONY: test-sancus
test-sancus:
	$(MAKE) \
		SLLVM_INSTALL_DIR=$(INSTALLDIR) -C $(SRCDIR_LEGACY_SANCUS) examples-sim

.PHONY: fetch
fetch: fetch-mspgcc
fetch: fetch-legacy-sancus
fetch: fetch-sllvm

.PHONY: configure
configure: configure-mspgcc
configure: configure-sllvm

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

.PHONY: fetch-sllvm
fetch-sllvm:
	$(GIT) clone $(LLVM_FORK) $(SRCDIR_SLLVM)
	cd $(SRCDIR_SLLVM) && $(GIT) remote add upstream $(LLVM_REPO)
	$(GIT) clone $(CLANG_FORK) $(SRCDIR_CLANG)
	cd $(SRCDIR_CLANG) && $(GIT) remote add upstream $(CLANG_REPO)

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

.PHONY: configure-sllvm
configure-sllvm:
	$(MKDIR) $(BUILDDIR_SLLVM)
	cd $(BUILDDIR_SLLVM) && $(CMAKE) $(CMAKE_FLAGS_SLLVM) $(SRCDIR_SLLVM)

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

.PHONY: build-sllvm
build-sllvm:
	$(CMAKE) --build $(BUILDDIR_SLLVM) -- -j$(JOBS)

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
		$(INSTALLDIR)/include
# Provide empty data and text ld files for compilation units without
# protected modules
	touch ${INSTALLDIR}/include/text.ld
	touch ${INSTALLDIR}/include/data.ld

.PHONY: install-sancus-core
install-sancus-core: build-sancus-core
	$(CMAKE) --build $(BUILDDIR_SANCUS_CORE) --target install

.PHONY: install-legacy-sancus-compiler
install-legacy-sancus-compiler: build-legacy-sancus-compiler
	$(CMAKE) --build $(BUILDDIR_SANCUS_COMPILER) --target install

.PHONY: install-sancus-support
install-sancus-support: build-sancus-support
	$(CMAKE) --build $(BUILDDIR_SANCUS_SUPPORT) --target install

.PHONY: install-sllvm
install-sllvm: build-sllvm
	$(CMAKE) --build $(BUILDDIR_SLLVM) --target install

.PHONY: status
status:
	$(GIT) status -sb
	-$(GIT) -C $(SRCDIR_LEGACY_SANCUS)   status -sb
	-$(GIT) -C $(SRCDIR_SANCUS_CORE)     status -sb
	-$(GIT) -C $(SRCDIR_SANCUS_COMPILER) status -sb
	-$(GIT) -C $(SRCDIR_SANCUS_SUPPORT)  status -sb
	-$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) status -sb
	-$(GIT) -C $(SRCDIR_SLLVM)           status -sb
	-$(GIT) -C $(SRCDIR_CLANG)           status -sb

.PHONY: pull
pull: pull-sancus
pull: pull-sllvm

.PHONY: pull-sancus
pull-sancus:
	$(GIT) -C $(SRCDIR_LEGACY_SANCUS)   pull
	$(GIT) -C $(SRCDIR_SANCUS_CORE)     pull
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) pull
	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT)  pull
	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) pull

.PHONY: pull-sllvm
pull-sllvm:
	$(GIT) -C $(SRCDIR_SLLVM) pull 
	$(GIT) -C $(SRCDIR_CLANG) pull 

.PHONY: push-sllvm
push-sllvm:
	$(GIT) -C $(SRCDIR_SLLVM) push 
	$(GIT) -C $(SRCDIR_CLANG) push 

.PHONY: sync
sync: sync-llvm
sync: sync-legacy-sancus

.PHONY: sync-llvm
sync-llvm:
	$(GIT) -C $(SRCDIR_SLLVM) fetch upstream
	$(GIT) -C $(SRCDIR_SLLVM) checkout master
	$(GIT) -C $(SRCDIR_SLLVM) merge upstream/master
	$(GIT) -C $(SRCDIR_CLANG) fetch upstream
	$(GIT) -C $(SRCDIR_CLANG) checkout master
	$(GIT) -C $(SRCDIR_CLANG) merge upstream/master

.PHONY: sync-legacy-sancus
sync-legacy-sancus:
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) fetch upstream
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) checkout master
	$(GIT) -C $(SRCDIR_SANCUS_COMPILER) merge upstream/master
	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT) fetch upstream
	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT) checkout master
	$(GIT) -C $(SRCDIR_SANCUS_SUPPORT) merge upstream/master
	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) fetch upstream
	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) checkout master
	$(GIT) -C $(SRCDIR_SANCUS_EXAMPLES) merge upstream/master

# TODO: clean target should not delete the install folder
.PHONY: clean
clean:
	$(RM) -r build
	$(RM) -r install

.PHONY: clean-fetch
clean-fetch:
	$(RM) $(TI_MSPGCC_TBZ)
	$(RM) -r $(SRCDIR_MSPGCC)
	$(RM) $(TI_MSPGCC_SUPPORT_ZIP)
	$(RM) -r $(TI_MSPGCC_SUPPORT)
	$(RM) -r $(SRCDIR_SLLVM)
	$(RM) -r $(SRCDIR_LEGACY_SANCUS)
	$(RM) -r $(SRCDIR_CLANG)
