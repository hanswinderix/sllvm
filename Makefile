-include Makefile.local

#BUILD_TYPE=Debug
BUILD_TYPE=Release
JOBS=4
SANCUS_SECURITY=64
SANCUS_KEY=deadbeefcafebabe

WGET  = wget
GIT   = git
TAR   = tar
UNZIP = unzip
SH    = bash
MKDIR = mkdir -p
CMAKE = cmake

BUILDDIR   = $(PWD)/build
INSTALLDIR = $(PWD)/install

LLVM_FORK          = https://github.com/hanswinderix/llvm.git
CLANG_FORK         = https://github.com/hanswinderix/clang.git
LEGACY_SANCUS_REPO = https://github.com/sancus-pma/sancus-main.git
LEGACY_SANCUS_FORK = https://github.com/hanswinderix/sancus-main.git

# See http://www.ti.com/tool/MSP430-GCC-OPENSOURCE
TI_MSPGCC_URL         = http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/exports
TI_MSPGCC_VER         = 7.3.2.154
TI_MSPGCC_VER_SUP     = 1.206
TI_MSPGCC_NAME        = msp430-gcc-$(TI_MSPGCC_VER)-source-patches
TI_MSPGCC_TBZ         = $(TI_MSPGCC_NAME).tar.bz2
TI_MSPGCC_SUPPORT     = msp430-gcc-support-files-$(TI_MSPGCC_VER_SUP)
TI_MSPGCC_SUPPORT_ZIP = $(TI_MSPGCC_SUPPORT).zip

SRCDIR_LEGACY_SANCUS   = $(PWD)/sancus-main
SRCDIR_SANCUS_COMPILER = $(SRCDIR_LEGACY_SANCUS)/sancus-compiler
SRCDIR_SANCUS_SUPPORT  = $(SRCDIR_LEGACY_SANCUS)/sancus-support
SRCDIR_LLVM            = $(PWD)/llvm
SRCDIR_CLANG           = $(SRCDIR_LLVM)/src/tools/clang
SRCDIR_MSPGCC          = $(PWD)/$(TI_MSPGCC_NAME)
SRCDIR_GCC             = $(SRCDIR_MSPGCC)/gcc
SRCDIR_BINUTILS        = $(SRCDIR_MSPGCC)/binutils
SRCDIR_NEWLIB          = $(SRCDIR_MSPGCC)/newlib

BUILDDIR_LLVM     = $(BUILDDIR)/llvm
BUILDDIR_GCC      = $(BUILDDIR)/gcc
BUILDDIR_BINUTILS = $(BUILDDIR)/binutils
BUILDDIR_GDB      = $(BUILDDIR)/gdb

BUILDDIR_SANCUS_COMPILER = $(BUILDDIR)/sancus-compiler
BUILDDIR_SANCUS_SUPPORT  = $(BUILDDIR)/sancus-support

CMAKE_FLAGS_LLVM =
CMAKE_FLAGS_LLVM += -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
CMAKE_FLAGS_LLVM += -DCMAKE_INSTALL_PREFIX=$(INSTALLDIR)
CMAKE_FLAGS_LLVM += -DLLVM_USE_LINKER=gold
CMAKE_FLAGS_LLVM += -DLLVM_ENABLE_ASSERTIONS=ON
CMAKE_FLAGS_LLVM += -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
#CMAKE_FLAGS_LLVM += -DLLVM_BUILD_TOOLS=OFF
CMAKE_FLAGS_LLVM += -DLLVM_TARGETS_TO_BUILD=MSP430
#CMAKE_FLAGS_LLVM +=-DLLVM_TARGETS_TO_BUILD="MSP430;X86"

CMAKE_FLAGS_SANCUS =
CMAKE_FLAGS_SANCUS += -DCMAKE_INSTALL_PREFIX=$(INSTALLDIR)
CMAKE_FLAGS_SANCUS += -DSECURITY=$(SANCUS_SECURITY)
CMAKE_FLAGS_SANCUS += -DMASTER_KEY=$(SANCUS_KEY)
#CMAKE_FLAGS_SANCUS += -DMSP430_GCC_PREFIX=msp430-elf

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

.PHONY: fetch
fetch: fetch-mspgcc
fetch: fetch-llvm
fetch: fetch-legacy-sancus

