BUILD_TYPE=Debug
#BUILD_TYPE=Release
JOBS=4

# BASEDIR has to be an absolute path
# If not, the symlink created by the sllvm-checkout target
# will not be correct for example.
BASEDIR=/home/hans/kul/thesis
LLVM_BASEDIR=${BASEDIR}/llvm
RUST_BASEDIR=${BASEDIR}/rust
SANCUS_BASEDIR=${BASEDIR}/sancus-main
SLLVM_BASEDIR=${BASEDIR}/sllvm

VIM=gvim

-include Makefile.local

LLVM_SRCDIR=${LLVM_BASEDIR}/src
#LLVM_OBJDIR=${LLVM_BASEDIR}/build-{BUILD_TYPE}
LLVM_OBJDIR=${LLVM_BASEDIR}/build
LLVM_INSTDIR=${LLVM_BASEDIR}/local

#############################################################################

CMAKE_FLAGS=
CMAKE_FLAGS+=-DCMAKE_BUILD_TYPE=${BUILD_TYPE}
CMAKE_FLAGS+=-DCMAKE_INSTALL_PREFIX=${LLVM_INSTDIR}
#CMAKE_FLAGS+=-DLLVM_TARGETS_TO_BUILD=MSP430
CMAKE_FLAGS+=-DLLVM_TARGETS_TO_BUILD="MSP430;X86"
CMAKE_FLAGS+=-DLLVM_USE_LINKER=gold
CMAKE_FLAGS+=-DLLVM_ENABLE_ASSERTIONS=ON
CMAKE_FLAGS+=-DCMAKE_EXPORT_COMPILE_COMMANDS=ON

LLVM_REPO=http://llvm.org/svn/llvm-project
RUST_REPO=https://github.com/rust-lang/rust.git
SGX_REPO=https://github.com/intel/linux-sgx.git

#############################################################################

all:

sllvm-checkout:
	svn co ${LLVM_REPO}/llvm/trunk ${LLVM_SRCDIR}
	svn co ${LLVM_REPO}/cfe/trunk ${LLVM_SRCDIR}/tools/clang
	#svn co ${LLVM_REPO}/test-suite/trunk ${LLVM_SRCDIR}/projects/test-suite
	find ${LLVM_SRCDIR} -name \*.cpp -exec chmod -w "{}" \;
	find ${LLVM_SRCDIR} -name \*.h -exec chmod -w "{}" \;
	find ${LLVM_SRCDIR} -name \*.td -exec chmod -w "{}" \;
	${RM} ${LLVM_SRCDIR}/projects/sllvm
	ln -s ${SLLVM_BASEDIR} ${LLVM_SRCDIR}/projects/sllvm
	${RM} ${LLVM_SRCDIR}/lib/Target/MSP430/MSP430Sancus.h
	ln -s ${SLLVM_BASEDIR}/lib/Target/MSP430/MSP430Sancus.h \
	      ${LLVM_SRCDIR}/lib/Target/MSP430/MSP430Sancus.h
	${RM} ${LLVM_SRCDIR}/include/llvm/SLLVM.h
	ln -s ${SLLVM_BASEDIR}/include/llvm/SLLVM.h \
	      ${LLVM_SRCDIR}/include/llvm/SLLVM.h
	#curl https://sh.rustup.rs -sSf | sh
	#git clone ${RUST_REPO}
	#git clone ${SGX_REPO}

# TODO:
# 
# 0. Install msp430-elf-gcc
#
# 1. Checkout and build Sancus (to install required tools mspgcc,...)
# ------------------------------------------------------------------
#   - An easy way to install required tools such as mspgcc
#   - I/O facilities from sancus_support (like msp430_io_init()) are also used
#
# 2. part of sllvm-configure ?
# ---------------------------
#   cd rust && mkdir local
#   cp rust/config.toml.example rust/config.toml 
#   CHANGE INSTALL DIR (prefix) in rust/config.toml (local)
#   
#   rustup toolchain link local /home/hans/devel/thesis/rust/local/
#   rustup default local
#
# 3. Xargo and sysroot building
# -----------------------------
#   cargo install xargo
#
# Build the sysroot (in a hello world cargo project (#[no_std])
#  XARGO_RUST_SRC=/home/hans/devel/thesis/rust/src/ xargo build \
#     --target=msp430-none-elf-legacy
#     

