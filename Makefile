WGET  = wget
GIT   = git
TAR   = tar
UNZIP = unzip

LLVM_REPO   = https://github.com/hanswinderix/llvm.git
CLANG_REPO  = https://github.com/hanswinderix/clang.git
SANCUS_REPO = https://github.com/hanswinderix/sancus-main.git

# See http://www.ti.com/tool/MSP430-GCC-OPENSOURCE
TI_MSPGCC_SRC_URL = http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/exports
TI_MSPGCC_VER     = 7.3.2.154
TI_MSPGCC_VER_SUP = 1.206
TI_MSPGCC_SRC_DIR = msp430-gcc-$(TI_MSPGCC_VER)-source-patches
TI_MSPGCC_SRC_TBZ = $(TI_MSPGCC_SRC_DIR).tar.bz2
TI_MSPGCC_SUP_DIR = msp430-gcc-support-files-$(TI_MSPGCC_VER_SUP)
TI_MSPGCC_SUP_ZIP = $(TI_MSPGCC_SUP_DIR).zip

.PHONY: fetch-external-sources
fetch-external-sources: clean-external-sources
	$(WGET) $(TI_MSPGCC_SRC_URL)/$(TI_MSPGCC_SRC_TBZ)
	$(TAR) -jxf $(TI_MSPGCC_SRC_TBZ)
	$(WGET) $(TI_MSPGCC_SRC_URL)/$(TI_MSPGCC_SUP_ZIP)
	$(UNZIP) $(TI_MSPGCC_SUP_ZIP)
	$(GIT) clone $(LLVM_REPO)
	$(GIT) clone $(CLANG_REPO) llvm/src/tools/clang
	$(GIT) clone $(SANCUS_REPO)

.PHONY: clean-external-sources
clean-external-sources:
	$(RM) $(TI_MSPGCC_SRC_TBZ)
	$(RM) -r $(TI_MSPGCC_SRC_DIR)
	$(RM) $(TI_MSPGCC_SUP_ZIP)
	$(RM) -r $(TI_MSPGCC_SUP_DIR)
	$(RM) -r clang
	$(RM) -r llvm
	$(RM) -r sancus-main