.PHONY: configure
configure: configure-mspgcc
configure: configure-llvm
configure: configure-legacy-sancus
	$(MKDIR) $(INSTALLDIR)

.PHONY: install-llvm
install-llvm:
	$(CMAKE) --build $(BUILDDIR_LLVM) --target install -- -j$(JOBS)

.PHONY: install-mspgcc
install-mspgcc:
	$(MAKE) -C $(BUILDDIR_BINUTILS) install # DESTDIR=$(INSTALLDIR)
	$(MAKE) -C $(BUILDDIR_GCC) install # DESTDIR=$(INSTALLDIR)
	#TODO: $(UNZIP) $(TI_MSPGCC_SUPPORT_ZIP)

.PHONY: fetch-llvm
fetch-llvm:
	$(GIT) clone $(LLVM_FORK) $(SRCDIR_LLVM)
	$(GIT) clone $(CLANG_FORK) $(SRCDIR_CLANG)

.PHONY: fetch-legacy-sancus
fetch-legacy-sancus:
	$(GIT) clone $(LEGACY_SANCUS_FORK) $(SRCDIR_LEGACY_SANCUS)
	cd $(SRCDIR_LEGACY_SANCUS) && $(GIT) remote add upstream $(LEGACY_SANCUS_REPO)

.PHONY: fetch-mpsgcc
fetch-mspgcc:
	$(WGET) $(TI_MSPGCC_URL)/$(TI_MSPGCC_TBZ)
	$(TAR) -jxf $(TI_MSPGCC_TBZ)
	cd $(SRCDIR_MSPGCC) && $(SH) README-apply-patches.sh
	$(WGET) $(TI_MSPGCC_URL)/$(TI_MSPGCC_SUPPORT_ZIP)

# Inspired on $(SRCDIR_MSPGCC)/README-build.sh
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
	cd $(BUILDDIR_GCC) && $(SRCDIR_GCC)/configure $(CONFIGURE_FLAGS_BINUTILS)

.PHONY: configure-llvm
configure-llvm:
	$(MKDIR) $(BUILDDIR_LLVM)
	cd $(BUILDDIR_LLVM) && $(CMAKE) $(CMAKE_FLAGS_LLVM) $(SRCDIR_LLVM)

.PHONY: configure-legacy-sancus
configure-legacy-sancus:
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) install_deps
	$(MKDIR) $(BUILDDIR_SANCUS_COMPILER)
	$(MKDIR) $(BUILDDIR_SANCUS_SUPPORT)

.PHONY: build-and-install-sancus-core
build-and-install-sancus-core:
	$(MAKE) -C $(SRCDIR_LEGACY_SANCUS) $(MAKE_FLAGS_LEGACY_SANCUS) core-build

.PHONY: build-and-install-legacy-sancus-compiler
build-and-install-legacy-sancus-compiler:
	cd $(BUILDDIR_SANCUS_COMPILER) && \
		$(CMAKE) $(CMAKE_FLAGS_SANCUS) $(SRCDIR_SANCUS_COMPILER)
	$(CMAKE) --build $(BUILDDIR_SANCUS_COMPILER) --target install

.PHONY: build-and-install-sancus-support
build-and-install-sancus-support:
	cd $(BUILDDIR_SANCUS_SUPPORT) && \
		$(CMAKE) $(CMAKE_FLAGS_SANCUS) $(SRCDIR_SANCUS_SUPPORT)
	$(CMAKE) --build $(BUILDDIR_SANCUS_SUPPORT) --target install

.PHONY: build-llvm
build-llvm:
	$(CMAKE) --build $(BUILDDIR_LLVM) -- -j$(JOBS)

.PHONY: build-mspgcc
build-mspgcc:
	$(MAKE) -C $(BUILDDIR_BINUTILS)
	$(MAKE) -C $(BUILDDIR_GCC)

.PHONY: clean
clean:
	$(RM) -r build
	$(RM) -r install

.PHONY: clean-fetch
clean-fetch:
	$(RM) $(TI_MSPGCC_TBZ)
	$(RM) -r $(SRCDIR_MSPGCC)
	$(RM) $(TI_MSPGCC_SUPPORT_ZIP)
	#$(RM) -r $(SRCDIR_LLVM)
	#$(RM) -r $(SRCDIR_LEGACY_SANCUS)
	#$(RM) -r $(SRCDIR_CLANG)