sllvm-update:
	svn update ${LLVM_SRCDIR}
	svn update ${LLVM_SRCDIR}/tools/clang
	#svn update ${LLVM_SRCDIR}/projects/test-suite
	#git pull   ${RUST_BASEDIR}/

sllvm-status:
	svn status ${LLVM_SRCDIR}
	svn status ${LLVM_SRCDIR}/tools/clang
	#svn status ${LLVM_SRCDIR}/projects/test-suite
	#cd ${RUST_BASEDIR} && git status

sllvm-configure:
	mkdir -p ${LLVM_OBJDIR}
	mkdir -p ${LLVM_INSTDIR}
	cd ${LLVM_OBJDIR} && cmake ${CMAKE_FLAGS} ${LLVM_SRCDIR}

sllvm-build:
	cmake --build ${LLVM_OBJDIR} -- -j${JOBS}
	#cmake --build ${LLVM_OBJDIR} -- VERBOSE=1 -j${JOBS}
	# cd rust && ./x.py build

sllvm-install:
	cmake --build ${LLVM_OBJDIR} --target install -- -j${JOBS}
	#cmake --build ${LLVM_OBJDIR} --target install -- VERBOSE=1 -j${JOBS}
	# cd rust && ./x.py install

sllvm-create-patch:
	cd ${LLVM_SRCDIR} && svn diff > ${SLLVM_BASEDIR}/llvm.patch
	cd ${LLVM_SRCDIR}/tools/clang && svn diff > ${SLLVM_BASEDIR}/clang.patch
	#cd ${RUST_BASEDIR} && git diff > ${SLLVM_BASEDIR}/rust.patch

sllvm-revert:
	svn stat ${LLVM_SRCDIR} | grep '^M' | tr -s ' ' | cut -d' ' -f2 | xargs svn revert
	svn stat ${LLVM_SRCDIR}/tools/clang | grep '^M' | tr -s ' ' | cut -d' ' -f2 | xargs svn revert

sllvm-apply-msp430-patch:
	svn stat ${LLVM_SRCDIR}/lib/Target | grep '^M' | tr -s ' ' | cut -d' ' -f2 | xargs svn revert
	cd ${LLVM_SRCDIR} && patch -p0 < ${SLLVM_BASEDIR}/llvm.patch

sllvm-apply-full-patch:
	svn stat ${LLVM_SRCDIR} | grep '^M' | tr -s ' ' | cut -d' ' -f2 | xargs svn revert
	svn stat ${LLVM_SRCDIR}/tools/clang | grep '^M' | tr -s ' ' | cut -d' ' -f2 | xargs svn revert
	cd ${LLVM_SRCDIR} && patch -p0 < ${SLLVM_BASEDIR}/llvm.patch
	cd ${LLVM_SRCDIR}/tools/clang && patch -p0 < ${SLLVM_BASEDIR}/clang.patch

sllvm-regtest:
	cmake --build ${LLVM_OBJDIR} --target check-all

#sllvm-test-suite:

sllvm-clean:
	rm -rf ${LLVM_OBJDIR}
	mkdir -p ${LLVM_OBJDIR}

.PHONY: edit
pdf:
	${MAKE} -C text

.PHONY: edit
edit:
	@${VIM} ${BASEDIR}/admin/todo.txt \
	        ${SLLVM_BASEDIR}/lib/Transforms/SLLVM/SancusTransformation.cpp \
				  ${LLVM_SRCDIR}/lib/Target/MSP430/MSP430ISelLowering.cpp \
				  ${LLVM_SRCDIR}/lib/Target/MSP430/MSP430InstrInfo.td

.PHONY: tags
tags:
	ctags -R ${LLVM_SRCDIR}
